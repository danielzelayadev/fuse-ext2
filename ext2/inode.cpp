#include "inode.h"
#include "super.h"
#include "groupdesc.h"
#include "blockgroup.h"
#include "dentry.h"
#include "alloc.h"
#include "../device/device.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

Ext2SuperBlock* sb = 0;
unsigned int inodesPerBlock, inodesPerGroup, itableBlockCount, 
             inodeCount, BLOCK_POINTERS_IN_BLOCK;

int loadSb() {
    if (!sb) {
        sb = new Ext2SuperBlock();
        if (!readSuperBlock(sb))
            return 0;
    }
    return 1;
}

void breakDownBlockNo(int block, int* ti, int* di, int* si, int* blk) {
    if (IS_SI) {
        *ti = *di = 0;
        *blk = block - EXT2_NDIR_BLOCKS;
    } else if (IS_DI) {
        *ti = 0;
        *si = (block - SI_UL) / SI_BLOCKS;
        *blk = (block - SI_UL) % SI_BLOCKS;
    } else if (IS_TI) {
        *di = (block - DI_UL) / DI_BLOCKS;
        *si = (block - DI_UL) / SI_BLOCKS;
        *blk = (block - DI_UL) % SI_BLOCKS;
    } else {
        *ti = *di = *si = 0;
        *blk = block;
    }
}

int accessSIBlock(int siBlockNo, int block, char* buff, int (*op)(int, char*)) {
    if (!siBlockNo) return 0;

    char siBlock[blockSize];
    
    if (!readBlock(siBlockNo, siBlock)) {
        printf("Failed to read SI block\n");
        return 0;
    }

    unsigned int blockNo;

    memcpy(&blockNo, &siBlock[block*(sizeof(unsigned int))], sizeof(unsigned int));

    return blockNo ? op(blockNo, buff) : 0;
}

int accessDIBlock(int diBlockNo, int siPointer, int block, char* buff, int (*op)(int, char*)) {
    if (!diBlockNo) return 0;

    char diBlock[blockSize];
    
    if (!readBlock(diBlockNo, diBlock)) {
        printf("Failed to read DI block\n");
        return 0;
    }

    unsigned int siBlockNo;

    memcpy(&siBlockNo, &diBlock[siPointer*(sizeof(unsigned int))], sizeof(unsigned int));

    return accessSIBlock(siBlockNo, block, buff, op);
}

int accessTIBlock(int tiBlockNo, int diPointer, int siPointer, int block, char* buff, int (*op)(int, char*)) {
    if (!tiBlockNo) return 0;

    char tiBlock[blockSize];
    
    if (!readBlock(tiBlockNo, tiBlock)) {
        printf("Failed to read TI block\n");
        return 0;
    }

    unsigned int diBlockNo;

    memcpy(&diBlockNo, &tiBlock[diPointer*(sizeof(unsigned int))], sizeof(unsigned int));

    return accessDIBlock(diBlockNo, siPointer, block, buff, op);
}

int accessInodeBlock(Ext2Inode inode, int block, char* buff, int (*op)(int, char*)) {
    if (indexOutOfBounds(block, inode.i_blocks))
        return 0;

    if (block < EXT2_NDIR_BLOCKS && inode.i_block[block])
        return op(inode.i_block[block], buff);

    int ti = inode.i_block[EXT2_TIND_BLOCK], 
        di = inode.i_block[EXT2_DIND_BLOCK], 
        si = inode.i_block[EXT2_IND_BLOCK], 
        b  = block;

    breakDownBlockNo(block, &ti, &di, &si, &b);

    if (IS_SI)
        return accessSIBlock(si, b, buff, op);
    if (IS_DI)
        return accessDIBlock(di, si, b, buff, op);
    if (IS_TI)
        return accessTIBlock(ti, di, si, b, buff, op);

    return 0;
}

int getBlockGroupOfInode(int inodeNo) {
    if (!loadSb()) return -1;
    return (inodeNo - 1) / sb->s_inodes_per_group;
}

int getInodeIndexInGroup(int inodeNo) {
    if (!loadSb()) return -1;
    return (inodeNo - 1) % sb->s_inodes_per_group;
}

int calcInodePositionInDevice(Ext2GroupDescriptor gd, int inodeIndexInGroup) {
    return (gd.bg_inode_table * blockSize) + (inodeIndexInGroup * INODE_SIZE);
}

int getInodePosition(int inodeNo) {
    if (!loadSb()) return -1;

    Ext2GroupDescriptor gd;

    int blockGroup = getBlockGroupOfInode(inodeNo);

    if (blockGroup < 0 || !readGroupDesc(blockGroup, &gd))
        return -1;

    int indxInGroup = getInodeIndexInGroup(inodeNo);

    int pos = calcInodePositionInDevice(gd, indxInGroup);

    sb = 0;

    return pos;
}

int getInodeByPath(string path, Ext2Inode* inode) {
    if (path == "/")
        return readInode(ROOT_DIR_INODE, inode);
    
    int inodeIndx = getInodeIndexByPath(path);    

    return inodeIndx && readInode(inodeIndx, inode);
}

int getInodeIndexByPath(string path) {
    Ext2Dentry dentry;
    return readDentry(path, &dentry) ? dentry.inode : 0;
}

void addSIBlockPointer(int siBlockNo, int b, int pointer) {
    if (!siBlockNo) return;
    write((blockSize*siBlockNo) + (sizeof(uint32_t) * b), &pointer, sizeof(uint32_t));
}

void addDIBlockPointer(int diBlockNo, int si, int b, int pointer) {
    if (!diBlockNo) return;

    int siBlockNo;
    
    if (!b) {
        siBlockNo = allocBlock();
        write((diBlockNo * blockSize) + (sizeof(uint32_t) * si), &siBlockNo, sizeof(uint32_t));
    } else
        read((diBlockNo * blockSize) + (sizeof(uint32_t) * si), &siBlockNo, sizeof(uint32_t));

    addSIBlockPointer(siBlockNo, b, pointer);
}

void addTIBlockPointer(int tiBlockNo, int di, int si, int b, int pointer) {
    if (!tiBlockNo) return;

    int diBlockNo;
    
    if (!si && !b) {
        diBlockNo = allocBlock();
        write((tiBlockNo * blockSize) + (sizeof(uint32_t) * di), &diBlockNo, sizeof(uint32_t));
    } else
        read((tiBlockNo * blockSize) + (sizeof(uint32_t) * di), &diBlockNo, sizeof(uint32_t));

    addDIBlockPointer(diBlockNo, si, b, pointer);
}

void addInodeBlockPointer(Ext2Inode* inode, int blockPointer) {
    int block = I_BLOCKS((*inode), blockSize);

    if (block < EXT2_NDIR_BLOCKS) {
        inode->i_block[block] = blockPointer;
        return;
    }
    
    int ti = inode->i_block[EXT2_TIND_BLOCK], 
        di = inode->i_block[EXT2_DIND_BLOCK], 
        si = inode->i_block[EXT2_IND_BLOCK], 
        b  = block;

    breakDownBlockNo(block, &ti, &di, &si, &b);

    if (IS_SI) {
        if (!b)
            inode->i_block[EXT2_IND_BLOCK] = allocBlock();
        addSIBlockPointer(si, b, blockPointer);
    }
    else if (IS_DI) {
        if (!si && !b)
            inode->i_block[EXT2_DIND_BLOCK] = allocBlock();
        addDIBlockPointer(di, si, b, blockPointer);
    }
    else if (IS_TI) {
        if (!di && !si && !b)
            inode->i_block[EXT2_TIND_BLOCK] = allocBlock();
        addTIBlockPointer(ti, di, si, b, blockPointer);
    }
}

int allocInodeBlock(int inodeNo, Ext2Inode* inode) {
    int allocatedBlock = allocBlock();

    if (allocatedBlock != -1) {
        addInodeBlockPointer(inode, allocatedBlock);
        inode->i_size += blockSize;
        inode->i_blocks = inode->i_size / 512;
        return writeInode(inodeNo, *inode) ? allocatedBlock : -1;
    } else
        printf("allocInodeBlock: Could not allocate block for inode.\n");

    return -1;
}

int writeInode(int inodeNo, Ext2Inode inode) {
    return write(getInodePosition(inodeNo), (void*)&inode, INODE_SIZE);
}

int writeInodeBlock(Ext2Inode inode, int block, char* buff) {
    return accessInodeBlock(inode, block, buff, writeBlock);
}

int writeInodeBlock(int inodeNo, Ext2Inode inode, void* buff, int pos, int size) {
    if ((pos + size) > inode.i_size) {
        int blocksNeeded = (((pos + size) - inode.i_size) / blockSize) + 1;

        for (int i = 0; i < blocksNeeded; i++)
            if (allocInodeBlock(inodeNo, &inode) == -1)
                return 0;
    }

    int blockNo = pos / blockSize;
    int posFirstBlock = pos % blockSize;
    int wleft = size, wcount = 0;
    char block[blockSize];

    for (int i = 0; (blockNo*blockSize) < inode.i_size && wleft > 0; i++) {
        if (!readInodeBlock(inode, blockNo, block))
            return 0;

        int bpos = i ? 0 : posFirstBlock;
        int bleft = blockSize - bpos;
        int wsize = wleft < bleft ? wleft : bleft;

        memcpy(&block[bpos], &(((char*)buff)[wcount]), wsize);

        if (!writeInodeBlock(inode, blockNo, block))
            return 0;

        blockNo++;
        wleft -= wsize;
        wcount += wsize;
    }

    return wcount;
}

int readInode(int inodeNo, Ext2Inode* inode) {
    return read(getInodePosition(inodeNo), (void*)inode, INODE_SIZE);
}

int readInodeBlock(Ext2Inode inode, int block, char* buff) {
    return accessInodeBlock(inode, block, buff, readBlock);
}

int readInodeBlock(Ext2Inode inode, void* buff, int pos, int size) {
    int blockNo = pos / blockSize;
    int posFirstBlock = pos % blockSize;
    int rleft = size > inode.i_size ? inode.i_size : size, rcount = 0;
    char block[blockSize];

    for (int i = 0; (blockNo*blockSize) < inode.i_size && rleft; i++) {
        if (!readInodeBlock(inode, blockNo, block))
            return 0;

        int bpos = i ? 0 : posFirstBlock;
        int bleft = blockSize - bpos;
        int rsize = rleft < bleft ? rleft : bleft;

        memcpy(&(((char*)buff)[rcount]), &block[bpos], rsize);

        blockNo++;
        rleft -= rsize;
        rcount += rsize;
    }

    return rcount;
}

void fillInodeStatBuff(int inodeNo, struct stat* statbuf) {
    Ext2Inode inode;
    if (readInode(inodeNo, &inode))
        fillInodeStatBuff(inode, statbuf);
}

void fillInodeStatBuff(Ext2Inode inode, struct stat* statbuf) {
    statbuf->st_mode    = inode.i_mode;
    statbuf->st_nlink   = inode.i_links_count;
    statbuf->st_size    = inode.i_size;
    statbuf->st_blocks  = inode.i_blocks;
    statbuf->st_uid     = inode.i_uid;
    statbuf->st_gid     = inode.i_gid;
    statbuf->st_atime   = inode.i_atime;
    statbuf->st_mtime   = inode.i_mtime;
    statbuf->st_ctime   = inode.i_ctime;
    statbuf->st_blksize = blockSize;
}

void printInode(Ext2Inode inode) {
    printf("Reading inode\n"
	       "File mode: %hu\n"
	       "Owner UID: %hu\n"
	       "Size     : %u bytes\n"
	       "Blocks   : %u\n"
	       ,
	       inode.i_mode,
	       inode.i_uid,
	       inode.i_size,
	       inode.i_blocks);

	for(int i=0; i<EXT2_N_BLOCKS; i++)
		if (i < EXT2_NDIR_BLOCKS)         /* direct blocks */
			printf("Block %2u : %u\n", i, inode.i_block[i]);
		else if (i == EXT2_IND_BLOCK)     /* single indirect block */
			printf("Single   : %u\n", inode.i_block[i]);
		else if (i == EXT2_DIND_BLOCK)    /* double indirect block */
			printf("Double   : %u\n", inode.i_block[i]);
		else if (i == EXT2_TIND_BLOCK)    /* triple indirect block */
			printf("Triple   : %u\n", inode.i_block[i]);
}
#include "inode.h"
#include "super.h"
#include "groupdesc.h"
#include "blockgroup.h"
#include "dentry.h"
#include "../device/device.h"
#include "utils.h"
#include <vector>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

Ext2SuperBlock* sb = 0;
unsigned int inodesPerBlock, itableBlockCount, 
             BLOCK_POINTERS_IN_BLOCK;

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

int readSIBlock(int siBlockNo, int block, char* buff) {
    if (!siBlockNo) return 0;

    char siBlock[blockSize];
    
    if (!readBlock(siBlockNo, siBlock)) {
        printf("Failed to read SI block\n");
        return 0;
    }

    unsigned int blockNo;

    memcpy(&blockNo, &siBlock[block], sizeof(unsigned int));

    return blockNo ? readBlock(blockNo, buff) : 0;
}

int readDIBlock(int diBlockNo, int siPointer, int block, char* buff) {
    if (!diBlockNo) return 0;

    char diBlock[blockSize];
    
    if (!readBlock(diBlockNo, diBlock)) {
        printf("Failed to read DI block\n");
        return 0;
    }

    unsigned int siBlockNo;

    memcpy(&siBlockNo, &diBlock[siPointer], sizeof(unsigned int));

    return readSIBlock(siBlockNo, block, buff);
}

int readTIBlock(int tiBlockNo, int diPointer, int siPointer, int block, char* buff) {
    if (!tiBlockNo) return 0;

    char tiBlock[blockSize];
    
    if (!readBlock(tiBlockNo, tiBlock)) {
        printf("Failed to read TI block\n");
        return 0;
    }

    unsigned int diBlockNo;

    memcpy(&diBlockNo, &tiBlock[diPointer], sizeof(unsigned int));

    return readDIBlock(diBlockNo, siPointer, block, buff);
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

int getInodeByPath(string path, Ext2Inode* inode) {
    if (path == "/")
        readInode(ROOT_DIR_INODE, inode);
    else {
        vector<string> filenames;
        split(path, '/', filenames);

        string currDir = "/";
        int currDirInodeNo = ROOT_DIR_INODE;

        for (int i = 0; i < filenames.size(); i++) {
            Ext2Inode dirInode;
            Ext2Dentry dentry;

            printf("\nFor Path %s, looking for File '%s' in Directory '%s'\n", 
                path.c_str(), filenames[i].c_str(), currDir.c_str());

            if (!readInode(currDirInodeNo, &dirInode)) {
                printf("ReadInode Failed...\n");
                return 0;
            }

            // printInode(dirInode);

            if (!readDentry(dirInode, filenames[i], &dentry)) {
                printf("Search for File '%s' in directory '%s' failed.\n", 
                    filenames[i].c_str(), currDir.c_str());
                return 0;
            }

            // printDentry(dentry);
            currDirInodeNo = dentry.inode;
            currDir = filenames[i];
        }

        return readInode(currDirInodeNo, inode);
    }
    
    return 1;
}

int readInode(int inodeNo, Ext2Inode* inode) {
    if (!loadSb()) return 0;

    Ext2GroupDescriptor gd;

    int blockGroup = getBlockGroupOfInode(inodeNo);

    if (blockGroup < 0 || !readGroupDesc(blockGroup, &gd))
        return 0;

    int indxInGroup = getInodeIndexInGroup(inodeNo);

    int pos = calcInodePositionInDevice(gd, indxInGroup);

    sb = 0;

    return read(pos, (void*)inode, INODE_SIZE);
}

int readInodeBlock(Ext2Inode inode, int block, char* buff) {
    if (indexOutOfBounds(block, inode.i_blocks))
        return 0;

    if (block < EXT2_NDIR_BLOCKS && inode.i_block[block])
        return readBlock(inode.i_block[block], buff);

    int ti = inode.i_block[EXT2_TIND_BLOCK], 
        di = inode.i_block[EXT2_DIND_BLOCK], 
        si = inode.i_block[EXT2_IND_BLOCK], 
        b  = block;

    breakDownBlockNo(block, &ti, &di, &si, &b);

    if (IS_SI)
        return readSIBlock(si, b, buff);
    if (IS_DI)
        return readDIBlock(di, si, b, buff);
    if (IS_TI)
        return readTIBlock(ti, di, si, b, buff);
        
    return 0;
}

void fillInodeStatBuff(int inodeNo, struct stat* statbuf) {
    Ext2Inode inode;
    if (readInode(inodeNo, &inode))
        fillInodeStatBuff(inode, statbuf);
}

void fillInodeStatBuff(Ext2Inode inode, struct stat* statbuf) {
    statbuf->st_mode   = inode.i_mode;
    statbuf->st_nlink  = inode.i_links_count;
    statbuf->st_size   = inode.i_size;
    statbuf->st_blocks = inode.i_blocks;
    statbuf->st_uid    = inode.i_uid;
    statbuf->st_gid    = inode.i_gid;
    statbuf->st_atime  = inode.i_atime;
    statbuf->st_mtime  = inode.i_mtime;
    statbuf->st_ctime  = inode.i_ctime;
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
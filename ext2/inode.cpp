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

            printf("\nLooking for File '%s' in Directory '%s'\n", filenames[i].c_str(), currDir.c_str());

            if (!readInode(currDirInodeNo, &dirInode)) {
                printf("ReadInode Failed...\n");
                return 0;
            }

            printInode(dirInode);

            if (!readDentry(dirInode, filenames[i], &dentry)) {
                printf("Search for File '%s' in directory '%s' failed.\n", 
                    filenames[i].c_str(), currDir.c_str());
                return 0;
            }

            printDentry(dentry);
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

    int pos = calcInodePositionInDevice(gd, inodeNo);

    sb = 0;

    return read(pos, (void*)inode, INODE_SIZE);
}

int readInodeBlock(Ext2Inode inode, int block, char* buff) {
    if (indexOutOfBounds(block, inode.i_blocks))
        return 0;
    
    if (block < EXT2_NDIR_BLOCKS && inode.i_block[block])
        return readBlock(inode.i_block[block], buff);
    if (IS_SI)
        return readInodeSIBlock(inode, block, buff);
    if (IS_DI)
        return readInodeDIBlock(inode, block, buff);
    if (IS_TI)
        return readInodeDIBlock(inode, block, buff);
        
    return 0;
}

int readInodeSIBlock(Ext2Inode inode, int block, char* buff) {
    return IS_SI ? readSIBlock(inode.i_block[EXT2_IND_BLOCK], block - EXT2_NDIR_BLOCKS, buff) : 0;
}

int readInodeDIBlock(Ext2Inode inode, int block, char* buff) {
    if (!IS_DI) return 0;

    int ti, di, si, b;

    breakDownBlockNo(block, &ti, &di, &si, &b);

    return readDIBlock(inode.i_block[EXT2_DIND_BLOCK], si, b, buff);
}

int readInodeTIBlock(Ext2Inode inode, int block, char* buff) {
    if (!IS_TI) return 0;

    int ti, di, si, b;

    breakDownBlockNo(block, &ti, &di, &si, &b);

    return readTIBlock(inode.i_block[EXT2_TIND_BLOCK], di, si, b, buff);
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

int getIndexOfInodeBlockInGroup(int indexInGroup) {
    int blockNo = indexInGroup / inodesPerBlock;
    return blockNo <  itableBlockCount ? blockNo : -1;
}

int getIndexOfInodeInBlock(int indexInGroup) {
    return indexInGroup % inodesPerBlock;
}

int calcInodePositionInDevice(Ext2GroupDescriptor gd, int inodeNo) {
    return (gd.bg_inode_table * blockSize) + ((inodeNo - 1)*INODE_SIZE);
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
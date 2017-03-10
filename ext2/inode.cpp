#include "inode.h"
#include "super.h"
#include "groupdesc.h"
#include "blockgroup.h"
#include "../device/device.h"
#include <stdio.h>

Ext2SuperBlock* sb = 0;
unsigned int inodesPerBlock, itableBlockCount;

int loadSb() {
    if (!sb) {
        sb = new Ext2SuperBlock();
        if (!readSuperBlock(sb))
            return 0;
    }
    return 1;
}

int getInodeByPath(string path, Ext2Inode* inode) {
    return 1;
}

int readInode(int inodeNo, Ext2Inode* inode) {
    if (!loadSb()) return 0;

    Ext2GroupDescriptor gd;

    int blockGroup = getBlockGroupOfInode(inodeNo);

    if (blockGroup < 0 || !readGroupDesc(blockGroup, &gd))
        return 0;

    int inodeIndexInGroup = getInodeIndexInGroup(inodeNo);

    if (inodeIndexInGroup < 0)
        return 0;

    int blockIndexInGroup = getIndexOfInodeBlockInGroup(inodeIndexInGroup);

    if (blockIndexInGroup < 0)
        return 0;

    int pos = calcInodePositionInDevice(gd, blockIndexInGroup, inodeIndexInGroup);

    sb = 0;

    return read(pos, (void*)inode, INODE_SIZE);
}

int getBlockGroupOfInode(int inodeNo) {
    if (!loadSb()) return -1;
    return inodeNo / sb->s_inodes_per_group;
}

int getInodeIndexInGroup(int inodeNo) {
    if (!loadSb()) return -1;
    return inodeNo % sb->s_inodes_per_group;
}

int getIndexOfInodeBlockInGroup(int indexInGroup) {
    int blockNo = indexInGroup / inodesPerBlock;
    return blockNo <  itableBlockCount ? blockNo : -1;
}

int getIndexOfInodeInBlock(int indexInGroup) {
    return indexInGroup % inodesPerBlock;
}

int calcInodePositionInDevice(Ext2GroupDescriptor gd, int blockIndexInGroup, int inodeIndexInBlock) {
    int blockInDevice = calcInodeBlockInDevice(gd, blockIndexInGroup);
    return (blockInDevice * blockSize) + (inodeIndexInBlock * INODE_SIZE);
}

int calcInodeBlockInDevice(Ext2GroupDescriptor gd, int blockIndexInGroup) {
    return gd.bg_inode_table + blockIndexInGroup;
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
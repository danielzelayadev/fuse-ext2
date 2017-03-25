#include "alloc.h"
#include "bitmap.h"
#include "super.h"
#include "inode.h"
#include "groupdesc.h"
#include "../device/device.h"

#include <stdio.h>

int getFirstFreeBlock() {
    for (int i = 3; i < blockCount; i++)
        if (bitIsOff(i, BLOCK_BITMAP))
            return i;
    return -1;
}

int getNextFreeInode() {
    for (int i = 0; i < inodeCount; i++)
        if (bitIsOff(i, INODE_BITMAP))
            return i;
    return -1;
}

int allocBlock() {
    int firstFreeBlock = getFirstFreeBlock();

    if (firstFreeBlock == -1) {
        printf("allocBlock: Out of space.\n");
        return -1;
    }

    if (turnOnBit(firstFreeBlock, BLOCK_BITMAP)) {
        Ext2SuperBlock sb;
        Ext2GroupDescriptor gd;
        int blockGroup = BIT_BLOCK_GROUP(firstFreeBlock, BLOCK_BITMAP);

        if (!readSuperBlock(&sb)) {
            printf("allocBlock: Could not read super block.\n");
            return -1;
        }

        sb.s_r_blocks_count++;
        sb.s_free_blocks_count--;

        if (!writeSuperBlock(&sb)) {
            printf("allocBlock: Could not update super block.\n");
            return -1;
        }

        if (!readGroupDesc(blockGroup, &gd)) {
            printf("allocBlock: Could not read group desc.\n");
            return -1;
        }

        gd.bg_free_blocks_count--;

        if (!writeGroupDesc(blockGroup, &gd)) {
            printf("allocBlock: Could not update group desc.\n");
            return -1;
        }

        return firstFreeBlock;
    }

    return -1;
}

int allocInode() {
    int firstFreeInode = getNextFreeInode();

    if (firstFreeInode == -1) {
        printf("allocInode: No more inodes.\n");
        return -1;
    }

    if (turnOnBit(firstFreeInode, INODE_BITMAP)) {
        Ext2SuperBlock sb;
        Ext2GroupDescriptor gd;
        int blockGroup = BIT_BLOCK_GROUP(firstFreeInode, INODE_BITMAP);

        if (!readSuperBlock(&sb)) {
            printf("allocInode: Could not read super block.\n");
            return -1;
        }

        sb.s_free_inodes_count--;

        if (!writeSuperBlock(&sb)) {
            printf("allocInode: Could not update super block.\n");
            return -1;
        }

        if (!readGroupDesc(blockGroup, &gd)) {
            printf("allocInode: Could not read group desc.\n");
            return -1;
        }

        gd.bg_free_inodes_count--;

        if (!writeGroupDesc(blockGroup, &gd)) {
            printf("allocInode: Could not update group desc.\n");
            return -1;
        }

        return firstFreeInode + 1;
    }

    return -1;
}
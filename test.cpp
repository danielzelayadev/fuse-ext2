#include <stdio.h>
#include <string.h>
#include "device/device.h"
#include "ext2/ext2.h"
#include "ext2/super.h"
#include "ext2/inode.h"
#include "ext2/groupdesc.h"
#include "ext2/blockgroup.h"
#include "ext2/alloc.h"
#include "ext2/bitmap.h"

void init() {
    Ext2SuperBlock sb;

    if (!readSuperBlock(&sb))
        return;

    blockSize = 1024 << sb.s_log_block_size;
    blockCount = sb.s_blocks_count;
    groupCount = 1 + (blockCount - 1) / sb.s_blocks_per_group;
    descriptorListSize = groupCount * GD_SIZE;
    inodesPerBlock = blockSize / INODE_SIZE;
    itableBlockCount = sb.s_inodes_per_group / inodesPerBlock;
    BLOCK_POINTERS_IN_BLOCK = blockSize/sizeof(uint32_t);
}

void printBitmapBlock(int blockNo, int type) {
    int start = blockNo * BITS_IN_BMP,
        end   = start + BITS_IN_BMP;
    int lowerBound, higherBound;

    for (int i = start; i < end; i++) {
        if (!(i % 8)) {
            if (i != start) printf("\n");
            lowerBound = i;
            higherBound = i + 7;
            printf("%d-%d ", lowerBound, higherBound);
        }
        if (bitIsOn(i, type))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

int main(int argc, char **argv) {
	if (!openDevice("ext2device")) {
		printf("Unable to open device.\n");
		return 1;
	}

    init();

    // for (int i = 0; i < 300; i++)
    //     printf("Block Allocated: %d.\n\n", allocBlock());
    // printBitmapBlock(0, BLOCK_BITMAP); 
    printBitmapBlock(0, BLOCK_BITMAP);    

    closeDevice();

	return 0;
}

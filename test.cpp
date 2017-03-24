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
#include "ext2/utils.h"

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
    inodesPerGroup = sb.s_inodes_per_group;
    inodeCount = inodesPerGroup * groupCount;
    BLOCK_POINTERS_IN_BLOCK = blockSize/sizeof(uint32_t);
}

void printBitmapBlock(int groupNo, int type) {
    int start = groupNo * BITS_IN_BMP,
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

void testBitmap(int groupNo, int type) {
    Ext2SuperBlock sb;
    readSuperBlock(&sb);
    Ext2GroupDescriptor gd;
    readGroupDesc(groupNo, &gd);

    int RBC = 0, FBC = 0;

    int start = BITS_IN_BMP*groupNo;
    int end = start + (type == BLOCK_BITMAP ? BITS_IN_BMP : inodesPerGroup);

    for (int i = start; i < end; i++)
        if (bitIsOff(i, type))
            FBC++;
        else
            RBC++;

    int sum = RBC + FBC;

    const char* result = 
        sum == (type == BLOCK_BITMAP ? BITS_IN_BMP : inodesPerGroup) &&
        FBC == (type == BLOCK_BITMAP ? gd.bg_free_blocks_count : gd.bg_free_inodes_count) ? 
        "SUCCESS" : "FAILED";

    printf("%s Group %d Bitmap Test Result: %s\n", (type == BLOCK_BITMAP ? "Block" : "Inode"), groupNo, result);
}

// Deberia de assert el groupdesc
void testBlockAllocation() {
    Ext2SuperBlock oldSb;
    readSuperBlock(&oldSb);

    int allocatedBlock = allocBlock();

    Ext2SuperBlock newSb;
    readSuperBlock(&newSb);

    char* result = "FAILED";

    if (allocatedBlock != -1 &&
        oldSb.s_r_blocks_count == newSb.s_r_blocks_count - 1 &&
        oldSb.s_free_blocks_count == newSb.s_free_blocks_count + 1)
        result = "SUCCESS";

    printf("Block Allocation Test Result: %s\n", result);
}

// Deberia de assert el groupdesc
void testInodeAllocation() {
    Ext2SuperBlock oldSb;
    readSuperBlock(&oldSb);

    int allocatedInode = allocInode();

    Ext2SuperBlock newSb;
    readSuperBlock(&newSb);

    char* result = "FAILED";

    if (allocatedInode != -1 &&
        oldSb.s_free_inodes_count == newSb.s_free_inodes_count + 1)
        result = "SUCCESS";

    printf("Inode Allocation Test Result: %s\n", result);
}

int main(int argc, char **argv) {
	if (!openDevice("dev")) {
		printf("Unable to open device.\n");
		return 1;
	}

    init();

    printf("\n/////// BLOCK BITMAP TESTS ///////\n\n");
    for (int i = 0; i < groupCount; i++)
        testBitmap(i, BLOCK_BITMAP);

    printf("\n/////// INODE BITMAP TESTS ///////\n\n");
    for (int i = 0; i < groupCount; i++)
        testBitmap(i, INODE_BITMAP);

    printf("\n/////// BLOCK ALLOCATION TESTS ///////\n\n");
    for (int i = 0; i < 20; i++)
        testBlockAllocation();

    printf("\n/////// INODE ALLOCATION TESTS ///////\n\n");
    for (int i = 0; i < 20; i++)
        testInodeAllocation();

    closeDevice();

	return 0;
}

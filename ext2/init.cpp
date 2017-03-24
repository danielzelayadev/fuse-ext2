#include "operations.h"
#include "super.h"
#include "inode.h"
#include "groupdesc.h"
#include "blockgroup.h"
#include "../device/device.h"

#include <stdio.h>

void* init(struct fuse_conn_info *conn) {
    printf("OPERATION: init...\n");

    Ext2SuperBlock sb;

    if (!readSuperBlock(&sb))
        return 0;

    printSuperBlock(sb);

    blockSize = 1024 << sb.s_log_block_size;
    blockCount = sb.s_blocks_count;
    groupCount = 1 + (blockCount - 1) / sb.s_blocks_per_group;
    descriptorListSize = groupCount * GD_SIZE;
    inodesPerBlock = blockSize / INODE_SIZE;
    itableBlockCount = sb.s_inodes_per_group / inodesPerBlock;
    inodesPerGroup = sb.s_inodes_per_group;
    BLOCK_POINTERS_IN_BLOCK = blockSize/sizeof(uint32_t);
}
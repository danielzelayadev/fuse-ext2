#include "operations.h"
#include "super.h"
#include <errno.h>
#include <stdio.h>
#include "../device/device.h"

int statfs(const char* path, struct statvfs* stbuf) {
    printf("OPERATION: statfs(path=%s)\n", path);

    Ext2SuperBlock sb;

    if (!readSuperBlock(&sb))
        return -EIO;

    stbuf->f_bsize   = blockSize;
    stbuf->f_frsize  = 1024 << sb.s_log_frag_size;
    stbuf->f_blocks  = sb.s_blocks_count;
    stbuf->f_bfree   = sb.s_free_blocks_count;
    stbuf->f_bavail  = sb.s_free_blocks_count;
    stbuf->f_files   = sb.s_inodes_count;
    stbuf->f_ffree   = sb.s_free_inodes_count;
    stbuf->f_favail  = sb.s_free_inodes_count;
    stbuf->f_namemax = EXT2_NAME_LEN;

    return 0;
}
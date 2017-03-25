#include "operations.h"
#include "inode.h"
#include <errno.h>
#include <stdio.h>

int write(const char* path, const char *buf, size_t size, off_t offset, struct fuse_file_info* fi) {
    printf("OPERATION: write(path=%s, size=%d, offset=%d)\n", path, size, offset);

    int bytesWritten = 0;

    Ext2Inode inode;

    printf("Path: %s === FH: %d\n", path, fi->fh);

    if (!readInode(fi->fh, &inode))
        return -EIO;

    bytesWritten = writeInodeBlock(fi->fh, inode, (void*)buf, offset, size);

    offset += bytesWritten;

    return bytesWritten;
}
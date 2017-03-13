#include "operations.h"
#include "inode.h"
#include <errno.h>
#include <stdio.h>

int read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    printf("OPERATION: read(path=%s, size=%d, offset=%d)\n", path, size, offset);

    int bytesRead = 0;

    Ext2Inode inode;

    printf("Path: %s === FH: %d\n", path, fileInfo->fh);

    if (!readInode(fileInfo->fh, &inode))
        return -EIO;

    // printInode(inode);

    bytesRead = readInodeBlock(inode, buf, offset, size);

    offset += bytesRead;

    // printf("Bytes Read: %d, Offset: %d\n", bytesRead, offset);

    return bytesRead;
}
#include "operations.h"
#include "inode.h"
#include "dentry.h"
#include "utils.h"
#include "../device/device.h"
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

int mknod(const char* path, mode_t mode, dev_t rdev) throw() {
    printf("OPERATION: mknod(path=%s, mode=%d, rdev=%d)\n", path, (int)mode, (int)rdev);

    Ext2Dentry dentry;

    if (!newDentry(path, mode))
            return -EIO;

    int inodeNo = getInodeIndexByPath(path);

    Ext2Inode inode;
    char clean[blockSize];

    memset(&inode, 0, INODE_SIZE);
    memset(clean, 0, blockSize);

    inode.i_mode  = mode;
    inode.i_uid   = _getuid();
    inode.i_atime = time(0);
    inode.i_ctime = time(0);
    inode.i_mtime = time(0);
    inode.i_dtime = 0;
    inode.i_gid   = _getgid();
    inode.i_links_count = 0;
    inode.i_flags = 0;

    allocInodeBlock(inodeNo, &inode);
    writeInodeBlock(inode, 0, clean);

    return 0;
}
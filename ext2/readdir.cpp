#include "operations.h"
#include "inode.h"
#include "dentry.h"
#include <errno.h>
#include <stdio.h>
#include "../device/device.h"
#include <string.h>


int readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    printf("OPERATION: readdir(path=%s, offset=%d)\n", path, offset);

    string p(path);
    Ext2Inode dirInode;

    if (!getInodeByPath(p, &dirInode))
        return -ENOENT;

    if (!S_ISDIR(dirInode.i_mode)) {
        printf("Not a directory.\n");
        return -ENOTDIR;
    }

    Ext2Dentry dentry;
    struct stat statbuf;

    while (readDentry(dirInode, offset, &dentry)) {
        // printDentry(dentry);

        offset += dentry.rec_len;

         if (!dentry.inode)
            continue;

        fillInodeStatBuff(dentry.inode, &statbuf);

        char name[dentry.name_len];
        getPrintableDentryName(dentry, name);
        
        if (filler(buf, name, &statbuf, offset))
            return 0;
    }
    
    return 0;
}
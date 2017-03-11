#include "operations.h"
#include "inode.h"
#include "types.h"
#include <errno.h>
#include <stdio.h>
#include <string>

using namespace std;

int getattr(const char *path, struct stat *statbuf) {
    printf("OPERATION: getattr(path=%s)\n", path);

    Ext2Inode inode;
    string p(path);

    if (!getInodeByPath(p, &inode))
        return -ENOENT;
    
    // printInode(inode);

    statbuf->st_mode   = inode.i_mode;
    statbuf->st_nlink  = inode.i_links_count;
    statbuf->st_size   = inode.i_size;
    statbuf->st_blocks = inode.i_blocks;
    statbuf->st_uid    = inode.i_uid;
    statbuf->st_gid    = inode.i_gid;
    statbuf->st_atime  = inode.i_atime;
    statbuf->st_mtime  = inode.i_mtime;
    statbuf->st_ctime  = inode.i_ctime;

    return 0;
}
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

    fillInodeStatBuff(inode, statbuf);

    return 0;
}
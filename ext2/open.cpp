#include "operations.h"
#include "inode.h"
#include <string>
#include <errno.h>
#include <stdio.h>

using namespace std;

int open(const char *path, struct fuse_file_info *fi) {
    printf("OPERATION: open(path=%s)\n", path);

     if((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    fi->fh = getInodeIndexByPath(string(path));

    return 0;
}
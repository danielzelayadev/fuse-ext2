#include "operations.h"
#include <stdio.h>

int open(const char *path, struct fuse_file_info *fileInfo) {
    printf("OPERATION: open(path=%s)\n", path);
    return 1;
}
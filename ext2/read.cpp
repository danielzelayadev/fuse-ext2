#include "operations.h"
#include <stdio.h>

int read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    printf("OPERATION: read(path=%s)\n", path);
    return 1;
}
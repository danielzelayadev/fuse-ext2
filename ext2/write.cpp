#include "operations.h"
#include <stdio.h>

int write(const char* path, const char *buf, size_t size, off_t offset, struct fuse_file_info* fi) {
    printf("OPERATION: write(path=%s, size=%d, offset=%d)\n", path, size, offset);
    return 0;
}
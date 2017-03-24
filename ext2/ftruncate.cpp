#include "operations.h"
#include <stdio.h>

int ftruncate(const char* path, off_t size, fuse_file_info* fi) {
    printf("OPERATION: ftruncate(path=%s, size=%d)\n", path, size);
    return 0;
}
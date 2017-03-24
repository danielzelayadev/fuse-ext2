#include "operations.h"
#include <stdio.h>

int mknod(const char* path, mode_t mode, dev_t rdev) throw() {
    printf("OPERATION: mknod(path=%s, mode=%d, rdev=%d)\n", path, (int)mode, (int)rdev);
    return 0;
}
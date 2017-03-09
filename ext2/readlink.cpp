#include "operations.h"
#include <stdio.h>

int readlink(const char *path, char *link, size_t size) {
    printf("OPERATION: readlink(path=%s, link=%s, size=%d)\n", path, link, size);
    return 1;
}
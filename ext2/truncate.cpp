#include "operations.h"
#include <stdio.h>

int truncate(const char* path, off_t size) {
    printf("OPERATION: truncate(path=%s, size=%d)\n", path, size);
    return 0;
}
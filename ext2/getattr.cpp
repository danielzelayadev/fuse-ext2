#include "operations.h"
#include <stdio.h>

int getattr(const char *path, struct stat *statbuf) {
    printf("OPERATION: getattr(path=%s)\n", path);
    return 1;
}
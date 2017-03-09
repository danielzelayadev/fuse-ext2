#include "operations.h"
#include <stdio.h>

int readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    printf("OPERATION: readdir(path=%s)\n", path);
    return 1;
}
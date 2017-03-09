#include "operations.h"
#include <stdio.h>

void* init(struct fuse_conn_info *conn) {
    printf("OPERATION: init...\n");
}
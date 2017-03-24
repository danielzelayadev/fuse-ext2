#include "operations.h"
#include "dentry.h"
#include <errno.h>
#include <stdio.h>
#include <string>

using namespace std;

int mkdir(const char* path, mode_t mode) throw() {
    printf("OPERATION: mkdir(path=%s, mode=%d)\n", path, (int)mode);

    Ext2Dentry dentry;

    if(!readDentry(string(path), &dentry))
        return -ENOENT;

    printDentry(dentry);

    return 0;
}
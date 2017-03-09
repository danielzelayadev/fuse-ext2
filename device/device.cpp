#include "device.h"
#include <stdlib.h>
#include <stdio.h>

static const char *path;
static FILE *f;

int openDevice(const char* path) {
    path = path;
    f = fopen(path, "r+");
    return f;
}

int readBlock(int blockNo, char* buffer) {
    fseek(f, blockNo*blockSize, SEEK_SET);
    return (fread(buffer, 1, blockSize, f) == blockSize);
}

int writeBlock(int blockNo, char* buffer) {
    fseek(f, blockNo*blockSize, SEEK_SET);
    return (fwrite(buffer, 1, blockSize, f) == blockSize);
}

void closeDevice() {
    fclose(f);
}
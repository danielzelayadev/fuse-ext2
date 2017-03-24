#include "device.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *path;
static FILE *f;
unsigned int blockSize = 1024;

int openDevice(const char* path) {
    path = path;
    f = fopen(path, "r+");
    return f != 0;
}

int readBlock(int blockNo, char* buffer) {
    fseek(f, blockNo*blockSize, SEEK_SET);
    return (fread(buffer, 1, blockSize, f) == blockSize);
}

int read(int pos, void* buffer, int size) {
    char b[size];
    fseek(f, pos, SEEK_SET);
    int res = fread(b, 1, size, f);

    if (res != size) 
        return 0;

    memcpy(buffer, b, size);

    return 1;
}

int write(int pos, void* buffer, int size) {
    fseek(f, pos, SEEK_SET);
    return fwrite(buffer, 1, size, f) == size;
}

int writeBlock(int blockNo, char* buffer) {
    fseek(f, blockNo*blockSize, SEEK_SET);
    return (fwrite(buffer, 1, blockSize, f) == blockSize);
}

void closeDevice() {
    fclose(f);
}
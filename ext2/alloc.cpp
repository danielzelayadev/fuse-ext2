#include "alloc.h"
#include "bitmap.h"
#include "super.h"

#include <stdio.h>

int getFirstFreeBlock() {
    for (int i = 3; i < blockCount; i++)
        if (bitIsOff(i, BLOCK_BITMAP))
            return i;
    return -1;
}

// TODO: Update Metadata
int allocBlock() {
    int firstFreeBlock = getFirstFreeBlock();

    if (firstFreeBlock == -1) {
        printf("allocBlock: Out of space.\n");
        return -1;
    }

    return turnOnBit(firstFreeBlock, BLOCK_BITMAP) ? firstFreeBlock : -1;
}
#include "bitmap.h"
#include "inode.h"
#include "groupdesc.h"
#include "utils.h"
#include "../device/device.h"
#include <stdio.h>

int loadBitmap(int block, char* bitmap) {
    if (block == -1)
        return 0;

    if (!readBlock(block, bitmap)) {
        printf("loadBitmap -- Failed to read block %d\n", 
            block);
        return 0;
    }

    return 1;
}

int writeBitmap(int block, char* bitmap) {
    if (block == -1)
        return 0;

    if (!writeBlock(block, bitmap)) {
        printf("writeBitmap -- Failed to write block %d\n", 
            block);
        return 0;
    }

    return 1;
}

int getBitmapBlock(int blockGroup, int type) {
    Ext2GroupDescriptor gd;

    if (!readGroupDesc(blockGroup, &gd)) {
        printf("getBitmapBlock -- Failed to read group desc of group %d\n", 
            blockGroup);
        return 0;
    }

    return type  == BLOCK_BITMAP ? gd.bg_block_bitmap :
           (type == INODE_BITMAP ? gd.bg_inode_bitmap : -1);
}

int loadBitmap(int blockGroup, char* bitmap, int type) {
    return loadBitmap(getBitmapBlock(blockGroup, type), bitmap);
}

int loadBitmapOfBit(int bit, char* bmp, int type) {
    if (bit < 0) {
        printf("loadBitmapOfBit: Invalid bit %d.\n", bit);
        return 0;
    }

    return loadBitmap(BIT_BLOCK_GROUP(bit, type), bmp, type);
}

int writeBitmap(int blockGroup, char* bitmap, int type) {
    return writeBitmap(getBitmapBlock(blockGroup, type), bitmap);
}

int writeBitmapOfBit(int bit, char* bmp, int type) {
    if (bit < 0) {
        printf("writeBitmapOfBit: Invalid bit %d.\n", bit);
        return 0;
    }

    return writeBitmap(BIT_BLOCK_GROUP(bit, type), bmp, type);
}

int turnOnBit(int bit, int type) {
    char bmp[blockSize];

    if (!loadBitmapOfBit(bit, bmp, type)) {
        printf("turnOnBit: Failed to load bitmap of bit %d\n", bit);
        return 0;
    }

    turnOnBit(&bmp[BYTE_POS(bit, type)], BIT_POS(bit));

    return writeBitmapOfBit(bit, bmp, type);
}

int turnOffBit(int bit, int type) {
    char bmp[blockSize];

    if (!loadBitmapOfBit(bit, bmp, type)) {
        printf("turnOffBit: Failed to load bitmap of bit %d\n", bit);
        return 0;
    }

    turnOffBit(&bmp[BYTE_POS(bit, type)], BIT_POS(bit));

    return writeBitmapOfBit(bit, bmp, type);
}

int bitIsOn(int bit, int type) {
    char bmp[blockSize];

    if (!loadBitmapOfBit(bit, bmp, type)) {
        printf("bitIsOn: Failed to load bitmap of bit %d\n", bit);
        return 0;
    }

    return bitIsOn(bmp[BYTE_POS(bit, type)], BIT_POS(bit));
}

int bitIsOff(int bit, int type) {
    char bmp[blockSize];

    if (!loadBitmapOfBit(bit, bmp, type)) {
        printf("bitIsOff: Failed to load bitmap of bit %d\n", bit);
        return 0;
    }

    return bitIsOff(bmp[BYTE_POS(bit, type)], BIT_POS(bit));
}
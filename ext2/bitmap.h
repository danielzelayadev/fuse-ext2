#ifndef BITMAP_H
#define BITMAP_H

#define BLOCK_BITMAP 0
#define INODE_BITMAP 1
#define BITS_IN_BMP (blockSize * 8)
#define BIT_BLOCK_GROUP(bit) (bit / BITS_IN_BMP)
#define BIT_POS(bit) (bit % 8)
#define BYTE_POS(bit) ((bit / 8) % blockSize)

int turnOnBit(int bit, int type);
int turnOffBit(int bit, int type);
int bitIsOn(int bit, int type);
int bitIsOff(int bit, int type);

#endif
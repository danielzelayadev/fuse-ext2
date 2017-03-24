#ifndef BITMAP_H
#define BITMAP_H

#define BLOCK_BITMAP 0
#define INODE_BITMAP 1
#define BITS_IN_BMP (blockSize * 8)
#define BIT_POS(num) (num % 8)
#define BYTE_POS(num) (num % blockSize)

int turnOnBit(int bit, int type);
int turnOffBit(int bit, int type);
int bitIsOn(int bit, int type);
int bitIsOff(int bit, int type);

#endif
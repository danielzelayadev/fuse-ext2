#ifndef SUPER_H
#define SUPER_H

#include "types.h"

#define SB_SIZE sizeof(Ext2SuperBlock)

extern int blockCount;

int readSuperBlock(Ext2SuperBlock* sb);

#endif
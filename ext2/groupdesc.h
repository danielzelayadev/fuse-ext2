#ifndef GROUPDESC_H
#define GROUPDESC_H

#include "types.h"

#define GD_SIZE sizeof(Ext2GroupDescriptor)
#define GDT_BASE GDT_BLOCK * blockSize
#define GDT_BLOCK 2

int readGroupDesc(int num, Ext2GroupDescriptor* gd);
int writeGroupDesc(int num, Ext2GroupDescriptor* gd);
void printGroupDesc(Ext2GroupDescriptor gd);

#endif
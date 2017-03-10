#ifndef INODE_H
#define INODE_H

#include "types.h"
#include <string>

using namespace std;

#define INODE_SIZE sizeof(Ext2Inode)
#define ROOT_DIR_INODE 1

extern unsigned int inodesPerBlock, itableBlockCount;

int getInodeByPath(string path, Ext2Inode* inode);
int readInode(int inodeNo, Ext2Inode* inode);
int getBlockGroupOfInode(int inodeNo);
int getInodeIndexInGroup(int inodeNo);
int getIndexOfInodeBlockInGroup(int indexInGroup);
int getIndexOfInodeInBlock(int indexInGroup);
int calcInodePositionInDevice(Ext2GroupDescriptor gd, int blockIndexInGroup, int inodeIndexInBlock);
int calcInodeBlockInDevice(Ext2GroupDescriptor gd, int blockIndexInGroup);
void printInode(Ext2Inode inode);

#endif
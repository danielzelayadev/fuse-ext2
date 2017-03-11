#ifndef INODE_H
#define INODE_H

#include "types.h"
#include <string>

using namespace std;

#define INODE_SIZE sizeof(Ext2Inode)
#define ROOT_DIR_INODE 2

#define IS_SI block < BLOCK_POINTERS_IN_BLOCK
#define IS_DI block < BLOCK_POINTERS_IN_BLOCK * BLOCK_POINTERS_IN_BLOCK
#define IS_TI block < BLOCK_POINTERS_IN_BLOCK * BLOCK_POINTERS_IN_BLOCK * BLOCK_POINTERS_IN_BLOCK

extern unsigned int inodesPerBlock, itableBlockCount, 
                    BLOCK_POINTERS_IN_BLOCK;

int getInodeByPath(string path, Ext2Inode* inode);
int readInode(int inodeNo, Ext2Inode* inode);
int getBlockGroupOfInode(int inodeNo);
int getInodeIndexInGroup(int inodeNo);
int getIndexOfInodeBlockInGroup(int indexInGroup);
int getIndexOfInodeInBlock(int indexInGroup);
int calcInodePositionInDevice(Ext2GroupDescriptor gd, int inodeNo);
int readInodeBlock(Ext2Inode inode, int block, char* buff);
int readInodeSIBlock(Ext2Inode inode, int block, char* buff);
int readInodeDIBlock(Ext2Inode inode, int block, char* buff);
int readInodeTIBlock(Ext2Inode inode, int block, char* buff);
void printInode(Ext2Inode inode);

#endif
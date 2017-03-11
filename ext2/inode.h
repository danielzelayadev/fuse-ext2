#ifndef INODE_H
#define INODE_H

#include "types.h"
#include <string>

using namespace std;

#define INODE_SIZE sizeof(Ext2Inode)
#define ROOT_DIR_INODE 2

#define SI_BLOCKS BLOCK_POINTERS_IN_BLOCK
#define DI_BLOCKS (SI_BLOCKS * SI_BLOCKS)
#define TI_BLOCKS (DI_BLOCKS * SI_BLOCKS)
#define SI_UL (SI_BLOCKS + EXT2_NDIR_BLOCKS)
#define DI_UL (DI_BLOCKS + SI_UL)
#define TI_UL (TI_BLOCKS + DI_UL)
#define IS_SI (block >= EXT2_NDIR_BLOCKS && block < SI_UL)
#define IS_DI (block >= SI_UL && block < DI_UL)
#define IS_TI (block >= DI_UL && block < TI_UL)

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
int readSIBlock(int siBlockNo, int block, char* buff);
int readDIBlock(int diBlockNo, int siPointer, int block, char* buff);
int readTIBlock(int tiBlockNo, int diPointer, int siPointer, int block, char* buff);
void printInode(Ext2Inode inode);
void breakDownBlockNo(int block, int* ti, int* di, int* si, int* blk);

#endif
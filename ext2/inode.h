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

#define I_BLOCKS(inode, blockSize) (inode.i_size / blockSize)

extern unsigned int inodesPerBlock, inodesPerGroup, itableBlockCount, 
                    inodeCount, BLOCK_POINTERS_IN_BLOCK;

int allocInodeBlock(int inodeNo, Ext2Inode* inode);
int writeInode(int inodeNo, Ext2Inode inode);
int writeInodeBlock(Ext2Inode inode, int block, char* buff);
int writeInodeBlock(int inodeNo, Ext2Inode inode, void* buff, int pos, int size);
int getInodeByPath(string path, Ext2Inode* inode);
int getInodeIndexByPath(string path);
int readInode(int inodeNo, Ext2Inode* inode);
int readInodeBlock(Ext2Inode inode, int block, char* buff);
int readInodeBlock(Ext2Inode inode, void* buff, int pos, int size);
void fillInodeStatBuff(Ext2Inode inode, struct stat* statbuf);
void fillInodeStatBuff(int inodeNo, struct stat* statbuf);
void printInode(Ext2Inode inode);

#endif
#ifndef DENTRY_H
#define DENTRY_H

#include "types.h"
#include <string>

using namespace std;

#define DENTRY_SIZE sizeof(Ext2Dentry)

int newDentry(string path, mode_t mode, int inodeNo = 0);
int getParentDentry(string path, Ext2Dentry* dentry);
int readDentry(string path, Ext2Dentry* dentry);
int readDentry(Ext2Inode dirInode, string filename, Ext2Dentry* dentry);
int readDentry(Ext2Inode dirInode, int offset, Ext2Dentry* dentry);
string getPrintableDentryName(Ext2Dentry dentry);
void printDentry(Ext2Dentry dentry);

#endif
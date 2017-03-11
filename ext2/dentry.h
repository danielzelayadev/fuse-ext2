#ifndef DENTRY_H
#define DENTRY_H

#include "types.h"
#include <string>

using namespace std;

#define DENTRY_SIZE sizeof(Ext2Dentry)

int readDentry(int directoryInodeNo, string filename, Ext2Dentry* dentry);
void printDentry(Ext2Dentry dentry);

#endif
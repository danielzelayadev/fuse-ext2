#include "dentry.h"
#include "inode.h"
#include "../device/device.h"
#include <string.h>
#include <iostream>

int readDentry(Ext2Inode dirInode, string filename, Ext2Dentry* dentry) {
    int offset = 0;
    
    while(readDentry(dirInode, offset, dentry)) {
        if (dentry->name == filename)
            return 1;
        offset += dentry->rec_len;
    }
    
    return 0;
}

int readDentry(Ext2Inode dirInode, int offset, Ext2Dentry* dentry) {
    if (offset < 0 || offset >= dirInode.i_size) return 0;

    char block[blockSize];
    int inodeBlock  = offset / blockSize,
        blockOffset = offset % blockSize;

    if (!readInodeBlock(dirInode, inodeBlock, block))
        return 0;

    memcpy(dentry, &block[blockOffset], DENTRY_SIZE);

    return 1;
}

void getPrintableDentryName(Ext2Dentry dentry, char* n) {
    memcpy(n, dentry.name, dentry.name_len);
    n[dentry.name_len] = 0;
}

void printDentry(Ext2Dentry dentry) {
    char name[dentry.name_len];
    getPrintableDentryName(dentry, name);
    
    cout << "\nPrinting Dentry: \n";
    cout << "Inode: " << dentry.inode << endl;
    cout << "Dentry Length: " << dentry.rec_len << endl;
    cout << "Name Length: " << (int)dentry.name_len << endl;
    cout << "Type: " << (int)dentry.type << endl;
    cout << "Name: " << name << endl << endl;
}
#include "dentry.h"
#include "inode.h"
#include "utils.h"
#include "../device/device.h"
#include <string.h>
#include <iostream>
#include <vector>

int readDentry(string path, Ext2Dentry* dentry) {
    vector<string> filenames;
    int currDirInodeNo = ROOT_DIR_INODE;

    split(path, '/', filenames);

    for (int i = 0; i < filenames.size(); i++) {
        Ext2Inode dirInode;

        if (!readInode(currDirInodeNo, &dirInode) || 
            !readDentry(dirInode, filenames[i], dentry)) {
                cout << "File not found: " << filenames[i] << endl;
                return 0;
            }

        currDirInodeNo = dentry->inode;
    }

    return 1;
}

int readDentry(Ext2Inode dirInode, string filename, Ext2Dentry* dentry) {
    int offset = 0;
    
    while(readDentry(dirInode, offset, dentry)) {
        if (getPrintableDentryName(*dentry) == filename)
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

string getPrintableDentryName(Ext2Dentry dentry) {
    char name[dentry.name_len];
    memcpy(name, dentry.name, dentry.name_len);
    name[dentry.name_len] = 0;
    return string(name);
}

void printDentry(Ext2Dentry dentry) {
    cout << "\nPrinting Dentry: \n";
    cout << "Inode: " << dentry.inode << endl;
    cout << "Dentry Length: " << dentry.rec_len << endl;
    cout << "Name Length: " << (int)dentry.name_len << endl;
    cout << "Type: " << (int)dentry.type << endl;
    cout << "Name: " << getPrintableDentryName(dentry) << endl << endl;
}
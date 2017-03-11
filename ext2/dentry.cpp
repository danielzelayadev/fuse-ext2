#include "dentry.h"
#include "inode.h"
#include "../device/device.h"
#include <string.h>
#include <iostream>

int readDentry(Ext2Inode dirInode, string filename, Ext2Dentry* dentry) {
    char block[blockSize];
    memset(block, '\0', blockSize);
    int currBlockIndex = 0;

    while ((currBlockIndex * blockSize) < dirInode.i_size) {
        if (!readInodeBlock(dirInode, currBlockIndex, block)) {
            printf("Read Inode Block Failed...\n");
            return 0;
        }

        int i = 0;

        while (i < blockSize) {
            Ext2Dentry* fileDentry = (Ext2Dentry*)(&block[i]);

            if (fileDentry->name == filename) {
                memcpy(dentry, fileDentry, DENTRY_SIZE);
                return 1;
            }

            i += fileDentry->rec_len;
        }

        currBlockIndex++;
    }
    
    return 0;
}

void printDentry(Ext2Dentry dentry) {
    cout << "\nPrinting Dentry: \n";
    cout << "Inode: " << dentry.inode << endl;
    cout << "Dentry Length: " << dentry.rec_len << endl;
    cout << "Name Length: " << dentry.name_len << endl;
    cout << "Name: " << dentry.name << endl << endl;
}
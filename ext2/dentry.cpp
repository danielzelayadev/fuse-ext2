#include "dentry.h"
#include "inode.h"
#include "../device/device.h"
#include <string.h>
#include <iostream>

int readDentry(int directoryInodeNo, string filename, Ext2Dentry* dentry) {
    Ext2Inode currInode;

    if (!readInode(directoryInodeNo, &currInode)) {
        printf("ReadInode Failed...\n");
        return 0;
    }

    printInode(currInode);

    char* block = new char[blockSize];
    int currBlockIndex = 0;

    while (currBlockIndex < EXT2_NDIR_BLOCKS && currBlockIndex < currInode.i_blocks) {
        if (!currInode.i_block[currBlockIndex])
            break;

        if (!readBlock(currInode.i_block[currBlockIndex], block)) {
            printf("Read Block Failed...\n");
            return 0;
        }

        int i = 0;

        while (i < blockSize) {
            Ext2Dentry* fileDentry = (Ext2Dentry*)block;
            // printDentry(*fileDentry);

            if (fileDentry->name == filename) {
                memcpy(dentry, fileDentry, DENTRY_SIZE);
                return 1;
            }

            block += fileDentry->rec_len;
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
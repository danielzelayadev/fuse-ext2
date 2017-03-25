#include "dentry.h"
#include "inode.h"
#include "utils.h"
#include "alloc.h"
#include "../device/device.h"
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

unsigned char getDentryType(mode_t mode) {
    if (S_ISBLK(mode))
        return 4;
    if (S_ISCHR(mode))
        return 3;
    if (S_ISDIR(mode))
        return 2;
    if (S_ISFIFO(mode))
        return 5;
    if (S_ISREG(mode))
        return 1;
    if (S_ISLNK(mode))
        return 7;
    return 0;
}

void setRecLen(Ext2Dentry* dentry) {
    dentry->rec_len = roundUp(8 + dentry->name_len, 4);
}

int getParentDentry(string path, Ext2Dentry* dentry) {
    vector<string> filenames;
    int currDirInodeNo = ROOT_DIR_INODE;

    split(path, '/', filenames);

    if (filenames.size() == 1)
        return readDentry("/.", dentry);

    for (int i = 0; i < filenames.size() - 1; i++) {
        Ext2Inode dirInode;

        if (!readInode(currDirInodeNo, &dirInode) || 
            !readDentry(dirInode, filenames[i], dentry)) {
                cout << "File not found: " << filenames[i] << endl;
                return 0;
            }

        currDirInodeNo = dentry->inode;
    }
}

int newDentry(string path, mode_t mode, int inodeNo) {
    vector<string> filenames;
    int currDirInodeNo = ROOT_DIR_INODE;

    split(path, '/', filenames);

    if (filenames[filenames.size() - 1].size() > EXT2_NAME_LEN) {
        printf("newDentry: Name '%s' too long.\n", filenames[filenames.size() - 1].c_str());
        return 0;
    }

    for (int i = 0; i < filenames.size() - 1; i++) {
        Ext2Inode dirInode;
        Ext2Dentry dentry;

        if (!readInode(currDirInodeNo, &dirInode) || 
            !readDentry(dirInode, filenames[i], &dentry)) {
                cout << "File not found: " << filenames[i] << endl;
                return 0;
            }

        currDirInodeNo = dentry.inode;
    }

    Ext2Inode parentInode;

    if (!readInode(currDirInodeNo, &parentInode)) {
        printf("newDentry: Could not read parent inode.\n");
        return 0;
    }

    if (!S_ISDIR(parentInode.i_mode)) {
        printf("newDentry: Cannot create a child of a nondirectory.\n");
        return 0;
    }

    printInode(parentInode);

    Ext2Dentry dentry;
    dentry.inode = inodeNo ? inodeNo : allocInode();
    dentry.name_len = filenames[filenames.size() - 1].size();
    dentry.type = getDentryType(mode);
    strcpy(dentry.name, filenames[filenames.size() - 1].c_str());
    setRecLen(&dentry);

    printDentry(dentry);

    int lastBlockIndex = I_BLOCKS(parentInode, blockSize) - 1;

    char block[blockSize];

    if (!readInodeBlock(parentInode, lastBlockIndex, block)) {
        printf("newDentry: Could not read inode block.\n");
        return 0;
    }

    if (inodeNo == currDirInodeNo) {
        int sz = dentry.rec_len;
        dentry.rec_len = blockSize;
        return writeInodeBlock(inodeNo, parentInode, &dentry, 0, sz);
    }

    int o = 0;
    Ext2Dentry d;

    while (o < blockSize) {
        memcpy(&d, block + o, DENTRY_SIZE);

        printDentry(d);

        o += d.rec_len;
    }

    uint16_t rl = d.rec_len;
    uint16_t ol = dentry.rec_len;
    setRecLen(&d);
    uint16_t remaining = rl - d.rec_len;

    if (remaining < dentry.rec_len) {
        if (allocInodeBlock(currDirInodeNo, &parentInode) == -1) {
            printf("newDentry: Could not alloc block for inode.\n");
            return 0;
        }
        lastBlockIndex++;
        dentry.rec_len = blockSize;
    } else {
        dentry.rec_len = remaining;
        if (!writeInodeBlock(currDirInodeNo, parentInode, &d, (lastBlockIndex*blockSize) + (blockSize - rl), d.rec_len)) {
            printf("newDentry: Could not update previous dentry.\n");
            return 0;
        } 
    }

    if (!writeInodeBlock(currDirInodeNo, parentInode, &dentry, (lastBlockIndex*blockSize) + (blockSize - dentry.rec_len), ol)) {
        printf("newDentry: Could not write new dentry.\n");
        return 0;
    }

    return 1;
}   

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
#include "groupdesc.h"
#include "../device/device.h"
#include <iostream>

using namespace std;

int readGroupDesc(int num, Ext2GroupDescriptor* gd) {
    int pos = (GDT_BASE) + (GD_SIZE * num);
    return read(pos, (void*)gd, GD_SIZE);
}

void printGroupDesc(Ext2GroupDescriptor gd) {
    cout << "Block Bitmap Block: " << gd.bg_block_bitmap << endl;
    cout << "Inode Bitmap Block: " << gd.bg_inode_bitmap << endl;
    cout << "Inode Table Block: " << gd.bg_inode_table << endl;
    cout << "Free Block Count: " << gd.bg_free_blocks_count << endl;
    cout << "Free Inode Count: " << gd.bg_free_inodes_count << endl;
    cout << "Used Dirs Count: " << gd.bg_used_dirs_count << endl;
    cout << "Pad: " << gd.bg_pad << endl;
    cout << "Reserved:\n";
    for (int i = 0; i < 3; i++)
        cout << "\t" << i + 1 << ") " << gd.bg_reserved[i] << endl;
}
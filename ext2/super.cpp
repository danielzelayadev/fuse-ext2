#include "super.h"
#include "../device/device.h"
#include <string.h>

#include <iostream>

using namespace std;

int blockCount;

int readSuperBlock(Ext2SuperBlock* sb) {
    char* buffer = new char[SB_SIZE];

    if (!readBlock(1, buffer)) return 0;

    mempcpy(sb, buffer, SB_SIZE);
    return 1;
}

int writeSuperBlock(Ext2SuperBlock* sb) {
    return write(blockSize, (void*)sb, SB_SIZE);
}

void printSuperBlock(Ext2SuperBlock sb) {
    cout << "\nSuper Block:\n\n";
    cout << "Inode Count: " << sb.s_inodes_count << endl;
    cout << "Block Count: " << sb.s_blocks_count << endl;
    cout << "Reserved Block Count: " << sb.s_r_blocks_count << endl;
    cout << "Free Block Count: " << sb.s_free_blocks_count << endl;
    cout << "Free Inode Count: " << sb.s_free_inodes_count << endl;
    cout << "First Data Block: " << sb.s_first_data_block << endl;
    cout << "Log Block Size: " << sb.s_log_block_size << endl;
    cout << "Log Fragment Size: " << sb.s_log_frag_size << endl;
    cout << "Blocks per Group: " << sb.s_blocks_per_group << endl;
    cout << "Fragments per Group: " << sb.s_frags_per_group << endl;
    cout << "Inodes per Group: " << sb.s_inodes_per_group << endl;
    cout << "Mount Time: " << sb.s_mtime << endl;
    cout << "Write Time: " << sb.s_wtime << endl;
    cout << "Mount Count: " << sb.s_mnt_count << endl;
    cout << "Max Mount Count: " << sb.s_max_mnt_count << endl;
    cout << "Magic Signature: " << sb.s_magic << endl;
    cout << "FS State: " << sb.s_state << endl;
    cout << "Errors: " << sb.s_errors << endl;
    cout << "Minor Revision Level: " << sb.s_minor_rev_level << endl;
    cout << "Time of Last Check: " << sb.s_lastcheck << endl;
    cout << "Max Time Between Checks: " << sb.s_checkinterval << endl;
    cout << "OS: " << sb.s_creator_os << endl;
    cout << "Revision Level: " << sb.s_rev_level << endl;
    cout << "Default UID for Reserved Blocks: " << sb.s_def_resuid << endl;
    cout << "Default GID for Reserved Blocks: " << sb.s_def_resgid << endl;
    cout << "First Free Inode: " << sb.s_first_ino << endl;
    cout << "Inode Struct Size: " << sb.s_inode_size << endl;
    cout << "Block Group # of this SB: " << sb.s_block_group_nr << endl;
    cout << "Compatible Feature Set: " << sb.s_feature_compat << endl;
    cout << "Incompatible Feature Set: " << sb.s_feature_incompat << endl;
    cout << "Read-only Compatible Feature Set: " << sb.s_feature_ro_compat << endl << endl;
}
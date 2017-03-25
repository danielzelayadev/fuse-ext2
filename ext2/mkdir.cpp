#include "operations.h"
#include "dentry.h"
#include "inode.h"
#include "groupdesc.h"
#include <errno.h>
#include <stdio.h>
#include "utils.h"
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <string>

using namespace std;

int mkdir(const char* path, mode_t mode) throw() {
    printf("OPERATION: mkdir(path=%s, mode=%d)\n", path, (int)mode);

    Ext2Dentry dentry;

    if(!readDentry(string(path), &dentry)) {
        if (!newDentry(path, mode))
            return -EIO;

        int inodeNo = getInodeIndexByPath(path);

        Ext2Inode inode;
        Ext2Inode parent;
        Ext2Dentry parentDentry;

        getParentDentry(path, &parentDentry);
        readInode(parentDentry.inode, &parent);

        memset(&inode, 0, INODE_SIZE);

        inode.i_mode  = S_IFDIR | mode;
        inode.i_uid   = _getuid();
        inode.i_atime = time(0);
        inode.i_ctime = time(0);
        inode.i_mtime = time(0);
        inode.i_dtime = 0;
        inode.i_gid   = _getgid();
        inode.i_links_count = 0;
        inode.i_flags = 0;

        allocInodeBlock(inodeNo, &inode);


        string dot(path);
        dot += "/.";
        newDentry(dot, S_IFDIR | mode, inodeNo);
        string dotdot(path);
        dotdot += "/..";
        newDentry(dotdot, parent.i_mode, parentDentry.inode);

        int groupNo = (parentDentry.inode - 1) / inodesPerGroup;
        Ext2GroupDescriptor gd;
        readGroupDesc(groupNo, &gd);
        gd.bg_used_dirs_count++;
        writeGroupDesc(groupNo, &gd);
    } else
        printf("Dentry already exists.\n");

    return 0;
}
#include "ext2.h"

#include <fuse.h>
#include <stdio.h>

struct fuse_operations ext2_oper;

int main(int argc, char *argv[]) {
	int i, fuse_stat;

	ext2_oper.getattr = ext2_getattr;
	ext2_oper.readlink = ext2_readlink;
	ext2_oper.getdir = NULL;
	ext2_oper.mknod = ext2_mknod;
	ext2_oper.mkdir = ext2_mkdir;
	ext2_oper.unlink = ext2_unlink;
	ext2_oper.rmdir = ext2_rmdir;
	ext2_oper.symlink = ext2_symlink;
	ext2_oper.rename = ext2_rename;
	ext2_oper.link = ext2_link;
	ext2_oper.chmod = ext2_chmod;
	ext2_oper.chown = ext2_chown;
	ext2_oper.truncate = ext2_truncate;
	ext2_oper.utime = ext2_utime;
	ext2_oper.open = ext2_open;
	ext2_oper.read = ext2_read;
	ext2_oper.write = ext2_write;
	ext2_oper.statfs = ext2_statfs;
	ext2_oper.flush = ext2_flush;
	ext2_oper.release = ext2_release;
	ext2_oper.fsync = ext2_fsync;
	ext2_oper.opendir = ext2_opendir;
	ext2_oper.readdir = ext2_readdir;
	ext2_oper.releasedir = ext2_releasedir;
	ext2_oper.fsyncdir = ext2_fsyncdir;
	ext2_oper.init = ext2_init;

	printf("mounting file system...\n");
	
	for(i = 1; i < argc && (argv[i][0] == '-'); i++) {
		if(i == argc) {
			return (-1);
		}
	}

	if (!device_open(realpath(argv[i], NULL)) ) {
	    printf("Cannot open device file %s\n", argv[i]);
	    return 1;
	}

	for(; i < argc; i++) {
		argv[i] = argv[i+1];
	}
	argc--;

	fuse_stat = fuse_main(argc, argv, &ext2_oper, NULL);

    device_close();
    
	printf("fuse_main returned %d\n", fuse_stat);

	return fuse_stat;
}



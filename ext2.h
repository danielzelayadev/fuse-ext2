#ifndef ext2_h
#define ext2_h

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NAME_LEN		    6
#define MAX_SECTORS_PER_FILE	26
#define MAX_FILE_SIZE           (MAX_SECTORS_PER_FILE * SECTOR_SIZE)
#define MAX_MAP_ENTRIES         256
#define MAX_ROOT_DENTRIES       16

struct dir_entry {
    char name[MAX_NAME_LEN];
    unsigned char sectors[MAX_SECTORS_PER_FILE];
};

struct file_info {
    struct dir_entry *f_dentry;
    int f_size;
    unsigned char f_data[MAX_FILE_SIZE];
    int need_update;
};

int ext2_getattr(const char *path, struct stat *statbuf);
int ext2_readlink(const char *path, char *link, size_t size);
int ext2_mknod(const char *path, mode_t mode, dev_t dev);
int ext2_mkdir(const char *path, mode_t mode);
int ext2_unlink(const char *path);
int ext2_rmdir(const char *path);
int ext2_symlink(const char *path, const char *link);
int ext2_rename(const char *path, const char *newpath);
int ext2_link(const char *path, const char *newpath);
int ext2_chmod(const char *path, mode_t mode);
int ext2_chown(const char *path, uid_t uid, gid_t gid);
int ext2_truncate(const char *path, off_t newSize);
int ext2_utime(const char *path, struct utimbuf *ubuf);
int ext2_open(const char *path, struct fuse_file_info *fileInfo);
int ext2_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
int ext2_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
int ext2_statfs(const char *path, struct statvfs *statInfo);
int ext2_flush(const char *path, struct fuse_file_info *fileInfo);
int ext2_release(const char *path, struct fuse_file_info *fileInfo);
int ext2_fsync(const char *path, int datasync, struct fuse_file_info *fi);
int ext2_opendir(const char *path, struct fuse_file_info *fileInfo);
int ext2_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);
int ext2_releasedir(const char *path, struct fuse_file_info *fileInfo);
int ext2_fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo);
void ext2_init(struct fuse_conn_info *conn);

#ifdef __cplusplus
}
#endif

#endif //ext2_h


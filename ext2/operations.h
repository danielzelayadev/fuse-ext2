#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <fuse.h>

void* init(struct fuse_conn_info *conn);
int mkdir(const char* path, mode_t mode);
int mknod(const char* path, mode_t mode, dev_t rdev);
int truncate(const char* path, off_t size);
int ftruncate(const char* path, off_t size, fuse_file_info* fi);
int write(const char* path, const char *buf, size_t size, off_t offset, struct fuse_file_info* fi);
int statfs(const char* path, struct statvfs* stbuf);
int getattr(const char *path, struct stat *statbuf);
int readlink(const char *path, char *link, size_t size);
int open(const char *path, struct fuse_file_info *fileInfo);
int read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
int readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);

#endif
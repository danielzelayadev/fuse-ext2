#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <fuse.h>

void* init(struct fuse_conn_info *conn);
int statfs(const char* path, struct statvfs* stbuf);
int getattr(const char *path, struct stat *statbuf);
int readlink(const char *path, char *link, size_t size);
int open(const char *path, struct fuse_file_info *fileInfo);
int read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
int readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);

#endif
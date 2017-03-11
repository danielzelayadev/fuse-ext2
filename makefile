CFLAGS = -g -w -std=c++11  -DFUSE_USE_VERSION=26 `pkg-config fuse --cflags`
LINKFLAGS = -w -std=c++11 `pkg-config fuse --libs`
OBJS = obj/main.o obj/args.o obj/device.o obj/ext2.o obj/init.o obj/open.o
OBJS += obj/read.o obj/readdir.o obj/readlink.o obj/getattr.o obj/inode.o
OBJS += obj/super.o obj/groupdesc.o obj/blockgroup.o obj/utils.o obj/dentry.o

all: bin/ext2

clean:
	rm -rf bin obj

bin: 
	mkdir -p bin

bin/ext2: bin $(OBJS)
	g++ -g -o bin/ext2 obj/* $(LINKFLAGS)

obj:
	mkdir -p obj

obj/main.o: obj main.cpp
	g++ $(CFLAGS) -c main.cpp -o $@

obj/args.o: obj args.cpp
	g++ $(CFLAGS) -c args.cpp -o $@

obj/device.o: obj ./device/device.cpp
	g++ $(CFLAGS) -c ./device/device.cpp -o $@

obj/ext2.o: obj ./ext2/ext2.cpp
	g++ $(CFLAGS) -c ./ext2/ext2.cpp -o $@

obj/super.o: obj ./ext2/super.cpp
	g++ $(CFLAGS) -c ./ext2/super.cpp -o $@

obj/inode.o: obj ./ext2/inode.cpp
	g++ $(CFLAGS) -c ./ext2/inode.cpp -o $@

obj/groupdesc.o: obj ./ext2/groupdesc.cpp
	g++ $(CFLAGS) -c ./ext2/groupdesc.cpp -o $@

obj/blockgroup.o: obj ./ext2/blockgroup.cpp
	g++ $(CFLAGS) -c ./ext2/blockgroup.cpp -o $@

obj/dentry.o: obj ./ext2/dentry.cpp
	g++ $(CFLAGS) -c ./ext2/dentry.cpp -o $@

obj/utils.o: obj ./ext2/utils.cpp
	g++ $(CFLAGS) -c ./ext2/utils.cpp -o $@

obj/getattr.o: obj ./ext2/getattr.cpp
	g++ $(CFLAGS) -c ./ext2/getattr.cpp -o $@

obj/readlink.o: obj ./ext2/readlink.cpp
	g++ $(CFLAGS) -c ./ext2/readlink.cpp -o $@

obj/getdir.o: obj ./ext2/getdir.cpp
	g++ $(CFLAGS) -c ./ext2/getdir.cpp -o $@

obj/mknod.o: obj ./ext2/mknod.cpp
	g++ $(CFLAGS) -c ./ext2/mknod.cpp -o $@

obj/mkdir.o: obj ./ext2/mkdir.cpp
	g++ $(CFLAGS) -c ./ext2/mkdir.cpp -o $@

obj/unlink.o: obj ./ext2/unlink.cpp
	g++ $(CFLAGS) -c ./ext2/unlink.cpp -o $@

obj/rmdir.o: obj ./ext2/rmdir.cpp
	g++ $(CFLAGS) -c ./ext2/rmdir.cpp -o $@

obj/symlink.o: obj ./ext2/symlink.cpp
	g++ $(CFLAGS) -c ./ext2/symlink.cpp -o $@

obj/rename.o: obj ./ext2/rename.cpp
	g++ $(CFLAGS) -c ./ext2/rename.cpp -o $@

obj/link.o: obj ./ext2/link.cpp
	g++ $(CFLAGS) -c ./ext2/link.cpp -o $@

obj/chmod.o: obj ./ext2/chmod.cpp
	g++ $(CFLAGS) -c ./ext2/chmod.cpp -o $@

obj/chown.o: obj ./ext2/chown.cpp
	g++ $(CFLAGS) -c ./ext2/chown.cpp -o $@

obj/truncate.o: obj ./ext2/truncate.cpp
	g++ $(CFLAGS) -c ./ext2/truncate.cpp -o $@

obj/utime.o: obj ./ext2/utime.cpp
	g++ $(CFLAGS) -c ./ext2/utime.cpp -o $@

obj/open.o: obj ./ext2/open.cpp
	g++ $(CFLAGS) -c ./ext2/open.cpp -o $@

obj/read.o: obj ./ext2/read.cpp
	g++ $(CFLAGS) -c ./ext2/read.cpp -o $@

obj/write.o: obj ./ext2/write.cpp
	g++ $(CFLAGS) -c ./ext2/write.cpp -o $@

obj/statfs.o: obj ./ext2/statfs.cpp
	g++ $(CFLAGS) -c ./ext2/statfs.cpp -o $@

obj/flush.o: obj ./ext2/flush.cpp
	g++ $(CFLAGS) -c ./ext2/flush.cpp -o $@

obj/release.o: obj ./ext2/release.cpp
	g++ $(CFLAGS) -c ./ext2/release.cpp -o $@

obj/fsync.o: obj ./ext2/fsync.cpp
	g++ $(CFLAGS) -c ./ext2/fsync.cpp -o $@

obj/opendir.o: obj ./ext2/opendir.cpp
	g++ $(CFLAGS) -c ./ext2/opendir.cpp -o $@

obj/readdir.o: obj ./ext2/readdir.cpp
	g++ $(CFLAGS) -c ./ext2/readdir.cpp -o $@

obj/releasedir.o: obj ./ext2/releasedir.cpp
	g++ $(CFLAGS) -c ./ext2/releasedir.cpp -o $@

obj/fsyncdir.o: obj ./ext2/fsyncdir.cpp
	g++ $(CFLAGS) -c ./ext2/fsyncdir.cpp -o $@

obj/init.o: obj ./ext2/init.cpp
	g++ $(CFLAGS) -c ./ext2/init.cpp -o $@
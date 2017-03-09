
CFLAGS = -g -Wall -DFUSE_USE_VERSION=26 `pkg-config fuse --cflags`
LINKFLAGS = -Wall `pkg-config fuse --libs`

all: bin/ext2

clean:
	rm -rf bin obj

bin: 
	mkdir -p bin

bin/ext2: bin obj/main.o
	g++ -g -o bin/ext2 obj/* $(LINKFLAGS)

obj:
	mkdir -p obj

obj/main.o: obj main.c
	gcc -g $(CFLAGS) -c main.c -o $@

obj/getattr.o: obj ./ext2/getattr.c
	g++ -g $(CFLAGS) -c ./ext2/getattr.c -o $@

obj/readlink.o: obj ./ext2/readlink.c
	g++ -g $(CFLAGS) -c ./ext2/readlink.c -o $@

obj/getdir.o: obj ./ext2/getdir.c
	g++ -g $(CFLAGS) -c ./ext2/getdir.c -o $@

obj/mknod.o: obj ./ext2/mknod.c
	g++ -g $(CFLAGS) -c ./ext2/mknod.c -o $@

obj/mkdir.o: obj ./ext2/mkdir.c
	g++ -g $(CFLAGS) -c ./ext2/mkdir.c -o $@

obj/unlink.o: obj ./ext2/unlink.c
	g++ -g $(CFLAGS) -c ./ext2/unlink.c -o $@

obj/rmdir.o: obj ./ext2/rmdir.c
	g++ -g $(CFLAGS) -c ./ext2/rmdir.c -o $@

obj/symlink.o: obj ./ext2/symlink.c
	g++ -g $(CFLAGS) -c ./ext2/symlink.c -o $@

obj/rename.o: obj ./ext2/rename.c
	g++ -g $(CFLAGS) -c ./ext2/rename.c -o $@

obj/link.o: obj ./ext2/link.c
	g++ -g $(CFLAGS) -c ./ext2/link.c -o $@

obj/chmod.o: obj ./ext2/chmod.c
	g++ -g $(CFLAGS) -c ./ext2/chmod.c -o $@

obj/chown.o: obj ./ext2/chown.c
	g++ -g $(CFLAGS) -c ./ext2/chown.c -o $@

obj/truncate.o: obj ./ext2/truncate.c
	g++ -g $(CFLAGS) -c ./ext2/truncate.c -o $@

obj/utime.o: obj ./ext2/utime.c
	g++ -g $(CFLAGS) -c ./ext2/utime.c -o $@

obj/open.o: obj ./ext2/open.c
	g++ -g $(CFLAGS) -c ./ext2/open.c -o $@

obj/read.o: obj ./ext2/read.c
	g++ -g $(CFLAGS) -c ./ext2/read.c -o $@

obj/write.o: obj ./ext2/write.c
	g++ -g $(CFLAGS) -c ./ext2/write.c -o $@

obj/statfs.o: obj ./ext2/statfs.c
	g++ -g $(CFLAGS) -c ./ext2/statfs.c -o $@

obj/flush.o: obj ./ext2/flush.c
	g++ -g $(CFLAGS) -c ./ext2/flush.c -o $@

obj/release.o: obj ./ext2/release.c
	g++ -g $(CFLAGS) -c ./ext2/release.c -o $@

obj/fsync.o: obj ./ext2/fsync.c
	g++ -g $(CFLAGS) -c ./ext2/fsync.c -o $@

obj/opendir.o: obj ./ext2/opendir.c
	g++ -g $(CFLAGS) -c ./ext2/opendir.c -o $@

obj/readdir.o: obj ./ext2/readdir.c
	g++ -g $(CFLAGS) -c ./ext2/readdir.c -o $@

obj/releasedir.o: obj ./ext2/releasedir.c
	g++ -g $(CFLAGS) -c ./ext2/releasedir.c -o $@

obj/fsyncdir.o: obj ./ext2/fsyncdir.c
	g++ -g $(CFLAGS) -c ./ext2/fsyncdir.c -o $@

obj/init.o: obj ./ext2/init.c
	g++ -g $(CFLAGS) -c ./ext2/init.c -o $@
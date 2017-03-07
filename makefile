
CFLAGS = -g -Wall -DFUSE_USE_VERSION=26 `pkg-config fuse --cflags`
LINKFLAGS = -Wall `pkg-config fuse --libs`

all: bin/ext2

clean:
	rm -rf bin obj

bin: 
	mkdir -p bin

bin/ext2: bin obj/ext2.o obj/device.o obj/main.o
	g++ -g -o bin/ext2 obj/* $(LINKFLAGS)

obj:
	mkdir -p obj

obj/main.o: obj main.c ext2.h
	gcc -g $(CFLAGS) -c main.c -o $@

obj/ext2.o: obj ext2.c ext2.h 
	g++ -g $(CFLAGS) -c ext2.c -o $@

obj/device.o: obj device.c device.h
	g++ -g $(CFLAGS) -c device.c -o $@
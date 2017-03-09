#include <fuse.h>
#include <stdio.h>
#include "args.h"
#include "device/device.h"
#include "ext2/ext2.h"

struct fuse_operations ext2_ops = {

};

int main(int argc, char **argv) {
	int fuseStat, parseResult;

	printf("Parsing args...\n");

	parseResult = parse(argc, argv);

	if (parseResult == BAD_ARGS) {
		show_help(argv[0]);
		return 1;
	} else if (parseResult == PARSE_FAILED) {
		printf("Failed to parse args.\n");
		return 1;
	}

	printf("Opening device...\n");

	if (!openDevice(args.device)) {
		printf("Unable to open device.\n");
		return 1;
	}

	printf("Checking magic...\n");

	if(!verifyMagic()) {
		printf("Not an EXT2 volume.\n");
		return 1;
	}

	printf("Mounting file system...\n");

	fuseStat = 0;//fuse_main(argc, argv, &ext2_ops, 0);

    // device_close();
    
	// printf("fuse_main returned %d\n", fuseStat);

	return fuseStat;
}

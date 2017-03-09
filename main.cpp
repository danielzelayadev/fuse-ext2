#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include "args.h"
#include "device/device.h"
#include "ext2/ext2.h"

struct fuse_operations ext2_ops;

void initOps() {
	ext2_ops.init = init;
}

int main(int argc, char **argv) {
	int fuseStat;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	options.device = strdup("");
	options.showHelp = 0;

	printf("Parsing args...\n");

	if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
        return 1;

	if (options.showHelp) {
		show_help(argv[0]);
		return 0;
	}

	printf("Opening device...\n");

	if (!openDevice(options.device)) {
		printf("Unable to open device.\n");
		return 1;
	}

	printf("Checking magic...\n");

	if(!verifyMagic()) {
		printf("Not an EXT2 volume.\n");
		return 1;
	}

	printf("Mounting file system...\n");

	initOps();

	fuseStat = fuse_main(args.argc, args.argv, &ext2_ops, 0);

    closeDevice();
    
	printf("fuse_main returned %d\n", fuseStat);

	return fuseStat;
}

#include <fuse.h>
#include <stdio.h>
#include "args.h"

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

	printf("Mounting file system...\n");

	fuseStat = 0;//fuse_main(argc, argv, &ext2_ops, 0);

    // device_close();
    
	printf("fuse_main returned %d\n", fuseStat);

	return fuseStat;
}

#ifndef ARGS_H
#define ARGS_H

#include <fuse.h>
#include <stddef.h>

static struct e2opts {
	char* device = "";
	int showHelp;
} options;

#define OPTION(t, p)                           \
    { t, offsetof(struct e2opts, p), 1 }

const static struct fuse_opt option_spec[] = {
        OPTION("--device=%s", device),
        OPTION("-h", showHelp),
        OPTION("--help", showHelp),
        FUSE_OPT_END
};

void show_help(char *progname);

#endif
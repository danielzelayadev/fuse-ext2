#include "args.h"
#include <stdio.h>

void show_help(char *progname) {
    printf("usage: %s [options] <mountpoint>\n\n", progname);
    printf("File-system specific options:\n"
           "    --device=<s>    Path of the device to mount\n\n");
}
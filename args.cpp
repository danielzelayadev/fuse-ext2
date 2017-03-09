#include "args.h"
#include <stdio.h>
#include <string.h>

e2args args;

int skipFlags(int* i, int argc, char** argv) {
    for (; *i < argc && argv[*i][0] == '-'; (*i)++) 
        if (*i == (argc - 1)) return 0;
    return *i < argc;
}

int parse(int argc, char** argv) {
    if (argc < args.argMin)
        return BAD_ARGS;
    
    int i = 1;
    int lel = skipFlags(&i, argc, argv);
    printf("%d\n", lel);
    if (!lel)
        return BAD_ARGS;
    printf("%d\n", lel);
    args.device = strdup(argv[i++]);

    lel = skipFlags(&i, argc, argv);
    printf("%d\n", lel);
    if (!lel)
        return BAD_ARGS;
    printf("%d\n", lel);
    args.mnt = strdup(argv[i]);

    return 0;
}

void show_help(char *progname) {
    printf("usage: %s [options] <device> <mountpoint>\n\n", progname);
}
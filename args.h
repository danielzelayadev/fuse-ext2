#ifndef ARGS_H
#define ARGS_H

#define PARSE_FAILED -1
#define BAD_ARGS -2

struct e2args {
	char* device = "";
	char* mnt = "";
    int argMin = 3;
};

extern e2args args;

int parse(int argc, char** argv);
void show_help(char *progname);

#endif
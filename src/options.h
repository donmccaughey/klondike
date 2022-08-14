#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED


#include <stdbool.h>


struct options {
    char *csv_path;
    bool statistics;
};


struct options *
alloc_options(int argc, char *argv[]);

void
free_options(struct options *options);


#endif

#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED


#include <stdbool.h>


struct options {
    int address_limit;
    int email_limit;
    char *output;
    int phone_limit;
    bool statistics;
};


struct options *
alloc_options(int argc, char *argv[]);

void
free_options(struct options *options);


#endif

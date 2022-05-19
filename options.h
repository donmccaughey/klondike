#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED


struct options {
    char *csv_path;
};


struct options *
alloc_options(int argc, char *argv[]);

void
free_options(struct options *options);


#endif

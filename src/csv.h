#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED


#include <stdio.h>


struct csv {
    FILE *out;
    int column_count;
};


struct csv *
alloc_csv(FILE *out);

void
free_csv(struct csv *csv);

void
print_field(struct csv *csv, char const *field);

void
new_record(struct csv *csv);


#endif

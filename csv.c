#include "csv.h"

#include <stdbool.h>
#include <stdlib.h>
#include "error.h"


static bool
needs_quotes(char const *field)
{
    if ('\0' == field[0]) return true;
    for (int i = 0; field[i]; ++i) {
        switch (field[i]) {
            case '\n': return true;
            case '\r': return true;
            case ' ': if (0 == i || '\0' == field[i + 1]) return true; break;
            case '"': return true;
            case ',': return true;
        }
    }
    return false;
}


static void
print_quoted_field(struct csv *csv, char const *field)
{
    fprintf(csv->out, "\"");
    char const *ch = field;
    while (*ch) {
        fprintf(csv->out, "%C", *ch);
        if ('"' == *ch) fprintf(csv->out, "\"");
        ++ch;
    }
    fprintf(csv->out, "\"");
}


struct csv *
alloc_csv(FILE *out)
{
    struct csv *csv = calloc(1, sizeof(struct csv));
    if (!csv) halt_on_out_of_memory();
    csv->out = out;
    csv->column_count = 0;
    return csv;
}


void
free_csv(struct csv *csv)
{
    free(csv);
}


void
print_field(struct csv *csv, char const *field)
{
    if (csv->column_count++) fprintf(csv->out, ",");
    if (needs_quotes(field)) {
        print_quoted_field(csv, field);
    } else {
        fprintf(csv->out, "%s", field);
    }
}


void
new_record(struct csv *csv)
{
    fprintf(csv->out, "\r\n");
    csv->column_count = 0;
}

#include "options.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#include "error.h"
#include "memory.h"


static struct option long_options[] = {
    {
        .name="help",
        .has_arg=no_argument,
        .flag=NULL,
        .val='?',
    },
    {
        .name="csv-path",
        .has_arg=required_argument,
        .flag=NULL,
        .val='c',
    },
};

static char const short_options[] = "?c:h";


static noreturn void
print_usage_and_exit(void)
{
    FILE *out = stdout;
    fprintf(out, "Usage:\n");
    fprintf(out, "  klondike [--csv-path PATH]\n");
    fprintf(out, "\n");
    fprintf(out, "  -?, -h, --help        Show this help message\n");
    fprintf(out, "  -c, --csv-path PATH   Path to write contacts in CSV format\n");
    fprintf(out, "                        (defaults to standard output)\n");
    fprintf(out, "\n");
    exit(EXIT_FAILURE);
}


struct options *
alloc_options(int argc, char *argv[])
{
    struct options *options = calloc(1, sizeof(struct options));
    if (!options) halt_on_out_of_memory();
    
    int long_option_index;
    int ch;
    while (-1 != (ch = getopt_long(argc, argv, short_options, long_options, &long_option_index))) {
        switch (ch) {
            case 'c':
                options->csv_path = strdup_or_halt(optarg);
                break;
            default:
                print_usage_and_exit();
                break;
        }
    }
    
    return options;
}


void
free_options(struct options *options)
{
    if (!options) return;
    free(options->csv_path);
    free(options);
}

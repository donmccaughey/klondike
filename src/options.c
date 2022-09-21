#include "options.h"

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#include "error.h"
#include "memory.h"


#define STR_EQ(S1, S2) (0 == strcmp((S1), (S2)))


static int long_option_only = 0;

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
    {
        .name="emails-limit",
        .has_arg=required_argument,
        .flag=&long_option_only,
        .val=0,
    },
    {
        .name="phones-limit",
        .has_arg=required_argument,
        .flag=&long_option_only,
        .val=0,
    },
    {
        .name="statistics",
        .has_arg=no_argument,
        .flag=NULL,
        .val='s',
    },
    {
        // last option must be zero-filled
        .name=NULL,
    }
};

static char const short_options[] = "?c:hs";


static bool
is_valid_limit(long limit)
{
    if (!limit && errno) return false;
    if (limit < 0) return false;
    if (limit > INT_MAX) return false;
    return true;
}


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
    fprintf(out, "  -s, --statistics      Print information about the contacts\n");
    fprintf(out, "  --emails-limit N      Only write N emails\n");
    fprintf(out, "  --phones-limit N      Only write N phones\n");
    fprintf(out, "\n");
    exit(EXIT_FAILURE);
}


static long to_long(char const *s)
{
    errno = 0;
    return strtol(s, NULL, 10);
}


struct options *
alloc_options(int argc, char *argv[])
{
    struct options *options = calloc(1, sizeof(struct options));
    if (!options) halt_on_out_of_memory();
    
    options->emails_limit = INT_MAX;
    options->phones_limit = INT_MAX;
    
    int i;
    int ch;
    int error_count = 0;
    while (-1 != (ch = getopt_long(argc, argv, short_options, long_options, &i))) {
        switch (ch) {
            case 'c':
                options->csv_path = strdup_or_halt(optarg);
                break;
            case 's':
                options->statistics = true;
                break;
            case 0:
                if (STR_EQ("emails-limit", long_options[i].name)) {
                    long emails_limit = to_long(optarg);
                    if (is_valid_limit(emails_limit)) {
                        options->emails_limit = (int)emails_limit;
                    } else {
                        report_error("Invalid emails limit '%s'", optarg);
                        ++error_count;
                    }
                } else if (STR_EQ("phones-limit", long_options[i].name)) {
                    long phones_limit = to_long(optarg);
                    if (is_valid_limit(phones_limit)) {
                        options->phones_limit = (int)phones_limit;
                    } else {
                        report_error("Invalid phones limit '%s'", optarg);
                        ++error_count;
                    }
                }
                break;
            default:
                ++error_count;
                break;
        }
    }
    
    if (error_count) print_usage_and_exit();
    
    return options;
}


void
free_options(struct options *options)
{
    if (!options) return;
    free(options->csv_path);
    free(options);
}

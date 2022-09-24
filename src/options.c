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
        .name="address-limit",
        .has_arg=required_argument,
        .flag=&long_option_only,
        .val=0,
    },
    {
        .name="email-limit",
        .has_arg=required_argument,
        .flag=&long_option_only,
        .val=0,
    },
    {
        .name="output",
        .has_arg=required_argument,
        .flag=NULL,
        .val='o',
    },
    {
        .name="phone-limit",
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

static char const short_options[] = "?ho:s";


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
    fprintf(out, "  -?, -h, --help      Show this help message\n");
    fprintf(out, "  -o, --output PATH   Path of file to write contacts to\n");
    fprintf(out, "                      (defaults to the standard output)\n");
    fprintf(out, "  -s, --statistics    Print information about the contacts\n");
    fprintf(out, "\n");
    fprintf(out, "  --address-limit N   Only write N addresses\n");
    fprintf(out, "  --email-limit N     Only write N emails\n");
    fprintf(out, "  --phone-limit N     Only write N phones\n");
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
    
    options->address_limit = INT_MAX;
    options->email_limit = INT_MAX;
    options->phone_limit = INT_MAX;
    
    int i;
    int ch;
    int error_count = 0;
    while (-1 != (ch = getopt_long(argc, argv, short_options, long_options, &i))) {
        switch (ch) {
            case 'o':
                options->output = strdup_or_halt(optarg);
                break;
            case 's':
                options->statistics = true;
                break;
            case 0:
                if (STR_EQ("address-limit", long_options[i].name)) {
                    long address_limit = to_long(optarg);
                    if (is_valid_limit(address_limit)) {
                        options->address_limit = (int)address_limit;
                    } else {
                        report_error("Invalid address limit '%s'", optarg);
                        ++error_count;
                    }
                } else if (STR_EQ("email-limit", long_options[i].name)) {
                    long email_limit = to_long(optarg);
                    if (is_valid_limit(email_limit)) {
                        options->email_limit = (int)email_limit;
                    } else {
                        report_error("Invalid email limit '%s'", optarg);
                        ++error_count;
                    }
                } else if (STR_EQ("phone-limit", long_options[i].name)) {
                    long phone_limit = to_long(optarg);
                    if (is_valid_limit(phone_limit)) {
                        options->phone_limit = (int)phone_limit;
                    } else {
                        report_error("Invalid phone limit '%s'", optarg);
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
    free(options->output);
    free(options);
}

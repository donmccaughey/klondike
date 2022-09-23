#include "error.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char out_of_memory_error_message[] = "Out of memory";

static struct error out_of_memory_error = {
    .type=error_tyoe_klondike,
    .code=error_code_out_of_memory,
    .message=out_of_memory_error_message,
};


struct error *
alloc_error(enum error_type type, int code, char const *message)
{
    struct error *error = calloc(1, sizeof(struct error));
    if (!error) return &out_of_memory_error;
    
    error->message = strdup(message);
    if (!error->message) {
        free(error);
        return &out_of_memory_error;
    }
    
    error->type = type;
    error->code = code;
    return error;
}


struct error *
alloc_stdlib_error(void)
{
    return alloc_error(error_type_stdlib, errno, strerror(errno));
}


void
free_error(struct error *error)
{
    if (error && error != &out_of_memory_error) {
        free(error->message);
        free(error);
    }
}


noreturn void
halt_on_error(struct error *error)
{
    print_error(error);
    free_error(error);
    exit(EXIT_FAILURE);
}


noreturn void
halt_on_internal_error(char const *message)
{
    fprintf(stderr, "Internal Error: %s\n", message);
    exit(EXIT_FAILURE);
}


noreturn void
halt_on_out_of_memory(void)
{
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILURE);
}


void
print_error(struct error const *error)
{
    if (error_type_stdlib == error->type || error_type_foundation == error->type) {
        fprintf(stderr, "Error: (%i) %s\n", error->code, error->message);
    } else {
        fprintf(stderr, "Error: %s\n", error->message);
    }
}


void
report_error(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    vfprintf(stderr, format, arguments);
    fprintf(stderr, "\n");
    va_end(arguments);
}

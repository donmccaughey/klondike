#include "memory.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "error.h"


char *
strdup_or_halt(char const *s)
{
    if (!s) return NULL;
    char *dup = strdup(s);
    if (!dup) halt_on_out_of_memory();
    return dup;
}


char *
strf_or_halt(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    char *s;
    int chars_formatted = vasprintf(&s, format, arguments);
    if (chars_formatted < 0) {
        struct error *error = alloc_stdlib_error();
        halt_on_error(error);
    }
    va_end(arguments);
    return s;
}

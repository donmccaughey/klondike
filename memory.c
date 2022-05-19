#include "memory.h"

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

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED


#include <stdnoreturn.h>


enum error_type {
    error_type_unknown = 0,
    error_type_out_of_memory,
    error_type_stdlib,
    error_type_foundation,
    error_type_no_access,
};


struct error {
    enum error_type type;
    int code;
    char *message;
};


struct error *
alloc_error(enum error_type type, int code, char const *message);

struct error *
alloc_stdlib_error(void);

void
free_error(struct error *error);

noreturn void
halt_on_error(struct error *error);

noreturn void
halt_on_internal_error(char const *message);

noreturn void
halt_on_out_of_memory(void);

void
print_error(struct error const *error);


#endif

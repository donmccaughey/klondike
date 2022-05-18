#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED


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

void
free_error(struct error *error);

void
halt_on_error(struct error *error);

void
halt_on_internal_error(char const *message);

void
halt_on_out_of_memory(void);

void
print_error(struct error const *error);


#endif

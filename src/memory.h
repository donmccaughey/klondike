#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED


char *
strdup_or_halt(char const *s);

char *
strf_or_halt(char const *format, ...)
__attribute__((format (printf, 1, 2)));


#endif

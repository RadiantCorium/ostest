#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void *memmove(void *dstptr, const void *srcptr, size_t size);

size_t strlen(const char *str);

#endif
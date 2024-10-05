#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void *kmemmove(void *dstptr, const void *srcptr, size_t size);

size_t kstrlen(const char *str);
char *kstrcat(char *dest, const char *src);

#endif
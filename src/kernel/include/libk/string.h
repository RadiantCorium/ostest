#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void *kmemcpy(void *dest, const void *src, size_t n);
void *kmemset(void *s, int c, size_t n);
void *kmemmove(void *dstptr, const void *srcptr, size_t size);
int kmemcmp(const void *s1, const void *s2, size_t n);

size_t kstrlen(const char *str);
char *kstrcat(char *dest, const char *src);
char *kstrncpy(char *dest, const char *src, size_t n);

#endif
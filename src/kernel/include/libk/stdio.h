#ifndef STDIO_H
#define STDIO_H

#define EOF (-1)

int kprintf(const char *__restrict format, ...);
int kputchar(int c);
int kputs(const char *str);

#endif
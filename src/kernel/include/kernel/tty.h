#ifndef TTY_H
#define TTY_H

#include <stddef.h>
#include <stdint.h>

void term_setSerial(uint16_t port);

void term_init(void);
void term_putchar(char c);
void term_write(const char *data, size_t size);
void term_writestring(const char *data);
void term_setcolor(uint8_t color);
void term_setcursor(int x, int y);
void term_setpos(int x, int y);
void term_clear();

#endif
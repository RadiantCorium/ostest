#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/serial.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

uint16_t term_serialPort = 0;

size_t term_row;
size_t term_column;
uint8_t term_color;
uint16_t *term_buffer;

void term_init(void)
{
    term_row = 0;
    term_column = 0;
    term_color = vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
    term_buffer = (uint16_t *)0xB8000;

    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            term_buffer[index] = vga_entry(' ', term_color);
        }
    }
}

void term_setcolor(uint8_t color)
{
    term_color = color;
}

void term_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    term_buffer[index] = vga_entry(c, color);
}

void term_putchar(char c)
{
    int isSpecial = 0;
    if (c == '\n')
    {
        term_row++;
        term_column = 0;
        isSpecial = 1;
    }
    if (isSpecial == 0)
    {
        term_putentryat(c, term_color, term_column, term_row);
        term_column++;
    }
    if (term_serialPort != 0)
            writeSerial(term_serialPort, c);
    if (term_column == VGA_WIDTH)
    {
        term_column = 0;
        term_row++;
    }

    if (term_row >= VGA_HEIGHT) {
        memmove(term_buffer, term_buffer + (VGA_WIDTH), (VGA_HEIGHT) * VGA_WIDTH * 2);

        for (size_t x = 0; x < VGA_WIDTH; x++) {
            size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
            term_buffer[index] = ' ';
        }

        term_row--;
        term_column = 0;
    }
}

void term_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        term_putchar(data[i]);
}

void term_writestring(const char *data)
{
    term_write(data, strlen(data));
}

void term_setSerial(uint16_t port)
{
    term_serialPort = port;
}
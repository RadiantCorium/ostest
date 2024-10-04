#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

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
    if (c == '\n')
    {
        term_row++;
        term_column = 0;
    }
    term_putentryat(c, term_color, term_column, term_row);
    if (++term_column == VGA_WIDTH)
    {
        term_column = 0;
        term_row++;
    }

    if (term_row == VGA_HEIGHT)
    {
        // Shift everyhing up by one and discard the top row
        for (size_t i = 0; i < VGA_HEIGHT; i++)
        {
            for (size_t j = 0; j < VGA_WIDTH; j++)
            {
                const size_t index = i * VGA_WIDTH + j;
                const size_t indexDn = i+1 * VGA_WIDTH + j;
                term_buffer[index] = term_buffer[indexDn];
                if (i == VGA_HEIGHT)
                    term_buffer[index] = ' ';
            }
        }
        term_row = VGA_HEIGHT - 1;
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
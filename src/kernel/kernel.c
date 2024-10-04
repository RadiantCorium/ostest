#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../libc/string.h"

// Prevent stuff from working if we're not using a cross compiler.
#if defined(__linux__)
#error "You are not using a cross-compiler."
#endif

enum vga_color {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GRAY,
    VGA_COLOR_DARK_GRAY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_LIGHT_BROWN,
    VGA_COLOR_WHITE
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

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
    term_putentryat(c, term_color, term_column, term_row);
    if (++term_column == VGA_WIDTH)
    {
        term_column = 0;
        if (++term_row == VGA_HEIGHT)
            term_row = 0;
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

void kernel_main(void)
{
    // Initialize the terminal interface
    term_init();

    term_writestring("Hello world");
}
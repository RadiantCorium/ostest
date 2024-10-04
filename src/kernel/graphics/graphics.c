#include "graphics.h"
#include "font.h"
#include "kernel/io/serial.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct limine_framebuffer *framebuffer = NULL;
uint64_t pixelwidth = NULL;

// Function for changing the color of a pixel
 void putpixel(int x, int y, int color)
{
    volatile uint32_t *fb_ptr = framebuffer->address;
    unsigned where = x*pixelwidth + y*framebuffer->pitch;
    fb_ptr[where] = color;
}

 void fillrect(int x, int y, int w, int h, int color)
{
    unsigned char *where = framebuffer->address;
    int i, j;

    for (i = 0; i < w; i++)
    {
        for (j = 0; j < h; j++) {
            where[(i+x)*pixelwidth+((j+y)*framebuffer->pitch)] = color & 255;
            where[(i+x)*pixelwidth+((j+y)*framebuffer->pitch)+1] = (color >> 8) & 255;
            where[(i+x)*pixelwidth+((j+y)*framebuffer->pitch)+2] = (color >> 16) & 255;
        }
    }
}

void clearScreen(int color)
{
    fillrect(0, 0, framebuffer->width, framebuffer->height, color);
    
}

void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor)
{
    int cx,cy;
    int set;

    char *bmp = font8x8_basic[c];

    for (cx = 0; cx < 8; cx++)
    {
        for (cy = 0; cy < 8; cy++)
        {
            set = bmp[cx] & 1  << cy;
            putpixel(x + cx, y + cx, set ? fgcolor : bgcolor);
        }
    }
}

void drawstring(char* input) {
    
}
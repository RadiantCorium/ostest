#include "graphics.h"
#include "font.h"
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
    int mask[8]={1,2,4,8,16,32,64,128};
    unsigned char *glyph=builtin_font+(int)c*16;

    for (cy = 0; cy < 16; cy++)
    {
        for (cx = 0; cx < 8; cx++)
        {
            putpixel(x-cx, y+cy-12, glyph[cy]&mask[cx]?fgcolor:bgcolor);
        }
    }
}

void drawstring(char* input) {
    
}
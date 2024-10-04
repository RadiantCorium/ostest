#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <limine.h>

extern struct limine_framebuffer *framebuffer;
extern uint64_t pixelwidth;

void putpixel(int x, int y, int color);

void fillrect(int x, int y, int w, int h, int color);

void clearScreen(int color);

void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor);

#endif
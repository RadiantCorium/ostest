#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

#include"kernel/graphics/graphics.h"
#include"kernel/io/serial.h"

// Set the base revision to 2, the latest base revision described by the Limine boot protocol specification.
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Define start and end markers for the limine requests.
__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_maker")))
static volatile LIMINE_REQUESTS_END_MARKER;

// Halt and catch fire function
static void hcf(void) {
    for (;;) {
        asm("hlt");
    }
}

// Kernel entry point
void kmain(void) {
    initSerial();

    writeSerialString("Serial port initialized!\n");

    writeSerialString("Checking Limine base revision...");

    // Ensure the bootloader actually understands our base revision
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    writeSerialString("\t\tGood!\n");

    writeSerialString("Checking for framebuffer...");

    // Ensure we have a framebuffer
    if (framebuffer_request.response == NULL
    ||  framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    writeSerialString("\t\tGood!\n");

    writeSerialString("Initializing framebuffer...");

    // Fetch the first framebuffer
    //struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    framebuffer = framebuffer_request.response->framebuffers[0];

    pixelwidth = (framebuffer->bpp/8);

    writeSerialString("\t\tGood!\n");

    // clearScreen(0xAAAAAA);

    drawchar('A', 0, 0, 0xFFFFFF, 0x000000);

    // we're done. halt.
    //asm volatile ("1: jmp 1b");
    hcf();
}
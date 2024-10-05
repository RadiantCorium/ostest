#include <kernel/serial.h>
#include <kernel/io.h>

int initSerial(uint16_t port)
{
    outb(port + 1, 0x00); // Disable all interrupts
    outb(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00); //                  (hi byte)
    outb(port + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(port + 0, 0xAE); // Test serial chip (send byte 0xAE to check if serial chip returns the same byte)

    // Check if serial is faulty (i.e not same byte as sent)
    if (inb(port + 0) != 0xAE)
    {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (Not loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(port + 4, 0x0F);
    return 0;
}

int serialRecieved(uint16_t port)
{
    return inb(port + 5) & 1;
}

char readSerial(uint16_t port)
{
    while (serialRecieved(port) == 0);

    return inb(port);
}

int isTransmitEmpty(uint16_t port)
{
    return inb(port + 5) & 0x20;
}

void writeSerial(uint16_t port, char a)
{
    while (isTransmitEmpty(port) == 0);

    outb(port, a);
}
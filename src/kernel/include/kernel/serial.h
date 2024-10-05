#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

int initSerial(uint16_t port);

int serialRecieved(uint16_t port);
char readSerial(uint16_t port);

int isTransmitEmpty(uint16_t port);
void writeSerial(uint16_t port, char a);

#endif
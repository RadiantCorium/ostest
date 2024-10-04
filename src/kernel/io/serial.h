#ifndef SERIAL_H
#define SERIAL_H

#define PORT 0x3f8

int initSerial();

int signalRecieved();
char readSerial();

int isTransmitEmpty();
void writeSerial(char a);
void writeSerialString(char *str);

#endif
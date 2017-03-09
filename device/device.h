#ifndef DEVICE_H
#define DEVICE_H

extern int blockSize = 1024;

int openDevice(const char* path);
int readBlock(int blockNo, char* buffer);
int writeBlock(int blockNo, char* buffer);
void closeDevice();

#endif
#ifndef DEVICE_H
#define DEVICE_H

extern unsigned int blockSize;

int openDevice(const char* path);
int readBlock(int blockNo, char* buffer);
int writeBlock(int blockNo, char* buffer);
int read(int pos, void* buffer, int size);
int write(int pos, void* buffer, int size);
void closeDevice();

#endif
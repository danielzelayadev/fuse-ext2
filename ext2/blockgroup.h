#ifndef BLOCKGROUP_H
#define BLOCKGROUP_H

extern unsigned int groupCount, descriptorListSize;

int readBlockInGroup(int groupNo, int blockNo, char* buffer);
int readInGroup(int groupNo, int pos, void* buffer, int size);

#endif
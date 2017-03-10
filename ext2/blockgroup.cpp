#include "blockgroup.h"
#include "../device/device.h"

unsigned int groupCount, descriptorListSize;

int readBlockInGroup(int groupNo, int blockNo, char* buffer) {
    return 1;
}

int readInGroup(int groupNo, int pos, void* buffer, int size) {
    return 1;
}
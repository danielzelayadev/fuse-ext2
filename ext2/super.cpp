#include "super.h"
#include "../device/device.h"
#include <string.h>

int readSuperBlock(Ext2SuperBlock* sb) {
    char* buffer = new char[SB_SIZE];

    if (!readBlock(1, buffer)) return 0;

    mempcpy(sb, buffer, SB_SIZE);
    return 1;
}
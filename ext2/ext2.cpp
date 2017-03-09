#include "ext2.h"
#include "types.h"
#include "../device/device.h"

#include <stdio.h>
#include <string.h>

bool verifyMagic() {
    char* buffer = new char[blockSize];
    readBlock(1, buffer);

    Ext2SuperBlock sb;

    mempcpy(&sb, buffer, sizeof(Ext2SuperBlock));

    return sb.s_magic == MAGIC;
}
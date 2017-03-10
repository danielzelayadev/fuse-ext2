#include "groupdesc.h"
#include "../device/device.h"

int readGroupDesc(int num, Ext2GroupDescriptor* gd) {
    int pos = (GDT_BASE) + (GD_SIZE * num);
    return read(pos, (void*)gd, GD_SIZE);
}
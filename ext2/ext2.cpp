#include "ext2.h"
#include "types.h"
#include "super.h"

bool verifyMagic() {
    Ext2SuperBlock sb;
    
    if (!readSuperBlock(&sb))
        return false;

    return sb.s_magic == MAGIC;
}
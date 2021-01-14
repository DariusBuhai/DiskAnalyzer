#include <stdlib.h>

#include "shared.h"

LD bytes_to_xb(LL bytes, int x){
    LD xb = (LD)bytes;
    for(int i=0;i<x;++i)
        xb/=1024;
    return xb;
}




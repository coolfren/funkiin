#include "utils.h"

unsigned long createRGBA(int r, int g, int b, int a)
{   
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
           + (a & 0xff);
}

int countarray(int array[])
{
    for(int i = 0; i<sizeof(array); i++){
        if(array[i] = 0) return i - 1;
    }
}
#include "RSDK/Core/RetroEngine.hpp"
#include <math.h>

using namespace RSDK;

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

int32 sin1024LookupTable[0x400];
int32 cos1024LookupTable[0x400];
int32 tan1024LookupTable[0x400];
int32 asin1024LookupTable[0x400];
int32 acos1024LookupTable[0x400];

int32 sin512LookupTable[0x200];
int32 cos512LookupTable[0x200];
int32 tan512LookupTable[0x200];
int32 asin512LookupTable[0x200];
int32 acos512LookupTable[0x200];

int32 sin256LookupTable[0x100];
int32 cos256LookupTable[0x100];
int32 tan256LookupTable[0x100];
int32 asin256LookupTable[0x100];
int32 acos256LookupTable[0x100];

uint8 arcTan256LookupTable[0x100 * 0x100];

uint32 randSeed = 0;

void CalculateTrigAngles()
{
    srand((uint32)time(NULL));
    randSeed = rand();

    for (int i = 0; i < 0x400; ++i) {
        sin1024LookupTable[i]  = (int32)(sin((i / 512.0) * M_PI) * 1024.0);
        cos1024LookupTable[i]  = (int32)(cos((i / 512.0) * M_PI) * 1024.0);
        tan1024LookupTable[i]  = (int32)(tan((i / 512.0) * M_PI) * 1024.0);
        asin1024LookupTable[i] = (int32)((asin(i / 1023.0) * 512.0) / M_PI);
        acos1024LookupTable[i] = (int32)((acos(i / 1023.0) * 512.0) / M_PI);
    }

    cos1024LookupTable[0x000] = 0x400;
    cos1024LookupTable[0x100] = 0;
    cos1024LookupTable[0x200] = -0x400;
    cos1024LookupTable[0x300] = 0;

    sin1024LookupTable[0x000] = 0;
    sin1024LookupTable[0x100] = 0x400;
    sin1024LookupTable[0x200] = 0;
    sin1024LookupTable[0x300] = -0x400;

    for (int i = 0; i < 0x200; ++i) {
        sin512LookupTable[i]  = (int32)(sin((i / 256.0) * M_PI) * 512.0);
        cos512LookupTable[i]  = (int32)(cos((i / 256.0) * M_PI) * 512.0);
        tan512LookupTable[i]  = (int32)(tan((i / 256.0) * M_PI) * 512.0);
        asin512LookupTable[i] = (int32)((asin(i / 511.0) * 256.0) / M_PI);
        acos512LookupTable[i] = (int32)((acos(i / 511.0) * 256.0) / M_PI);
    }

    cos512LookupTable[0x00]  = 0x200;
    cos512LookupTable[0x80]  = 0;
    cos512LookupTable[0x100] = -0x200;
    cos512LookupTable[0x180] = 0;

    sin512LookupTable[0x00]  = 0;
    sin512LookupTable[0x80]  = 0x200;
    sin512LookupTable[0x100] = 0;
    sin512LookupTable[0x180] = -0x200;

    for (int i = 0; i < 0x100; i++) {
        sin256LookupTable[i]  = (int32)((sin512LookupTable[i * 2] >> 1));
        cos256LookupTable[i]  = (int32)((cos512LookupTable[i * 2] >> 1));
        tan256LookupTable[i]  = (int32)((tan512LookupTable[i * 2] >> 1));
        asin256LookupTable[i] = (int32)((asin(i / 255.0) * 128.0) / M_PI);
        acos256LookupTable[i] = (int32)((acos(i / 255.0) * 128.0) / M_PI);
    }

    for (int y = 0; y < 0x100; ++y) {
        uint8 *arcTan = (uint8 *)&arcTan256LookupTable[y];

        for (int x = 0; x < 0x100; ++x) {
            // 40.743664 = 0x100 / (2 * M_PI) (roughly)
            *arcTan = (int)(float)((float)atan2((float)y, x) * 40.743664);
            arcTan += 0x100;
        }
    }
}

uint8 ArcTanLookup(int32 X, int32 Y)
{
    int32 x = abs(X);
    int32 y = abs(Y);

    if (x <= y) {
        while (y > 0xFF) {
            x >>= 4;
            y >>= 4;
        }
    }
    else {
        while (x > 0xFF) {
            x >>= 4;
            y >>= 4;
        }
    }
    if (X <= 0) {
        if (Y <= 0)
            return arcTan256LookupTable[(x << 8) + y] + 0x80;
        else
            return 0x80 - arcTan256LookupTable[(x << 8) + y];
    }
    else if (Y <= 0)
        return -arcTan256LookupTable[(x << 8) + y];
    else
        return arcTan256LookupTable[(x << 8) + y];
}

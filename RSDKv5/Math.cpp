#include "RetroEngine.hpp"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

int32 sinVal1024[0x400];
int32 cosVal1024[0x400];
int32 tanVal1024[0x400];
int32 aSinVal1024[0x400];
int32 aCosVal1024[0x400];

int32 sinVal512[0x200];
int32 cosVal512[0x200];
int32 tanVal512[0x200];
int32 aSinVal512[0x200];
int32 aCosVal512[0x200];

int32 sinVal256[0x100];
int32 cosVal256[0x100];
int32 tanVal256[0x100];
int32 aSinVal256[0x100];
int32 aCosVal256[0x100];

uint8 atanVal256[0x100 * 0x100];

uint32 randKey = 0;

void CalculateTrigAngles()
{
    srand(time(NULL));
    randKey = rand();

    for (int i = 0; i < 0x400; ++i) {
        sinVal1024[i]  = (int32)(sin((i / 512.0) * M_PI) * 1024.0);
        cosVal1024[i]  = (int32)(cos((i / 512.0) * M_PI) * 1024.0);
        tanVal1024[i]  = (int32)(tan((i / 512.0) * M_PI) * 1024.0);
        aSinVal1024[i] = (int32)((asin(i / 1023.0) * 512.0) / M_PI);
        aCosVal1024[i] = (int32)((acos(i / 1023.0) * 512.0) / M_PI);
    }

    cosVal1024[0x000] = 0x400;
    cosVal1024[0x100] = 0;
    cosVal1024[0x200] = -0x400;
    cosVal1024[0x300] = 0;

    sinVal1024[0x000] = 0;
    sinVal1024[0x100] = 0x400;
    sinVal1024[0x200] = 0;
    sinVal1024[0x300] = -0x400;

    for (int i = 0; i < 0x200; ++i) {
        sinVal512[i]   = (int32)(sin((i / 256.0) * M_PI) * 512.0);
        cosVal512[i]   = (int32)(cos((i / 256.0) * M_PI) * 512.0);
        tanVal512[i]   = (int32)(tan((i / 256.0) * M_PI) * 512.0);
        aSinVal512[i]  = (int32)((asin(i / 511.0) * 256.0) / M_PI);
        aCosVal512[i]  = (int32)((acos(i / 511.0) * 256.0) / M_PI);
    }

    cosVal512[0x00]  = 0x200;
    cosVal512[0x80]  = 0;
    cosVal512[0x100] = -0x200;
    cosVal512[0x180] = 0;
    sinVal512[0x00]  = 0;
    sinVal512[0x80]  = 0x200;
    sinVal512[0x100] = 0;
    sinVal512[0x180] = -0x200;

    for (int i = 0; i < 0x100; i++) {
        sinVal256[i]  = (int)((sinVal512[i * 2] >> 1));
        cosVal256[i]  = (int)((cosVal512[i * 2] >> 1));
        tanVal256[i]  = (int)((tanVal512[i * 2] >> 1));
        aSinVal256[i] = (int)((asin(i / 255.0) * 128.0) / M_PI);
        aCosVal256[i] = (int)((acos(i / 255.0) * 128.0) / M_PI);
    }

    for (int y = 0; y < 0x100; ++y) {
        uint8 *arcTan = (uint8 *)&atanVal256[y];
        for (int x = 0; x < 0x100; ++x) {
            *arcTan = (uint8)(atan2(y, x) * 40.743664);
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
            return atanVal256[(x << 8) + y] + 0x80;
        else
            return 0x80 - atanVal256[(x << 8) + y];
    }
    else if (Y <= 0)
        return -atanVal256[(x << 8) + y];
    else
        return atanVal256[(x << 8) + y];
}
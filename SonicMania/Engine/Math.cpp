#include "RetroEngine.hpp"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

int sinVal1024[0x400];
int cosVal1024[0x400];

int ATanVal1024[0x400];
int ASinVal1024[0x400];
int ACosVal1024[0x400];

int sinVal512[0x200];
int cosVal512[0x200];

int ATanVal512[0x200];
int ASinVal512[0x200];
int ACosVal512[0x200];

int sinVal256[0x100];
int cosVal256[0x100];

int ATanVal256[0x100];
int ASinVal256[0x100];
int ACosVal256[0x100];

byte atanVal256[0x100 * 0x100];

uint randKey = 0;

void CalculateTrigAngles()
{
    randKey = rand();

    cosVal1024[0]   = 0x400;
    cosVal1024[256] = 0;
    cosVal1024[512] = -0x400;
    cosVal1024[768] = 0;
    sinVal1024[0]   = 0;
    sinVal1024[256] = 0x400;
    sinVal1024[512] = 0;
    sinVal1024[768] = -0x400;

    for (int i = 0; i < 0x200; ++i) {
        float val    = sinf(((float)i / 256) * M_PI);
        sinVal512[i] = (signed int)(val * 512.0);
        val          = cosf(((float)i / 256) * M_PI);
        cosVal512[i] = (signed int)(val * 512.0);
    }

    cosVal512[0]   = 0x200;
    cosVal512[128] = 0;
    cosVal512[256] = -0x200;
    cosVal512[384] = 0;
    sinVal512[0]   = 0;
    sinVal512[128] = 0x200;
    sinVal512[256] = 0;
    sinVal512[384] = -0x200;

    for (int i = 0; i < 0x100; i++) {
        sinVal256[i] = (sinVal512[i * 2] >> 1);
        cosVal256[i] = (cosVal512[i * 2] >> 1);
    }

    for (int Y = 0; Y < 0x100; ++Y) {
        byte *ATan = (byte *)&atanVal256[Y];
        for (int X = 0; X < 0x100; ++X) {
            float angle = atan2f(Y, X);
            *ATan       = (signed int)(angle * 40.743664f);
            ATan += 0x100;
        }
    }
}

byte ArcTanLookup(int X, int Y)
{
    int x = abs(X);
    int y = abs(Y);

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
            return atanVal256[(x << 8) + y] + -0x80;
        else
            return -0x80 - atanVal256[(x << 8) + y];
    }
    else if (Y <= 0)
        return -atanVal256[(x << 8) + y];
    else
        return atanVal256[(x << 8) + y];
}
#ifndef MATH_H
#define MATH_H

#define maxVal(a, b) (a >= b ? a : b)
#define minVal(a, b) (a <= b ? a : b)

struct Vector2 {
    int x;
    int y;
};

#define MEM_ZERO(x)  memset(&(x), 0, sizeof((x)))
#define MEM_ZEROP(x) memset((x), 0, sizeof(*(x)))

extern int sinVal1024[0x400];
extern int cosVal1024[0x400];
extern int tanVal1024[0x400];
extern int aSinVal1024[0x400];
extern int aCosVal1024[0x400];

extern int sinVal512[0x200];
extern int cosVal512[0x200];
extern int tanVal512[0x200];
extern int aSinVal512[0x200];
extern int aCosVal512[0x200];

extern int sinVal256[0x100];
extern int cosVal256[0x100];
extern int tanVal256[0x100];
extern int aSinVal256[0x100];
extern int aCosVal256[0x100];

extern byte atanVal256[0x100 * 0x100];

// Setup angles
void CalculateTrigAngles();

inline int sin1024(int angle) { return sinVal1024[angle & 0x3FF]; }
inline int cos1024(int angle) { return cosVal1024[angle & 0x3FF]; }
inline int tan1024(short angle) { return tanVal1024[angle & 0x3FF]; }
inline int aSin1024(int angle)
{
    if (angle > 0x3FF)
        return 0;
    if (angle < 0)
        return -aSinVal1024[-angle];
    return aSinVal1024[angle];
}
inline int aCos1024(int angle)
{
    if (angle > 0x3FF)
        return 0;
    if (angle < 0)
        return -aCosVal1024[-angle];
    return aCosVal1024[angle];
}

inline int sin512(int angle) { return sinVal512[angle & 0x1FF]; }
inline int cos512(int angle) { return cosVal512[angle & 0x1FF]; }
inline int tan512(short angle) { return tanVal512[angle & 0x1FF]; }
inline int aSin512(int angle)
{
    if (angle > 0x1FF)
        return 0;
    if (angle < 0)
        return -aSinVal512[-angle];
    return aSinVal512[angle];
}
inline int aCos512(int angle)
{
    if (angle > 0x1FF)
        return 0;
    if (angle < 0)
        return -aCosVal512[-angle];
    return aCosVal512[angle];
}

inline int sin256(int angle) { return sinVal256[angle & 0xFF]; }
inline int cos256(int angle) { return cosVal256[angle & 0xFF]; }
inline int tan256(int angle) { return tanVal256[angle & 0xFF]; }
inline int aSin256(int angle)
{
    if (angle > 0xFF)
        return 0;
    if (angle < 0)
        return -aSinVal256[-angle];
    return aSinVal256[angle];
}
inline int aCos256(int angle)
{
    if (angle > 0xFF)
        return 0;
    if (angle < 0)
        return -aCosVal256[-angle];
    return aCosVal256[angle];
}

// Get Arc Tan value
byte ArcTanLookup(int x, int y);

extern uint randKey;

inline void setRandKey(int key) { randKey = key; }
inline int RSDK_random(int min, int max)
{
    uint v2       = 1103515245 * randKey + 12345;
    uint v3       = 1103515245 * v2 + 12345;
    randKey       = 1103515245 * v3 + 12345;
    signed int v4 = ((randKey >> 16) & 0x7FF) ^ ((((v3 >> 16) & 0x7FF) ^ ((v2 >> 6) & 0x1FFC00)) << 10);
    if (min >= max)
        return (uint)(v4 - v4 / abs(max - min) * abs(max - min) + max);
    else
        return (uint)(v4 - v4 / abs(max - min) * abs(max - min) + min);
}
inline int RSDK_random2(int min, int max, int *randKey)
{
    if (!randKey)
        return 0;
    uint v2       = 1103515245 * *randKey + 12345;
    uint v3       = 1103515245 * v2 + 12345;
    *randKey      = 1103515245 * v3 + 12345;
    signed int v4 = ((*randKey >> 16) & 0x7FF) ^ ((((v3 >> 16) & 0x7FF) ^ ((v2 >> 6) & 0x1FFC00)) << 10);
    if (min >= max)
        return (uint)(v4 - v4 / abs(max - min) * abs(max - min) + max);
    else
        return (uint)(v4 - v4 / abs(max - min) * abs(max - min) + min);
}

#endif // !MATH_H

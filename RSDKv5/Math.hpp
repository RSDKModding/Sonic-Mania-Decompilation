#ifndef MATH_H
#define MATH_H

// not "math" but works best here
#define intToVoid(x) (void *)(size_t)(x)
#define voidToInt(x) (int)(size_t)(x)

#define maxVal(a, b) (a >= b ? a : b)
#define minVal(a, b) (a <= b ? a : b)

struct Vector2 {
    int32 x;
    int32 y;
};

#define MEM_ZERO(x)  memset(&(x), 0, sizeof((x)))
#define MEM_ZEROP(x) memset((x), 0, sizeof(*(x)))

extern int32 sinVal1024[0x400];
extern int32 cosVal1024[0x400];
extern int32 tanVal1024[0x400];
extern int32 aSinVal1024[0x400];
extern int32 aCosVal1024[0x400];

extern int32 sinVal512[0x200];
extern int32 cosVal512[0x200];
extern int32 tanVal512[0x200];
extern int32 aSinVal512[0x200];
extern int32 aCosVal512[0x200];

extern int32 sinVal256[0x100];
extern int32 cosVal256[0x100];
extern int32 tanVal256[0x100];
extern int32 aSinVal256[0x100];
extern int32 aCosVal256[0x100];

extern uint8 atanVal256[0x100 * 0x100];

// Setup angles
void CalculateTrigAngles();

inline int32 sin1024(int32 angle) { return sinVal1024[angle & 0x3FF]; }
inline int32 cos1024(int32 angle) { return cosVal1024[angle & 0x3FF]; }
inline int32 tan1024(int32 angle) { return tanVal1024[angle & 0x3FF]; }
inline int32 aSin1024(int32 angle)
{
    if (angle > 0x3FF)
        return 0;
    if (angle < 0)
        return -aSinVal1024[-angle];
    return aSinVal1024[angle];
}
inline int32 aCos1024(int32 angle)
{
    if (angle > 0x3FF)
        return 0;
    if (angle < 0)
        return -aCosVal1024[-angle];
    return aCosVal1024[angle];
}

inline int32 sin512(int32 angle) { return sinVal512[angle & 0x1FF]; }
inline int32 cos512(int32 angle) { return cosVal512[angle & 0x1FF]; }
inline int32 tan512(int32 angle) { return tanVal512[angle & 0x1FF]; }
inline int32 aSin512(int32 angle)
{
    if (angle > 0x1FF)
        return 0;
    if (angle < 0)
        return -aSinVal512[-angle];
    return aSinVal512[angle];
}
inline int32 aCos512(int32 angle)
{
    if (angle > 0x1FF)
        return 0;
    if (angle < 0)
        return -aCosVal512[-angle];
    return aCosVal512[angle];
}

inline int32 sin256(int32 angle) { return sinVal256[angle & 0xFF]; }
inline int32 cos256(int32 angle) { return cosVal256[angle & 0xFF]; }
inline int32 tan256(int32 angle) { return tanVal256[angle & 0xFF]; }
inline int32 aSin256(int32 angle)
{
    if (angle > 0xFF)
        return 0;
    if (angle < 0)
        return -aSinVal256[-angle];
    return aSinVal256[angle];
}
inline int32 aCos256(int32 angle)
{
    if (angle > 0xFF)
        return 0;
    if (angle < 0)
        return -aCosVal256[-angle];
    return aCosVal256[angle];
}

// Get Arc Tan value
uint8 ArcTanLookup(int32 x, int32 y);

extern uint32 randKey;

inline void setRandKey(int32 key) { randKey = key; }
inline int32 RSDK_random(int32 min, int32 max)
{
    int32 val = 1103515245 * randKey + 12345;
    randKey = 1103515245 * (1103515245 * val + 12345) + 12345;

    if (min > max)
        return max + ((((((val >> 16) & 0x7FF) << 10) ^ ((1103515245 * val + 12345) >> 16) & 0x7FF) << 10) ^ (randKey >> 16) & 0x7FF) % (min - max);
    else if (min < max)
        return min + ((((((val >> 16) & 0x7FF) << 10) ^ ((1103515245 * val + 12345) >> 16) & 0x7FF) << 10) ^ (randKey >> 16) & 0x7FF) % (max - min);
    else 
        return max;
}
inline int32 RSDK_random2(int32 min, int32 max, int32 *randKey)
{
    if (!randKey)
        return 0;

    int32 val = 1103515245 * *randKey + 12345;
    *randKey = 1103515245 * (1103515245 * val + 12345) + 12345;

    if (min > max)
        return max + ((((((val >> 16) & 0x7FF) << 10) ^ ((1103515245 * val + 12345) >> 16) & 0x7FF) << 10) ^ (*randKey >> 16) & 0x7FF) % (min - max);
    else if (min < max)
        return min + ((((((val >> 16) & 0x7FF) << 10) ^ ((1103515245 * val + 12345) >> 16) & 0x7FF) << 10) ^ (*randKey >> 16) & 0x7FF) % (max - min);
    else
        return max;
}

#endif // !MATH_H

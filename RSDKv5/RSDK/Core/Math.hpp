#ifndef MATH_H
#define MATH_H

namespace RSDK
{

// not "math" but works best here
#define intToVoid(x) (void *)(size_t)(x)
#define voidToInt(x) (int)(size_t)(x)

#define minVal(a, b)                      ((a) < (b) ? (a) : (b))
#define maxVal(a, b)                      ((a) > (b) ? (a) : (b))
#define clampVal(value, minimum, maximum) (((value) < (minimum)) ? (minimum) : (((value) > (maximum)) ? (maximum) : (value)))

struct Vector2 {
    int32 x;
    int32 y;
};

#define MEM_ZERO(x) memset(&(x), 0, sizeof((x)))

extern int32 sin1024LookupTable[0x400];
extern int32 cos1024LookupTable[0x400];
extern int32 tan1024LookupTable[0x400];
extern int32 asin1024LookupTable[0x400];
extern int32 acos1024LookupTable[0x400];

extern int32 sin512LookupTable[0x200];
extern int32 cos512LookupTable[0x200];
extern int32 tan512LookupTable[0x200];
extern int32 asin512LookupTable[0x200];
extern int32 acos512LookupTable[0x200];

extern int32 sin256LookupTable[0x100];
extern int32 cos256LookupTable[0x100];
extern int32 tan256LookupTable[0x100];
extern int32 asin256LookupTable[0x100];
extern int32 acos256LookupTable[0x100];

extern uint8 arcTan256LookupTable[0x100 * 0x100];

// Setup angles
void CalculateTrigAngles();

inline int32 Sin1024(int32 angle) { return sin1024LookupTable[angle & 0x3FF]; }
inline int32 Cos1024(int32 angle) { return cos1024LookupTable[angle & 0x3FF]; }
inline int32 Tan1024(int32 angle) { return tan1024LookupTable[angle & 0x3FF]; }
inline int32 ASin1024(int32 angle)
{
    if (angle > 0x3FF)
        return 0;
    if (angle < 0)
        return -asin1024LookupTable[-angle];
    return asin1024LookupTable[angle];
}
inline int32 ACos1024(int32 angle)
{
    if (angle > 0x3FF)
        return 0;
    if (angle < 0)
        return -acos1024LookupTable[-angle];
    return acos1024LookupTable[angle];
}

inline int32 Sin512(int32 angle) { return sin512LookupTable[angle & 0x1FF]; }
inline int32 Cos512(int32 angle) { return cos512LookupTable[angle & 0x1FF]; }
inline int32 Tan512(int32 angle) { return tan512LookupTable[angle & 0x1FF]; }
inline int32 ASin512(int32 angle)
{
    if (angle > 0x1FF)
        return 0;
    if (angle < 0)
        return -asin512LookupTable[-angle];
    return asin512LookupTable[angle];
}
inline int32 ACos512(int32 angle)
{
    if (angle > 0x1FF)
        return 0;
    if (angle < 0)
        return -acos512LookupTable[-angle];
    return acos512LookupTable[angle];
}

inline int32 Sin256(int32 angle) { return sin256LookupTable[angle & 0xFF]; }
inline int32 Cos256(int32 angle) { return cos256LookupTable[angle & 0xFF]; }
inline int32 Tan256(int32 angle) { return tan256LookupTable[angle & 0xFF]; }
inline int32 ASin256(int32 angle)
{
    if (angle > 0xFF)
        return 0;
    if (angle < 0)
        return -asin256LookupTable[-angle];
    return asin256LookupTable[angle];
}
inline int32 ACos256(int32 angle)
{
    if (angle > 0xFF)
        return 0;
    if (angle < 0)
        return -acos256LookupTable[-angle];
    return acos256LookupTable[angle];
}

// Get Arc Tan value
uint8 ArcTanLookup(int32 x, int32 y);

extern uint32 randSeed;

inline void SetRandSeed(int32 key) { randSeed = key; }
inline int32 GetRandomValue(int32 min, int32 max)
{
    int32 seed1 = 1103515245 * randSeed + 12345;
    int32 seed2 = 1103515245 * seed1 + 12345;
    randSeed    = 1103515245 * seed2 + 12345;

    int32 result = ((randSeed >> 16) & 0x7FF) ^ ((((seed1 >> 6) & 0x1FFC00) ^ ((seed2 >> 16) & 0x7FF)) << 10);
    int32 size   = abs(max - min);

    if (min > max)
        return (result - result / size * size + max);
    else if (min < max)
        return (result - result / size * size + min);
    else
        return max;
}
inline int32 GetSeededRandomValue(int32 min, int32 max, int32 *randSeed)
{
    if (!randSeed)
        return 0;

    int32 seed1 = 1103515245 * *randSeed + 12345;
    int32 seed2 = 1103515245 * seed1 + 12345;
    *randSeed   = 1103515245 * seed2 + 12345;

    int32 result = ((*randSeed >> 16) & 0x7FF) ^ ((((seed1 >> 6) & 0x1FFC00) ^ ((seed2 >> 16) & 0x7FF)) << 10);
    int32 size   = abs(max - min);

    if (min > max)
        return (result - result / size * size + max);
    else if (min < max)
        return (result - result / size * size + min);
    else
        return max;
}

} // namespace RSDK

#endif // !MATH_H

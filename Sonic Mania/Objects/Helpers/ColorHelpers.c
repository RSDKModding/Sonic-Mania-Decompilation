#include "SonicMania.h"

ObjectColorHelpers *ColorHelpers = NULL;

void ColorHelpers_Update(void) {}

void ColorHelpers_LateUpdate(void) {}

void ColorHelpers_StaticUpdate(void) {}

void ColorHelpers_Draw(void) {}

void ColorHelpers_Create(void *data) {}

void ColorHelpers_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void ColorHelpers_EditorDraw(void) {}

void ColorHelpers_EditorLoad(void) {}
#endif

uint16 ColorHelpers_PackRGB(uint8 r, uint8 g, uint8 b) { return (b >> 3) | (8 * ((g | (32 * (r & 0xFFF8))) & 0xFFFC)); }

void ColorHelpers_Unknown1(int32 r, int32 g, int32 b, uint32 *rPtr, uint32 *gPtr, uint32 *bPtr)
{
    if (!r && !g && !b) {
        if (rPtr)
            *rPtr = 0;
        if (gPtr)
            *gPtr = 0;
        if (bPtr)
            *bPtr = 0;
    }

    uint8 val1 = (g < r ? g : r) >= b ? b : (g < r ? g : r);
    uint8 val2 = (g > r ? g : r) <= b ? b : (g > r ? g : r);

    int div = val2 - val1;
    if (val2) {
        int32 newR = 0;

        if (val2 == val1) {
            if (rPtr)
                *rPtr = 0;
            if (gPtr)
                *gPtr = 0;
            if (bPtr)
                *bPtr = val1;
        }
        else {
            if (g == val2) {
                newR = 60 * (r - b) / div;
            }
            else if (r == val2) {
                newR = 60 * (b - g) / div + 120;
            }
            else {
                newR = 60 * (g - r) / div + 240;
            }
            if (rPtr) {
                if (newR >= 0)
                    *rPtr = newR;
                else
                    *rPtr = newR + 360;
            }
            if (gPtr)
                *gPtr = 255 * div / val2;
            if (bPtr)
                *bPtr = val2;
        }
    }
}

void ColorHelpers_Unknown2(int32 a1, int32 a2, int32 brightness, uint32 *r, uint32 *g, uint32 *b)
{
    uint32 newR = 0, newG = 0, newB = 0;

    if (a1) {
        int32 val = brightness * a1 / 255;

        switch (a2 / 60) {
            case 0:
                newR = brightness;
                newG = brightness - (val * (60 - a2 % 60)) / 60;
                newB = brightness - val;
                break;
            case 1:
                newR = brightness - a2 % 60 * val / 60;
                newG = brightness;
                newB = brightness - val;
                break;
            case 2:
                newR = brightness - val;
                newG = brightness;
                newB = brightness - (val * (60 - a2 % 60)) / 60;
                break;
            case 3:
                newG = brightness - a2 % 60 * val / 60;
                newR = (brightness - val);
                newB = brightness;
                break;
            case 4:
                newG = brightness - val;
                newB = brightness;
                newR = *g;
                break;
            case 5:
                newG = brightness - val;
                newR = brightness;
                newB = brightness - a2 % 60 * val / 60;
                break;
            default:
                newG = *g;
                newB = *g;
                newR = *g;
                break;
        }
    }
    else {
        newR = brightness;
        newG = brightness;
        newB = brightness;
    }
    if (g)
        *g = newR;
    if (r)
        *r = newG;
    if (b)
        *b = newB;
}

void ColorHelpers_Serialize(void) {}

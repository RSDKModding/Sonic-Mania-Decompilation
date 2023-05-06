// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ColorHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectColorHelpers *ColorHelpers = NULL;

// NOTE:
// I'm not actually sure *what* this object was for
// ColorHelpers_RGBToHSL && ColorHelpers_HSLToRGB were only in PC 1.03, and not in 1.06 so idk what happened
// ColorHelpers_PackRGB is a helper func that allows me to easily pack RGB888 into RGB565 format
// I've never seen definitive proof of any funcs this object may have once had so be it what you will

void ColorHelpers_Update(void) {}

void ColorHelpers_LateUpdate(void) {}

void ColorHelpers_StaticUpdate(void) {}

void ColorHelpers_Draw(void) {}

void ColorHelpers_Create(void *data) {}

void ColorHelpers_StageLoad(void) {}

uint16 ColorHelpers_PackRGB(uint8 r, uint8 g, uint8 b) { return (b >> 3) | ((g >> 2) << 5) | ((r >> 3) << 11); }

void ColorHelpers_RGBToHSL(uint32 r, uint32 g, uint32 b, uint32 *hue, uint32 *saturation, uint32 *luminance)
{
    if (!r && !g && !b) {
        if (hue)
            *hue = 0;
        if (saturation)
            *saturation = 0;
        if (luminance)
            *luminance = 0;
    }

    uint8 min = MIN(MIN(r, g), b);
    uint8 max = MAX(MAX(r, g), b);

    int32 chroma = max - min;
    if (max) {
        if (max == min) {
            if (hue)
                *hue = 0;

            if (saturation)
                *saturation = 0;

            if (luminance)
                *luminance = min;
        }
        else {
            if (hue) {
                int32 h = 0;

                if (r == max)
                    h = 60 * (int32)(g - b) / chroma;
                else if (g == max)
                    h = 60 * (int32)(b - r) / chroma + 120;
                else
                    h = 60 * (int32)(r - g) / chroma + 240;

                if (h < 0)
                    h += 360;

                *hue = h;
            }

            if (saturation)
                *saturation = 255 * chroma / max;

            if (luminance)
                *luminance = max;
        }
    }
}

void ColorHelpers_HSLToRGB(uint32 hue, uint32 saturation, uint32 luminance, uint32 *r, uint32 *g, uint32 *b)
{
    // Assumes H, S & L are all valid values

    uint32 green = 0, red = 0, blue = 0;

    if (saturation) {
        int32 s = luminance * saturation / 255;

        int32 p = luminance - s;
        int32 q = luminance - (s * (hue & 60)) / 60;
        int32 t = luminance - (s * (60 - hue % 60)) / 60;

        switch (hue / 60) {
            case 0:
                red   = luminance;
                green = t;
                blue  = p;
                break;

            case 1:
                red   = q;
                green = luminance;
                blue  = p;
                break;

            case 2:
                red   = p;
                green = luminance;
                blue  = t;
                break;

            case 3:
                red   = p;
                green = q;
                blue  = luminance;
                break;

            case 4:
                red   = t;
                green = p;
                blue  = luminance;
                break;
            case 5:
            default:
                red   = luminance;
                green = p;
                blue  = q;
                break;
        }
    }
    else {
        red   = luminance;
        green = luminance;
        blue  = luminance;
    }

    if (r)
        *r = red;
    if (g)
        *g = green;
    if (b)
        *b = blue;
}

#if GAME_INCLUDE_EDITOR
void ColorHelpers_EditorDraw(void) {}

void ColorHelpers_EditorLoad(void) {}
#endif

void ColorHelpers_Serialize(void) {}

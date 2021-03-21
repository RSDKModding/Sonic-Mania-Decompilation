#ifndef PALETTE_H
#define PALETTE_H

#define PALETTE_COUNT (0x8)
#define PALETTE_SIZE  (0x100)

union Colour {
    byte bytes[4];
    uint colour;
};

extern ushort rIndexes[0x100];
extern ushort gIndexes[0x100];
extern ushort bIndexes[0x100];

extern ushort globalPalette[PALETTE_COUNT][PALETTE_SIZE];
extern ushort activeGlobalRows[PALETTE_COUNT];
extern ushort activeStageRows[PALETTE_COUNT];

extern ushort fullPalette[PALETTE_COUNT][PALETTE_SIZE];

extern byte gfxLineBuffer[SCREEN_YSIZE]; // Pointers to active palette

extern int maskColour;
extern ushort* lookUpBuffer;

#define RGB888_TO_RGB565(r, g, b)  ((b) >> 3) | (((g) >> 2) << 5) | (((r) >> 3) << 11)       // used in pc vers

#define PACK_RGB888(r, g, b) RGB888_TO_RGB565(r, g, b)

#if RETRO_USE_PLUS
void LoadPalette(byte paletteID, const char *filePath, ushort rowFlags);
#endif

inline void SetActivePalette(byte newActivePal, int startLine, int endLine)
{
    if (newActivePal < PALETTE_COUNT)
        for (int l = startLine; l < endLine && l < SCREEN_YSIZE; l++) gfxLineBuffer[l] = newActivePal;
}

inline uint GetPaletteEntry(byte paletteID, byte index)
{
    ushort clr = fullPalette[(paletteID & 7)][index];
    return 8 * (clr & 0x1F | 4 * (clr & 0x7E0 | 8 * (clr & 0xF800)));
}

inline void SetPaletteEntry(byte paletteID, byte index, uint colour)
{
    fullPalette[paletteID][index] = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];
}

inline void SetPaletteMask(uint colour)
{
    maskColour = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];
}

inline void SetLookupTable(ushort *tablePtr) { lookUpBuffer = tablePtr; }

inline void CopyPalette(byte sourcePalette, byte srcPaletteStart, byte destinationPalette, byte destPaletteStart, ushort count)
{
    if (sourcePalette < PALETTE_COUNT && destinationPalette < PALETTE_COUNT) {
        for (int i = 0; i < count; ++i) {
            fullPalette[destinationPalette][destPaletteStart + i] = fullPalette[sourcePalette][srcPaletteStart + i];
        }
    }
}

inline void RotatePalette(byte palID, byte startIndex, byte endIndex, bool32 right)
{
    ushort startClr = fullPalette[palID][endIndex];
    if (right) {
        for (int i = endIndex; i > startIndex; --i) {
            fullPalette[palID][i] = fullPalette[palID][i - 1];
        }
        fullPalette[palID][startIndex] = startClr;
    }
    else {
        for (int i = startIndex; i < endIndex; ++i) {
            fullPalette[palID][i] = fullPalette[palID][i + 1];
        }
        fullPalette[palID][endIndex] = startClr;
    }
}

void SetPaletteFade(byte destPaletteID, byte srcPaletteA, byte srcPaletteB, ushort blendAmount, int startIndex, int endIndex);
#if RETRO_USE_PLUS
void BlendColours(byte paletteID, byte *coloursA, byte *coloursB, int alpha, int index, int count);
#endif

#endif

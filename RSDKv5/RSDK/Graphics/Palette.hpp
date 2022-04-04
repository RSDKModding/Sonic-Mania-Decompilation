#ifndef PALETTE_H
#define PALETTE_H

#define PALETTE_COUNT (0x8)
#define PALETTE_SIZE  (0x100)

union Colour {
    uint8 bytes[4];
    uint32 colour;
};

extern uint16 rgb32To16_R[0x100];
extern uint16 rgb32To16_G[0x100];
extern uint16 rgb32To16_B[0x100];

extern uint16 globalPalette[PALETTE_COUNT][PALETTE_SIZE];
extern uint16 activeGlobalRows[PALETTE_COUNT];
extern uint16 activeStageRows[PALETTE_COUNT];
extern uint16 stagePalette[PALETTE_COUNT][PALETTE_SIZE];

extern uint16 fullPalette[PALETTE_COUNT][PALETTE_SIZE];

extern uint8 gfxLineBuffer[SCREEN_YSIZE]; // Pointers to active palette

extern int32 maskColour;

#if RETRO_REV02
extern uint16 *lookupTable;
#else
extern uint16 lookupTable[0x10000];
#endif

#define RGB888_TO_RGB565(r, g, b)  ((b) >> 3) | (((g) >> 2) << 5) | (((r) >> 3) << 11)

#define PACK_RGB888(r, g, b) RGB888_TO_RGB565(r, g, b)

#if RETRO_REV02
void LoadPalette(uint8 paletteID, const char *filePath, uint16 rowFlags);
#endif

inline void SetActivePalette(uint8 newActivePal, int32 startLine, int32 endLine)
{
    if (newActivePal < PALETTE_COUNT)
        for (int32 l = startLine; l < endLine && l < SCREEN_YSIZE; l++) gfxLineBuffer[l] = newActivePal;
}

inline uint32 GetPaletteEntry(uint8 paletteID, uint8 index)
{
    //0x1F   = 0000 0000 0001 1111 = B
    //0x7E0  = 0000 0111 1110 0000 = G
    //0xF800 = 1111 1000 0000 0000 = R
    uint16 clr = fullPalette[paletteID & 7][index];
    
    int32 R = (clr & 0xF800) << 8;
    int32 G = (clr & 0x7E0) << 5;
    int32 B = (clr & 0x1F) << 3;
    return R | G | B;
}

inline void SetPaletteEntry(uint8 paletteID, uint8 index, uint32 colour)
{
    fullPalette[paletteID][index] = rgb32To16_B[(colour >> 0) & 0xFF] | rgb32To16_G[(colour >> 8) & 0xFF] | rgb32To16_R[(colour >> 16) & 0xFF];
}

inline void SetPaletteMask(uint32 colour)
{
    maskColour = rgb32To16_B[(colour >> 0) & 0xFF] | rgb32To16_G[(colour >> 8) & 0xFF] | rgb32To16_R[(colour >> 16) & 0xFF];
}

#if RETRO_REV02
inline void SetLookupTable(uint16 *tablePtr) { lookupTable = tablePtr; }
#else
inline uint16 *GetLookupTable() { return lookupTable; }
#endif

inline void CopyPalette(uint8 sourcePalette, uint8 srcPaletteStart, uint8 destinationPalette, uint8 destPaletteStart, uint16 count)
{
    if (sourcePalette < PALETTE_COUNT && destinationPalette < PALETTE_COUNT) {
        for (int32 i = 0; i < count; ++i) {
            fullPalette[destinationPalette][destPaletteStart + i] = fullPalette[sourcePalette][srcPaletteStart + i];
        }
    }
}

inline void RotatePalette(uint8 palID, uint8 startIndex, uint8 endIndex, bool32 right)
{
    if (right) {
        uint16 startClr = fullPalette[palID][endIndex];
        for (int32 i = endIndex; i > startIndex; --i) {
            fullPalette[palID][i] = fullPalette[palID][i - 1];
        }
        fullPalette[palID][startIndex] = startClr;
    }
    else {
        uint16 startClr = fullPalette[palID][startIndex];
        for (int32 i = startIndex; i < endIndex; ++i) {
            fullPalette[palID][i] = fullPalette[palID][i + 1];
        }
        fullPalette[palID][endIndex] = startClr;
    }
}

void SetPaletteFade(byte destPaletteID, byte srcPaletteA, byte srcPaletteB, short blendAmount, int32 startIndex, int32 endIndex);
#if RETRO_REV02
void BlendColours(byte paletteID, byte *coloursA, byte *coloursB, int32 alpha, int32 index, int32 count);
#endif

#endif

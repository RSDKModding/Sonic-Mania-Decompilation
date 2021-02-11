#include "RetroEngine.hpp"

ushort rIndexes[0x100];
ushort gIndexes[0x100];
ushort bIndexes[0x100];

ushort globalPalette[PALETTE_COUNT][PALETTE_SIZE];
ushort activeGlobalRows[PALETTE_COUNT];
ushort activeStageRows[PALETTE_COUNT];

ushort fullPalette[PALETTE_COUNT][PALETTE_SIZE];

byte gfxLineBuffer[SCREEN_YSIZE];

int maskColour = 0;
ushort *lookUpBuffer = NULL;


void LoadPalette(const char *filePath, int paletteID, int startPaletteIndex, int startIndex, int endIndex)
{
    //FileInfo info;
    //char fullPath[0x80];

    
    // StrCopy(fullPath, "Data/Palettes/");
    // StrAdd(fullPath, filePath);

    //if (LoadFile(fullPath, &info)) {
        //SetFilePosition(3 * startIndex);
        if (paletteID >= PALETTE_COUNT || paletteID < 0)
            paletteID = 0;

        byte colour[3];
        for (int i = startIndex; i < endIndex; ++i) {
            //FileRead(&colour, 3);
            SetPaletteEntryInternal(paletteID, startPaletteIndex++, colour[0], colour[1], colour[2]);
        }

        //CloseFile();
    //}
}

void SetPaletteFade(byte destPaletteID, byte srcPaletteA, byte srcPaletteB, ushort blendAmount, int startIndex, int endIndex)
{
    if (destPaletteID >= PALETTE_COUNT || srcPaletteA >= PALETTE_COUNT || srcPaletteB >= PALETTE_COUNT)
        return;

    if (blendAmount >= PALETTE_SIZE) {
        blendAmount = PALETTE_SIZE - 1;
    }

    if (startIndex >= endIndex)
        return;

    uint blendA         = 0xFF - blendAmount;
    ushort *srcA        = &fullPalette[srcPaletteA][startIndex];
    ushort *srcB        = &fullPalette[srcPaletteB][startIndex];
    ushort *dst         = &fullPalette[destPaletteID][startIndex];
    for (int l = startIndex; l < endIndex; ++l) {
        *dst = rIndexes[((blendAmount * *srcB & 0xF8) + (blendA * *srcA & 0xF8)) >> 8]
               | gIndexes[((blendAmount * *srcB & 0xFFFC) + (blendA * *srcA & 0xFFFC)) >> 8]
               | bIndexes[((blendAmount * (*srcB << 3)) + (blendA * (*srcA << 3))) >> 8];

        ++dst;
    }
}

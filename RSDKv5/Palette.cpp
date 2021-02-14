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


void LoadPalette(byte paletteID, const char *filePath, ushort rowFlags)
{
    FileInfo info;
    MEM_ZERO(info);

    char buffer[0x80];    
    StrCopy(buffer, "Data/Palettes/");
    StrAdd(buffer, filePath);

    if (LoadFile(&info, buffer)) {
        for (int r = 0; r < 0x10; ++r) {
            for (int r = 0; r < 0x10; ++r) {
                if (!(rowFlags >> r & 1)) {
                    for (int c = 0; c < 0x10; ++c) {
                        byte red                       = ReadInt8(&info);
                        byte green                     = ReadInt8(&info);
                        byte blue                      = ReadInt8(&info);
                        fullPalette[paletteID][(r << 4) + c] = bIndexes[blue] | gIndexes[green] | rIndexes[red];
                    }
                }
                else {
                    Seek_Cur(&info, 0x10 * (3 * sizeof(byte)));
                }
            }
        }

        CloseFile(&info);
    }
}

void SetPaletteFade(byte destPaletteID, byte srcPaletteA, byte srcPaletteB, ushort blendAmount, int startIndex, int endIndex)
{
    if (destPaletteID >= PALETTE_COUNT || srcPaletteA >= PALETTE_COUNT || srcPaletteB >= PALETTE_COUNT)
        return;

    if (blendAmount > 0xFF) {
        blendAmount = 0xFF;
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

void BlendColours(byte paletteID, byte* coloursA, byte* coloursB, int alpha, int index, int count) {

    if (paletteID >= PALETTE_COUNT || !coloursA || !coloursB)
        return;

    if (alpha > 0xFF) {
        alpha = 0xFF;
    }
    else if (alpha < 0) {
        alpha = 0;
    }

    byte alpha2        = 0xFF - alpha;
    ushort *palettePtr = &fullPalette[paletteID][index];
    for (int i = index; i < index + count; ++i) {
        // bgrx formatted array
        int r = alpha * coloursB[2] + alpha2 * coloursA[2];
        int g = alpha * coloursB[1] + alpha2 * coloursA[1];
        int b = alpha * coloursB[0] + alpha2 * coloursA[0];
        coloursA += 4;
        coloursB += 4;
        *palettePtr = rIndexes[(r >> 8) & 0xFF] | gIndexes[(g >> 8) & 0xFF] | bIndexes[(b >> 8) & 0xFF];
        ++palettePtr;
    }
}
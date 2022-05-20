#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

uint16 RSDK::rgb32To16_R[0x100];
uint16 RSDK::rgb32To16_G[0x100];
uint16 RSDK::rgb32To16_B[0x100];

uint16 RSDK::globalPalette[PALETTE_BANK_COUNT][PALETTE_BANK_SIZE];
uint16 RSDK::activeGlobalRows[PALETTE_BANK_COUNT];
uint16 RSDK::activeStageRows[PALETTE_BANK_COUNT];
uint16 RSDK::stagePalette[PALETTE_BANK_COUNT][PALETTE_BANK_SIZE];

uint16 RSDK::fullPalette[PALETTE_BANK_COUNT][PALETTE_BANK_SIZE];

uint8 RSDK::gfxLineBuffer[SCREEN_YSIZE];

int32 RSDK::maskColor = 0;
#if RETRO_REV02
uint16 *RSDK::tintLookupTable = NULL;
#else
uint16 RSDK::tintLookupTable[0x10000];
#endif

#if RETRO_REV02
void RSDK::LoadPalette(uint8 bankID, const char *filename, uint16 rowFlags)
{
    char fullFilePath[0x80];    
    sprintf(fullFilePath, "Data/Palettes/%s", filename);

    FileInfo info;
    InitFileInfo(&info);
    if (LoadFile(&info, fullFilePath, FMODE_RB)) {
        for (int32 r = 0; r < 0x10; ++r) {
            if (!(rowFlags >> r & 1)) {
                for (int32 c = 0; c < 0x10; ++c) {
                    uint8 red                             = ReadInt8(&info);
                    uint8 green                           = ReadInt8(&info);
                    uint8 blue                            = ReadInt8(&info);
                    fullPalette[bankID][(r << 4) + c] = rgb32To16_B[blue] | rgb32To16_G[green] | rgb32To16_R[red];
                }
            }
            else {
                Seek_Cur(&info, 0x10 * (3 * sizeof(uint8)));
            }
        }

        CloseFile(&info);
    }
}
#endif

void RSDK::SetPaletteFade(uint8 destBankID, uint8 srcBankA, uint8 srcBankB, int16 blendAmount, int32 startIndex, int32 endIndex)
{
    if (destBankID >= PALETTE_BANK_COUNT || srcBankA >= PALETTE_BANK_COUNT || srcBankB >= PALETTE_BANK_COUNT)
        return;

    blendAmount = clampVal(blendAmount, 0x00, 0xFF);
    endIndex    = minVal(endIndex, 0x100);

    if (startIndex >= endIndex)
        return;

    uint32 blendA         = 0xFF - blendAmount;
    uint16 *dst         = &fullPalette[destBankID][startIndex];
    for (int32 l = startIndex; l <= endIndex; ++l) {
        uint32 clrA = GetPaletteEntry(srcBankA, l);
        uint32 clrB = GetPaletteEntry(srcBankB, l);
        int32 rA    = (clrA >> 16) & 0xFF;
        int32 rB    = (clrB >> 16) & 0xFF;
        int32 gA    = (clrA >> 8) & 0xFF;
        int32 gB    = (clrB >> 8) & 0xFF;
        int32 bA    = (clrA >> 0) & 0xFF;
        int32 bB    = (clrB >> 0) & 0xFF;
        
        *dst = PACK_RGB888((uint8)((uint16)(rB * blendAmount + blendA * rA) >> 8),
                                (uint8)((uint16)(gB * blendAmount + blendA * gA) >> 8),
                                (uint8)((uint16)(bB * blendAmount + blendA * bA) >> 8));

        ++dst;
    }
}

#if RETRO_REV02
void RSDK::BlendColors(uint8 bankID, uint8 *colorsA, uint8 *colorsB, int32 blendAmount, int32 startIndex, int32 count)
{

    if (bankID >= PALETTE_BANK_COUNT || !colorsA || !colorsB)
        return;

    blendAmount = clampVal(blendAmount, 0x00, 0xFF);

    uint8 blendA       = 0xFF - blendAmount;
    uint16 *palettePtr = &fullPalette[bankID][startIndex];
    for (int32 i = startIndex; i < startIndex + count; ++i) {
        // bgrx formatted array
        int32 r = blendAmount * colorsB[2] + blendA * colorsA[2];
        int32 g = blendAmount * colorsB[1] + blendA * colorsA[1];
        int32 b = blendAmount * colorsB[0] + blendA * colorsA[0];

        *palettePtr = PACK_RGB888((uint8)(r >> 8), (uint8)(g >> 8), (uint8)(b >> 8));

        colorsA += 4;
        colorsB += 4;
        ++palettePtr;
    }
}
#endif

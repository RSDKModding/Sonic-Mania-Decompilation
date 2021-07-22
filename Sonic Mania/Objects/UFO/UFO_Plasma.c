#include "SonicMania.h"

ObjectUFO_Plasma *UFO_Plasma;

void UFO_Plasma_Update(void) {}

void UFO_Plasma_LateUpdate(void) {}

void UFO_Plasma_StaticUpdate(void) {}

void UFO_Plasma_Draw(void)
{
    ScanlineInfo *scanlinePtr = UFO_Plasma->scanlines;

    int y       = (UFO_Setup->timer + 2 * RSDK_screens->position.y) << 14;
    byte defPos = ((RSDK_screens->position.y >> 1) + 2 * UFO_Setup->timer);

    for (int i = 0; i < RSDK_screens->height; ++i) {
        ScanlineInfo *scanline  = (ScanlineInfo *)&UFO_Plasma->scanlineData[defPos++ * sizeof(ScanlineInfo)];
        scanlinePtr->position.x = scanline->position.x + (RSDK_screens->position.x << 16);
        scanlinePtr->position.y = y;
        scanlinePtr->deform.x   = scanline->deform.x;
        scanlinePtr->deform.y   = 0;

        ScanlineInfo *data = (ScanlineInfo *)&Smog->scanlineData[defPos * sizeof(ScanlineInfo)];
        y += data->deform.y;
        ++scanlinePtr;
    }
    RSDK.DrawDeformedSprite(UFO_Plasma->spriteIndex, INK_MASKED, 0x100);
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    RSDK.SetActivePalette(0, 0, RSDK_screens->height);
}

void UFO_Plasma_Create(void *data)
{
    RSDK_THIS(UFO_Plasma);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = true;
        entity->drawOrder = 3;
    }
}

void UFO_Plasma_StageLoad(void)
{
    UFO_Plasma->spriteIndex = RSDK.LoadSpriteSheet("SpecialUFO/Plasma.gif", SCOPE_STAGE);
    UFO_Plasma->scanlines   = RSDK.GetScanlines();

    int angle   = 0;
    ScanlineInfo *scanline = (ScanlineInfo *)UFO_Plasma->scanlineData;
    for (int i = 0; i < 0x100; ++i) {
        scanline[i].deform.x   = (RSDK.Sin256(angle >> 1) + 0x400) << 6;
        scanline[i].deform.y   = (RSDK.Sin256(angle >> 1) + 0x800) << 5;
        scanline[i].position.x = (RSDK.Sin256(angle) << 10) - scanline->deform.x * RSDK_screens->centerX;
        scanline[i].position.y = 0;
        angle += 2;
    }

    for (int i = 0; i < 8; ++i) RSDK.SetPaletteEntry(i, 0xDB, 0xF000);

    RSDK.SetPaletteMask(0xF000);
    RSDK.ResetEntitySlot(SLOT_UFO_PLASMA, UFO_Plasma->objectID, NULL);
    RSDK.SetDrawLayerProperties(3, false, NULL);
}

void UFO_Plasma_EditorDraw(void) {}

void UFO_Plasma_EditorLoad(void) {}

void UFO_Plasma_Serialize(void) {}

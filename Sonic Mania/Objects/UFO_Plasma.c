#include "../SonicMania.h"

ObjectUFO_Plasma *UFO_Plasma;

void UFO_Plasma_Update(void)
{

}

void UFO_Plasma_LateUpdate(void)
{

}

void UFO_Plasma_StaticUpdate(void)
{

}

void UFO_Plasma_Draw(void)
{
    int screenX             = RSDK_screens->position.x << 16;
    int screenY             = RSDK_screens->position.y;
    ScanlineInfo *scanlines = UFO_Plasma->scanlines;

    int posY    = (UFO_Setup->timer + 2 * screenY) << 14;
    byte defPos = ((screenY >> 1) + 2 * UFO_Setup->timer);

    for (int i = 0; i < RSDK_screens->height; ++i) {
        int *deform           = &UFO_Plasma->deform[4 * defPos];
        scanlines->position.x = deform[2];
        scanlines->position.x += screenX;
        defPos++;
        scanlines->position.y = posY;
        scanlines->deform.y   = 0;
        posY += deform[3];
        ++scanlines;
    }
    RSDK.DrawDeformedSprite(UFO_Plasma->spriteIndex, INK_MASKED, 0x100);
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    RSDK.SetActivePalette(0, 0, RSDK_screens->height);
}

void UFO_Plasma_Create(void* data)
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

    int angle = 0;
    int *deform        = UFO_Plasma->deform;
    for (int i = 0; i < 0x100; ++i) {
        deform[2] = (RSDK.Sin256(angle >> 1) + 0x400) << 6;
        deform[3] = (RSDK.Sin256(angle >> 1) + 0x800) << 5;
        deform[0] = (RSDK.Sin256(angle) << 10) - deform[2] * RSDK_screens->centerX;
        deform += 4;
        angle += 2;
    }

    for (int i = 0; i < 8; ++i)
        RSDK.SetPaletteEntry(i, 0xDB, 0xF000);

    RSDK.SetPaletteMask(0xF000);
    RSDK.ResetEntitySlot(SLOT_UFO_CAMERA, UFO_Plasma->objectID, 0);
    RSDK.SetDrawLayerProperties(3, 0, 0);
}

void UFO_Plasma_EditorDraw(void)
{

}

void UFO_Plasma_EditorLoad(void)
{

}

void UFO_Plasma_Serialize(void)
{

}


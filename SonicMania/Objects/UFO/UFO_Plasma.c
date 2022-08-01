// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Plasma Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_Plasma *UFO_Plasma;

void UFO_Plasma_Update(void) {}

void UFO_Plasma_LateUpdate(void) {}

void UFO_Plasma_StaticUpdate(void) {}

void UFO_Plasma_Draw(void)
{
    ScanlineInfo *scanlinePtr = UFO_Plasma->scanlines;

    int32 y          = (UFO_Setup->timer + 2 * ScreenInfo->position.y) << 14;
    uint8 scanlineID = ((ScreenInfo->position.y >> 1) + 2 * UFO_Setup->timer);

    for (int32 i = 0; i < ScreenInfo->size.y; ++i) {
        ScanlineInfo *scanline  = (ScanlineInfo *)&UFO_Plasma->scanlineData[scanlineID++ * sizeof(ScanlineInfo)];
        scanlinePtr->position.x = scanline->position.x + (ScreenInfo->position.x << 16);
        scanlinePtr->position.y = y;
        scanlinePtr->deform.x   = scanline->deform.x;
        scanlinePtr->deform.y   = 0;

        ScanlineInfo *nextScanline = (ScanlineInfo *)&UFO_Plasma->scanlineData[scanlineID * sizeof(ScanlineInfo)];
        y += nextScanline->deform.y;
        ++scanlinePtr;
    }

    RSDK.DrawDeformedSprite(UFO_Plasma->aniFrames, INK_MASKED, 0x100);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
    RSDK.SetActivePalette(0, 0, ScreenInfo->size.y);
}

void UFO_Plasma_Create(void *data)
{
    RSDK_THIS(UFO_Plasma);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = 3;
    }
}

void UFO_Plasma_StageLoad(void)
{
    UFO_Plasma->aniFrames = RSDK.LoadSpriteSheet("SpecialUFO/Plasma.gif", SCOPE_STAGE);
    UFO_Plasma->scanlines = RSDK.GetScanlines();

    int32 angle            = 0;
    ScanlineInfo *scanline = (ScanlineInfo *)UFO_Plasma->scanlineData;
    for (int32 i = 0; i < 0x100; ++i) {
        scanline[i].deform.x = (RSDK.Sin256(angle >> 1) + 0x400) << 6;
        scanline[i].deform.y = (RSDK.Sin256(angle >> 1) + 0x800) << 5;

        scanline[i].position.x = (RSDK.Sin256(angle) << 10) - scanline->deform.x * ScreenInfo->center.x;
        scanline[i].position.y = 0;

        angle += 2;
    }

    for (int32 b = 0; b < 8; ++b) RSDK.SetPaletteEntry(b, 0xDB, 0x00F000);

    RSDK.SetPaletteMask(0x00F000);
    RSDK.ResetEntitySlot(SLOT_UFO_PLASMA, UFO_Plasma->classID, NULL);
    RSDK.SetDrawGroupProperties(3, false, StateMachine_None);
}

#if RETRO_INCLUDE_EDITOR
void UFO_Plasma_EditorDraw(void) {}

void UFO_Plasma_EditorLoad(void) {}
#endif

void UFO_Plasma_Serialize(void) {}

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
    int32 y          = (UFO_Setup->timer + 2 * ScreenInfo->position.y) << 14;
    uint8 scanlineID = ((ScreenInfo->position.y >> 1) + 2 * UFO_Setup->timer);

    ScanlineInfo *scanline = UFO_Plasma->scanlines;
    for (int32 i = 0; i < ScreenInfo->size.y; ++i) {
        scanline->position.x = TO_FIXED(ScreenInfo->position.x) + UFO_Plasma->scanlineList[scanlineID].position.x;
        scanline->position.y = y;
        scanline->deform.x   = UFO_Plasma->scanlineList[scanlineID].deform.x;
        scanline->deform.y   = 0;
    
        y += UFO_Plasma->scanlineList[(scanlineID + 1) & 0xFF].deform.y;
        scanline++;
        scanlineID++;
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
    ScanlineInfo *scanline = (ScanlineInfo *)UFO_Plasma->scanlineList;
    for (int32 i = 0; i < 0x100; ++i) {
        scanline[i].deform.x   = (RSDK.Sin256(angle >> 1) << 6) + 0x10000;
        scanline[i].deform.y   = (RSDK.Sin256(angle >> 1) << 5) + 0x10000;
        scanline[i].position.x = (RSDK.Sin256(angle) << 10) - scanline[i].deform.x * ScreenInfo->center.x;
        scanline[i].position.y = 0;

        angle += 2;
    }

    for (int32 b = 0; b < 8; ++b) RSDK.SetPaletteEntry(b, 0xDB, 0x00F000);

    RSDK.SetPaletteMask(0x00F000);
    RSDK.ResetEntitySlot(SLOT_UFO_PLASMA, UFO_Plasma->classID, NULL);
    RSDK.SetDrawGroupProperties(3, false, StateMachine_None);
}

#if GAME_INCLUDE_EDITOR
void UFO_Plasma_EditorDraw(void) {}

void UFO_Plasma_EditorLoad(void) {}
#endif

void UFO_Plasma_Serialize(void) {}

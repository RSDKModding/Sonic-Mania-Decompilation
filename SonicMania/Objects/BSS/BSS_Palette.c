// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BSS_Palette Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBSS_Palette *BSS_Palette;

void BSS_Palette_Update(void) {}

void BSS_Palette_LateUpdate(void) {}

void BSS_Palette_StaticUpdate(void) {}

void BSS_Palette_Draw(void) {}

void BSS_Palette_Create(void *data)
{
    RSDK_THIS(BSS_Palette);

    if (!SceneInfo->inEditor)
        destroyEntity(self);
}

void BSS_Palette_StageLoad(void)
{
    BSS_Palette->skyAlpha   = 0x100;
    BSS_Palette->globeAlpha = 0x80;

    foreach_all(BSS_Palette, palette)
    {
        if (palette->useStageConfig) {
            // Used for Randomized modes
            BSS_Palette->startColorID = 16 * palette->paletteID;
        }
        else {
            // Used for the 32 Bonus Stages
            RSDK.SetPaletteEntry(1, 0, palette->playfieldA);
            RSDK.SetPaletteEntry(1, 1, palette->playfieldB);
            RSDK.SetPaletteEntry(1, 2, palette->bgColor1);
            RSDK.SetPaletteEntry(1, 3, palette->bgColor2);
            RSDK.SetPaletteEntry(1, 4, palette->bgColor3);
        }

        BSS_Palette->skyAlpha   = palette->skyAlpha;
        BSS_Palette->globeAlpha = palette->globeAlpha;
    }
}

#if GAME_INCLUDE_EDITOR
void BSS_Palette_EditorDraw(void)
{
    RSDK_THIS(BSS_Palette);

    RSDK.DrawRect(self->position.x - TO_FIXED(16), self->position.y - TO_FIXED(8), TO_FIXED(16), TO_FIXED(16), self->playfieldA, 0xFF, INK_NONE,
                  false);
    RSDK.DrawRect(self->position.x + TO_FIXED(0), self->position.y - TO_FIXED(8), TO_FIXED(16), TO_FIXED(16), self->playfieldB, 0xFF, INK_NONE,
                  false);

    RSDK.DrawRect(self->position.x - TO_FIXED(18), self->position.y + TO_FIXED(12), TO_FIXED(12), TO_FIXED(12), self->bgColor1, 0xFF, INK_NONE,
                  false);
    RSDK.DrawRect(self->position.x - TO_FIXED(6), self->position.y + TO_FIXED(12), TO_FIXED(12), TO_FIXED(12), self->bgColor2, 0xFF, INK_NONE, false);
    RSDK.DrawRect(self->position.x + TO_FIXED(6), self->position.y + TO_FIXED(12), TO_FIXED(12), TO_FIXED(12), self->bgColor3, 0xFF, INK_NONE, false);
}

void BSS_Palette_EditorLoad(void) {}
#endif

void BSS_Palette_Serialize(void)
{
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_BOOL, useStageConfig);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_ENUM, paletteID);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_UINT8, skyAlpha);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_UINT8, globeAlpha);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOR, playfieldA);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOR, playfieldB);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOR, bgColor1);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOR, bgColor2);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOR, bgColor3);
}

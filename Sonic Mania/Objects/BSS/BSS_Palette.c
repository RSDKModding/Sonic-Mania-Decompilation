// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BSS_Palette Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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

    foreach_all(BSS_Palette, entity)
    {
        if (entity->useStageConfig) {
            // Used for Randomized modes
            BSS_Palette->startColorID = 16 * entity->paletteID;
        }
        else {
            // Used for the 32 Bonus Stages
            RSDK.SetPaletteEntry(1, 0, entity->playfieldA);
            RSDK.SetPaletteEntry(1, 1, entity->playfieldB);
            RSDK.SetPaletteEntry(1, 2, entity->bgColor1);
            RSDK.SetPaletteEntry(1, 3, entity->bgColor2);
            RSDK.SetPaletteEntry(1, 4, entity->bgColor3);
        }

        BSS_Palette->skyAlpha      = entity->skyAlpha;
        BSS_Palette->globeAlpha    = entity->globeAlpha;
    }
}

#if RETRO_INCLUDE_EDITOR
void BSS_Palette_EditorDraw(void) {}

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

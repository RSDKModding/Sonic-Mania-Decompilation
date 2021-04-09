#include "../SonicMania.h"

ObjectBSS_Palette *BSS_Palette;

void BSS_Palette_Update(void) {}

void BSS_Palette_LateUpdate(void) {}

void BSS_Palette_StaticUpdate(void) {}

void BSS_Palette_Draw(void) {}

void BSS_Palette_Create(void *data)
{
    if (!RSDK_sceneInfo->inEditor)
        RSDK.ResetEntityPtr(RSDK_sceneInfo->entity, TYPE_BLANK, 0);
}

void BSS_Palette_StageLoad(void)
{
    BSS_Palette->alpha1 = 0x100;
    BSS_Palette->alpha2 = 0x80;

    foreach_all(BSS_Palette, entity)
    {
        if (entity->useStageConfig) {
            BSS_Palette->startColourID = 16 * entity->paletteID;
        }
        else {
            RSDK.SetPaletteEntry(1, 0, entity->playfieldA);
            RSDK.SetPaletteEntry(1, 1, entity->playfieldB);
            RSDK.SetPaletteEntry(1, 2, entity->bgColor1);
            RSDK.SetPaletteEntry(1, 3, entity->bgColor2);
            RSDK.SetPaletteEntry(1, 4, entity->bgColor3);
        }
        BSS_Palette->alpha1 = entity->skyAlpha;
        BSS_Palette->alpha2 = entity->globeAlpha;
    }
}

void BSS_Palette_EditorDraw(void) {}

void BSS_Palette_EditorLoad(void) {}

void BSS_Palette_Serialize(void)
{
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_BOOL, useStageConfig);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_ENUM, paletteID);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_UINT8, skyAlpha);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_UINT8, globeAlpha);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOUR, playfieldA);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOUR, playfieldB);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOUR, bgColor1);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOUR, bgColor2);
    RSDK_EDITABLE_VAR(BSS_Palette, VAR_COLOUR, bgColor3);
}

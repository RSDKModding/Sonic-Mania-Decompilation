#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectPalette *Palette;

void Palette_Update(void) {}

void Palette_LateUpdate(void) {}

void Palette_StaticUpdate(void) {}

void Palette_Draw(void) {}

void Palette_Create(void *data)
{
    char text[32];

    RSDK_THIS(Palette);
    if (!RSDK_sceneInfo->inEditor && (!entity->loadOnce || Palette->count <= 1)) {
        RSDK.GetCString(text, &entity->paletteFile);
        RSDK.LoadPalette(entity->bankID, text, entity->rowFlags);
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void Palette_StageLoad(void)
{
    if (Palette->count < 2)
        Palette->count++;
}

void Palette_EditorDraw(void) {}

void Palette_EditorLoad(void) {}

void Palette_Serialize(void)
{
    RSDK_EDITABLE_VAR(Palette, VAR_STRING, paletteFile);
    RSDK_EDITABLE_VAR(Palette, VAR_UINT16, rowFlags);
    RSDK_EDITABLE_VAR(Palette, VAR_UINT8, bankID);
    RSDK_EDITABLE_VAR(Palette, VAR_BOOL, loadOnce);
}
#endif

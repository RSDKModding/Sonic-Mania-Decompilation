#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
ObjectPalette *Palette;

void Palette_Update() {}

void Palette_LateUpdate() {}

void Palette_StaticUpdate() {}

void Palette_Draw() {}

void Palette_Create(void *data)
{
    char text[32];

    EntityPalette *entity = (EntityPalette *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor && (!entity->loadOnce || Palette->count <= 1)) {
        RSDK.CopyString(text, &entity->paletteFile);
        RSDK.LoadPalette(entity->bankID, text, entity->rowFlags);
        RSDK.DestroyEntity(entity, 0, NULL);
    }
}

void Palette_StageLoad()
{
    if (Palette->count < 2)
        Palette->count++;
}

void Palette_EditorDraw() {}

void Palette_EditorLoad() {}

void Palette_Serialize()
{
    RSDK_EDITABLE_VAR(Palette, VAR_STRING, paletteFile);
    RSDK_EDITABLE_VAR(Palette, VAR_UINT16, rowFlags);
    RSDK_EDITABLE_VAR(Palette, VAR_UINT8, bankID);
    RSDK_EDITABLE_VAR(Palette, VAR_BOOL, loadOnce);
}
#endif

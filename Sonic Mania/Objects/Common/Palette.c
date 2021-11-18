#include "SonicMania.h"

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
    if (!SceneInfo->inEditor && (!self->loadOnce || Palette->count <= 1)) {
        RSDK.GetCString(text, &self->paletteFile);
        RSDK.LoadPalette(self->bankID, text, self->rowFlags);
        RSDK.ResetEntityPtr(self, TYPE_BLANK, NULL);
    }
}

void Palette_StageLoad(void)
{
    if (Palette->count < 2)
        Palette->count++;
}

void Palette_EditorDraw(void)
{
    RSDK_THIS(Palette);
    RSDK.SetSpriteAnimation(Palette->aniFrames, 0, &self->animator, true, 9);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Palette_EditorLoad(void) { BGSwitch->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }

void Palette_Serialize(void)
{
    RSDK_EDITABLE_VAR(Palette, VAR_STRING, paletteFile);
    RSDK_EDITABLE_VAR(Palette, VAR_UINT16, rowFlags);
    RSDK_EDITABLE_VAR(Palette, VAR_UINT8, bankID);
    RSDK_EDITABLE_VAR(Palette, VAR_BOOL, loadOnce);
}
#endif

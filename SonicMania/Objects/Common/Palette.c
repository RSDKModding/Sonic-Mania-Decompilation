// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Palette Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPalette *Palette;

void Palette_Update(void) {}

void Palette_LateUpdate(void) {}

void Palette_StaticUpdate(void) {}

void Palette_Draw(void) {}

void Palette_Create(void *data)
{
    RSDK_THIS(Palette);

    if (!SceneInfo->inEditor && (!self->loadOnce || Palette->count <= 1)) {
        char text[32];
        RSDK.GetCString(text, &self->paletteFile);
        RSDK.LoadPalette(self->bankID, text, self->rowFlags);

        destroyEntity(self);
    }
}

void Palette_StageLoad(void)
{
    if (Palette->count < 2)
        Palette->count++;
}

#if GAME_INCLUDE_EDITOR
void Palette_EditorDraw(void)
{
    RSDK_THIS(Palette);

    RSDK.SetSpriteAnimation(Palette->aniFrames, 0, &self->animator, true, 9);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Palette_EditorLoad(void)
{
    Palette->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Palette, bankID);
    RSDK_ENUM_VAR("Bank 0", PALETTE_BANK_0);
    RSDK_ENUM_VAR("Bank 1", PALETTE_BANK_1);
    RSDK_ENUM_VAR("Bank 2", PALETTE_BANK_2);
    RSDK_ENUM_VAR("Bank 3", PALETTE_BANK_3);
    RSDK_ENUM_VAR("Bank 4", PALETTE_BANK_4);
    RSDK_ENUM_VAR("Bank 5", PALETTE_BANK_5);
    RSDK_ENUM_VAR("Bank 6", PALETTE_BANK_6);
    RSDK_ENUM_VAR("Bank 7", PALETTE_BANK_7);
}
#endif

void Palette_Serialize(void)
{
    RSDK_EDITABLE_VAR(Palette, VAR_STRING, paletteFile);
    RSDK_EDITABLE_VAR(Palette, VAR_UINT16, rowFlags);
    RSDK_EDITABLE_VAR(Palette, VAR_UINT8, bankID);
    RSDK_EDITABLE_VAR(Palette, VAR_BOOL, loadOnce);
}
#endif

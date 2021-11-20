#include "SonicMania.h"

ObjectGreenScreen *GreenScreen;

void GreenScreen_Update(void) {}

void GreenScreen_LateUpdate(void) {}

void GreenScreen_StaticUpdate(void)
{
    int count = 0;
    foreach_active(GreenScreen, screen)
    {
        RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(screen));
        ++count;
    }
    GreenScreen->greenScreenActive = count > 0;
}

void GreenScreen_Draw(void) { GreenScreen_DrawSprites(); }

void GreenScreen_Create(void *data)
{
    RSDK_THIS(GreenScreen);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->field_6C      = true;
    if (!self->paraYFactor)
        self->paraYFactor = 32;
    GreenScreen->hitbox.left   = -104;
    GreenScreen->hitbox.top    = -64;
    GreenScreen->hitbox.right  = 104;
    GreenScreen->hitbox.bottom = 64;
}

void GreenScreen_StageLoad(void)
{
    GreenScreen->aniFrames     = RSDK.LoadSpriteAnimation("SPZ2/GreenScreen.bin", SCOPE_STAGE);
    GreenScreen->hitbox.left   = -104;
    GreenScreen->hitbox.top    = -64;
    GreenScreen->hitbox.right  = 104;
    GreenScreen->hitbox.bottom = 64;
    GreenScreen->field_C[0]    = 0;
    GreenScreen->field_C[1]    = 42;
    GreenScreen->field_C[2]    = 64;
    GreenScreen->field_C[3]    = 96;
    GreenScreen->field_C[4]    = 112;
    GreenScreen->field_20[0]   = 0x100000;
    GreenScreen->field_20[1]   = 0xB40000;
    GreenScreen->field_20[2]   = 0xBE0000;
    GreenScreen->field_20[3]   = 0xC00000;
    GreenScreen->field_20[4]   = 0xC00000;
}

void GreenScreen_Unknown1(int fieldC, uint8 id, int a3, int a4)
{
    RSDK_THIS(GreenScreen);
    Vector2 drawPos;

    int offset = fieldC * (-a3 / 128);
    while (offset >= 0) {
        offset -= GreenScreen->field_20[id];
    }

    drawPos = self->position;
    drawPos.y += (self->paraYFactor * (-a4 / 128));
    drawPos.x += offset + (GreenScreen->hitbox.left << 16);

    int sizwX = self->position.x + (GreenScreen->hitbox.right << 16);
    RSDK.SetSpriteAnimation(GreenScreen->aniFrames, 1, &self->animator, true, id);
    while (drawPos.x < sizwX) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += GreenScreen->field_20[id];
    }
}

void GreenScreen_Unknown2(void)
{
    RSDK_THIS(GreenScreen);

    int clipX1 = 0, clipY1 = 0, clipX2 = 0, clipY2 = 0;
    if (!SceneInfo->inEditor) {
        clipX1 = ScreenInfo->clipBound_X1;
        clipY1 = ScreenInfo->clipBound_Y1;
        clipX2 = ScreenInfo->clipBound_X2;
        clipY2 = ScreenInfo->clipBound_Y2;
        RSDK.SetClipBounds(SceneInfo->currentScreenID, (self->position.x >> 16) + GreenScreen->hitbox.left - ScreenInfo->position.x,
                           (self->position.y >> 16) + GreenScreen->hitbox.top - ScreenInfo->position.y,
                           (self->position.x >> 16) + GreenScreen->hitbox.right - ScreenInfo->position.x,
                           (self->position.y >> 16) + GreenScreen->hitbox.bottom - ScreenInfo->position.y);
    }

    for (int i = 0; i < 5; ++i) {
        GreenScreen_Unknown1(GreenScreen->field_C[i], i, self->position.x - self->startPos.x + self->paraOffset.x,
                             self->position.y - self->startPos.y + self->paraOffset.y);
    }

    if (!SceneInfo->inEditor)
        RSDK.SetClipBounds(SceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
}

void GreenScreen_DrawSprites(void)
{
    RSDK_THIS(GreenScreen);

    if ((SceneInfo->currentDrawGroup != Zone->drawOrderHigh && self->field_6C) || (SceneInfo->inEditor && self->paraPreview))
        GreenScreen_Unknown2();

    if (SceneInfo->currentDrawGroup == Zone->drawOrderHigh || SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(GreenScreen->aniFrames, 0, &self->animator, true, 0);
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void GreenScreen_EditorDraw(void) { GreenScreen_DrawSprites(); }

void GreenScreen_EditorLoad(void)
{
    GreenScreen->aniFrames   = RSDK.LoadSpriteAnimation("SPZ2/GreenScreen.bin", SCOPE_STAGE);
    GreenScreen->field_C[0]  = 0;
    GreenScreen->field_C[1]  = 42;
    GreenScreen->field_C[2]  = 64;
    GreenScreen->field_C[3]  = 96;
    GreenScreen->field_C[4]  = 112;
    GreenScreen->field_20[0] = 0x100000;
    GreenScreen->field_20[1] = 0xB40000;
    GreenScreen->field_20[2] = 0xBE0000;
    GreenScreen->field_20[3] = 0xC00000;
    GreenScreen->field_20[4] = 0xC00000;
}
#endif

void GreenScreen_Serialize(void)
{
    RSDK_EDITABLE_VAR(GreenScreen, VAR_VECTOR2, paraOffset);
    RSDK_EDITABLE_VAR(GreenScreen, VAR_ENUM, paraYFactor);
    RSDK_EDITABLE_VAR(GreenScreen, VAR_BOOL, paraPreview);
}

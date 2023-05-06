// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GreenScreen Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGreenScreen *GreenScreen;

void GreenScreen_Update(void) {}

void GreenScreen_LateUpdate(void) {}

void GreenScreen_StaticUpdate(void)
{
    int32 count = 0;

    foreach_active(GreenScreen, screen)
    {
        RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(screen));
        ++count;
    }

    GreenScreen->greenScreenActive = count > 0;
}

void GreenScreen_Draw(void) { GreenScreen_DrawSprites(); }

void GreenScreen_Create(void *data)
{
    RSDK_THIS(GreenScreen);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->showBG        = true;

    if (!self->paraYFactor)
        self->paraYFactor = 32;

    // A... Secondary Setup....?
    GreenScreen->hitbox.left   = -104;
    GreenScreen->hitbox.top    = -64;
    GreenScreen->hitbox.right  = 104;
    GreenScreen->hitbox.bottom = 64;
}

void GreenScreen_StageLoad(void)
{
    GreenScreen->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/GreenScreen.bin", SCOPE_STAGE);

    GreenScreen->hitbox.left   = -104;
    GreenScreen->hitbox.top    = -64;
    GreenScreen->hitbox.right  = 104;
    GreenScreen->hitbox.bottom = 64;

    GreenScreen->parallaxFactor[0] = 0;
    GreenScreen->parallaxFactor[1] = 42;
    GreenScreen->parallaxFactor[2] = 64;
    GreenScreen->parallaxFactor[3] = 96;
    GreenScreen->parallaxFactor[4] = 112;

    GreenScreen->bgSize[0] = 16 << 16;
    GreenScreen->bgSize[1] = 180 << 16;
    GreenScreen->bgSize[2] = 190 << 16;
    GreenScreen->bgSize[3] = 192 << 16;
    GreenScreen->bgSize[4] = 192 << 16;
}

void GreenScreen_DrawBG(int32 x, int32 y, uint8 bgID, int32 parallaxFactor)
{
    RSDK_THIS(GreenScreen);
    Vector2 drawPos;

    int32 paraX = parallaxFactor * (-x / 128);
    while (paraX >= 0) paraX -= GreenScreen->bgSize[bgID];

    drawPos = self->position;
    drawPos.y += (self->paraYFactor * (-y / 128));
    drawPos.x += (GreenScreen->hitbox.left << 16) + paraX;

    int32 sizeX = self->position.x + (GreenScreen->hitbox.right << 16);
    RSDK.SetSpriteAnimation(GreenScreen->aniFrames, 1, &self->animator, true, bgID);
    while (drawPos.x < sizeX) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += GreenScreen->bgSize[bgID];
    }
}

void GreenScreen_DrawBackgrounds(void)
{
    RSDK_THIS(GreenScreen);

    int32 clipX1 = 0, clipY1 = 0, clipX2 = 0, clipY2 = 0;
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

    for (int32 bgID = 0; bgID < 5; ++bgID) {
        GreenScreen_DrawBG(self->position.x - self->startPos.x + self->paraOffset.x, self->position.y - self->startPos.y + self->paraOffset.y, bgID,
                           GreenScreen->parallaxFactor[bgID]);
    }

    if (!SceneInfo->inEditor)
        RSDK.SetClipBounds(SceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
}

void GreenScreen_DrawSprites(void)
{
    RSDK_THIS(GreenScreen);

    if ((SceneInfo->currentDrawGroup != Zone->objectDrawGroup[1] && self->showBG) || (SceneInfo->inEditor && self->paraPreview))
        GreenScreen_DrawBackgrounds();

    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[1] || SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(GreenScreen->aniFrames, 0, &self->animator, true, 0);
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

#if GAME_INCLUDE_EDITOR
void GreenScreen_EditorDraw(void)
{
    RSDK_THIS(GreenScreen);

    self->active        = ACTIVE_BOUNDS;
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->showBG        = false;

    if (!self->paraYFactor)
        self->paraYFactor = 32;

    GreenScreen_DrawSprites();
}

void GreenScreen_EditorLoad(void)
{
    GreenScreen->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/GreenScreen.bin", SCOPE_STAGE);

    GreenScreen->parallaxFactor[0] = 0;
    GreenScreen->parallaxFactor[1] = 42;
    GreenScreen->parallaxFactor[2] = 64;
    GreenScreen->parallaxFactor[3] = 96;
    GreenScreen->parallaxFactor[4] = 112;

    GreenScreen->bgSize[0] = 16 << 16;
    GreenScreen->bgSize[1] = 180 << 16;
    GreenScreen->bgSize[2] = 190 << 16;
    GreenScreen->bgSize[3] = 192 << 16;
    GreenScreen->bgSize[4] = 192 << 16;
}
#endif

void GreenScreen_Serialize(void)
{
    RSDK_EDITABLE_VAR(GreenScreen, VAR_VECTOR2, paraOffset);
    RSDK_EDITABLE_VAR(GreenScreen, VAR_ENUM, paraYFactor);
    RSDK_EDITABLE_VAR(GreenScreen, VAR_BOOL, paraPreview);
}

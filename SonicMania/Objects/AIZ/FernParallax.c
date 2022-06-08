// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FernParallax Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectFernParallax *FernParallax;

void FernParallax_Update(void) {}

void FernParallax_LateUpdate(void) {}

void FernParallax_StaticUpdate(void) {}

void FernParallax_Draw(void)
{
    RSDK_THIS(FernParallax);
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    int32 screenX = (screen->position.x + screen->centerX) << 16;
    int32 screenY = (screen->position.y + screen->centerY) << 16;

    Vector2 drawPos;
    drawPos.x = self->position.x + ((self->position.x - screenX) << self->parallaxFactor.x);
    drawPos.y = self->position.y + ((self->position.y - screenY) << self->parallaxFactor.y);

    Hitbox *otherHitbox = RSDK.GetHitbox(&self->animator, 0);

    Hitbox thisHitbox;
    thisHitbox.left   = -screen->centerX;
    thisHitbox.right  = screen->centerX;
    thisHitbox.top    = -screen->centerY;
    thisHitbox.bottom = screen->centerY;

    Vector2 *bufPos = (Vector2 *)&FernParallax->entityBuffer[0 * ENTITY_SIZE];
    bufPos->x       = screenX;
    bufPos->y       = screenY;

    bufPos    = (Vector2 *)&FernParallax->entityBuffer[1 * ENTITY_SIZE];
    bufPos->x = drawPos.x;
    bufPos->y = drawPos.y;

    int32 bottom = ((screen->height + screen->position.y) << 16) - (otherHitbox->bottom << 16);
    if (RSDK.CheckObjectCollisionTouchBox(&FernParallax->entityBuffer[0], &thisHitbox, &FernParallax->entityBuffer[1], otherHitbox)
        && bottom - drawPos.y > 0) {
        drawPos.y = bottom - ((bottom - drawPos.y) >> 3);
    }
    RSDK.DrawSprite(&self->animator, &drawPos, false);
}

void FernParallax_Create(void *data)
{
    RSDK_THIS(FernParallax);
    self->active    = ACTIVE_NORMAL;
    self->visible   = true;
    self->drawOrder = Zone->objectDrawHigh + 1;
    RSDK.SetSpriteAnimation(FernParallax->aniFrames, self->aniID, &self->animator, true, 0);
}

void FernParallax_StageLoad(void)
{
    FernParallax->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
    memset(FernParallax->entityBuffer, 0, 2 * ENTITY_SIZE);
}

#if RETRO_INCLUDE_EDITOR
void FernParallax_EditorDraw(void)
{
    RSDK_THIS(FernParallax);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void FernParallax_EditorLoad(void)
{
    FernParallax->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(FernParallax, aniID);
    RSDK_ENUM_VAR("Leaf 1", AIZ_DECOR_LEAF1);
    RSDK_ENUM_VAR("Leaf 2", AIZ_DECOR_LEAF2);
    RSDK_ENUM_VAR("Leaf 3", AIZ_DECOR_LEAF3);
    RSDK_ENUM_VAR("Leaf 4", AIZ_DECOR_LEAF4);
}
#endif

void FernParallax_Serialize(void)
{
    RSDK_EDITABLE_VAR(FernParallax, VAR_UINT8, aniID);
    RSDK_EDITABLE_VAR(FernParallax, VAR_VECTOR2, parallaxFactor);
}
#endif

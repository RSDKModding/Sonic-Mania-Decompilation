// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PaintingEyes Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPaintingEyes *PaintingEyes;

void PaintingEyes_Update(void)
{
    RSDK_THIS(PaintingEyes);

    foreach_active(Player, player)
    {
        if ((player->direction && player->position.x >= self->position.x - 0x80000)
            || (!player->direction && player->position.x <= self->position.x + 0x80000)) {
            if (player->onGround)
                self->hideEyes = false;
            else if (self->timer < 30)
                self->timer = 0;
        }
        else
            self->hideEyes = true;
    }

    if (self->hideEyes) {
        if (self->coverPos < 0x80000)
            self->coverPos += 0x20000;

        self->timer = 0;
    }
    else {
        if (self->timer >= 30) {
            if (self->coverPos > 0)
                self->coverPos -= 0x20000;
        }
        else {
            self->timer++;
        }
    }
}

void PaintingEyes_LateUpdate(void) {}

void PaintingEyes_StaticUpdate(void) {}

void PaintingEyes_Draw(void)
{
    RSDK_THIS(PaintingEyes);

    Vector2 drawPos;
    drawPos.x = self->position.x + CLAMP(((ScreenInfo->position.x + ScreenInfo->center.x) << 10) - (self->position.x >> 6), -0x10000, 0x10000);
    drawPos.y = self->position.y + CLAMP(((ScreenInfo->position.y + ScreenInfo->center.y) << 10) - (self->position.y >> 6), -0x10000, 0x10000);
    RSDK.DrawSprite(&self->irisAnimator, &drawPos, false);

    drawPos.x = self->position.x + CLAMP(((ScreenInfo->position.x + ScreenInfo->center.x) << 11) - (self->position.x >> 5), -0x20000, 0x20000);
    drawPos.y = self->position.y + CLAMP(((ScreenInfo->position.y + ScreenInfo->center.y) << 11) - (self->position.y >> 5), -0x20000, 0x20000);
    RSDK.DrawSprite(&self->pupilAnimator, &drawPos, false);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + self->coverPos;
    RSDK.DrawSprite(&self->coverAnimator, &drawPos, false);
}

void PaintingEyes_Create(void *data)
{
    RSDK_THIS(PaintingEyes);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->coverAnimator, true, 0);
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->irisAnimator, true, 1);
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->pupilAnimator, true, 2);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x100000;
        self->updateRange.y = 0x100000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0] - 1;
    }
}

void PaintingEyes_StageLoad(void) { PaintingEyes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/PaintingEyes.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void PaintingEyes_EditorDraw(void)
{
    RSDK_THIS(PaintingEyes);

    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->coverAnimator, false, 0);
    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->irisAnimator, false, 1);
    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->pupilAnimator, false, 2);

    RSDK.DrawSprite(&self->irisAnimator, NULL, false);
    RSDK.DrawSprite(&self->pupilAnimator, NULL, false);
    RSDK.DrawSprite(&self->coverAnimator, NULL, false);
}

void PaintingEyes_EditorLoad(void) { PaintingEyes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/PaintingEyes.bin", SCOPE_STAGE); }
#endif

void PaintingEyes_Serialize(void) {}

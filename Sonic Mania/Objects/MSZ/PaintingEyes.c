// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PaintingEyes Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPaintingEyes *PaintingEyes;

void PaintingEyes_Update(void)
{
    RSDK_THIS(PaintingEyes);
    foreach_active(Player, player)
    {
        if ((player->direction && player->position.x >= self->position.x - 0x80000)
            || (!player->direction && player->position.x <= self->position.x + 0x80000)) {
            if (player->onGround) {
                self->flag = false;
            }
            else if (self->timer < 30) {
                self->timer = 0;
            }
        }
        else {
            self->flag = true;
        }
    }

    if (self->flag) {
        if (self->lookPos < 0x80000)
            self->lookPos += 0x20000;
        self->timer = 0;
    }
    else {
        if (self->timer >= 30) {
            if (self->lookPos > 0)
                self->lookPos -= 0x20000;
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

    drawPos.x = clampVal(((ScreenInfo->position.x + ScreenInfo->centerX) << 10) - (self->position.x >> 6), -0x10000, 0x10000)
                + self->position.x;
    drawPos.y =
        clampVal(((ScreenInfo->position.y + ScreenInfo->centerY) << 10) - (self->position.y >> 6), -0x10000, 0x10000) + self->position.y;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    drawPos.x =
        clampVal(((ScreenInfo->position.x + ScreenInfo->centerX) << 11) - (self->position.x >> 5), -0x10000, 0x20000) + self->position.x;
    drawPos.y =
        clampVal(((ScreenInfo->position.y + ScreenInfo->centerY) << 11) - (self->position.y >> 5), -0x10000, 0x20000) + self->position.y;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + self->lookPos;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
}

void PaintingEyes_Create(void *data)
{
    RSDK_THIS(PaintingEyes);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->animator2, true, 1);
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->animator3, true, 2);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x100000;
        self->updateRange.y = 0x100000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow - 1;
    }
}

void PaintingEyes_StageLoad(void) { PaintingEyes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/PaintingEyes.bin", SCOPE_STAGE); }

void PaintingEyes_EditorDraw(void)
{
    RSDK_THIS(PaintingEyes);
    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->animator2, false, 1);
    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &self->animator3, false, 2);

    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void PaintingEyes_EditorLoad(void) { PaintingEyes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/PaintingEyes.bin", SCOPE_STAGE); }

void PaintingEyes_Serialize(void) {}

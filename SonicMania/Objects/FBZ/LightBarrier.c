// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LightBarrier Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLightBarrier *LightBarrier;

void LightBarrier_Update(void) {}

void LightBarrier_LateUpdate(void)
{
    RSDK_THIS(LightBarrier);

    if (self->enabled) {
        if (self->barrierAlpha < 0xA0)
            self->barrierAlpha += 4;

        foreach_active(Player, player)
        {
            if (self->position.x > (ScreenInfo->position.x + ScreenInfo->center.x) << 16 || player->velocity.x < 0)
                Player_CheckCollisionBox(player, self, &self->hitboxBarrier);
        }

        self->timer = (self->timer + 2) & 0x3F;
        self->alpha = self->barrierAlpha + (RSDK.Sin256(2 * Zone->timer) >> 3);
    }
}

void LightBarrier_StaticUpdate(void) {}

void LightBarrier_Draw(void)
{
    RSDK_THIS(LightBarrier);

    Vector2 drawPos = self->position;
    self->inkEffect = INK_NONE;
    self->direction = FLIP_NONE;

    drawPos.y += self->size << 15;
    RSDK.DrawSprite(&self->emitterAnimator, &drawPos, false);

    drawPos.y -= self->size << 16;
    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->emitterAnimator, &drawPos, false);

    self->direction = FLIP_NONE;
    if (self->enabled) {
        self->inkEffect    = INK_ADD;
        int32 timer        = self->timer;
        SpriteFrame *frame = RSDK.GetFrame(LightBarrier->aniFrames, 0, 1);
        drawPos.y += 0x80000;

        int32 size = self->size - 8;
        for (int32 i = 8; i < size;) {
            int32 height = 64 - timer;
            if (64 - timer + i > size)
                height = size - i;

            frame->sprY   = timer + (self->sprY & 0xFFFF);
            frame->height = height;
            RSDK.DrawSprite(&self->barrierAnimator, &drawPos, false);

            i += height;
            drawPos.y += height << 16;
            timer = (timer + height) & 0x3F;
        }
    }
}

void LightBarrier_Create(void *data)
{
    RSDK_THIS(LightBarrier);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->updateRange.x = 0x800000;
        self->updateRange.y = self->size << 15;

        RSDK.SetSpriteAnimation(LightBarrier->aniFrames, 0, &self->emitterAnimator, true, 0);
        RSDK.SetSpriteAnimation(LightBarrier->aniFrames, 0, &self->barrierAnimator, true, 1);
        SpriteFrame *frame = RSDK.GetFrame(LightBarrier->aniFrames, 0, 1);
        self->sprY         = frame->sprY;

        self->hitboxBarrier.left   = -8;
        self->hitboxBarrier.top    = -(self->size >> 1);
        self->hitboxBarrier.right  = 8;
        self->hitboxBarrier.bottom = self->size >> 1;

        if (self->enabled)
            self->barrierAlpha = 0xA0;
    }
}

void LightBarrier_StageLoad(void) { LightBarrier->aniFrames = RSDK.LoadSpriteAnimation("FBZ/LightBarrier.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void LightBarrier_EditorDraw(void)
{
    RSDK_THIS(LightBarrier);
    self->alpha = 0xA0;

    self->updateRange.x = 0x800000;
    self->updateRange.y = self->size << 15;

    RSDK.SetSpriteAnimation(LightBarrier->aniFrames, 0, &self->emitterAnimator, true, 0);
    RSDK.SetSpriteAnimation(LightBarrier->aniFrames, 0, &self->barrierAnimator, true, 1);
    SpriteFrame *frame = RSDK.GetFrame(LightBarrier->aniFrames, 0, 1);
    self->sprY         = frame->sprY;

    LightBarrier_Draw();

    frame->sprY = self->sprY;
}

void LightBarrier_EditorLoad(void) { LightBarrier->aniFrames = RSDK.LoadSpriteAnimation("FBZ/LightBarrier.bin", SCOPE_STAGE); }
#endif

void LightBarrier_Serialize(void)
{
    RSDK_EDITABLE_VAR(LightBarrier, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(LightBarrier, VAR_BOOL, enabled);
}

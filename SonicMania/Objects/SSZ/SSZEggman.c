// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZEggman Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectSSZEggman *SSZEggman;

void SSZEggman_Update(void)
{
    RSDK_THIS(SSZEggman);

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
}

void SSZEggman_LateUpdate(void) {}

void SSZEggman_StaticUpdate(void) {}

void SSZEggman_Draw(void)
{
    RSDK_THIS(SSZEggman);

    Vector2 drawPos;
    drawPos.x = self->position.x + self->offset.x;
    drawPos.y = self->position.y + self->offset.y;

    RSDK.DrawSprite(&self->seatAnimator, NULL, false);
    RSDK.DrawSprite(&self->eggmanAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->mobileAnimator, NULL, false);
    RSDK.DrawSprite(&self->panelAnimator, NULL, false);
}

void SSZEggman_Create(void *data)
{
    RSDK_THIS(SSZEggman);

    if (!SceneInfo->inEditor) {

        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->drawFX        = FX_FLIP;
        self->alpha         = 0x40;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 2, &self->eggmanAnimator, true, 0);

        if (data) {
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 6, &self->panelAnimator, true, 0);
        }
        else {
            self->offset.y = -0x100000;
            self->state    = SSZEggman_State_Init;
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 4, &self->seatAnimator, true, 0);
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 5, &self->mobileAnimator, true, 0);
        }
    }
}

void SSZEggman_StageLoad(void) { SSZEggman->aniFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanSSZ.bin", SCOPE_STAGE); }

void SSZEggman_State_Init(void)
{
    RSDK_THIS(SSZEggman);

    foreach_all(PhantomRuby, ruby) { self->ruby = ruby; }

    self->originPos.x = self->position.x;
    self->originPos.y = self->position.y;
    self->state       = SSZEggman_State_HoldingRuby;
}

void SSZEggman_State_HoldingRuby(void)
{
    RSDK_THIS(SSZEggman);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);

    if (self->ruby) {
        self->ruby->position.x = self->position.x - 0xC0000;
        self->ruby->position.y = self->position.y - 0x100000;
    }

    foreach_active(MetalSonic, metal)
    {
        if (metal->state == MetalSonic_State_ObtainRuby) {
            self->speed = -0x28000;
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 3, &self->eggmanAnimator, true, 0);
            self->state = SSZEggman_State_ThrownRuby;

            if (self->ruby) {
                self->ruby->state      = PhantomRuby_State_MoveGravity;
                self->ruby->velocity.x = -0x40000;
                self->ruby->velocity.y = -0x40000;
            }
        }
    }
}

void SSZEggman_State_ThrownRuby(void)
{
    RSDK_THIS(SSZEggman);

    self->speed += 0x3800;
    self->offset.y += self->speed;

    if (self->offset.y >= 0) {
        self->offset.y = 0;
        self->speed    = 0;
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 0, &self->eggmanAnimator, true, 0);
        self->state = SSZEggman_State_WatchMetalTransform;
    }

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);
}

void SSZEggman_State_WatchMetalTransform(void)
{
    RSDK_THIS(SSZEggman);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);

    if (++self->timer == 120)
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 1, &self->eggmanAnimator, true, 0);

    if (self->timer == 180) {
        self->timer     = 0;
        self->direction = FLIP_X;
        self->active    = ACTIVE_NORMAL;
        self->state     = SSZEggman_State_FlyAway;
    }

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);
}

void SSZEggman_State_FlyAway(void)
{
    RSDK_THIS(SSZEggman);

    self->velocity.x += 0x1000;
    self->position.x += self->velocity.x;
    self->originPos.y -= 0x8000;

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void SSZEggman_EditorDraw(void)
{
    RSDK_THIS(SSZEggman);

    self->offset.y = -0x100000;

    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 2, &self->eggmanAnimator, true, 0);
    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 4, &self->seatAnimator, true, 0);
    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 5, &self->mobileAnimator, true, 0);

    SSZEggman_Draw();
}

void SSZEggman_EditorLoad(void) { SSZEggman->aniFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanSSZ.bin", SCOPE_STAGE); }
#endif

void SSZEggman_Serialize(void) {}
#endif

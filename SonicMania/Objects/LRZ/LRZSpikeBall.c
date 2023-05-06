// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZSpikeBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLRZSpikeBall *LRZSpikeBall;

void LRZSpikeBall_Update(void)
{
    RSDK_THIS(LRZSpikeBall);

    StateMachine_Run(self->state);

    LRZSpikeBall_CheckPlayerBaseCollisions();
    LRZSpikeBall_CheckPlayerBallCollisions();

    self->updateRange.y = abs(self->ballOffset.y) + 0x800000;
    RSDK.ProcessAnimation(&self->ballAnimator);
}

void LRZSpikeBall_LateUpdate(void) {}

void LRZSpikeBall_StaticUpdate(void) {}

void LRZSpikeBall_Draw(void)
{
    RSDK_THIS(LRZSpikeBall);

    self->position.x += self->ballOffset.x;
    self->position.y += self->ballOffset.y;
    RSDK.DrawSprite(&self->ballAnimator, NULL, false);

    self->position.x -= self->ballOffset.x;
    self->position.y -= self->ballOffset.y;
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);
}

void LRZSpikeBall_Create(void *data)
{
    RSDK_THIS(LRZSpikeBall);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (!self->interval)
        self->interval = 240;

    self->hitboxBase.left   = -16;
    self->hitboxBase.top    = 8;
    self->hitboxBase.right  = 8;
    self->hitboxBase.bottom = 16;

    self->hitboxBall.left   = -10;
    self->hitboxBall.top    = -10;
    self->hitboxBall.right  = 10;
    self->hitboxBall.bottom = 10;

    self->state = LRZSpikeBall_State_Init;
}

void LRZSpikeBall_StageLoad(void)
{
    LRZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZSpikeBall.bin", SCOPE_STAGE);

    LRZSpikeBall->sfxCharge    = RSDK.GetSfx("LRZ/Charge.wav");
    LRZSpikeBall->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
}

int32 LRZSpikeBall_GetBaseFrameID(void)
{
    RSDK_THIS(LRZSpikeBall);

    if (self->timer <= 0)
        return 0;

    int32 time = 0, inc = 0, nextInc = 14;
    bool32 useOnFrames = false;

    for (; time < self->timer; time += inc) {
        inc = nextInc;
        if (useOnFrames) {
            useOnFrames = false;
            nextInc     = MAX(nextInc - 2, 1);
        }
        else
            useOnFrames = true;
    }

    if (!useOnFrames)
        return 0;

    if (time - self->timer < 2 || time - self->timer > nextInc - 2)
        return 1;
    else
        return 2;
}

void LRZSpikeBall_CheckPlayerBaseCollisions(void)
{
    RSDK_THIS(LRZSpikeBall);

    foreach_active(Player, player) { Player_CheckCollisionBox(player, self, &self->hitboxBase); }
}

void LRZSpikeBall_CheckPlayerBallCollisions(void)
{
    RSDK_THIS(LRZSpikeBall);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x += self->ballOffset.x;
    self->position.y += self->ballOffset.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitboxBall)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x400, true, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void LRZSpikeBall_State_Init(void)
{
    RSDK_THIS(LRZSpikeBall);

    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &self->ballAnimator, true, 0);

    self->ballOffset.x = 0;
    self->ballOffset.y = 0;
    self->state        = LRZSpikeBall_State_AwaitInterval;
}

void LRZSpikeBall_State_AwaitInterval(void)
{
    RSDK_THIS(LRZSpikeBall);

    if (!self->interval || !((self->intervalOffset + Zone->timer) % self->interval)) {
        self->timer  = 0;
        self->active = ACTIVE_NORMAL;
        self->state  = LRZSpikeBall_State_ChargeUp;
        RSDK.PlaySfx(LRZSpikeBall->sfxCharge, false, 255);
    }
}

void LRZSpikeBall_State_ChargeUp(void)
{
    RSDK_THIS(LRZSpikeBall);

    self->baseAnimator.frameID = LRZSpikeBall_GetBaseFrameID();

    if (self->timer++ >= 142) {
        self->velocity.y = -0x8000 * self->strength;
        self->state      = LRZSpikeBall_State_LaunchedBall;
        RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 2, &self->ballAnimator, true, 0);
        RSDK.PlaySfx(LRZSpikeBall->sfxExplosion, false, 255);
    }
}

void LRZSpikeBall_State_LaunchedBall(void)
{
    RSDK_THIS(LRZSpikeBall);

    self->ballOffset.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y >= 0 && self->ballOffset.y >= 0) {
        self->ballOffset.y = 0;
        self->velocity.y   = 0;
        RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &self->ballAnimator, true, 0);
        self->active = ACTIVE_BOUNDS;
        self->state  = LRZSpikeBall_State_AwaitInterval;
    }
}

#if GAME_INCLUDE_EDITOR
void LRZSpikeBall_EditorDraw(void)
{
    RSDK_THIS(LRZSpikeBall);

    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &self->ballAnimator, true, 0);

    self->ballOffset.x = 0;
    self->ballOffset.y = 0;

    self->inkEffect = INK_NONE;

    LRZSpikeBall_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->inkEffect  = INK_BLEND;
        self->velocity.y = -0x8000 * self->strength;
        self->ballOffset.y += self->velocity.y;

        while (self->velocity.y < 0 && self->ballOffset.y < 0) {
            self->ballOffset.y += self->velocity.y;
            self->velocity.y += 0x3800;
        }

        self->position.x += self->ballOffset.x;
        self->position.y += self->ballOffset.y;
        RSDK.DrawSprite(&self->ballAnimator, NULL, false);

        self->position.x -= self->ballOffset.x;
        self->position.y -= self->ballOffset.y;

        RSDK_DRAWING_OVERLAY(false);
    }
}

void LRZSpikeBall_EditorLoad(void) { LRZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZSpikeBall.bin", SCOPE_STAGE); }
#endif

void LRZSpikeBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, interval);
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, intervalOffset);
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, strength);
}

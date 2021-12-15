// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZSpikeBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectLRZSpikeBall *LRZSpikeBall;

void LRZSpikeBall_Update(void)
{
    RSDK_THIS(LRZSpikeBall);
    StateMachine_Run(self->state);
    LRZSpikeBall_CheckPlayerBaseCollisions();
    LRZSpikeBall_CheckPlayerBallCollisions();
    self->updateRange.y = abs(self->offset.y) + 0x800000;
    RSDK.ProcessAnimation(&self->animator2);
}

void LRZSpikeBall_LateUpdate(void) {}

void LRZSpikeBall_StaticUpdate(void) {}

void LRZSpikeBall_Draw(void)
{
    RSDK_THIS(LRZSpikeBall);
    self->position.x += self->offset.x;
    self->position.y += self->offset.y;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->position.x -= self->offset.x;
    self->position.y -= self->offset.y;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void LRZSpikeBall_Create(void *data)
{
    RSDK_THIS(LRZSpikeBall);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
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
    self->state             = LRZSpikeBall_State_Setup;
}

void LRZSpikeBall_StageLoad(void)
{
    LRZSpikeBall->aniFrames    = RSDK.LoadSpriteAnimation("LRZ2/LRZSpikeBall.bin", SCOPE_STAGE);
    LRZSpikeBall->sfxCharge    = RSDK.GetSfx("LRZ/Charge.wav");
    LRZSpikeBall->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
}

int LRZSpikeBall_GetFrameID(void)
{
    RSDK_THIS(LRZSpikeBall);

    if (self->timer <= 0)
        return 0;

    int id = 0, inc = 0, nextInc = 14;
    bool32 flag = false;
    for (; id < self->timer; id += inc) {
        inc = nextInc;
        if (flag) {
            flag = false;
            if (nextInc - 2 > 1)
                nextInc -= 2;
            else
                nextInc = 1;
        }
        else {
            flag = true;
        }
        id += inc;
    }

    if (!flag)
        return 0;

    if (id - self->timer < 2 || id - self->timer > nextInc - 2)
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

    int storeX = self->position.x;
    int storeY = self->position.y;
    self->position.x += self->offset.x;
    self->position.y += self->offset.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitboxBall)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, true, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void LRZSpikeBall_State_Setup(void)
{
    RSDK_THIS(LRZSpikeBall);

    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &self->animator2, true, 0);
    self->offset.x = 0;
    self->offset.y = 0;
    self->state    = LRZSpikeBall_State_Unknown1;
}

void LRZSpikeBall_State_Unknown1(void)
{
    RSDK_THIS(LRZSpikeBall);

    if (!self->interval || !((self->intervalOffset + Zone->timer) % self->interval)) {
        self->timer  = 0;
        self->active = ACTIVE_NORMAL;
        self->state  = LRZSpikeBall_State_Unknown2;
        RSDK.PlaySfx(LRZSpikeBall->sfxCharge, false, 255);
    }
}

void LRZSpikeBall_State_Unknown2(void)
{
    RSDK_THIS(LRZSpikeBall);

    self->animator1.frameID = LRZSpikeBall_GetFrameID();

    if (self->timer++ >= 142) {
        self->velocity.y = -0x8000 * self->strength;
        self->state      = LRZSpikeBall_State_Unknown3;
        RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 2, &self->animator2, true, 0);
        RSDK.PlaySfx(LRZSpikeBall->sfxExplosion, false, 255);
    }
}

void LRZSpikeBall_State_Unknown3(void)
{
    RSDK_THIS(LRZSpikeBall);

    self->offset.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (self->velocity.y >= 0 && self->offset.y >= 0) {
        self->offset.y   = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &self->animator2, true, 0);
        self->active = ACTIVE_BOUNDS;
        self->state  = LRZSpikeBall_State_Unknown1;
    }
}

#if RETRO_INCLUDE_EDITOR
void LRZSpikeBall_EditorDraw(void)
{
    RSDK_THIS(LRZSpikeBall);
    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &self->animator2, true, 0);
    self->offset.x = 0;
    self->offset.y = 0;

    self->inkEffect = INK_NONE;

    LRZSpikeBall_Draw();

    self->inkEffect  = INK_BLEND;
    self->velocity.y = -0x8000 * self->strength;

    do {
        self->offset.y += self->velocity.y;
        self->velocity.y += 0x3800;
    } while (self->velocity.y < 0 && self->offset.y < 0);

    self->position.x += self->offset.x;
    self->position.y += self->offset.y;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->position.x -= self->offset.x;
    self->position.y -= self->offset.y;
}

void LRZSpikeBall_EditorLoad(void) { LRZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZSpikeBall.bin", SCOPE_STAGE); }
#endif

void LRZSpikeBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, interval);
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, intervalOffset);
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, strength);
}

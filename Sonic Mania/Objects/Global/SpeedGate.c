// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpeedGate Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSpeedGate *SpeedGate;

void SpeedGate_Update(void)
{
    RSDK_THIS(SpeedGate);
    foreach_active(Player, player)
    {
        int32 vel = 0;
        if (player->onGround)
            vel = player->groundVel;
        else
            vel = player->velocity.x;
        int32 spd = vel >> 15;
        if (spd) {
            if (Player_CheckCollisionTouch(player, self, &SpeedGate->hitbox)) {
                int32 s = spd - 2;
                if (spd >= 0)
                    s = spd + 2;
                if (abs(s) > abs(self->angleVel))
                    self->angleVel = s;
                self->activeTimer = 0;
            }
        }
    }

    self->angle += self->angleVel;
    if (self->angleVel <= 0) {
        if (self->angle <= -0x200) {
            ++self->activeTimer;
            self->angle += 512;

            self->angleVel += 4;
            if (self->angleVel > -2)
                self->angleVel = -2;

            if (self->angleVel == -2) {
                self->angleVel = 0;
                self->angle    = 0;
            }
        }
    }
    else {
        if (self->angle >= 0x200) {
            ++self->activeTimer;
            self->angle -= 0x200;
            self->angleVel -= 4;
            if (self->angleVel < 2)
                self->angleVel = 2;

            if (self->angleVel == 2) {
                self->angleVel = 0;
                self->angle    = 0;
            }
        }
    }

    StateMachine_Run(self->state);
}

void SpeedGate_LateUpdate(void) {}

void SpeedGate_StaticUpdate(void) {}

void SpeedGate_Draw(void)
{
    RSDK_THIS(SpeedGate);
    Vector2 drawPos;

    self->rotation = self->baseRotation;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->topAnimator, NULL, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);

    drawPos.y    = self->position.y;
    self->drawFX = FX_SCALE;

    self->scale.x              = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                  = self->position.x + 0x30000;
    self->finsAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->scale.x              = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                  = self->position.x - 0x30000;
    self->finsAnimator.frameID = 0;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);
    drawPos.x                  = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finsAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->scale.x              = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                  = 0xB40 * RSDK.Cos512(self->angle & 0x7F) + self->position.x;
    self->finsAnimator.frameID = 2;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->scale.x              = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                  = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finsAnimator.frameID = 0;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->scale.x              = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                  = self->position.x - 0xB40 * RSDK.Sin512(self->angle & 0x7F);
    self->finsAnimator.frameID = 2;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->rotation = self->timerRotation;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->timerAnimator, &self->timerPos, false);

    self->drawFX = FX_NONE;
    if (self->state == SpeedGate_State_WaitForStart)
        RSDK.DrawSprite(&self->bubbleAnimator, NULL, false);
}

void SpeedGate_Create(void *data)
{
    RSDK_THIS(SpeedGate);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 1, &self->topAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 2, &self->timerAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 5, &self->bubbleAnimator, true, 0);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->playerDrawHigh;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->scale.y       = 0x200;
        if (self->timer <= 0) { //timer == 0 means finish line
            RSDK.SetSpriteAnimation(-1, 0, &self->timerAnimator, true, 0);
            RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 4, &self->finsAnimator, true, 0);
            self->timerPos.x = self->position.x;
            self->timerPos.y = self->position.y - 0x200000;
        }
        else { //otherwise its a start line
            RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 3, &self->finsAnimator, true, 0);
            self->timerPos.x = self->position.x;
            self->timerPos.y = self->position.y - 0x200000;
            self->state      = SpeedGate_State_WaitForStart;
        }
    }
}

void SpeedGate_StageLoad(void)
{
    SpeedGate->aniFrames     = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);
    SpeedGate->sfxStarPost   = RSDK.GetSfx("Global/StarPost.wav");
    SpeedGate->hitbox.left   = -8;
    SpeedGate->hitbox.top    = -44;
    SpeedGate->hitbox.right  = 8;
    SpeedGate->hitbox.bottom = 20;
}

void SpeedGate_State_WaitForStart(void)
{
    RSDK_THIS(SpeedGate);

    int32 playerBit = 1;
    foreach_active(Player, player)
    {
        if (!(playerBit & self->activePlayers)) {
            if (Player_CheckCollisionTouch(player, self, &SpeedGate->hitbox)) {
                self->state            = SpeedGate_State_ProcessGate;
                self->active           = ACTIVE_BOUNDS;
                self->playerPtr        = player;
                self->playerDistance.x = self->position.x - player->position.x;
                self->playerDistance.y = self->position.y - player->position.y;

                self->baseRotation = 0;
                if (self->angleVel < 0)
                    self->baseRotation = 0x200;
                RSDK.PlaySfx(SpeedGate->sfxStarPost, false, 255);
                foreach_return;
            }
        }
        playerBit <<= 1;
    }
}

void SpeedGate_State_ProcessGate(void)
{
    RSDK_THIS(SpeedGate);

    if (self->baseRotation > 0x100)
        self->baseRotation -= 0x08;
    else if (self->baseRotation < 0x100)
        self->baseRotation += 0x08;

    if (self->timerRotation < 0x100)
        self->timerRotation += 0x10;

    if (self->timer <= 0) {
        RSDK.SetSpriteAnimation(-1, 0, &self->timerAnimator, true, 0);
        self->active = ACTIVE_BOUNDS;
        self->state  = StateMachine_None;
    }
    else {
        self->timer--;
        EntityPlayer *player = self->playerPtr;
        if (self->playerPtr) {
            int32 vel = 0x100000;
            if (self->playerPtr->direction == FLIP_X)
                vel = -0x100000;

            self->velocity.x += +((int32)(vel - self->playerDistance.x) >> 6) - (self->velocity.x >> 4);
            self->velocity.y += ((int32)(-0x200000 - self->playerDistance.y) >> 6) - (self->velocity.y >> 4);

            self->playerDistance.x += self->velocity.x;
            self->playerDistance.y += self->velocity.y;
            self->timerPos.x = self->playerDistance.x;
            self->timerPos.y = self->playerDistance.y;
            self->timerPos.x += player->position.x;
            self->timerPos.y += player->position.y;

            EntitySpeedGate *finishLine = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, SpeedGate);
            if (Player_CheckCollisionTouch(player, finishLine, &SpeedGate->hitbox)) {
                RSDK.SetSpriteAnimation(-1, 0, &self->timerAnimator, true, 0);
                self->state               = StateMachine_None;
                self->angle               = 0;
                self->active              = ACTIVE_BOUNDS;
                finishLine->velocity.x    = player->velocity.x;
                finishLine->velocity.y    = -0x40000;
                finishLine->timerPos.x    = self->timerPos.x;
                finishLine->timerPos.y    = self->timerPos.y;
                finishLine->timerRotation = self->timerRotation;
                finishLine->active        = ACTIVE_NORMAL;
                finishLine->state         = SpeedGate_State_HandleFinished;
                RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 2, &finishLine->timerAnimator, true, 0);
                finishLine->baseRotation = 0;
                if (finishLine->angleVel < 0)
                    finishLine->baseRotation = 0x200;
                RSDK.PlaySfx(SpeedGate->sfxStarPost, false, 255);
            }
        }
    }
}

void SpeedGate_State_HandleFinished(void)
{
    RSDK_THIS(SpeedGate);
    int32 x = self->position.x;
    int32 y = self->position.y - 0x200000;

    self->velocity.x += ((x - self->timerPos.x) >> 5) - (self->velocity.x >> 3);
    self->velocity.y += ((y - self->timerPos.y) >> 5) - (self->velocity.y >> 3);
    self->timerPos.x += self->velocity.x;
    self->timerPos.y += self->velocity.y;

    if (self->baseRotation > 0x100) 
        self->baseRotation -= 0x08;
    else if (self->baseRotation < 0x100) 
        self->baseRotation += 0x08;

    if (self->timerRotation < 0x200) 
        self->timerRotation += 0x10;

    if (++self->timer == 48) {
        self->timerPos.x = self->position.x;
        self->timer      = 0;
        self->timerPos.y = y;
        self->active     = ACTIVE_BOUNDS;
        self->state      = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void SpeedGate_EditorDraw(void)
{
    RSDK_THIS(SpeedGate);
    Vector2 drawPos;

    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 1, &self->topAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 2, &self->timerAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 5, &self->bubbleAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpeedGate->aniFrames, 3, &self->finsAnimator, true, 0);

    self->rotation = self->baseRotation;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->topAnimator, NULL, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);

    drawPos.y    = self->position.y;
    self->drawFX = FX_SCALE;

    self->scale.x              = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                  = self->position.x + 0x30000;
    self->finsAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->scale.x              = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                  = self->position.x - 0x30000;
    self->finsAnimator.frameID = 0;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);
    drawPos.x                  = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finsAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->scale.x              = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                  = 0xB40 * RSDK.Cos512(self->angle & 0x7F) + self->position.x;
    self->finsAnimator.frameID = 2;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->scale.x              = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                  = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finsAnimator.frameID = 0;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->scale.x              = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                  = self->position.x - 0xB40 * RSDK.Sin512(self->angle & 0x7F);
    self->finsAnimator.frameID = 2;
    RSDK.DrawSprite(&self->finsAnimator, &drawPos, false);

    self->rotation = self->timerRotation;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->timerAnimator, &self->timerPos, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->bubbleAnimator, NULL, false);
}

void SpeedGate_EditorLoad(void)
{
    SpeedGate->aniFrames = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SpeedGate, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void SpeedGate_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, id);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_ENUM, prize);
    RSDK_EDITABLE_VAR(SpeedGate, VAR_UINT8, direction);
}

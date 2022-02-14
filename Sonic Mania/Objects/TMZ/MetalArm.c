// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MetalArm Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMetalArm *MetalArm;

void MetalArm_Update(void)
{
    RSDK_THIS(MetalArm);
    self->moveOffset.x = -self->armPosition.x;
    self->moveOffset.y = -self->armPosition.y;

    int32 timerA = self->moveTimer;
    if (self->durationA < self->moveTimer)
        timerA = self->durationA;
    int32 interpolateA = ((self->endAngleA - self->startAngleA) << 16) / self->durationA;
    self->armAngle.x   = (self->startAngleA << 16) + interpolateA * timerA;

    int32 timerB = self->moveTimer;
    if (self->durationB < self->moveTimer)
        timerB = self->durationB;
    int32 interpolateB = ((self->endAngleB - self->startAngleB) << 16) / self->durationB;
    self->armAngle.y   = (self->startAngleB << 16) + interpolateB * timerB;

    self->armPosition = MetalArm_GetArmPosition();
    self->armPosition.x &= 0xFFFF0000;
    self->armPosition.y &= 0xFFFF0000;
    self->position.x = self->armPosition.x;
    self->position.y = self->armPosition.y;
    self->moveOffset.x += self->position.x;
    self->moveOffset.y += self->position.y;
    MetalArm_CheckPlayerCollisions();
    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
    StateMachine_Run(self->state);
}

void MetalArm_LateUpdate(void) {}

void MetalArm_StaticUpdate(void) {}

void MetalArm_Draw(void)
{
    RSDK_THIS(MetalArm);
    self->rotation = 0;
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);

    int32 x = 0x2400 * RSDK.Cos512((self->armAngle.x >> 16)) + self->position.x;
    int32 y = 0x2400 * RSDK.Sin512((self->armAngle.x >> 16)) + self->position.y;

    Vector2 drawPos;
    drawPos            = MetalArm_GetArmPosition();
    self->position.x = x;
    self->position.y = y;
    self->position.x &= 0xFFFF0000;
    self->position.y &= 0xFFFF0000;
    self->rotation = (self->armAngle.x + self->armAngle.y) >> 16;
    RSDK.DrawSprite(&self->armBAnimator, NULL, false);

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
    self->position.x &= 0xFFFF0000;
    self->position.y &= 0xFFFF0000;
    self->rotation = (self->armAngle.x >> 16);
    RSDK.DrawSprite(&self->armAAnimator, NULL, false);

    self->rotation = 0;
    self->position = drawPos;
    self->position.x &= 0xFFFF0000;
    self->position.y &= 0xFFFF0000;
    RSDK.DrawSprite(&self->platformAnimator, &drawPos, false);

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

void MetalArm_Create(void *data)
{
    RSDK_THIS(MetalArm);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderHigh;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;

    if (!self->durationA)
        self->durationA = 60;

    if (!self->durationB)
        self->durationB = 40;

    if (!self->holdDuration)
        self->holdDuration = 60;

    self->hitbox.left   = -56;
    self->hitbox.top    = -27;
    self->hitbox.right  = 35;
    self->hitbox.bottom = -7;
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 1, &self->armAAnimator, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 2, &self->armBAnimator, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 3, &self->platformAnimator, true, 0);
    self->armAngle.x = self->startAngleA << 16;
    self->armAngle.y = self->startAngleB << 16;
    self->moveTimer      = 0;
    self->armPosition = MetalArm_GetArmPosition();
    self->state      = MetalArm_State_Idle;
}

void MetalArm_StageLoad(void)
{
    MetalArm->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MetalArm.bin", SCOPE_STAGE);
    Soundboard_LoadSFX("TMZ1/MetalArm.wav", true, MetalArm_SfxCheckCB, NULL);
}

bool32 MetalArm_SfxCheckCB(void)
{
    int32 activeCount = 0;
    foreach_active(MetalArm, arm)
    {
        if (arm->state == MetalArm_State_MoveToHold || arm->state == MetalArm_State_MoveToStart)
            ++activeCount;
    }
    return activeCount > 0;
}

Vector2 MetalArm_GetArmPosition(void)
{
    RSDK_THIS(MetalArm);
    Vector2 armPos;

    int32 x    = 0x2400 * RSDK.Cos512((self->armAngle.x >> 16)) + self->position.x;
    int32 y    = 0x2400 * RSDK.Sin512((self->armAngle.x >> 16)) + self->position.y;
    armPos.x = x + 0x3800 * RSDK.Cos512((self->armAngle.x + self->armAngle.y) >> 16);
    armPos.y = y + 0x3800 * RSDK.Sin512((self->armAngle.x + self->armAngle.y) >> 16);
    return armPos;
}

void MetalArm_CheckPlayerCollisions(void)
{
    RSDK_THIS(MetalArm);

    foreach_active(Player, player)
    {
        int32 id = RSDK.GetEntityID(player);
        if ((1 << id) & self->stoodPlayers) {
            player->position.x += self->moveOffset.x;
            player->position.y += self->moveOffset.y;
            player->position.y += 0x10000;
        }

        if (!Player_CheckCollisionPlatform(player, self, &self->hitbox))
            self->stoodPlayers &= ~(1 << id);
        else
            self->stoodPlayers |= (1 << id);
    }
}

void MetalArm_State_Idle(void)
{
    RSDK_THIS(MetalArm);
    if ((self->stoodPlayers & 1)) {
        self->moveTimer  = 0;
        self->holdTimer = 0;
        self->active = ACTIVE_NORMAL;
        self->state  = MetalArm_State_MoveToHold;
    }
}

void MetalArm_State_MoveToHold(void)
{
    RSDK_THIS(MetalArm);

    int32 duration = self->durationB;
    if (self->durationA > self->durationB)
        duration = self->durationA;
    if (self->moveTimer >= duration) {
        self->holdTimer = 0;
        self->state  = MetalArm_State_Holding;
    }
    else {
        self->moveTimer++;
    }
}

void MetalArm_State_Holding(void)
{
    RSDK_THIS(MetalArm);
    if (self->stoodPlayers & 1) {
        self->holdTimer = 0;
    }
    else {
        if (self->holdTimer >= self->holdDuration) {
            self->state = MetalArm_State_MoveToStart;
        }
        else {
            self->holdTimer++;
        }
    }
}

void MetalArm_State_MoveToStart(void)
{
    RSDK_THIS(MetalArm);
    if (self->moveTimer <= 0) {
        self->state  = MetalArm_State_Idle;
        self->active = ACTIVE_BOUNDS;
    }
    else {
        self->moveTimer--;
    }
}

#if RETRO_INCLUDE_EDITOR
void MetalArm_EditorDraw(void)
{
    RSDK_THIS(MetalArm);
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->armAngle.x = self->startAngleA << 16;
    self->armAngle.y = self->startAngleB << 16;
    self->armPosition = MetalArm_GetArmPosition();

    MetalArm_Draw();
}

void MetalArm_EditorLoad(void) { MetalArm->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MetalArm.bin", SCOPE_STAGE); }
#endif

void MetalArm_Serialize(void)
{
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, startAngleA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, startAngleB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, endAngleA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, endAngleB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, durationA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, durationB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, holdDuration);
}

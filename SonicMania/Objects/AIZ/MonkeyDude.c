// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MonkeyDude Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMonkeyDude *MonkeyDude;

void MonkeyDude_Update(void)
{
    RSDK_THIS(MonkeyDude);
    StateMachine_Run(self->state);
#if !MANIA_USE_PLUS
    for (self->armNodeID = 0; self->armNodeID < MonkeyDude_ArmJointCount; ++self->armNodeID) {
        StateMachine_Run(self->armStates[self->armNodeID]);
    }
#endif
}

void MonkeyDude_LateUpdate(void) {}

void MonkeyDude_StaticUpdate(void) {}

void MonkeyDude_Draw(void)
{
    RSDK_THIS(MonkeyDude);
#if MANIA_USE_PLUS
    if (self->state == MonkeyDude_State_Coconut) {
        RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
    }
    else {
#endif
        Vector2 drawPos;

        if (!self->direction)
            drawPos.x = self->position.x - 0xD0000;
        else
            drawPos.x = self->position.x + 0xD0000;

        if (!self->bodyAnimator.frameID)
            drawPos.y = self->position.y + -0x20000;
        else
            drawPos.y = self->position.y + -0x40000;
        RSDK.DrawSprite(&MonkeyDude->armAnimator, &drawPos, false);

        for (self->armNodeID = 0; self->armNodeID < MonkeyDude_ArmJointCount; ++self->armNodeID) {
            if (self->direction)
                drawPos.x += RSDK.Sin256(self->armAngles[self->armNodeID]) << 11;
            else
                drawPos.x -= RSDK.Sin256(self->armAngles[self->armNodeID]) << 11;

            drawPos.y += RSDK.Cos256(self->armAngles[self->armNodeID]) << 11;

            Animator *animator = NULL;
            if (self->armNodeID == MonkeyDude_ArmJointCount - 1) {
                if (self->throwCount >= 4)
                    animator = &self->handAnimator;
                else
                    animator = &MonkeyDude->coconutAnimator;
            }
            else {
                animator = &MonkeyDude->armAnimator;
            }
            RSDK.DrawSprite(animator, &drawPos, false);
        }

        RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

        drawPos.x = self->position.x;
        drawPos.y = self->armY;
        RSDK.DrawSprite(&MonkeyDude->tailAnimator, &drawPos, false);
#if MANIA_USE_PLUS
    }
#endif
}

void MonkeyDude_Create(void *data)
{
    RSDK_THIS(MonkeyDude);
    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

#if MANIA_USE_PLUS
    if (!self->nummoves)
        self->nummoves = 3;

    if (data) {
        RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 4, &self->bodyAnimator, true, 0);
        self->state = MonkeyDude_State_Coconut;
    }
    else {
#endif
        RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 0, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 3, &self->handAnimator, true, 0);
        self->state = MonkeyDude_State_Init;
#if MANIA_USE_PLUS
    }
#endif
}

void MonkeyDude_StageLoad(void)
{
#if MANIA_USE_PLUS
    MonkeyDude->aniFrames = RSDK.LoadSpriteAnimation("AIZ/MonkeyDude.bin", SCOPE_STAGE);

    MonkeyDude->hitboxBadnik.left   = -10;
    MonkeyDude->hitboxBadnik.top    = -21;
    MonkeyDude->hitboxBadnik.right  = 7;
    MonkeyDude->hitboxBadnik.bottom = 4;

    MonkeyDude->hitboxCoconut.left   = -4;
    MonkeyDude->hitboxCoconut.top    = -4;
    MonkeyDude->hitboxCoconut.right  = 4;
    MonkeyDude->hitboxCoconut.bottom = 4;
#else
    MonkeyDude->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/MonkeyDude.bin", SCOPE_STAGE);

    // strange hitbox, doesn't match either one above lol
    MonkeyDude->hitboxBadnik.left   = -8;
    MonkeyDude->hitboxBadnik.top    = -8;
    MonkeyDude->hitboxBadnik.right  = 8;
    MonkeyDude->hitboxBadnik.bottom = 8;
#endif

    RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 2, &MonkeyDude->armAnimator, true, 0);
    RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 1, &MonkeyDude->tailAnimator, true, 0);
    RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 4, &MonkeyDude->coconutAnimator, true, 0);

#if MANIA_USE_PLUS
    MonkeyDude->sfxDrop = RSDK.GetSfx("Stage/Drop.wav");
#endif

    DEBUGMODE_ADD_OBJ(MonkeyDude);
}

void MonkeyDude_DebugDraw(void)
{
#if MANIA_USE_PLUS
    RSDK_THIS(MonkeyDude);

    RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
#endif
}

void MonkeyDude_DebugSpawn(void)
{
#if MANIA_USE_PLUS
    RSDK_THIS(MonkeyDude);

    EntityMonkeyDude *monkeyDude = CREATE_ENTITY(MonkeyDude, NULL, self->position.x, self->position.y);
    monkeyDude->direction        = self->direction;
    monkeyDude->startDir         = self->direction;
#endif
}

void MonkeyDude_State_Init(void)
{
    RSDK_THIS(MonkeyDude);
    self->armY           = self->position.y;
    self->active         = ACTIVE_NORMAL;
    self->velocity.y     = 0x10000;
    self->moveCount      = 1;
    self->timer          = 1;
    self->angleVel       = 4;
    self->activeArmNodes = 0;
    self->throwCount     = 0;
    self->state          = MonkeyDude_State_Laugh;
#if MANIA_USE_PLUS
    RSDK.ProcessAnimation(&self->bodyAnimator);
#else
    self->bodyAnimator.frameID = (self->coconutFrame++ >> 3) & 1;
#endif
    if (--self->timer <= 0) {
        self->coconutFrame = 0;
        self->timer        = 8;
        self->state        = MonkeyDude_State_MoveArm;
    }
#if MANIA_USE_PLUS
    MonkeyDude_HandleStates();
#endif

    int32 delay = 0;
    for (int32 i = 0; i < MonkeyDude_ArmJointCount; ++i) {
        self->armAngles[i] = 0;
        self->armTimers[i] = delay;
        self->armStates[i] = MonkeyDude_StateBody_ArmRaise;

        delay += 24 / (MonkeyDude_ArmJointCount - 1);
    }
}

void MonkeyDude_HandleBodyPart(void)
{
    RSDK_THIS(MonkeyDude);
    self->armTimers[self->armNodeID] = self->armNodeID;
    switch (self->throwCount) {
        case 0:
        case 4:
            self->angleVel = 4;
            self->armTimers[self->armNodeID] *= 4;
            self->armMoveDir[self->armNodeID] ^= 1;

            if (!self->armNodeID) {
                self->currentAngle = self->armAngles[0];
                if (self->armMoveDir[0]) {
                    self->currentAngle -= RSDK.Rand(0, 96);
                    if (self->currentAngle < 32)
                        self->currentAngle = 32;
                }
                else {
                    self->currentAngle += RSDK.Rand(0, 96);
                    if (self->currentAngle > 128)
                        self->currentAngle = 128;
                }
            }
            if (self->armNodeID == MonkeyDude_ArmJointCount - 1)
                self->activeArmNodes = 0;
            break;

        case 1:
            self->angleVel = 5;
            self->armTimers[self->armNodeID] *= 2;
            self->currentAngle                = 176;
            self->armMoveDir[self->armNodeID] = 0;
            if (self->armNodeID == MonkeyDude_ArmJointCount - 1) {
                ++self->throwCount;
                self->activeArmNodes = 0;
            }
            break;

        case 2:
            self->armTimers[self->armNodeID] *= 2;
            self->currentAngle                = 96;
            self->armMoveDir[self->armNodeID] = 1;
            if (self->armNodeID == MonkeyDude_ArmJointCount - 1) {
                ++self->throwCount;
                self->activeArmNodes = 0;
            }
            break;
    }

    if (self->armNodeID == MonkeyDude_ArmJointCount - 1)
        self->activeArmNodes = 0;
}

#if MANIA_USE_PLUS
void MonkeyDude_HandleStates(void)
{
    RSDK_THIS(MonkeyDude);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &MonkeyDude->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }

    if (self->classID) {
        if (self->throwCount) {
            if (self->throwCount == 3 && self->armAngles[MonkeyDude_ArmJointCount - 1] <= 164) {
                int32 spawnX = self->position.x + 0xD0000;
                if (!self->direction)
                    spawnX = self->position.x - 0xD0000;
                int32 spawnY = self->position.y;

                for (int32 i = 0; i < MonkeyDude_ArmJointCount; ++i) {
                    if (self->direction)
                        spawnX += RSDK.Sin256(self->armAngles[i]) << 11;
                    else
                        spawnX -= RSDK.Sin256(self->armAngles[i]) << 11;
                    spawnY += (RSDK.Cos256(self->armAngles[i]) << 11);
                }

                EntityMonkeyDude *coconut = CREATE_ENTITY(MonkeyDude, INT_TO_VOID(1), spawnX, spawnY);
                if (!self->direction)
                    coconut->velocity.x = -0x20000;
                else
                    coconut->velocity.x = 0x20000;
                coconut->velocity.y = -0x40000;
                coconut->direction  = self->direction;
                RSDK.PlaySfx(MonkeyDude->sfxDrop, false, 0xFF);
                ++self->throwCount;
            }
        }
        else {
            EntityPlayer *target = Player_GetNearestPlayer();
            if (!self->direction) {
                if (target->position.x < self->position.x) {
                    if (abs(self->position.x - target->position.x) <= 0x800000) {
                        if (target->position.y > self->position.y - 0x200000 && target->position.y - self->position.y <= 0x800000
                            && self->onScreen == 1) {
                            self->throwCount   = 1;
                            self->currentAngle = self->armAngles[0];
                        }
                    }
                }
            }
        }

        for (self->armNodeID = 0; self->armNodeID < MonkeyDude_ArmJointCount; ++self->armNodeID) {
            StateMachine_Run(self->armStates[self->armNodeID]);
        }

        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            self->position  = self->startPos;
            self->direction = self->startDir;
            MonkeyDude_Create(NULL);
        }
    }
}
#endif

void MonkeyDude_State_Laugh(void)
{
    RSDK_THIS(MonkeyDude);
#if MANIA_USE_PLUS
    RSDK.ProcessAnimation(&self->bodyAnimator);
#else
    self->bodyAnimator.frameID = (self->coconutFrame++ >> 3) & 1;
#endif
    if (--self->timer <= 0) {
        self->coconutFrame = 0;
        self->timer        = 8;
        self->state        = MonkeyDude_State_MoveArm;
    }
#if MANIA_USE_PLUS
    MonkeyDude_HandleStates();
#endif
}

void MonkeyDude_State_MoveArm(void)
{
    RSDK_THIS(MonkeyDude);
    self->armY += self->velocity.y;
    self->bodyAnimator.frameID = 0;
    if (--self->timer <= 0) {
        self->timer = 8;
        self->state = MonkeyDude_State_MoveBody;
    }
#if MANIA_USE_PLUS
    MonkeyDude_HandleStates();
#endif
}

void MonkeyDude_State_MoveBody(void)
{
    RSDK_THIS(MonkeyDude);
    self->position.y += self->velocity.y;
    if (--self->timer <= 0) {
        if (--self->moveCount > 0) {
            self->timer = 8;
            self->state = MonkeyDude_State_MoveArm;
        }
        else {
            self->timer      = 60;
            self->velocity.y = -self->velocity.y;
            self->moveCount  = self->nummoves;
            self->state      = MonkeyDude_State_Laugh;
        }
    }
#if MANIA_USE_PLUS
    MonkeyDude_HandleStates();
#endif
}

void MonkeyDude_StateBody_ArmRaise(void)
{
    RSDK_THIS(MonkeyDude);
    if (self->activeArmNodes == ((1 << MonkeyDude_ArmJointCount) - 1)) {
        self->armStates[self->armNodeID] = MonkeyDude_StateBody_Throw;
        MonkeyDude_HandleBodyPart();
    }

    if (self->armTimers[self->armNodeID]) {
        self->armTimers[self->armNodeID]--;
    }
    else if (self->armAngles[self->armNodeID] >= 128) {
        self->activeArmNodes |= (1 << self->armNodeID);
    }
    else {
        self->armAngles[self->armNodeID] += self->angleVel;
    }
}

void MonkeyDude_StateBody_Throw(void)
{
    RSDK_THIS(MonkeyDude);
    if (self->activeArmNodes == ((1 << MonkeyDude_ArmJointCount) - 1))
        MonkeyDude_HandleBodyPart();

    if (self->armTimers[self->armNodeID]) {
        self->armTimers[self->armNodeID]--;
    }
    else {
        if (self->armMoveDir[self->armNodeID]) {
            if (self->armAngles[self->armNodeID] <= self->currentAngle) {
                self->activeArmNodes |= (1 << self->armNodeID);
            }
            else {
                self->armAngles[self->armNodeID] -= self->angleVel;
            }
        }
        else {
            if (self->armAngles[self->armNodeID] >= self->currentAngle) {
                self->activeArmNodes |= (1 << self->armNodeID);
            }
            else {
                self->armAngles[self->armNodeID] += self->angleVel;
            }
        }
    }
}

#if MANIA_USE_PLUS
void MonkeyDude_State_Coconut(void)
{
    RSDK_THIS(MonkeyDude);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ProcessAnimation(&self->bodyAnimator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &MonkeyDude->hitboxCoconut))
                Player_ProjectileHurt(player, self);
        }
    }
    else {
        destroyEntity(self);
    }
}
#endif

#if GAME_INCLUDE_EDITOR
void MonkeyDude_EditorDraw(void) { MonkeyDude_Draw(); }

void MonkeyDude_EditorLoad(void)
{
#if MANIA_USE_PLUS
    MonkeyDude->aniFrames = RSDK.LoadSpriteAnimation("AIZ/MonkeyDude.bin", SCOPE_STAGE);
#else
    MonkeyDude->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/MonkeyDude.bin", SCOPE_STAGE);
#endif
}
#endif

void MonkeyDude_Serialize(void) { RSDK_EDITABLE_VAR(MonkeyDude, VAR_ENUM, nummoves); }

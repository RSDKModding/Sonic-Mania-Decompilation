// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DDWrecker Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDDWrecker *DDWrecker;

void DDWrecker_Update(void)
{
    RSDK_THIS(DDWrecker);
    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->animator);

    if (self->type != DDWRECKER_CORE) {
        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        StateMachine_Run(self->stateBall);
    }
}

void DDWrecker_LateUpdate(void) {}

void DDWrecker_StaticUpdate(void) {}

void DDWrecker_Draw(void)
{
    RSDK_THIS(DDWrecker);
    if (self->type == DDWRECKER_BALL1 || self->type == DDWRECKER_BALL2) {
        RSDK.SetActivePalette(1, 0, ScreenInfo->size.y);

        self->direction = self->animator.frameID >= 16;
        if (!(self->invincibilityTimer & 2)) {
            RSDK.SetLimitedFade(1, 2, 3, self->blendAmount, 1, 28);
        }
        else {
            for (int32 i = 1; i < 0x1C; ++i) {
                RSDK.SetPaletteEntry(1, i, 0xF0F0F0);
            }
        }
    }
    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK.SetActivePalette(0, 0, ScreenInfo->size.y);
}

void DDWrecker_Create(void *data)
{
    RSDK_THIS(DDWrecker);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK || globals->gameMode >= MODE_TIMEATTACK) {
            destroyEntity(self);
        }
        else if (data) {
            self->type   = VOID_TO_INT(data);
            self->active = ACTIVE_NORMAL;
            switch (self->type) {
                case DDWRECKER_BALL1: // main body
                case DDWRECKER_BALL2:
                    RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, true, 0);
                    self->drawFX        = FX_FLIP | FX_ROTATE;
                    self->hitbox.left   = -20;
                    self->hitbox.top    = -20;
                    self->hitbox.right  = 20;
                    self->hitbox.bottom = 20;
                    self->health        = 3;
                    break;

                case DDWRECKER_CHAIN: // chains
                    RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 4, &self->animator, true, 0);
                    break;

                case DDWRECKER_CORE: // core
                    RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 5, &self->animator, true, 0);
                    break;

                default: break;
            }

            self->visible   = true;
            self->drawGroup = Zone->objectDrawGroup[0];
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->visible       = false;
            self->state         = DDWrecker_State_SetupArena;
        }
    }
}

void DDWrecker_StageLoad(void)
{
    DDWrecker->aniFrames = RSDK.LoadSpriteAnimation("GHZ/DDWrecker.bin", SCOPE_STAGE);

    DDWrecker->sfxBossHit   = RSDK.GetSfx("Stage/BossHit.wav");
    DDWrecker->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    DDWrecker->sfxDrop      = RSDK.GetSfx("Stage/Drop.wav");
    DDWrecker->sfxImpact    = RSDK.GetSfx("Stage/Impact2.wav");
    DDWrecker->sfxAssemble  = RSDK.GetSfx("Stage/Assemble.wav");
    DDWrecker->sfxRotate    = RSDK.GetSfx("Stage/Rotate.wav");
    DDWrecker->sfxSharp     = RSDK.GetSfx("Stage/Sharp.wav");
}

void DDWrecker_State_SetupArena(void)
{
    RSDK_THIS(DDWrecker);
    if (++self->timer >= 8) {
        self->timer = 0;

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsL[0]      = FROM_FIXED(self->position.x) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0]      = FROM_FIXED(self->position.x) + ScreenInfo->center.x;
        Zone->cameraBoundsB[0]      = FROM_FIXED(self->position.y);

        DDWrecker->camBoundL  = self->position.x + ((160 - ScreenInfo->center.x) << 16);
        DDWrecker->camBoundR  = self->position.x + ((ScreenInfo->center.x - 160) << 16);
        DDWrecker->bossBoundL = self->position.x + ((32 - ScreenInfo->center.x) << 16);
        DDWrecker->bossBoundR = self->position.x + ((ScreenInfo->center.x - 32) << 16);
        DDWrecker->bossBoundT = self->position.y - 0xC00000;

        DDWrecker->attackVelocities[0] = CLAMP(ScreenInfo->center.x - 168, 0, 24);
        DDWrecker->attackVelocities[0] = (DDWrecker->attackVelocities[0] + 32) << 11;
        DDWrecker->attackVelocities[1] = DDWrecker->attackVelocities[0] >> 2;
        DDWrecker->attackVelocities[2] = 288 * DDWrecker->attackVelocities[0] >> 8;

        self->state = DDWrecker_State_InitChildren;
    }
}

void DDWrecker_State_InitChildren(void)
{
    RSDK_THIS(DDWrecker);
    if (self->timer) {
        self->timer++;
        if (self->timer == 60) {
            EntityDDWrecker *chain1 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, DDWrecker);
            RSDK.ResetEntity(chain1, DDWrecker->classID, INT_TO_VOID(DDWRECKER_CHAIN));
            chain1->position.x = self->position.x;
            chain1->position.y = self->position.y + 0x400000;

            EntityDDWrecker *chain2 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, DDWrecker);
            RSDK.ResetEntity(chain2, DDWrecker->classID, INT_TO_VOID(DDWRECKER_CHAIN));
            chain2->position.x = self->position.x;
            chain2->position.y = self->position.y + 0x400000;

            EntityDDWrecker *chain3 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, DDWrecker);
            RSDK.ResetEntity(chain3, DDWrecker->classID, INT_TO_VOID(DDWRECKER_CHAIN));
            chain3->position.x = self->position.x;
            chain3->position.y = self->position.y + 0x400000;

            EntityDDWrecker *chain4 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 5, DDWrecker);
            RSDK.ResetEntity(chain4, DDWrecker->classID, INT_TO_VOID(DDWRECKER_CHAIN));
            chain4->position.x = self->position.x;
            chain4->position.y = self->position.y + 0x400000;

            EntityDDWrecker *core = RSDK_GET_ENTITY(SceneInfo->entitySlot + 3, DDWrecker);
            RSDK.ResetEntity(core, DDWrecker->classID, INT_TO_VOID(DDWRECKER_CORE));
            core->position.x = self->position.x;
            core->position.y = self->position.y;
            core->position.y += 0x200000;
            core->originPos.x = self->position.x;
            core->originPos.y = self->position.y;
            core->originPos.y -= 0x800000;
            core->velocity.y      = -0x98000;
            core->siblingSlots[0] = SceneInfo->entitySlot + 6; // Ball 1
            core->siblingSlots[1] = SceneInfo->entitySlot + 1; // Chain 1
            core->siblingSlots[2] = SceneInfo->entitySlot + 2; // Chain 2
            core->siblingSlots[3] = SceneInfo->entitySlot + 4; // Chain 3
            core->siblingSlots[4] = SceneInfo->entitySlot + 5; // Chain 4
            core->siblingSlots[5] = SceneInfo->entitySlot + 7; // Ball 2
            core->bodyA           = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, DDWrecker);
            core->bodyB           = RSDK_GET_ENTITY(SceneInfo->entitySlot + 7, DDWrecker);
            core->state           = DDWrecker_State_Assemble;

            EntityDDWrecker *ball1 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, DDWrecker);
            RSDK.ResetEntity(ball1, DDWrecker->classID, INT_TO_VOID(DDWRECKER_BALL1));
            ball1->position.x = self->position.x;
            ball1->position.y = self->position.y;
            ball1->position.y += 0x400000;
            ball1->siblingSlots[1] = SceneInfo->entitySlot + 2; // Chain 1
            ball1->siblingSlots[2] = SceneInfo->entitySlot + 3; // Chain 2
            ball1->siblingSlots[0] = SceneInfo->entitySlot + 1; // Core
            ball1->siblingSlots[3] = SceneInfo->entitySlot + 4; // Chain 3
            ball1->siblingSlots[4] = SceneInfo->entitySlot + 5; // Chain 4
            ball1->siblingSlots[5] = SceneInfo->entitySlot + 7; // Ball 2
            ball1->bodyA           = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, DDWrecker);
            ball1->bodyB           = RSDK_GET_ENTITY(SceneInfo->entitySlot + 7, DDWrecker);
            ball1->radius          = 64;

            EntityDDWrecker *ball2 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 7, DDWrecker);
            RSDK.ResetEntity(ball2, DDWrecker->classID, INT_TO_VOID(DDWRECKER_BALL2));
            ball2->position.x = self->position.x;
            ball2->position.y = self->position.y;
            ball2->position.y += 0x400000;
            ball2->siblingSlots[0] = SceneInfo->entitySlot + 1; // Chain 1
            ball2->siblingSlots[1] = SceneInfo->entitySlot + 2; // Chain 2
            ball2->siblingSlots[2] = SceneInfo->entitySlot + 3; // Core
            ball2->siblingSlots[3] = SceneInfo->entitySlot + 4; // Chain 3
            ball2->siblingSlots[4] = SceneInfo->entitySlot + 5; // Chain 4
            ball2->siblingSlots[5] = SceneInfo->entitySlot + 6; // Ball 1
            ball2->bodyB           = RSDK_GET_ENTITY(SceneInfo->entitySlot + 7, DDWrecker);
            ball2->bodyA           = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, DDWrecker);
            ball2->radius          = 64;

            destroyEntity(self);
        }
    }
    else {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (player->position.x > self->position.x) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++self->timer;
        }
    }
}

void DDWrecker_State_Assemble(void)
{
    RSDK_THIS(DDWrecker);

    self->velocity.y += 0x4000;
    self->position.y += self->velocity.y;

    if (self->position.y > self->originPos.y && self->velocity.y > 0) {
        self->position.y = self->originPos.y;
        self->velocity.y = 0;
        self->radius     = 2048;
        self->spinTimer  = 128;
        self->state      = DDWrecker_State_EnterWreckers;
        RSDK.PlaySfx(DDWrecker->sfxAssemble, false, 255);
    }
}

void DDWrecker_State_EnterWreckers(void)
{
    RSDK_THIS(DDWrecker);

    self->spinAngle = (self->spinAngle + (self->spinTimer >> 1)) & 0x3FF;
    self->radius += (64 - self->radius) >> 4;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    DDWrecker_Spin();

    if (self->spinTimer <= 0) {
        self->timer = 30;
        self->state = DDWrecker_State_AttackDelay;
        foreach_active(DDWrecker, child)
        {
            if (child->type == DDWRECKER_BALL1 || child->type == DDWRECKER_BALL2)
                child->stateBall = DDWrecker_StateBall_Vulnerable;
        }
    }
    else {
        self->spinTimer--;
    }
}

void DDWrecker_State_AttackDelay(void)
{
    RSDK_THIS(DDWrecker);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    DDWrecker_Spin();

    if (--self->timer < 1) {
        if (self->bodyA->position.y - self->bodyB->position.y < 0) {
            self->bodyA->originPos.x = self->originPos.x;
            self->bodyA->originPos.y = self->originPos.y - 0x400000;
            self->bodyA->velocity.x  = 0;
            self->bodyA->angle       = self->angle;
            self->bodyA->spinAngle   = 0;
            self->bodyA->timer       = 4;
            self->bodyA->state       = DDWrecker_State_SwingLeft;
        }
        else {
            self->bodyB->originPos.x = self->originPos.x;
            self->bodyB->originPos.y = self->originPos.y - 0x400000;
            self->bodyB->velocity.x  = 0;
            self->bodyB->angle       = self->angle;
            self->bodyB->spinAngle   = 512;
            self->bodyB->timer       = 4;
            self->bodyB->state       = DDWrecker_State_SwingRight;
        }

        if (self->swapBalls) {
            self->bodyA->stateBall = DDWrecker_StateBall_Vulnerable;
            self->bodyB->stateBall = DDWrecker_StateBall_Spiked;
        }
        else {
            self->bodyA->stateBall = DDWrecker_StateBall_Spiked;
            self->bodyB->stateBall = DDWrecker_StateBall_Vulnerable;
        }

        RSDK.PlaySfx(DDWrecker->sfxSharp, false, 255);
        self->state = StateMachine_None;
    }
}

void DDWrecker_State_SwingRight(void)
{
    RSDK_THIS(DDWrecker);

    if (self->spinAngle < 0x200) {
        self->position.x += self->velocity.x;
        if (self->position.x < DDWrecker->camBoundL)
            self->position.x = DDWrecker->camBoundL;
    }

    self->spinAngle += 4;
    self->spinAngle &= 0x3FF;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    DDWrecker_Swing();

    if (self->spinAngle >= 0x300) {
        if (--self->timer <= 0) {
            EntityDDWrecker *core = RSDK_GET_ENTITY(self->siblingSlots[2], DDWrecker);
            core->velocity.x      = (core->originPos.x - core->position.x) >> 6;
            core->velocity.y      = (core->originPos.y - core->position.y) >> 6;
            core->spinTimer       = -2;
            core->spinAngle       = self->spinAngle;
            core->angle           = 0;
            core->timer           = 64;
            core->state           = DDWrecker_State_SwingMoveToCenter;
            if (self == self->bodyB)
                core->spinAngle += 0x200;
            self->state = StateMachine_None;

            self->bodyA->stateBall = DDWrecker_StateBall_Spiked;
            self->bodyB->stateBall = DDWrecker_StateBall_Spiked;
            RSDK.PlaySfx(DDWrecker->sfxSharp, false, 255);
        }
        else {
            if (self->velocity.x)
                self->velocity.x = DDWrecker->attackVelocities[0];
            else
                self->velocity.x = DDWrecker->attackVelocities[1];

            self->state = DDWrecker_State_SwingLeft;

            if (self->timer == 2) {
                if (self->bodyA->stateBall == DDWrecker_StateBall_Vulnerable)
                    self->bodyA->stateBall = DDWrecker_StateBall_Spiked;
                else
                    self->bodyA->stateBall = DDWrecker_StateBall_Vulnerable;

                if (self->bodyB->stateBall == DDWrecker_StateBall_Vulnerable)
                    self->bodyB->stateBall = DDWrecker_StateBall_Spiked;
                else
                    self->bodyB->stateBall = DDWrecker_StateBall_Vulnerable;
                RSDK.PlaySfx(DDWrecker->sfxSharp, false, 0xFF);
            }
        }
    }
}

void DDWrecker_State_SwingLeft(void)
{
    RSDK_THIS(DDWrecker);

    if (self->spinAngle >= 0x200) {
        self->position.x += self->velocity.x;
        if (self->position.x > DDWrecker->camBoundR)
            self->position.x = DDWrecker->camBoundR;
    }

    self->spinAngle += 4;
    self->spinAngle &= 0x3FF;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    DDWrecker_Swing();

    if (self->spinAngle >= 0x100 && self->spinAngle < 0x200) {
        if (--self->timer <= 0) {
            EntityDDWrecker *core = RSDK_GET_ENTITY(self->siblingSlots[2], DDWrecker);
            core->velocity.x      = (core->originPos.x - core->position.x) >> 6;
            core->velocity.y      = (core->originPos.y - core->position.y) >> 6;
            core->spinTimer       = 2;
            core->spinAngle       = self->spinAngle;
            core->angle           = 0;
            core->timer           = 64;
            core->state           = DDWrecker_State_SwingMoveToCenter;
            if (self == self->bodyB)
                core->spinAngle += 512;
            self->state = StateMachine_None;

            self->bodyA->stateBall = DDWrecker_StateBall_Spiked;
            self->bodyB->stateBall = DDWrecker_StateBall_Spiked;
            RSDK.PlaySfx(DDWrecker->sfxSharp, false, 0xFF);
        }
        else {
            if (self->velocity.x)
                self->velocity.x = -DDWrecker->attackVelocities[0];
            else
                self->velocity.x = -DDWrecker->attackVelocities[1];

            self->state = DDWrecker_State_SwingRight;

            if (self->timer == 2) {
                if (self->bodyA->stateBall == DDWrecker_StateBall_Vulnerable)
                    self->bodyA->stateBall = DDWrecker_StateBall_Spiked;
                else
                    self->bodyA->stateBall = DDWrecker_StateBall_Vulnerable;

                if (self->bodyB->stateBall == DDWrecker_StateBall_Vulnerable)
                    self->bodyB->stateBall = DDWrecker_StateBall_Spiked;
                else
                    self->bodyB->stateBall = DDWrecker_StateBall_Vulnerable;
                RSDK.PlaySfx(DDWrecker->sfxSharp, false, 0xFF);
            }
        }
    }
}

void DDWrecker_State_SwingMoveToCenter(void)
{
    RSDK_THIS(DDWrecker);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->velocity.x > 0) {
        if (self->position.x > self->originPos.x) {
            self->velocity.x = 0;
            self->position.x = self->originPos.x;
        }
    }
    else if (self->velocity.x < 0) {
        if (self->position.x < self->originPos.x) {
            self->velocity.x = 0;
            self->position.x = self->originPos.x;
        }
    }

    if (self->velocity.y < 0) {
        if (self->position.y < self->originPos.y) {
            self->velocity.y = 0;
            self->position.y = self->originPos.y;
        }
    }
    else if (self->velocity.y > 0) {
        if (self->position.y > self->originPos.y) {
            self->velocity.y = 0;
            self->position.y = self->originPos.y;
        }
    }

    if (!self->velocity.x && !self->velocity.y)
        self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    if (self->spinTimer <= 0) {
        if (self->spinTimer > -141) {
            self->spinTimer--;
        }
        else {
            self->state = DDWrecker_State_HandleSpinning;
            RSDK.PlaySfx(DDWrecker->sfxAssemble, false, 255);
        }
    }
    else if (self->spinTimer >= 144) {
        self->state = DDWrecker_State_HandleSpinning;
        RSDK.PlaySfx(DDWrecker->sfxAssemble, false, 255);
    }
    else {
        self->spinTimer++;
    }

    self->spinAngle = (self->spinAngle + (self->spinTimer >> 2)) & 0x3FF;

    DDWrecker_Spin();
}

void DDWrecker_State_HandleSpinning(void)
{
    RSDK_THIS(DDWrecker);

    self->spinAngle  = (self->spinAngle + (self->spinTimer >> 2)) & 0x3FF;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    DDWrecker_Spin();

    if (!--self->timer) {
        self->state = DDWrecker_State_SwingSlowDown;

        self->bodyA->stateBall = DDWrecker_StateBall_Vulnerable;
        self->bodyB->stateBall = DDWrecker_StateBall_Vulnerable;
    }
}

void DDWrecker_State_SwingSlowDown(void)
{
    RSDK_THIS(DDWrecker);

    if (self->spinTimer <= 0) {
        if (self->spinTimer < -8)
            self->spinTimer++;
    }
    else {
        if (self->spinTimer > 8)
            self->spinTimer--;
    }

    self->spinAngle  = (self->spinAngle + (self->spinTimer >> 2)) & 0x3FF;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    DDWrecker_Spin();

    if (!(self->spinAngle & 0x1FC)) {
        if (abs(self->spinTimer) <= 8) {
            self->spinTimer = 0;
            self->timer     = 30;
            self->spinAngle = (self->spinAngle + 0x80) & 0x300;
            self->state     = DDWrecker_State_AttackDelay;
        }
    }
}

void DDWrecker_State_PrepareBounceAttack(void)
{
    RSDK_THIS(DDWrecker);

    self->velocity.y -= 0x800;
    self->position.x += self->velocity.x;
    self->originPos.y += self->velocity.y;

    if (self->originPos.y < DDWrecker->bossBoundT) {
        self->originPos.y = DDWrecker->bossBoundT;
        self->velocity.y  = 0;
    }

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    self->angleVel = 8;
    if (self->position.x <= DDWrecker->bossBoundL) {
        self->position.x = DDWrecker->bossBoundL;
        self->velocity.x = 0;

        if (self->originPos.y <= DDWrecker->bossBoundT)
            self->state = DDWrecker_State_SignalBounceAttackStart;
    }
    else if (self->position.x >= DDWrecker->bossBoundR) {
        self->position.x = DDWrecker->bossBoundR;
        self->velocity.x = 0;

        if (self->originPos.y <= DDWrecker->bossBoundT)
            self->state = DDWrecker_State_SignalBounceAttackStart;
    }

    if (self->rotation < 0) {
        if (self->rotation + 2 > 0)
            self->rotation = 0;
        else
            self->rotation += 2;
    }
    else if (self->rotation > 0) {
        self->rotation -= 2;
        if (self->rotation < 0)
            self->rotation = 0;
    }

    if (self->animator.animationID == 2) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, true, 0);
    }
    else {
        if (self->animator.animationID == 3 && !self->animator.frameID)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 2, &self->animator, true, 0);
    }
}

void DDWrecker_State_SignalBounceAttackStart(void)
{
    RSDK_THIS(DDWrecker);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    if (self->animator.animationID) {
        if (self->animator.animationID == 1) {
            if (self->animator.frameID == self->animator.frameCount - 1)
                RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 3, &self->animator, true, 0);
        }
        else {
            if (self->animator.animationID == 3) {
                if (self->animator.speed < 0x100) {
                    self->animator.speed += 2;
                }
            }
        }
    }
    else {
        RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 1, &self->animator, true, 0);
        RSDK.PlaySfx(DDWrecker->sfxSharp, false, 255);
    }

    if (++self->timer == 30) {
        self->timer      = 0;
        self->velocity.y = -0x20000;
        self->state      = DDWrecker_State_HandleBounceAttack;
        RSDK.PlaySfx(DDWrecker->sfxDrop, false, 255);
    }
}

void DDWrecker_State_HandleBounceAttack(void)
{
    RSDK_THIS(DDWrecker);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->animator.speed = 0x100 - 32 * self->timer;

    if (self->velocity.y > 0) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, 0, 0, 0, 0x180000, true)) {
            ++self->timer;
            Camera_ShakeScreen(0, 0, 3);
            RSDK.PlaySfx(DDWrecker->sfxImpact, false, 255);
            self->velocity.y = (self->velocity.y >> 3) - self->velocity.y;

            if (self->timer >= 2) {
                if (self->timer == 3) {
                    self->velocity.y  = 0;
                    self->timer       = 0;
                    self->originPos.y = self->position.y;
                    self->angleVel    = 8;
                    self->state       = DDWrecker_State_EndBounceAttack;
                }
            }
            else {
                if (self->position.x - (DDWrecker->bossBoundL + 0x100000) > 0)
                    self->velocity.x = -DDWrecker->attackVelocities[2];
                else
                    self->velocity.x = DDWrecker->attackVelocities[2];
            }
        }
    }
}

void DDWrecker_State_EndBounceAttack(void)
{
    RSDK_THIS(DDWrecker);

    if (self->animator.animationID == 2) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, true, 0);
    }
    else {
        if (self->animator.animationID == 3 && !self->animator.frameID)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 2, &self->animator, true, 0);
    }

    if (++self->timer == 30) {
        self->timer = 0;
        self->state = DDWrecker_State_PrepareBounceAttack;
    }
}

void DDWrecker_StateBall_Vulnerable(void)
{
    RSDK_THIS(DDWrecker);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            DDWrecker_Hit();
            RSDK.PlaySfx(DDWrecker->sfxBossHit, false, 255);
        }
    }

    if (self->angleVel > 0) {
        if (self->angleVel > 8)
            self->angleVel--;

        self->unusedAngle += self->angleVel;
        if (self->unusedAngle >= 0x400) {
            self->unusedAngle = 0;
            self->angleVel    = 0;
        }
    }

    if (self->animator.animationID == 2) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, true, 0);
    }
    else if (self->animator.animationID == 3 && !self->animator.frameID) {
        RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 2, &self->animator, true, 0);
    }

    if (self->blendAmount > 0)
        self->blendAmount -= 0x10;
}

void DDWrecker_StateBall_Spiked(void)
{
    RSDK_THIS(DDWrecker);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox)) {
            if (player->invincibleTimer || player->blinkTimer > 0 || self->animator.animationID < 3) {
                if (Player_CheckBossHit(player, self)) {
                    DDWrecker_Hit();
                    RSDK.PlaySfx(DDWrecker->sfxBossHit, false, 255);
                }
            }
            else {
                Player_Hurt(player, self);
            }
        }
    }

    if (self->animator.animationID) {
        if (self->animator.animationID == 1) {
            if (self->animator.frameID == self->animator.frameCount - 1)
                RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 3, &self->animator, true, 0);
        }
        else if (self->animator.animationID == 3) {
            if (self->animator.speed < 0x100)
                self->animator.speed += 2;
        }
    }
    else {
        RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 1, &self->animator, true, 0);
    }

    if (self->angleVel < 48)
        self->angleVel += 2;

    self->unusedAngle = (self->unusedAngle + self->angleVel) & 0x3FF;

    if (self->blendAmount < 0x100)
        self->blendAmount += 0x10;
}

void DDWrecker_StateBall_Partnerless(void)
{
    RSDK_THIS(DDWrecker);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox)) {
            if (player->invincibleTimer || player->blinkTimer > 0 || self->animator.animationID < 3) {
                if (Player_CheckBossHit(player, self)) {
                    DDWrecker_Hit();
                    RSDK.PlaySfx(DDWrecker->sfxBossHit, false, 255);
                }
            }
            else {
                Player_Hurt(player, self);
            }
        }
    }

    self->unusedAngle = (self->unusedAngle + self->angleVel) & 0x3FF;

    if (self->animator.animationID == 0) {
        if (self->blendAmount > 0)
            self->blendAmount -= 0x10;
    }
    else if (self->blendAmount < 0x100)
        self->blendAmount += 0x10;
}
void DDWrecker_Hit(void)
{
    RSDK_THIS(DDWrecker);

    if (--self->health <= 0) {
        self->state     = DDWrecker_State_Die;
        self->stateBall = StateMachine_None;
        self->timer     = 0;
        foreach_active(DDWrecker, child)
        {
            if (self != child) {
                switch (child->type) {
                    case DDWRECKER_BALL1:
                    case DDWRECKER_BALL2:
                        if (child->stateBall) {
                            child->state       = DDWrecker_State_EndBounceAttack;
                            child->originPos.y = child->position.y;
                            child->velocity.x  = -DDWrecker->attackVelocities[2];
                            child->stateBall   = DDWrecker_StateBall_Partnerless;
                        }
                        break;

                    case DDWRECKER_CHAIN:
                    case DDWRECKER_CORE:
                        if (child->state != DDWrecker_State_Debris) {
                            child->state      = DDWrecker_State_Debris;
                            child->velocity.x = RSDK.Rand(-0x20000, 0x20000);
                            child->velocity.y = RSDK.Rand(-0x20000, 0x20000);
                        }
                        break;

                    default: break;
                }
            }
        }
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(DDWrecker->sfxBossHit, false, 255);
    }
}
void DDWrecker_Spin(void)
{
    RSDK_THIS(DDWrecker);

    for (int32 i = 0; i < 6; ++i) {
        EntityDDWrecker *chain = RSDK_GET_ENTITY(self->siblingSlots[i], DDWrecker);
        chain->position.x      = self->position.x + (DDWrecker->spinOffset[i] * RSDK.Sin1024(self->spinAngle) * self->radius);
        chain->position.y      = self->position.y + (DDWrecker->spinOffset[i] * RSDK.Cos1024(self->spinAngle) * self->radius);

        int32 rot = chain->rotation;
        if (rot < 0) {
            if (rot < 0) {
                rot += 2;
                if (rot > 0)
                    chain->rotation = 0;
                else
                    chain->rotation = rot;
            }
        }
        else if (rot > 0) {
            rot -= 2;
            if (rot - 2 < 0)
                chain->rotation = 0;
            else
                chain->rotation = rot;
        }
    }
}
void DDWrecker_Swing(void)
{
    RSDK_THIS(DDWrecker);
    int32 angle    = RSDK.Sin1024(self->spinAngle) >> 2;
    self->rotation = RSDK.Sin1024(-self->spinAngle) >> 6;

    if (!self->angleVel)
        self->unusedAngle = RSDK.Sin1024(-self->spinAngle) >> 5;

    EntityDDWrecker *chain = NULL;
    for (int32 i = 0; i < 6; ++i) {
        chain             = RSDK_GET_ENTITY(self->siblingSlots[i], DDWrecker);
        chain->position.x = self->position.x + (DDWrecker->swingOffset[i] * RSDK.Sin1024(angle) * self->radius);
        chain->position.y = self->position.y + (DDWrecker->swingOffset[i] * RSDK.Cos1024(angle) * self->radius);
    }
    chain->rotation = RSDK.Sin1024(-self->spinAngle) >> 5;
}

void DDWrecker_Explode(void)
{
    RSDK_THIS(DDWrecker);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(DDWrecker->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x                    = self->position.x + (RSDK.Rand(-20, 20) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-20, 20) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);

            explosion->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void DDWrecker_State_Debris(void)
{
    RSDK_THIS(DDWrecker);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void DDWrecker_State_Die(void)
{
    RSDK_THIS(DDWrecker);

    DDWrecker_Explode();

    if (++self->timer == 80) {
        int32 cnt = 0;
        foreach_active(DDWrecker, child) { ++cnt; }

        if (cnt != 1) {
            destroyEntity(self);
        }
        else {
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
            self->timer            = 0;
            self->visible          = false;
            self->state            = DDWrecker_State_SpawnSignpost;
            SceneInfo->timeEnabled = false;
            Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        }
    }
}
void DDWrecker_State_SpawnSignpost(void)
{
    RSDK_THIS(DDWrecker);

    if (++self->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->state      = SignPost_State_Falling;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void DDWrecker_EditorDraw(void)
{
    RSDK_THIS(DDWrecker);

    RSDK.SetSpriteAnimation(DDWrecker->aniFrames, 0, &self->animator, false, 0);

    RSDK.DrawSprite(&self->animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 0, 1 | 0 | 4 | 8, 0x00C0F0);

        int32 slot = RSDK.GetEntitySlot(self);
        for (int32 i = 0; i < 6; ++i) {
            EntityDDWrecker *child = RSDK_GET_ENTITY(slot + 1 + i, DDWrecker);

            if (child)
                DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xFFFF00, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void DDWrecker_EditorLoad(void) { DDWrecker->aniFrames = RSDK.LoadSpriteAnimation("GHZ/DDWrecker.bin", SCOPE_STAGE); }
#endif

void DDWrecker_Serialize(void) {}

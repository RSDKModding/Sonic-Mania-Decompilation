// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MeterDroid Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMeterDroid *MeterDroid;

void MeterDroid_Update(void)
{
    RSDK_THIS(MeterDroid);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);
}

void MeterDroid_LateUpdate(void) {}

void MeterDroid_StaticUpdate(void) {}

void MeterDroid_Draw(void)
{
    RSDK_THIS(MeterDroid);

    if (self->bgFadeAmount > 0)
        RSDK.SetLimitedFade(0, 1, 3, self->bgFadeAmount, 32, 47);
    else if (self->invincibilityTimer & 1)
        RSDK.CopyPalette(2, 32, 0, 32, 16);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }

    if (self->bgFadeAmount > 0 || (self->invincibilityTimer & 1))
        RSDK.CopyPalette(1, 32, 0, 32, 16);
}

void MeterDroid_Create(void *data)
{
    RSDK_THIS(MeterDroid);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(self);
        }
        else {
            self->origin        = self->position;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->health        = 6;
            self->drawGroup     = Zone->objectDrawGroup[0];
            self->state         = MeterDroid_State_Init;

            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 8, &self->propellorAnimator, true, 0);
        }
    }
}

void MeterDroid_StageLoad(void)
{
    MeterDroid->aniFrames = RSDK.LoadSpriteAnimation("OOZ/MeterDroid.bin", SCOPE_STAGE);

    MeterDroid->hitboxBoss.left   = -14;
    MeterDroid->hitboxBoss.top    = -14;
    MeterDroid->hitboxBoss.right  = 14;
    MeterDroid->hitboxBoss.bottom = 14;

    MeterDroid->hitboxPropellor.left   = 14;
    MeterDroid->hitboxPropellor.top    = -12;
    MeterDroid->hitboxPropellor.right  = 32;
    MeterDroid->hitboxPropellor.bottom = 12;

    MeterDroid->hitboxWrench.left   = -14;
    MeterDroid->hitboxWrench.top    = -14;
    MeterDroid->hitboxWrench.right  = 14;
    MeterDroid->hitboxWrench.bottom = 14;

    MeterDroid->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    MeterDroid->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    MeterDroid->sfxDestroy   = RSDK.GetSfx("Global/Destroy.wav");
    MeterDroid->sfxToss      = RSDK.GetSfx("OOZ/Toss.wav");
    MeterDroid->sfxGrab      = RSDK.GetSfx("Global/Grab.wav");
    MeterDroid->sfxValve     = RSDK.GetSfx("OOZ/Valve.wav");
    MeterDroid->sfxWrench    = RSDK.GetSfx("OOZ/Wrench.wav");
}

void MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, self, &MeterDroid->hitboxBoss) && Player_CheckBossHit(player, self)) {
                MeterDroid_Hit();
            }

            if (!self->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, self, &MeterDroid->hitboxPropellor)) {
#if MANIA_USE_PLUS
                    if (!Player_CheckMightyUnspin(player, 0x400, false, &player->uncurlTimer))
#endif
                        Player_Hurt(player, self);
                }
            }
        }
    }

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;
}

void MeterDroid_CheckPlayerCollisions_Wrench_NoFlip(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;

        if (!self->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, self, &MeterDroid->hitboxBoss) && Player_CheckBossHit(player, self)) {
                MeterDroid_Hit();
            }

            if (!self->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, self, &MeterDroid->hitboxPropellor)) {
#if MANIA_USE_PLUS
                    if (!Player_CheckMightyUnspin(player, 0x400, false, &player->uncurlTimer))
#endif
                        Player_Hurt(player, self);
                }
            }
        }

        self->position.x = self->wrenchPos.x;
        self->position.y = self->wrenchPos.y;

        if (Player_CheckCollisionTouch(player, self, &MeterDroid->hitboxWrench)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x600, false, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void MeterDroid_CheckPlayerCollisions_NoWrench_NoFlip(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, self, &MeterDroid->hitboxBoss) && Player_CheckBossHit(player, self)) {
                MeterDroid_Hit();
            }

            if (!self->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, self, &MeterDroid->hitboxPropellor)) {
#if MANIA_USE_PLUS
                    if (!Player_CheckMightyUnspin(player, 0x400, false, &player->uncurlTimer))
#endif
                        Player_Hurt(player, self);
                }
            }
        }
    }
}

void MeterDroid_Hit(void)
{
    RSDK_THIS(MeterDroid);

    if (--self->health <= 0) {
        self->state = MeterDroid_State_Destroyed;
        self->timer = 0;

        if (self->stateDraw == MeterDroid_Draw_ThrownWrench) {
            int32 x = RSDK.Rand(MeterDroid->hitboxBoss.left, MeterDroid->hitboxBoss.right) << 16;
            int32 y = RSDK.Rand(MeterDroid->hitboxBoss.top, MeterDroid->hitboxBoss.bottom) << 16;

            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), x + self->wrenchPos.x, y + self->wrenchPos.y)->drawGroup = Zone->objectDrawGroup[1];
            self->stateDraw                                                                                                = MeterDroid_Draw_Normal;
        }

        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(MeterDroid->sfxHit, false, 0xFF);
    }
}

void MeterDroid_Explode(void)
{
    RSDK_THIS(MeterDroid);

    if (!(Zone->timer % 3)) {
        if (self->state == MeterDroid_State_FinishAct) {
            if (self->timer < 180) {
                if (self->timer <= 60)
                    RSDK.PlaySfx(MeterDroid->sfxExplosion, Zone->timer % 3, 255);
                else
                    RSDK.PlaySfx(MeterDroid->sfxDestroy, Zone->timer % 3, 255);
            }
        }
        else {
            RSDK.PlaySfx(MeterDroid->sfxExplosion, false, 255);
        }

        if (Zone->timer & 4) {
            int32 x                    = (RSDK.Rand(-208, 208) + ScreenInfo->center.x + ScreenInfo->position.x) << 16;
            int32 y                    = (RSDK.Rand(-112, 112) + ScreenInfo->center.y + ScreenInfo->position.y) << 16;
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(2 * (RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1];
        }
    }
}

void MeterDroid_FindTargetValve(void)
{
    RSDK_THIS(MeterDroid);

    EntityValve *valves[5];
    for (int32 i = 0; i < 5; ++i) valves[i] = NULL;

    int32 id = 1;
    foreach_active(Valve, valve) { valves[id++] = valve; }

    valves[0]         = valves[RSDK.Rand(1, id)];
    self->targetValve = valves[0];
    self->direction   = valves[0]->direction ^ FLIP_X;
    self->targetPos.x = valves[0]->position.x;
    self->targetPos.y = valves[0]->position.y;

    self->targetPos.x += valves[0]->direction == FLIP_NONE ? 0x2E0000 : -0x2E0000;
    self->targetPos.y -= 0xC0000;

    self->moveVel.x = (self->targetPos.x - self->position.x) >> 11;
    self->moveVel.y = (self->targetPos.y - self->position.y) >> 11;

    self->velocity.x = 0;
    self->velocity.y = 0;
}

void MeterDroid_PopPlatforms(void)
{
    int32 delay = 30;

    foreach_active(GasPlatform, platform)
    {
        platform->type  = GASPLATFORM_BOSS;
        platform->timer = delay;
        platform->state = GasPlatform_State_Shaking;
        delay += 16;
    }
}

void MeterDroid_Draw_Normal(void)
{
    RSDK_THIS(MeterDroid);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->propellorAnimator, NULL, false);
}

void MeterDroid_Draw_SpinningValve(void)
{
    RSDK_THIS(MeterDroid);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->armAnimator, NULL, false);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->propellorAnimator, NULL, false);
}

void MeterDroid_Draw_ThrownWrench(void)
{
    RSDK_THIS(MeterDroid);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->propellorAnimator, NULL, false);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->wrenchAnimator, &self->wrenchPos, false);
}

void MeterDroid_State_Init(void)
{
    RSDK_THIS(MeterDroid);

    if (++self->timer >= 8) {
        self->timer = 0;

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsT[0]      = Zone->cameraBoundsB[0] - ScreenInfo->size.y - 64;

        MeterDroid->boundsL = (Zone->cameraBoundsL[0] + 64) << 16;
        MeterDroid->boundsR = (Zone->cameraBoundsR[0] - 64) << 16;
        MeterDroid->boundsM = self->position.x;
        MeterDroid->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
        MeterDroid->boundsB = (Zone->cameraBoundsB[0] - 8) << 16;

        self->position.x += 0x800000;
        self->visible   = true;
        self->stateDraw = MeterDroid_Draw_Normal;
        self->state     = MeterDroid_State_StartFight;
    }
}

void MeterDroid_State_StartFight(void)
{
    RSDK_THIS(MeterDroid);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 4, 10);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    if (self->timer) {
        if (++self->timer == 120) {
            self->timer = 0;
            self->state = MeterDroid_State_Idle;
        }
    }
    else {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (player1->position.x > self->position.x - 0x800000) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++self->timer;
        }
    }
}

void MeterDroid_State_Idle(void)
{
    RSDK_THIS(MeterDroid);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 4, 10);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    if (++self->timer == 90) {
        self->timer = 0;
        self->state = MeterDroid_State_PickMoveDir;
    }

    MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip();
}

void MeterDroid_State_PickMoveDir(void)
{
    RSDK_THIS(MeterDroid);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 4, 10);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    if (++self->timer == 60) {
        self->timer      = 0;
        self->moveRadius = 0;
        self->angle      = 64;

        if (self->position.x < MeterDroid->boundsM) {
            self->direction = FLIP_X;
            self->state     = MeterDroid_State_MoveLeft;
        }
        else {
            self->direction = FLIP_NONE;
            self->state     = MeterDroid_State_MoveRight;
        }
    }

    MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip();
}

void MeterDroid_State_MoveRight(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    self->position.x += (self->moveRadius * RSDK.Cos256(self->angle)) >> 8;
    self->position.y += (self->moveRadius * RSDK.Sin256(self->angle)) >> 8;

    if (self->angle > 0x80) {
        self->moveRadius -= 0x1400;
        if (self->position.y < self->origin.y)
            self->position.y = self->origin.y;
    }
    else {
        self->moveRadius += 0x1400;
    }

    if (++self->angle == 0xC0) {
        self->origin.x = self->position.x;
        self->origin.y = self->position.y;
        self->angle    = 0;
        self->state    = MeterDroid_State_ThrowWrench;
    }

    MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip();
}

void MeterDroid_State_MoveLeft(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    self->position.x += (self->moveRadius * RSDK.Cos256(self->angle)) >> 8;
    self->position.y += (self->moveRadius * RSDK.Sin256(self->angle)) >> 8;

    if (self->angle < 0) {
        self->moveRadius -= 0x1400;

        if (self->position.y < self->origin.y)
            self->position.y = self->origin.y;
    }
    else {
        self->moveRadius += 0x1400;
    }

    if (--self->angle == -0x40) {
        self->origin.x = self->position.x;
        self->origin.y = self->position.y;
        self->angle    = 0;
        self->state    = MeterDroid_State_ThrowWrench;
    }

    MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip();
}

void MeterDroid_State_ThrowWrench(void)
{
    RSDK_THIS(MeterDroid);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 4, 10);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);
    RSDK.ProcessAnimation(&self->wrenchAnimator);

    if (++self->timer >= 18) {
        if (self->mainAnimator.animationID == 2 && self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 0, &self->mainAnimator, true, 0);
    }
    else if (self->timer == 16) {
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 2, &self->mainAnimator, true, 0);
    }

    if (self->timer == 36) {
        self->wrenchPos.x = self->position.x + (self->direction == FLIP_NONE ? -0x300000 : 0x300000);
        self->wrenchPos.y = self->position.y - 0x100000;

        self->targetPos.x = self->wrenchPos.x;
        self->targetPos.y = self->wrenchPos.y;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        self->wrenchMoveVel.x = (player1->position.x + 16 * player1->velocity.x - self->wrenchPos.x) >> 5;
        self->wrenchMoveVel.y = (player1->position.y + 16 * player1->velocity.y - self->wrenchPos.y) >> 5;
        self->wrenchMoveInc.x = -(self->wrenchMoveVel.x >> 6);
        self->wrenchMoveInc.y = -(self->wrenchMoveVel.y >> 6);

        RSDK.PlaySfx(MeterDroid->sfxToss, false, 255);
        RSDK.PlaySfx(MeterDroid->sfxWrench, false, 255);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 9, &self->wrenchAnimator, true, 0);

        self->stateDraw = MeterDroid_Draw_ThrownWrench;
    }

    if (self->stateDraw == MeterDroid_Draw_ThrownWrench) {
        self->wrenchMoveVel.x += self->wrenchMoveInc.x;
        self->wrenchMoveVel.y += self->wrenchMoveInc.y;
        self->wrenchPos.x += self->wrenchMoveVel.x;
        self->wrenchPos.y += self->wrenchMoveVel.y;

        if (self->timer == 60)
            self->state = MeterDroid_State_ThrownWrench;

        MeterDroid_CheckPlayerCollisions_Wrench_NoFlip();
    }
    else {
        MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip();
    }
}

void MeterDroid_State_ThrownWrench(void)
{
    RSDK_THIS(MeterDroid);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 4, 10);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);
    RSDK.ProcessAnimation(&self->wrenchAnimator);

    self->wrenchMoveVel.x += self->wrenchMoveInc.x;
    self->wrenchMoveVel.y += self->wrenchMoveInc.y;
    self->wrenchPos.x += self->wrenchMoveVel.x;
    self->wrenchPos.y += self->wrenchMoveVel.y;

    int32 rx = abs(self->targetPos.x - self->wrenchPos.x) >> 16;
    int32 ry = abs(self->targetPos.y - self->wrenchPos.y) >> 16;

    if (rx * rx + ry * ry < 0x200) {
        RSDK.PlaySfx(MeterDroid->sfxGrab, false, 0xFF);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 3, &self->mainAnimator, true, 0);

        self->stateDraw = MeterDroid_Draw_Normal;
        self->state     = MeterDroid_State_CaughtWrench;
    }

    MeterDroid_CheckPlayerCollisions_Wrench_NoFlip();
}

void MeterDroid_State_CaughtWrench(void)
{
    RSDK_THIS(MeterDroid);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 4, 10);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip();

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &self->mainAnimator, true, 0);
        MeterDroid_FindTargetValve();
        self->state = MeterDroid_State_MoveToValve;
    }

    MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip();
}

void MeterDroid_State_MoveToValve(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    if (abs(self->velocity.x) < 0x40000) {
        if (abs(self->velocity.x) < 0x40000)
            self->velocity.x += self->moveVel.x;
    }

    self->velocity.y += self->moveVel.y;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int32 rx = abs(self->targetPos.x - self->position.x) >> 16;
    int32 ry = abs(self->targetPos.y - self->position.y) >> 16;

    if (rx * rx + ry * ry < 96) {
        self->direction = self->targetValve->direction;
        self->drawGroup = Zone->objectDrawGroup[0] - 1;
        self->state     = MeterDroid_State_MoveIntoBG;
    }

    MeterDroid_CheckPlayerCollisions_NoWrench_NoFlip();
}

void MeterDroid_State_MoveIntoBG(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    self->bgFadeAmount += 10;

    self->velocity.x = self->velocity.x - (self->velocity.x >> 3);
    self->velocity.y = self->velocity.y - (self->velocity.y >> 3);
    self->position.x += ((self->targetPos.x - self->position.x) >> 3) + self->velocity.x;
    self->position.y += ((self->targetPos.y - self->position.y) >> 3) + self->velocity.y;

    if (++self->timer == 24) {
        self->position.x = self->targetPos.x;
        self->position.y = self->targetPos.y;
        self->timer      = 0;

        EntityValve *valve = self->targetValve;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 4, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 6, &self->armAnimator, true, 0);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 0, &valve->valveAnimator, true, 0);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 2, &valve->wheelAnimator, true, 0);
        RSDK.PlaySfx(MeterDroid->sfxValve, false, 255);

        self->state     = MeterDroid_State_TurningValve;
        self->stateDraw = MeterDroid_Draw_SpinningValve;
    }
    MeterDroid_CheckPlayerCollisions_NoWrench_NoFlip();
}

void MeterDroid_State_TurningValve(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    if (++self->timer == 60)
        MeterDroid_PopPlatforms();

    if (self->timer == 90) {
        self->timer = 0;

        EntityValve *valve = self->targetValve;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 5, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 7, &self->armAnimator, true, 0);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 1, &valve->valveAnimator, true, 0);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 3, &valve->wheelAnimator, true, 0);

        self->state = MeterDroid_State_StopTurningValve;
    }
}

void MeterDroid_State_StopTurningValve(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->armAnimator);
    RSDK.ProcessAnimation(&self->propellorAnimator);

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
        self->state = MeterDroid_State_WatchPlatformsPopUp;
}

void MeterDroid_State_WatchPlatformsPopUp(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&self->propellorAnimator);

    ++self->timer;
    if (self->timer > 30) {
        if (self->bgFadeAmount > 0)
            self->bgFadeAmount -= 16;
    }

    if (self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &self->mainAnimator, true, 0);

        self->origin.x  = self->position.x;
        self->origin.y  = self->position.y;
        self->drawGroup = Zone->objectDrawGroup[0];
        self->state     = MeterDroid_State_PickMoveDir;
        self->stateDraw = MeterDroid_Draw_Normal;
    }
}

void MeterDroid_State_Destroyed(void)
{
    RSDK_THIS(MeterDroid);

    MeterDroid_Explode();

    if (++self->timer == 60) {
        MeterDroid->debrisSpeeds[2]  = self->direction;
        MeterDroid->debrisSpeeds[6]  = self->direction;
        MeterDroid->debrisSpeeds[10] = self->direction;
        MeterDroid->debrisSpeeds[14] = self->direction;

        if (self->direction == FLIP_X) {
            MeterDroid->debrisSpeeds[3]  = -MeterDroid->debrisSpeeds[3];
            MeterDroid->debrisSpeeds[7]  = -MeterDroid->debrisSpeeds[7];
            MeterDroid->debrisSpeeds[11] = -MeterDroid->debrisSpeeds[11];
            MeterDroid->debrisSpeeds[15] = -MeterDroid->debrisSpeeds[15];
        }

        Debris_CreateFromEntries(MeterDroid->aniFrames, MeterDroid->debrisSpeeds, 10);
        self->visible = false;
    }
    else if (self->timer == 90) {
        self->timer   = 0;
        self->visible = false;
        Music_FadeOut(0.025);
        CREATE_ENTITY(OOZFlames, NULL, self->position.x, self->position.y);
        self->state = MeterDroid_State_FinishAct;
    }
}

void MeterDroid_State_FinishAct(void)
{
    RSDK_THIS(MeterDroid);

    MeterDroid_Explode();

    if (++self->timer == 180) {
        for (int32 p = 0; p < Player->playerCount; ++p) StarPost->postIDs[p] = 0;

        SaveGame_SavePlayerState();
        globals->enableIntro = true;
        ++SceneInfo->listPos;
        if (!RSDK.CheckValidScene())
            RSDK.SetScene("Presentation", "Title Screen");

        Zone_StartFadeOut(10, 0xF0F0F0);
    }

    if (self->timer == 240)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void MeterDroid_EditorDraw(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &self->mainAnimator, false, 0);
    RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 8, &self->propellorAnimator, false, 0);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE - 64, WIDE_SCR_XCENTER, 0, 1 | 0 | 4 | 0, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void MeterDroid_EditorLoad(void) { MeterDroid->aniFrames = RSDK.LoadSpriteAnimation("OOZ/MeterDroid.bin", SCOPE_STAGE); }
#endif

void MeterDroid_Serialize(void) {}

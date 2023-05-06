// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Drillerdroid Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDrillerdroid *Drillerdroid;

void Drillerdroid_Update(void)
{
    RSDK_THIS(Drillerdroid);

    StateMachine_Run(self->state);
}

void Drillerdroid_LateUpdate(void) {}

void Drillerdroid_StaticUpdate(void)
{
    if (Drillerdroid->drillSfxTimer) {
        if (!Drillerdroid->playingDrillSfx) {
            RSDK.PlaySfx(Drillerdroid->sfxDrill, 43643, 0xFF);
            Drillerdroid->playingDrillSfx = true;
        }

        Drillerdroid->drillSfxTimer = 0;
    }
    else if (Drillerdroid->playingDrillSfx) {
        RSDK.StopSfx(Drillerdroid->sfxDrill);
        Drillerdroid->playingDrillSfx = false;
    }
}

void Drillerdroid_Draw(void)
{
    RSDK_THIS(Drillerdroid);

    StateMachine_Run(self->stateDraw);
}

void Drillerdroid_Create(void *data)
{
    RSDK_THIS(Drillerdroid);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->visible = true;
            if (data)
                self->type = VOID_TO_INT(data);

            switch (self->type) {
                case DRILLERDROIDO_MAIN:
                    self->active        = ACTIVE_BOUNDS;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &self->armorAnimator, true, 0);
                    self->drawGroup    = Zone->objectDrawGroup[1];
                    Drillerdroid->boss = self;
                    self->state        = Drillerdroid_State_SetupArena;
                    self->stateDraw    = Drillerdroid_Draw_Boss;
                    break;

                case DRILLERDROID_TARGET:
                case DRILLERDROID_UNUSED:
                    self->active        = ACTIVE_NORMAL;
                    self->drawFX        = FX_FLIP;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 2, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 3, &self->armorAnimator, true, 0);
                    self->drawGroup = Zone->objectDrawGroup[1];
                    self->state     = Drillerdroid_State_Target;
                    self->stateDraw = Drillerdroid_Draw_Target;
                    break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void Drillerdroid_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Drillerdroid->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);

    Drillerdroid->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    Drillerdroid->hitboxCore.left   = -25;
    Drillerdroid->hitboxCore.top    = -16;
    Drillerdroid->hitboxCore.right  = 25;
    Drillerdroid->hitboxCore.bottom = 16;

    Drillerdroid->hitboxPistonL.left   = -58;
    Drillerdroid->hitboxPistonL.top    = -74;
    Drillerdroid->hitboxPistonL.right  = -25;
    Drillerdroid->hitboxPistonL.bottom = 16;

    Drillerdroid->hitboxPistonR.left   = 25;
    Drillerdroid->hitboxPistonR.top    = -74;
    Drillerdroid->hitboxPistonR.right  = 58;
    Drillerdroid->hitboxPistonR.bottom = 16;

    Drillerdroid->hitboxDrillL.left   = -58;
    Drillerdroid->hitboxDrillL.top    = 16;
    Drillerdroid->hitboxDrillL.right  = -25;
    Drillerdroid->hitboxDrillL.bottom = 53;

    Drillerdroid->hitboxDrillR.left   = 25;
    Drillerdroid->hitboxDrillR.top    = 16;
    Drillerdroid->hitboxDrillR.right  = 58;
    Drillerdroid->hitboxDrillR.bottom = 53;

    Drillerdroid->pistonPos[0] = 0;
    Drillerdroid->pistonPos[1] = 0;

    Drillerdroid->drillPos[0] = 0x100000;
    Drillerdroid->drillPos[1] = 0x100000;

    Drillerdroid->pistonDelay[0] = 4;
    Drillerdroid->pistonDelay[1] = 0;

    Drillerdroid->drillDelay[0] = 4;
    Drillerdroid->drillDelay[1] = 0;

    Drillerdroid->pistonMoveDir[0] = 0;
    Drillerdroid->pistonMoveDir[1] = 0;

    Drillerdroid->drillMoveDir[0] = 0;
    Drillerdroid->drillMoveDir[1] = 0;

    Drillerdroid->platformActive[0] = true;
    Drillerdroid->platformActive[1] = true;
    Drillerdroid->platformActive[2] = true;
    Drillerdroid->platformActive[3] = true;
    Drillerdroid->platformActive[4] = true;

    Drillerdroid->active = ACTIVE_ALWAYS;

    Drillerdroid->drillSfxTimer   = 0;
    Drillerdroid->playingDrillSfx = false;

    Drillerdroid->armorHealth    = 3;
    Drillerdroid->jumpsRemaining = 0;

    Drillerdroid->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    Drillerdroid->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    Drillerdroid->sfxDrill     = RSDK.GetSfx("LRZ/Drill.wav");
    Drillerdroid->sfxImpact    = RSDK.GetSfx("Stage/Impact4.wav");
    Drillerdroid->sfxJump      = RSDK.GetSfx("LRZ/DrillJump.wav");
    Drillerdroid->sfxTargeting = RSDK.GetSfx("Stage/Targeting1.wav");
    Drillerdroid->sfxSizzle    = RSDK.GetSfx("LRZ/Sizzle.wav");
    Drillerdroid->sfxDrop      = RSDK.GetSfx("Stage/Drop.wav");
    Drillerdroid->sfxFail      = RSDK.GetSfx("Stage/Fail.wav");
    Drillerdroid->sfxClang     = RSDK.GetSfx("Stage/Clang2.wav");
}

void Drillerdroid_Hit(void)
{
    RSDK_THIS(Drillerdroid);

    if (--self->health > 0) {
        RSDK.PlaySfx(Drillerdroid->sfxHit, false, 255);
        self->invincibilityTimer = 45;
    }
    else {
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->invincibilityTimer = 60;
        self->state              = Drillerdroid_State_Destroyed;
    }
}

void Drillerdroid_CheckPlayerCollisions(void)
{
    RSDK_THIS(Drillerdroid);

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer) {
                int32 playerID = RSDK.GetEntitySlot(player);

                if (Drillerdroid->playerTimers[playerID]) {
                    Drillerdroid->playerTimers[playerID]--;
                }
                else if (Player_CheckBadnikTouch(player, self, &Drillerdroid->hitboxCore)) {
                    if (player->position.y < self->position.y) {
                        Player_Hurt(player, self);
                    }
                    else if (self->alpha > 0x80 && player->shield != SHIELD_FIRE && !player->blinkTimer && !player->invincibleTimer) {
                        Player_Hurt(player, self);
                    }

                    if (Player_CheckBossHit(player, self)) {
                        if (Drillerdroid->armorHealth <= 1) {
                            Drillerdroid_Hit();
                        }
                        else {
                            RSDK.PlaySfx(Drillerdroid->sfxClang, false, 255);
                            Drillerdroid->playerTimers[playerID] = 15;
                        }
                    }
                }
                else {
                    if (Player_CheckBadnikTouch(player, self, &Drillerdroid->hitboxPistonL)
                        || Player_CheckBadnikTouch(player, self, &Drillerdroid->hitboxPistonR)) {
                        if (self->alpha > 0x80 && player->shield != SHIELD_FIRE && !player->blinkTimer && !player->invincibleTimer) {
                            Player_Hurt(player, self);
                        }
                        else if (Player_CheckBossHit(player, self)) {
                            if (!Drillerdroid->armorHealth) {
                                Drillerdroid_Hit();
                            }
                            else {
                                RSDK.PlaySfx(Drillerdroid->sfxClang, false, 255);
                                Drillerdroid->playerTimers[playerID] = 15;
                            }
                        }
                    }
                    else if (Player_CheckCollisionTouch(player, self, &Drillerdroid->hitboxDrillL)
                             || Player_CheckCollisionTouch(player, self, &Drillerdroid->hitboxDrillR)) {
                        Player_Hurt(player, self);
                    }
                }
            }
        }
    }
}

void Drillerdroid_Explode(void)
{
    RSDK_THIS(Drillerdroid);

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(Drillerdroid->sfxExplosion, false, 255);

        if (!(Zone->timer & 3)) {
            int32 x = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y = self->position.y + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void Drillerdroid_SpawnDebris(int32 offset)
{
    RSDK_THIS(Drillerdroid);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x      = RSDK.Rand(0, 6) << 15;
    debris->velocity.y      = RSDK.Rand(-12, -8) << 15;
    debris->gravityStrength = 0x3800;
    debris->drawGroup       = Zone->objectDrawGroup[0];
    debris->updateRange.x   = 0x400000;
    debris->updateRange.y   = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x      = RSDK.Rand(-6, 0) << 15;
    debris->velocity.y      = RSDK.Rand(-12, -8) << 15;
    debris->gravityStrength = 0x3800;
    debris->drawGroup       = Zone->objectDrawGroup[0];
    debris->updateRange.x   = 0x400000;
    debris->updateRange.y   = 0x400000;
}

void Drillerdroid_State_SetupArena(void)
{
    RSDK_THIS(Drillerdroid);

    if (++self->timer >= 2) {
        self->timer = 0;

        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + 324;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 96;
        Zone->cameraBoundsT[0]      = Zone->cameraBoundsB[0] - SCREEN_YSIZE;

        self->startY     = self->position.y;
        self->active     = ACTIVE_NORMAL;
        self->position.y = (ScreenInfo->position.y - 192) << 16;
        self->visible    = true;
        self->state      = Drillerdroid_State_AwaitPlayer;
    }
}

void Drillerdroid_State_AwaitPlayer(void)
{
    RSDK_THIS(Drillerdroid);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    if (player1->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - 328;

        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        self->health                                                                                          = 6;
        CREATE_ENTITY(Drillerdroid, INT_TO_VOID(DRILLERDROID_TARGET), self->position.x, self->startY)->target = player1;
        self->position.x                                                                                      = 0;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        self->state = Drillerdroid_State_DecidingDropPos;
    }
}

void Drillerdroid_State_Dropping(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    Drillerdroid_CheckPlayerCollisions();

    if (self->position.y >= ((Zone->cameraBoundsB[0] - 112) << 16)) {
        self->position.y = ((Zone->cameraBoundsB[0] - 112) << 16);

        RSDK.PlaySfx(Drillerdroid->sfxImpact, false, 255);
        Camera_ShakeScreen(0, 0, 4);

        self->velocity.y >>= 1;
        self->startY = self->position.y + 0x100000;

        if (!Drillerdroid->canBreakSegment) {
            for (int32 i = 0; i < 4; ++i) {
                Drillerdroid_SpawnDebris(-0x300000);
                Drillerdroid_SpawnDebris(0x300000);
                Drillerdroid_SpawnDebris(-0x190000);
                Drillerdroid_SpawnDebris(0x190000);
            }

            Drillerdroid->jumpsRemaining = 2;
        }

        self->timer = 7;
        self->angle = 0;
        self->state = Drillerdroid_State_Landed;
    }
}

void Drillerdroid_State_Landed(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    Drillerdroid->drillPos[0] = self->startY - self->position.y;
    Drillerdroid->drillPos[1] = self->startY - self->position.y;

    self->velocity.y -= 0xE000;

    Drillerdroid_CheckPlayerCollisions();

    if (self->velocity.y < 0) {
        if (self->position.y < self->startY)
            self->state = Drillerdroid_State_LandRecoil;
    }
}

void Drillerdroid_State_LandRecoil(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    Drillerdroid->drillPos[0] = self->startY - self->position.y;
    Drillerdroid->drillPos[1] = self->startY - self->position.y;

    self->velocity.y += 0x14000;

    Drillerdroid_CheckPlayerCollisions();

    if (self->velocity.y > 0) {
        if (self->position.y > self->startY) {
            self->position.y          = self->startY;
            Drillerdroid->drillPos[0] = 0;
            Drillerdroid->drillPos[1] = 0;
            self->state               = Drillerdroid_State_DecideNextMove;
        }
    }
}

void Drillerdroid_State_DecideNextMove(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();

    if (--self->timer <= 0) {
        if (Drillerdroid->jumpsRemaining) {
            self->timer      = 240;
            self->velocity.y = 0x27000;
            RSDK.PlaySfx(Drillerdroid->sfxJump, false, 255);
            self->state = Drillerdroid_State_PrepareJump;
        }
        else if (Drillerdroid->canBreakSegment) {
            self->timer                   = 180;
            Drillerdroid->stalatiteOffset = 2 * (3 * RSDK.Rand(-2, 3));
            self->state                   = Drillerdroid_State_Drilling;
        }
        else {
            self->timer = 60;
            self->state = Drillerdroid_State_Overheat;
        }
    }
}

void Drillerdroid_State_PrepareJump(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    Drillerdroid->drillPos[0] = self->startY - self->position.y;
    Drillerdroid->drillPos[1] = self->startY - self->position.y;

    self->velocity.y -= 0x8000;

    Drillerdroid_CheckPlayerCollisions();

    if (self->velocity.y < 0) {
        if (Drillerdroid->drillPos[0] >= 0x40000) {
            self->timer               = self->velocity.y;
            Drillerdroid->drillPos[0] = 0x40000;
            Drillerdroid->drillPos[1] = 0x40000;
            self->state               = Drillerdroid_State_Jumping;
        }
    }
}

void Drillerdroid_State_Jumping(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    Drillerdroid_CheckPlayerCollisions();

    if (self->velocity.y > 0) {
        if (self->position.y >= (Zone->cameraBoundsB[0] - 96) << 16) {
            self->position.y = (Zone->cameraBoundsB[0] - 96) << 16;

            RSDK.PlaySfx(Drillerdroid->sfxImpact, false, 255);
            Camera_ShakeScreen(0, 0, 4);

            self->velocity.y >>= 1;
            self->startY = self->position.y;
            self->angle  = 0;

            if (--Drillerdroid->jumpsRemaining) {
                RSDK.PlaySfx(Drillerdroid->sfxJump, false, 255);
                self->state = Drillerdroid_State_PrepareJump;
            }
            else {
                RSDK.PlaySfx(Drillerdroid->sfxDrop, false, 255);
                EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

                EntityBuckwildBall *ball = CREATE_ENTITY(BuckwildBall, NULL, self->position.x, ScreenInfo->position.y << 16);
                ball->startPos.x         = 0;

                int32 ballSlot = 0;
                if (Drillerdroid->arenaSegment) {
                    if (Drillerdroid->arenaSegment == 4 || player1->position.x < self->position.x) {
                        ball->position.x -= 0x800000;
                        ball->direction = FLIP_NONE;
                        ballSlot        = Drillerdroid->arenaSegment - 1;
                    }
                    else {
                        ball->position.x += 0x800000;
                        ball->direction = FLIP_X;
                        ballSlot        = Drillerdroid->arenaSegment + 1;
                    }
                }
                else {
                    ball->position.x += 0x800000;
                    ball->direction = FLIP_X;
                    ballSlot        = Drillerdroid->arenaSegment + 1;
                }

                ball->bossBallSlot = ballSlot;
                ball->visible      = true;
                ball->active       = ACTIVE_NORMAL;
                ball->drawGroup    = Zone->objectDrawGroup[0];
                ball->respawn      = false;
                ball->timerSfx     = 0;
                RSDK.SetSpriteAnimation(BuckwildBall->aniFrames, 0, &ball->animator, true, 0);

                ball->state = BuckwildBall_State_Falling;
                self->state = Drillerdroid_State_Landed;
            }
        }
    }
}

void Drillerdroid_State_Drilling(void)
{
    RSDK_THIS(Drillerdroid);

    ++Drillerdroid->drillSfxTimer;
    RSDK.ProcessAnimation(&self->mainAnimator);

    for (int32 i = 0; i < 2; ++i) {
        if (Drillerdroid->pistonDelay[i]) {
            Drillerdroid->pistonDelay[i]--;
        }
        else if (Drillerdroid->pistonMoveDir[i]) {
            Drillerdroid->pistonPos[i] -= 0x20000;

            if (!Drillerdroid->pistonPos[i]) {
                Drillerdroid->pistonMoveDir[i] = FLIP_NONE;
                Drillerdroid->pistonDelay[i]   = 10;
            }
        }
        else {
            Drillerdroid->pistonPos[i] += 0x40000;

            if (Drillerdroid->pistonPos[i] == 0x100000)
                Drillerdroid->pistonMoveDir[i] = FLIP_X;
        }

        if (Drillerdroid->drillDelay[i]) {
            Drillerdroid->drillDelay[i]--;
        }
        else if (Drillerdroid->drillMoveDir[i]) {
            Drillerdroid->drillPos[i] -= 0x40000;

            if (!Drillerdroid->drillPos[i]) {
                Drillerdroid->drillMoveDir[i] = FLIP_NONE;
                Drillerdroid->drillDelay[i]   = 5;
            }
        }
        else {
            Drillerdroid->drillPos[i] += 0x80000;

            if (Drillerdroid->drillPos[i] == 0x100000)
                Drillerdroid->drillMoveDir[i] = FLIP_X;
        }
    }

    if (Drillerdroid->canBreakSegment) {
        if (self->timer == 120) {
            EntityLavaGeyser *geyser = CREATE_ENTITY(LavaGeyser, NULL, self->position.x, self->position.y + 0x580000);
            geyser->duration         = 60;
            geyser->active           = ACTIVE_NORMAL;
            geyser->force            = 56 << 12;
            geyser->drawGroup        = Zone->objectDrawGroup[1] - 1;
            geyser->state            = LavaGeyser_HandleSetup;
        }
        else if (self->timer < 60) {
            if (self->alpha < 0x200)
                self->alpha += 4;
        }
    }
    else if (Drillerdroid->drillPos[0] == 0x80000) {
        Drillerdroid_SpawnDebris(-0x300000);
        Drillerdroid_SpawnDebris(0x300000);
    }
    else if (Drillerdroid->drillPos[1] == 0x80000) {
        Drillerdroid_SpawnDebris(-0x190000);
        Drillerdroid_SpawnDebris(0x190000);
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (!camera->shakePos.y)
        camera->shakePos.y = 4;

    if (!(Zone->timer & 7)) {
        if (Zone->timer & 0xF) {
            int32 x              = (32 * RSDK.Rand(0, ScreenInfo->size.x >> 5) + ScreenInfo->position.x) << 16;
            int32 y              = (ScreenInfo->position.y + 24) << 16;
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, x, y);

            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 7, &debris->animator, true, RSDK.Rand(0, 4));
            debris->gravityStrength = 0x1800;
            debris->drawGroup       = Zone->objectDrawGroup[0];
            debris->updateRange.x   = 0x400000;
            debris->updateRange.y   = 0x400000;
        }
        else {
            int32 x = (48 * RSDK.Rand(0, ScreenInfo->size.x / 48) + Drillerdroid->stalatiteOffset + ScreenInfo->position.x) << 16;
            int32 y = (ScreenInfo->position.y + 24) << 16;
            EntityStalactite *stalactite = CREATE_ENTITY(Stalactite, Stalactite_State_Falling_Boss, x, y);

            stalactite->updateRange.x = 0x400000;
            stalactite->updateRange.y = 0x400000;
        }
    }

    self->position.y ^= 0x10000;
    Drillerdroid_CheckPlayerCollisions();

    if (--self->timer <= 0)
        self->state = Drillerdroid_State_FinishDrilling;
}

void Drillerdroid_State_FinishDrilling(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid->pistonPos[0] -= 0x20000;
    if (Drillerdroid->pistonPos[0] < 0)
        Drillerdroid->pistonPos[0] = 0;

    Drillerdroid->pistonPos[1] -= 0x20000;
    if (Drillerdroid->pistonPos[1] < 0)
        Drillerdroid->pistonPos[1] = 0;

    Drillerdroid->drillPos[0] -= 0x20000;
    if (Drillerdroid->drillPos[0] < 0)
        Drillerdroid->drillPos[0] = 0;

    Drillerdroid->drillPos[1] -= 0x20000;
    if (Drillerdroid->drillPos[1] < 0)
        Drillerdroid->drillPos[1] = 0;

    if (!Drillerdroid->pistonPos[0] && !Drillerdroid->pistonPos[1] && !Drillerdroid->drillPos[0] && !Drillerdroid->drillPos[1]) {
        self->timer = 60;
        self->state = Drillerdroid_State_Overheat;
    }
}

void Drillerdroid_State_Overheat(void)
{
    RSDK_THIS(Drillerdroid);

    if (--self->timer <= 0) {
        if (Drillerdroid->canBreakSegment) {
            Drillerdroid->canBreakSegment = false;
            RSDK.PlaySfx(Drillerdroid->sfxSizzle, false, 255);

            EntityDebris *debris = NULL;
            switch (Drillerdroid->armorHealth) {
                default: break;

                case 0: Drillerdroid_Hit(); break;

                case 1: {
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
                    debris->velocity.x      = -0x10000;
                    debris->velocity.y      = -0x30000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
                    debris->direction       = FLIP_X;
                    debris->drawFX          = FX_FLIP;
                    debris->velocity.x      = 0x10000;
                    debris->velocity.y      = -0x30000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    int32 spawnX = self->position.x - 0x300000;
                    debris       = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x      = -0x30000;
                    debris->velocity.y      = -0x40000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    spawnX += 0xD0000;
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x      = 0x20000;
                    debris->velocity.y      = -0x40000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    spawnX += 0x530000;
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x      = 0x30000;
                    debris->velocity.y      = -0x40000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX - 851968, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
                    debris->velocity.x      = -0x20000;
                    debris->velocity.y      = -0x40000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;
                    break;
                }

                case 2:
                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 4);
                    debris->velocity.x      = -0x10000;
                    debris->velocity.y      = -0x40000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x300000;
                    debris->updateRange.y   = 0x300000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 5);
                    debris->velocity.x      = 0x10000;
                    debris->velocity.y      = -0x40000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x300000;
                    debris->updateRange.y   = 0x300000;
                    break;
            }

            if (Drillerdroid->armorHealth > 0) {
                RSDK.PlaySfx(Drillerdroid->sfxHit, false, 255);
                self->invincibilityTimer = 45;
                --Drillerdroid->armorHealth;
            }

            self->state = Drillerdroid_State_OverheatRecoil_DestroyedSegment;
        }
        else {
            self->state = Drillerdroid_State_OverheatRecoil;
        }
    }

    Drillerdroid_CheckPlayerCollisions();
}

void Drillerdroid_State_OverheatRecoil(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid->drillPos[0] += 0x10000;
    Drillerdroid->drillPos[0] += 0x10000;

    self->position.y -= 0x10000;

    Drillerdroid_CheckPlayerCollisions();

    if (Drillerdroid->drillPos[0] == 0x100000) {
        self->timer = 15;
        self->state = Drillerdroid_State_JumpTargetDelay;
    }
}

void Drillerdroid_State_JumpTargetDelay(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();

    if (--self->timer <= 0) {
        self->velocity.y = 0x78000;
        RSDK.PlaySfx(Drillerdroid->sfxJump, false, 0xFF);
        self->state = Drillerdroid_State_PrepareJumpTarget;
    }
}

void Drillerdroid_State_PrepareJumpTarget(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1C000;

    Drillerdroid->drillPos[0] = self->startY - self->position.y;
    Drillerdroid->drillPos[1] = self->startY - self->position.y;

    Drillerdroid_CheckPlayerCollisions();

    if (self->velocity.y < 0) {
        if (Drillerdroid->drillPos[0] >= 0x100000) {
            Drillerdroid->drillPos[0] = 0x100000;
            Drillerdroid->drillPos[1] = 0x100000;
            self->state               = Drillerdroid_State_JumpTargeting;
        }
    }
}

void Drillerdroid_State_JumpTargeting(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y >= 0) {
        Drillerdroid->pistonPos[0] = 0;
        Drillerdroid->pistonPos[1] = 0;

        Drillerdroid->drillPos[0] = 0x100000;
        Drillerdroid->drillPos[1] = 0x100000;

        Drillerdroid->pistonDelay[0] = 4;
        Drillerdroid->pistonDelay[1] = 0;

        Drillerdroid->drillDelay[0] = 4;
        Drillerdroid->drillDelay[0] = 0;

        Drillerdroid->pistonMoveDir[0] = FLIP_NONE;
        Drillerdroid->pistonMoveDir[1] = FLIP_NONE;

        Drillerdroid->drillMoveDir[0] = FLIP_NONE;
        Drillerdroid->drillMoveDir[1] = FLIP_NONE;

        self->position.x = 0;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(Drillerdroid, INT_TO_VOID(DRILLERDROID_TARGET), player1->position.x, player1->position.y)->target = player1;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        self->state = Drillerdroid_State_DecidingDropPos;
    }
}

void Drillerdroid_State_DecidingDropPos(void)
{
    RSDK_THIS(Drillerdroid);

    if (self->position.x) {
        self->timer      = 240;
        self->velocity.y = -0x40000;
        self->state      = Drillerdroid_State_Dropping;
        self->position.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x + 0x400000) & 0xFF800000;

        bool32 failed         = self->position.x < Zone->cameraBoundsL[0] << 16 || self->position.x > Zone->cameraBoundsR[0] << 16;
        bool32 platformActive = false;
        if (!failed) {
            Drillerdroid->arenaSegment = (((self->position.x >> 16) - Zone->cameraBoundsL[0] + 64) >> 7) - 1;
            platformActive             = Drillerdroid->platformActive[Drillerdroid->arenaSegment];
            failed                     = platformActive == 0xFF;
        }

        if (failed) {
            self->position.x = 0;
            RSDK.PlaySfx(Drillerdroid->sfxFail, false, 255);
            RSDK.StopSfx(Drillerdroid->sfxTargeting);
            self->timer = 60;
            self->state = Drillerdroid_State_DropFailReset;
        }
        else if (platformActive) {
            EntityLRZRockPile *rocks      = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1 + Drillerdroid->arenaSegment, LRZRockPile);
            rocks->active                 = ACTIVE_NORMAL;
            rocks->timer                  = 120;
            rocks->canCollapse            = true;
            Drillerdroid->canBreakSegment = true;
            Drillerdroid->platformActive[Drillerdroid->arenaSegment] = false;
        }
    }
}

void Drillerdroid_State_DropFailReset(void)
{
    RSDK_THIS(Drillerdroid);

    if (--self->timer <= 0) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(Drillerdroid, INT_TO_VOID(DRILLERDROID_TARGET), player1->position.x, player1->position.y)->target = player1;
        RSDK.PlaySfx(Drillerdroid->sfxTargeting, false, 255);
        self->state = Drillerdroid_State_DecidingDropPos;
    }
}

void Drillerdroid_State_OverheatRecoil_DestroyedSegment(void)
{
    RSDK_THIS(Drillerdroid);

    // Leftover from DrillerdroilO
    ++self->rotation;

    Drillerdroid->drillPos[0] += 0x4000;
    Drillerdroid->drillPos[1] += 0x4000;

    Drillerdroid->pistonPos[0] += 0x4000;
    Drillerdroid->pistonPos[1] += 0x4000;

    self->position.y -= 0x4000;

    Drillerdroid_CheckPlayerCollisions();

    if (Drillerdroid->drillPos[0] == 0x100000) {
        self->timer = 90;
        self->state = Drillerdroid_State_Cooldown;
    }
}

void Drillerdroid_State_Cooldown(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_CheckPlayerCollisions();

    self->alpha -= 8;
    if (self->alpha <= 0) {
        self->alpha = 0;
        self->state = Drillerdroid_State_ResetFromCooldown;
    }
}

void Drillerdroid_State_ResetFromCooldown(void)
{
    RSDK_THIS(Drillerdroid);

    // Leftover from DrillerdroilO
    self->rotation -= 4;

    Drillerdroid->pistonPos[0] -= 0x10000;
    Drillerdroid->pistonPos[1] -= 0x10000;

    Drillerdroid_CheckPlayerCollisions();

    if (!Drillerdroid->pistonPos[0]) {
        self->timer = 30;
        self->state = Drillerdroid_State_JumpTargetDelay;
    }
}

void Drillerdroid_State_Destroyed(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid_Explode();

    if (!--self->invincibilityTimer) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        if (Drillerdroid->armorHealth == 1) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
            debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x400000;
            debris->updateRange.y   = 0x400000;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 6);
            debris->direction       = FLIP_X;
            debris->drawFX          = FX_FLIP;
            debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
            debris->gravityStrength = 14336;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x400000;
            debris->updateRange.y   = 0x400000;

            int32 spawnX = self->position.x - 0x300000;
            debris       = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x400000;
            debris->updateRange.y   = 0x400000;

            spawnX += 0xD0000;
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x400000;
            debris->updateRange.y   = 0x400000;

            spawnX += 0x530000;
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x400000;
            debris->updateRange.y   = 0x400000;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, spawnX - 851968, self->position.y);
            RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &debris->animator, true, 7);
            debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x400000;
            debris->updateRange.y   = 0x400000;
        }

        self->velocity.x           = RSDK.Rand(-6, 6) << 15;
        self->velocity.y           = RSDK.Rand(-10, -6) << 15;
        self->mainAnimator.frameID = 4;
        self->stateDraw            = Drillerdroid_Draw_Simple;
        self->state                = Drillerdroid_State_Finish;

        foreach_active(SignPost, signPost) { signPost->position.x = self->position.x; }
    }
}

void Drillerdroid_State_Finish(void)
{
    RSDK_THIS(Drillerdroid);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    Drillerdroid_Explode();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->timer = 0;
        self->state = Drillerdroid_State_DropSignPost;
    }
}

void Drillerdroid_State_DropSignPost(void)
{
    RSDK_THIS(Drillerdroid);

    if (++self->timer == 60) {
        foreach_all(SignPost, signPost)
        {
            signPost->active = ACTIVE_NORMAL;
            signPost->state  = SignPost_State_Falling;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }

        LRZ1Setup->fadeTimer = 1;
        destroyEntity(self);
    }
}

void Drillerdroid_Draw_Boss(void)
{
    RSDK_THIS(Drillerdroid);

    RSDK.SetLimitedFade(0, 1, 2, self->alpha, 32, 41);

    if (self->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }

    // Piston (L1)
    self->mainAnimator.frameID = 1;
    Vector2 drawPos;
    drawPos.x = self->position.x - 0x300000;
    drawPos.y = self->position.y - Drillerdroid->pistonPos[0];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Drill (L1)
    self->mainAnimator.frameID = 2;
    drawPos.y                  = self->position.y + Drillerdroid->drillPos[0];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Piston (L2)
    self->mainAnimator.frameID = 1;
    drawPos.x += 0xD0000;
    drawPos.y = self->position.y - Drillerdroid->pistonPos[1];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Drill (L2)
    self->mainAnimator.frameID = 2;
    drawPos.y                  = self->position.y + Drillerdroid->drillPos[1];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Piston (R1)
    self->mainAnimator.frameID = 1;
    drawPos.x += 0x530000;
    drawPos.y = self->position.y - Drillerdroid->pistonPos[0];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Drill (R1)
    self->mainAnimator.frameID = 2;
    drawPos.y                  = self->position.y + Drillerdroid->drillPos[0];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Piston (R2)
    self->mainAnimator.frameID = 1;
    drawPos.x -= 0xD0000;
    drawPos.y = self->position.y - Drillerdroid->pistonPos[1];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Drill (R2)
    self->mainAnimator.frameID = 2;
    drawPos.y                  = self->position.y + Drillerdroid->drillPos[1];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Main Body
    self->mainAnimator.frameID = 0;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    switch (Drillerdroid->armorHealth) {
        default:
        case 0: break;

        case 1:
            // Drill (L) Armor
            drawPos                     = self->position;
            self->armorAnimator.frameID = 6;
            RSDK.DrawSprite(&self->armorAnimator, NULL, false);

            // Drill (R) Armor
            self->drawFX |= FX_FLIP;
            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->armorAnimator, NULL, false);

            self->direction             = FLIP_NONE;
            self->drawFX                = FX_NONE;
            self->armorAnimator.frameID = 7;

            // Piston (L1) Armor
            drawPos.x -= 0x300000;
            drawPos.y -= Drillerdroid->pistonPos[0];
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

            // Piston (L2) Armor
            drawPos.x += 0xD0000;
            drawPos.y = self->position.y - Drillerdroid->pistonPos[1];
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

            // Piston (R1) Armor
            drawPos.x += 0x530000;
            drawPos.y = self->position.y - Drillerdroid->pistonPos[0];
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

            // Piston (R2) Armor
            drawPos.x -= 0xD0000;
            drawPos.y = self->position.y - Drillerdroid->pistonPos[1];
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);
            break;

        case 2:
            // Core Armor (L)
            drawPos.x                   = self->position.x + 0x10000;
            drawPos.y                   = self->position.y - 0x140000;
            self->armorAnimator.frameID = 4;
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

            self->armorAnimator.frameID = 5;
            //[Fallthrough]

        case 3:
            if (Drillerdroid->armorHealth == 3) {
                // Core Armor (R)
                drawPos.x                   = self->position.x + 0x10000;
                drawPos.y                   = self->position.y - 0x140000;
                self->armorAnimator.frameID = 0;
                RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

                self->armorAnimator.frameID = 1;
            }

            // Core Armor (R)
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

            // Drill (L) Armor
            drawPos                     = self->position;
            self->armorAnimator.frameID = 2;
            RSDK.DrawSprite(&self->armorAnimator, NULL, false);

            // Drill (R) Armor
            self->drawFX |= FX_FLIP;
            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->armorAnimator, NULL, false);

            self->direction             = FLIP_NONE;
            self->drawFX                = FX_NONE;
            self->armorAnimator.frameID = 3;

            // Piston (L1) Armor
            drawPos.x -= 0x300000;
            drawPos.y = self->position.y - Drillerdroid->pistonPos[0];
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

            // Piston (L2) Armor
            drawPos.x += 0xD0000;
            drawPos.y = self->position.y - Drillerdroid->pistonPos[1];
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

            // Piston (R1) Armor
            drawPos.x += 0x530000;
            drawPos.y = self->position.y - Drillerdroid->pistonPos[0];
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

            // Piston (R2) Armor
            drawPos.x -= 0xD0000;
            drawPos.y = self->position.y - Drillerdroid->pistonPos[1];
            RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);
            break;
    }

    RSDK.CopyPalette(1, 32, 0, 32, 10);
    RSDK.SetPaletteEntry(0, 32, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void Drillerdroid_Draw_Simple(void)
{
    RSDK_THIS(Drillerdroid);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void Drillerdroid_State_Target(void)
{
    RSDK_THIS(Drillerdroid);

    RSDK.ProcessAnimation(&self->armorAnimator);
    RSDK.ProcessAnimation(&self->targetLockAnimator);

    if (self->target) {
        self->position.x = self->target->position.x;
        self->position.y = self->target->position.y;
    }

    self->alpha += 0x20;
    self->targetEdgeOffset.x -= 0x20000;
    if (self->targetEdgeOffset.x <= 0xC0000) {
        self->alpha              = 0;
        self->targetEdgeOffset.x = 0x2C0000;
    }

    self->targetEdgeOffset.y = self->targetEdgeOffset.x;
    if (++self->timer == 60)
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 4, &self->targetLockAnimator, true, 0);

    if (self->timer == 96) {
        self->target                   = NULL;
        Drillerdroid->boss->position.x = self->position.x;

        if (self->position.x < (Zone->cameraBoundsL[0] + 64) << 16 || self->position.x > (Zone->cameraBoundsR[0] - 64) << 16)
            Drillerdroid->boss->position.x = (Zone->cameraBoundsL[0] + 64) << 16;
        else if (self->position.x > (Zone->cameraBoundsR[0] - 64) << 16)
            Drillerdroid->boss->position.x = (Zone->cameraBoundsR[0] - 64) << 16;
    }

    if (self->timer == 128)
        destroyEntity(self);
}

void Drillerdroid_Draw_Target(void)
{
    RSDK_THIS(Drillerdroid);

    int32 x = ((self->position.x + 0x400000) & 0xFF800000) - 0x400000;
    if (!(Zone->timer & 8)) {
        RSDK.DrawLine(x + 0x10000, self->position.y, x + 0x7F0000, self->position.y, 0xE0E0E0, 192, INK_ADD, false);
        RSDK.DrawLine(x, self->position.y - 0x80000, x, self->position.y + 0x80000, 0xE0E0E0, 160, INK_ADD, false);
        RSDK.DrawLine(x + 0x800000, self->position.y - 0x80000, x + 0x800000, self->position.y + 0x80000, 0xE0E0E0, 160, INK_ADD, false);
    }

    // Top-Left Edge
    self->mainAnimator.frameID = 0;
    self->inkEffect            = INK_ALPHA;
    self->direction            = FLIP_NONE;
    Vector2 drawPos;
    drawPos.x = self->position.x - self->targetEdgeOffset.x;
    drawPos.y = self->position.y - self->targetEdgeOffset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Top-Right Edge
    self->direction = FLIP_X;
    drawPos.x       = self->position.x + self->targetEdgeOffset.x;
    drawPos.y       = self->position.y - self->targetEdgeOffset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Bottom-Left Edge
    self->mainAnimator.frameID = 1;
    self->direction            = FLIP_NONE;
    drawPos.x                  = self->position.x - self->targetEdgeOffset.x;
    drawPos.y                  = self->position.y + self->targetEdgeOffset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Bottom-Right Edge
    self->direction = FLIP_X;
    drawPos.x       = self->position.x + self->targetEdgeOffset.x;
    drawPos.y       = self->position.y + self->targetEdgeOffset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Target Reticle
    self->inkEffect = INK_NONE;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->armorAnimator, NULL, false);
    RSDK.DrawSprite(&self->targetLockAnimator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void Drillerdroid_EditorDraw(void)
{
    RSDK_THIS(Drillerdroid);

    Drillerdroid->pistonPos[0] = 0;
    Drillerdroid->pistonPos[1] = 0;

    Drillerdroid->drillPos[0] = 0x100000;
    Drillerdroid->drillPos[1] = 0x100000;

    self->drawFX = FX_NONE;
    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &self->mainAnimator, true, 0);
    RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 5, &self->armorAnimator, true, 0);

    Drillerdroid_Draw_Boss();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-328, -SCREEN_YSIZE, 324, 96, 1 | 2 | 4 | 8, 0x00C0F0);

        if (CollapsingPlatform) {
            int32 slot = SceneInfo->entitySlot + 1;
            for (int32 s = 0; s < 5; ++s) {
                EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(slot++, CollapsingPlatform);

                if (platform && platform->classID == CollapsingPlatform->classID)
                    DrawHelpers_DrawArrow(self->position.x, self->position.y, platform->position.x, platform->position.y, 0xFFFF00, INK_NONE, 0xFF);
            }
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Drillerdroid_EditorLoad(void)
{
    Drillerdroid->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    Drillerdroid->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Drillerdroid, type);
    RSDK_ENUM_VAR("Drillerdroid", DRILLERDROID_MAIN);
}
#endif

void Drillerdroid_Serialize(void) { RSDK_EDITABLE_VAR(Drillerdroid, VAR_UINT8, type); }

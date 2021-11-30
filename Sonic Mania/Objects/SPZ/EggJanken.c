#include "SonicMania.h"

ObjectEggJanken *EggJanken = NULL;

void EggJanken_Update(void)
{
    RSDK_THIS(EggJanken);

    self->solidMoveOffset.x = self->position.x & 0xFFFF0000;
    self->solidPos.x        = self->position.x & 0xFFFF0000;
    self->solidMoveOffset.y = self->position.y & 0xFFFF0000;
    self->solidPos.y        = self->position.y & 0xFFFF0000;

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
        if (!(self->invincibilityTimer & 1)) {
            if (self->invincibilityTimer & 2)
                RSDK.SetPaletteEntry(0, 128, 0xFFFFFF);
            else
                RSDK.SetPaletteEntry(0, 128, 0x000000);
        }
    }
    self->state1();

    self->rotation   = self->fullRotation >> 8;
    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
    if (self->isMoving)
        EggJanken_HandleMovement();
    self->solidMoveOffset.x -= self->position.x & 0xFFFF0000;
    self->solidMoveOffset.y -= self->position.y & 0xFFFF0000;
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator5);

    self->state2();

    for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
        for (self->armJointID = 0; self->armJointID < EggJanken_SegmentCount; ++self->armJointID) {
            self->stateArm[self->armID]();
        }
    }

    if (self->stateDraw != EggJanken_StateDraw_Unknown3)
        EggJanken_CheckPlayerCollisions();
}

void EggJanken_LateUpdate(void) {}

void EggJanken_StaticUpdate(void) {}

void EggJanken_Draw(void)
{
    RSDK_THIS(EggJanken);
    StateMachine_Run(self->stateDraw);
}

void EggJanken_Create(void *data)
{
    RSDK_THIS(EggJanken);

    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 7, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 2, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 4, &self->animator4, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 5, &self->animator5, true, 0);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->visible       = false;
    self->drawOrder     = Zone->drawOrderLow;
    self->drawFX |= FX_ROTATE | FX_FLIP;
    self->startPos.x        = self->position.x;
    self->startPos.y        = self->position.y;
    self->yCap              = self->position.y + 0x100000;
    self->state2            = EggJanken_State2_Unknown3;
    self->eyeFrames[0]      = 3;
    self->eyeFrames[1]      = 4;
    self->health            = 3;
    self->animator3.frameID = 1;
    self->stateArm[0]       = EggJanken_StateArm_Unknown1;
    self->stateArm[1]       = EggJanken_StateArm_Unknown1;
    self->state1            = EggJanken_State1_SetupArena;
}

void EggJanken_StageLoad(void)
{
    EggJanken->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE);

    EggJanken->hitbox1.left   = -32;
    EggJanken->hitbox1.top    = -35;
    EggJanken->hitbox1.right  = 32;
    EggJanken->hitbox1.bottom = 29;

    EggJanken->hitbox2.left   = -16;
    EggJanken->hitbox2.top    = 29;
    EggJanken->hitbox2.right  = 16;
    EggJanken->hitbox2.bottom = 37;

    EggJanken->hitbox3.left   = -16;
    EggJanken->hitbox3.top    = 29;
    EggJanken->hitbox3.right  = 16;
    EggJanken->hitbox3.bottom = 41;

    EggJanken->hitbox4.left   = -12;
    EggJanken->hitbox4.top    = -12;
    EggJanken->hitbox4.right  = 12;
    EggJanken->hitbox4.bottom = 12;

    EggJanken->stateJankenResult[0] = EggJanken_StateResult_Draw;
    EggJanken->stateJankenResult[3] = EggJanken_StateResult_Lose;
    EggJanken->stateJankenResult[6] = EggJanken_StateResult_Win;
    EggJanken->stateJankenResult[1] = EggJanken_StateResult_Win;
    EggJanken->stateJankenResult[4] = EggJanken_StateResult_Draw;
    EggJanken->stateJankenResult[7] = EggJanken_StateResult_Lose;
    EggJanken->stateJankenResult[2] = EggJanken_StateResult_Lose;
    EggJanken->stateJankenResult[5] = EggJanken_StateResult_Win;
    EggJanken->stateJankenResult[8] = EggJanken_StateResult_Draw;
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 3, &EggJanken->animator, true, 0);
    EggJanken->sfxBeep3     = RSDK.GetSfx("Stage/Beep3.wav");
    EggJanken->sfxBeep4     = RSDK.GetSfx("Stage/Beep4.wav");
    EggJanken->sfxFail      = RSDK.GetSfx("Stage/Fail.wav");
    EggJanken->sfxClick     = RSDK.GetSfx("Stage/Click.wav");
    EggJanken->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    EggJanken->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    EggJanken->sfxDrop      = RSDK.GetSfx("Stage/Drop.wav");
    EggJanken->sfxImpact4   = RSDK.GetSfx("Stage/Impact4.wav");
    EggJanken->sfxImpact3   = RSDK.GetSfx("Stage/Impact3.wav");
}

void EggJanken_CheckPlayerCollisions(void)
{
    RSDK_THIS(EggJanken);

    int storeX = self->position.x;
    int storeY = self->position.y;

    foreach_active(Player, player)
    {
        self->position.x = self->solidPos.x;
        self->position.y = self->solidPos.y;
        switch (Player_CheckCollisionBox(player, self, &EggJanken->hitbox1)) {
            default: break;
            case C_TOP:
                player->position.x -= self->solidMoveOffset.x;
                player->position.y -= self->solidMoveOffset.y;
                player->position.y &= 0xFFFF0000;
                break;
            case C_BOTTOM:
                if (player->onGround)
                    player->deathType = PLAYER_DEATH_DIE_USESFX;
                break;
        }

        self->position.x = storeX;
        self->position.y = storeY;

        if (Player_CheckValidState(player)) {
            Player_CheckCollisionBox(player, self, &EggJanken->hitbox2);

            if (Player_CheckCollisionTouch(player, self, &EggJanken->hitbox3)) {
                if (player->velocity.y < 0 && !self->animator3.frameID) {
                    self->animator3.frameID = 1;
                    if (self->state1 != EggJanken_State1_Unknown2) {
                        RSDK.StopSfx(EggJanken->sfxBeep3);
                        RSDK.PlaySfx(EggJanken->sfxClick, false, 255);
                        self->state1           = EggJanken_State1_Unknown7;
                        self->stateArm[0]      = EggJanken_StateArm_Unknown8;
                        self->stateArm[1]      = EggJanken_StateArm_Unknown8;
                        self->state2           = EggJanken_State2_Unknown3;
                        self->jankenResult2[0] = self->eyeFrames[0];
                        self->jankenResult2[1] = self->eyeFrames[1];
                        self->eyeFrames[0]     = 11;
                        self->eyeFrames[1]     = 11;
                        self->storedXVel       = self->velocity.x;
                        self->velocity.x       = 0;
                        self->timer            = 0;
                    }
                }
            }

            for (int i = 0; i < EggJanken_ArmCount; ++i) {
                self->position.x = self->armPos[i].x;
                self->position.y = self->armPos[i].y;
                if (Player_CheckCollisionTouch(player, self, &EggJanken->hitbox4)) {
                    // This object goes unused so it wasn't updated for plus, but if it was there'd 100% be a checkMightyUnspin call here
                    Player_CheckHit(player, self);
                }
            }
            self->position.x = storeX;
            self->position.y = storeY;
        }
    }
}

void EggJanken_HandleMovement(void)
{
    RSDK_THIS(EggJanken);

    if (self->velocity.x >= 0) {
        if (self->startPos.x >= (ScreenInfo->position.x + ScreenInfo->width - 48) << 16)
            self->velocity.x = -self->velocity.x;
    }
    else if (self->startPos.x <= (ScreenInfo->position.x + 48) << 16)
        self->velocity.x = -self->velocity.x;

    self->startPos.x += self->velocity.x;

    // this is not the same code as BadnikHelpers_Oscillate, guess this is an older variant?
    self->position.y += RSDK.Sin256(self->angle) << 10;
    self->angle = (self->angle + 4) & 0xFF;
}

void EggJanken_StateResult_Win(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxBeep4, false, 255);
    self->state1 = EggJanken_State1_JankenWin;
}

void EggJanken_StateResult_Lose(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxFail, false, 255);
    self->state1 = EggJanken_State1_JankenLose;
}

void EggJanken_StateResult_Draw(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
    self->state1 = EggJanken_State1_JankenDraw;
}

void EggJanken_State1_SetupArena(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer >= 2) {
        self->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->cameraBoundsB[0]     = (self->position.y >> 16) + 208;
        self->startPos.y -= 0x1000000;
        self->active = ACTIVE_NORMAL;
        self->state1 = EggJanken_State1_StartFight;
    }
}

void EggJanken_State1_StartFight(void)
{
    RSDK_THIS(EggJanken);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]     = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        self->visible             = true;
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsR[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
        self->state1              = EggJanken_State1_Unknown1;
        self->stateDraw           = EggJanken_StateDraw_Unknown1;
        self->timer               = 272;
    }
}

void EggJanken_State1_Unknown1(void)
{
    RSDK_THIS(EggJanken);

    if (!--self->timer) {
        self->isMoving          = true;
        self->animator3.frameID = 0;
        RSDK.PlaySfx(EggJanken->sfxClick, false, 255);
        self->state1 = EggJanken_State1_Unknown2;
    }
    else {
        self->startPos.y += 0x10000;
    }
}

void EggJanken_State1_Unknown2(void)
{
    RSDK_THIS(EggJanken);

    if (self->animator3.frameID == 1) {
        RSDK.SetSpriteAnimation(EggJanken->aniFrames, 8, &self->animator2, true, 0);
        self->state1 = EggJanken_State1_Unknown3;

        EntityEggJankenPart *part = CREATE_ENTITY(EggJankenPart, intToVoid(1), self->position.x, self->position.y);
        part->velocity.x          = -0x20000;
        part->velocity.y          = -0x20000;
        part->angle               = -1;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(2), self->position.x, self->position.y);
        part->velocity.x = 0x20000;
        part->velocity.y = -0x20000;
        part->angle      = 1;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(3), self->position.x, self->position.y);
        part->velocity.x = -0x10000;
        part->velocity.y = -0x10000;
        part->angle      = -1;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(4), self->position.x, self->position.y);
        part->velocity.x = 0x10000;
        part->velocity.y = -0x10000;
        part->angle      = 1;

        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
    }
}

void EggJanken_State1_Unknown3(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer >= 120) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(EggJanken->aniFrames, 1, &self->animator2, true, 0);
        self->state1    = EggJanken_State1_Unknown4;
        self->stateDraw = EggJanken_StateDraw_Unknown2;
        for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
            self->armRadiusSpeed[self->armID] = 0x40000;
            for (int s = 0; s < EggJanken_SegmentCount; ++s) self->jointAngles[EggJanken_SegmentCount * self->armID + s] = 0xC0;
            self->stateArm[self->armID]                                 = EggJanken_StateArm_Unknown2;
        }

        EntityEggJankenPart *part = CREATE_ENTITY(EggJankenPart, intToVoid(5), self->position.x, self->position.y);
        part->velocity.x          = -0x40000;
        part->velocity.y          = -0x20000;
        part->angle               = -3;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(6), self->position.x, self->position.y);
        part->velocity.x = -0x30000;
        part->velocity.y = -0x10000;
        part->angle      = -3;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(5), self->position.x, self->position.y);
        part->direction  = FLIP_X;
        part->velocity.x = 0x40000;
        part->velocity.y = -0x20000;
        part->angle      = 3;

        part             = CREATE_ENTITY(EggJankenPart, intToVoid(6), self->position.x, self->position.y);
        part->direction  = FLIP_X;
        part->velocity.x = 0x30000;
        part->velocity.y = -0x10000;
        part->angle      = 3;

        RSDK.PlaySfx(EggJanken->sfxHit, false, 255);
    }
}

void EggJanken_State1_Unknown4(void)
{
    RSDK_THIS(EggJanken);

    if (self->radius < 0x2300)
        self->radius += 0x100;
    if (self->stateArm[0] == EggJanken_StateArm_Unknown7) {
        for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
            self->jointFlags[self->armID]     = 0;
            self->jointAngleVels[self->armID] = 4;
            for (self->armJointID = 0; self->armJointID < EggJanken_SegmentCount; ++self->armJointID) {
                self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID]       = 0;
                self->jointTargetAngles[EggJanken_SegmentCount * self->armID + self->armJointID] = 0;
                self->jointDirection[EggJanken_SegmentCount * self->armID + self->armJointID]    = 0;
                self->jointDelays[EggJanken_SegmentCount * self->armID + self->armJointID]       = 4 * self->armJointID;
            }
        }

        self->stateArm[0] = EggJanken_StateArm_Unknown3;
        self->stateArm[1] = EggJanken_StateArm_Unknown3;
        self->state1      = EggJanken_State1_Unknown5;
    }
}

void EggJanken_State1_Unknown5(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_StateArm_Unknown7) {
        self->stateArm[0] = EggJanken_StateArm_Unknown4;
        self->stateArm[1] = EggJanken_StateArm_Unknown4;
    }

    if ((self->jointAngles[0] & 0xF8) == 0x88) {
        if ((self->jointAngles[1] & 0xF8) == 0x80) {
            self->velocity.x = 0x10000;
            self->state1     = EggJanken_State1_Unknown6;
            self->state2     = EggJanken_State2_Unknown1;
        }
    }
}

void EggJanken_State1_Unknown6(void) {}

void EggJanken_State1_Destroyed(void)
{
    RSDK_THIS(EggJanken);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(EggJanken->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x                                                                 = self->position.x + (RSDK.Rand(-24, 24) << 16);
            int y                                                                 = self->position.y + (RSDK.Rand(-24, 24) << 16);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    EntityEggJankenPart *part = NULL;
    switch (++self->timer) {
        case 60:
            part                      = CREATE_ENTITY(EggJankenPart, intToVoid(11), self->position.x, self->position.y);
            self->animator3.frameID = 2;
            break;
        case 90:
            self->stateDraw = EggJanken_StateDraw_Unknown1;
            self->armID     = 0;
            for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
                part             = CREATE_ENTITY(EggJankenPart, intToVoid(7), self->armPos[self->armID].x, self->armPos[self->armID].y);
                part->velocity.x = -0x20000;
                part->velocity.y = -0x20000;

                part             = CREATE_ENTITY(EggJankenPart, intToVoid(8), self->armPos[self->armID].x, self->armPos[self->armID].y);
                part->velocity.x = 0x20000;
                part->velocity.y = -0x20000;

                part             = CREATE_ENTITY(EggJankenPart, intToVoid(9), self->armPos[self->armID].x, self->armPos[self->armID].y);
                part->velocity.x = -0x10000;
                part->velocity.y = -0x10000;

                part             = CREATE_ENTITY(EggJankenPart, intToVoid(10), self->armPos[self->armID].x, self->armPos[self->armID].y);
                part->velocity.x = 0x10000;
                part->velocity.y = -0x10000;

                self->armPos[self->armID].x = 0;
                self->armPos[self->armID].y = 0;
            }
            break;
        case 120:
            self->stateDraw = EggJanken_StateDraw_Unknown3;
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
            break;
        case 180: {
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
            EntitySignPost *signPost = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, SignPost);
            signPost->position.x     = self->position.x;
            signPost->state          = SignPost_State_Fall;
            self->state1           = EggJanken_State1_Unknown6;
            break;
        }
        default: break;
    }
    if (self->timer > 120) {
        self->startPos.y += self->velocity.y;
        self->position.y = self->startPos.y;
        self->velocity.y += 0x3800;
    }
}

void EggJanken_State1_Unknown7(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_StateArm_Unknown7 && ++self->timer == 30) {
        int choice1          = self->jankenResult2[0];
        int choice2          = self->jankenResult2[1];
        self->eyeFrames[0] = choice1;
        self->eyeFrames[1] = choice2;
        EggJanken->stateJankenResult[3 * choice1 + choice2]();
        self->timer = 0;
    }
}

void EggJanken_State1_JankenWin(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 60) {
        self->eyeFrames[0] = 5;
        self->eyeFrames[1] = 6;
    }

    if (self->timer == 90) {
        for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
            self->armRadiusSpeed[self->armID] = 0x40000;
            for (int s = 0; s < EggJanken_SegmentCount; ++s) self->jointAngles[EggJanken_SegmentCount * self->armID + s] = 0xC0;
            self->stateArm[self->armID]                                 = EggJanken_StateArm_Unknown2;
        }
        self->state1 = EggJanken_State1_Unknown9;
        self->timer  = 0;
    }

    if (self->timer > 60 && (Zone->timer & 1)) {
        if (self->fullRotation == 0x800)
            self->fullRotation = 0x1F800;
        else
            self->fullRotation = 0x800;
    }
}

void EggJanken_State1_Unknown9(void)
{
    RSDK_THIS(EggJanken);

    if (Zone->timer & 1) {
        if (self->fullRotation == 0x800)
            self->fullRotation = 0x1F800;
        else
            self->fullRotation = 0x800;
    }

    if (self->stateArm[0] == EggJanken_StateArm_Unknown7) {
        self->fullRotation = 0;
        self->timer        = 0;
        if (!self->health) {
            self->isMoving            = false;
            SceneInfo->timeEnabled = false;
            self->state1              = EggJanken_State1_Destroyed;
            EntityPlayer *player1       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            if (player1->superState == SUPERSTATE_SUPER)
                player1->superState = SUPERSTATE_FADEOUT;
            Player_GiveScore(player1, 1000);
            self->invincibilityTimer = 0;
        }
        else {
            self->state1 = EggJanken_State1_Unknown10;
        }
    }
}

void EggJanken_Unknown18(void)
{
    RSDK_THIS(EggJanken);

    self->state1      = EggJanken_State1_Unknown11;
    self->stateArm[0] = EggJanken_StateArm_Unknown3;
    self->stateArm[1] = EggJanken_StateArm_Unknown3;

    for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
        self->jointFlags[self->armID]     = 0;
        self->jointAngleVels[self->armID] = 4;
        for (self->armJointID = 0; self->armJointID < EggJanken_SegmentCount; ++self->armJointID) {
            self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID]       = 0;
            self->jointTargetAngles[EggJanken_SegmentCount * self->armID + self->armJointID] = 0;
            self->jointDirection[EggJanken_SegmentCount * self->armID + self->armJointID]    = 0;
            self->jointDelays[EggJanken_SegmentCount * self->armID + self->armJointID]       = 4 * self->armJointID;
        }
    }

    self->eyeFrames[0] = 3;
    self->eyeFrames[1] = 4;
}

void EggJanken_State1_Unknown10(void)
{
    RSDK_THIS(EggJanken);

    if (!self->invincibilityTimer && ++self->timer == 30) {
        EggJanken_Unknown18();
        self->timer = 0;
    }
}

void EggJanken_State1_Unknown11(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_StateArm_Unknown7) {
        self->state1      = EggJanken_State1_Unknown12;
        self->stateArm[0] = EggJanken_StateArm_Unknown4;
        self->stateArm[1] = EggJanken_StateArm_Unknown4;
    }
}

void EggJanken_State1_Unknown12(void)
{
    RSDK_THIS(EggJanken);

    if ((self->jointAngles[0] & 0xF8) == 0x88) {
        if ((self->jointAngles[1] & 0xF8) == 0x80) {
            self->velocity.x = self->storedXVel;
            self->state1     = EggJanken_State1_Unknown6;
            self->state2     = EggJanken_State2_Unknown1;
        }
    }
}

void EggJanken_State1_JankenDraw(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 60) {
        self->eyeFrames[0] = 10;
        self->eyeFrames[1] = 10;
    }

    if (self->timer == 120) {
        self->state1                                 = EggJanken_State1_Unknown14;
        self->attackingArmID                         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;
        self->armRadiusSpeed[self->attackingArmID] = 0x40000;
        self->jointAngles[EggJanken_SegmentCount * self->attackingArmID + 3] = 0xC0;
        self->jointAngles[EggJanken_SegmentCount * self->attackingArmID + 2] = 0xC0;
        self->jointAngles[EggJanken_SegmentCount * self->attackingArmID + 1] = 0xC0;
        self->jointAngles[EggJanken_SegmentCount * self->attackingArmID + 0] = 0xC0;
        self->stateArm[self->attackingArmID]                                 = EggJanken_StateArm_Unknown3;
        self->timer                                                            = 0;
    }
}

void EggJanken_State1_Unknown14(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[self->attackingArmID] == EggJanken_StateArm_Unknown7) {
        self->fullRotation = 0;

        self->jointAngleVels[self->attackingArmID] = 8;
        for (self->armJointID = 0; self->armJointID < EggJanken_SegmentCount; ++self->armJointID) {
            int slot                        = EggJanken_SegmentCount * self->attackingArmID + self->armJointID;
            self->jointDelays[slot]       = 4 * self->armJointID;
            self->jointTargetAngles[slot] = 240;
            self->jointDirection[slot]    = 0;
        }

        self->stateArm[self->attackingArmID] = EggJanken_StateArm_Unknown5;
        self->state1                           = EggJanken_State1_Unknown15;
    }
}

void EggJanken_State1_Unknown15(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[self->attackingArmID] == EggJanken_StateArm_Unknown7) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        self->jointAngleVels[self->attackingArmID] = 8;
        for (self->armJointID = 0; self->armJointID < EggJanken_SegmentCount; ++self->armJointID) {
            int slot = EggJanken_SegmentCount * self->attackingArmID + self->armJointID;

            self->jointDelays[slot] = 4 * self->armJointID;
            int distX                 = self->position.x - player1->position.x;
            int distY                 = (self->position.y - player1->position.y) >> 16;
            if (self->attackingArmID)
                self->jointTargetAngles[slot] = (uint8)(0x40 - RSDK.ATan2((distX + 0x280000) >> 16, distY));
            else
                self->jointTargetAngles[slot] = (uint8)(RSDK.ATan2((distX - 0x280000) >> 16, distY) - 0x40);
            self->jointDirection[slot] = 1;
        }

        self->stateArm[self->attackingArmID]       = EggJanken_StateArm_Unknown5;
        self->armRadiusSpeed[self->attackingArmID] = 0x38000;

        if (self->attackingArmID) {
            int distX = abs((self->position.x - player1->position.x + 0x280000) >> 16);
            int distY = abs((self->position.y - player1->position.y) >> 16);
            int inc   = 0x180 * (distX + distY - 120);
            self->armRadiusSpeed[self->attackingArmID] += inc;
        }
        else {
            int distX = abs((self->position.x - player1->position.x - 0x280000) >> 16);
            int distY = abs((self->position.y - player1->position.y) >> 16);
            int inc   = 0x180 * (distY + distX - 120);
            self->armRadiusSpeed[self->attackingArmID] += inc;
        }
        self->state1 = EggJanken_State1_Unknown16;
    }
    else if (self->attackingArmID) {
        self->fullRotation -= 0x200;
    }
    else {
        self->fullRotation += 0x200;
    }
}

void EggJanken_State1_Unknown16(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[self->attackingArmID] == EggJanken_StateArm_Unknown7) {
        self->state1 = EggJanken_State1_Unknown17;
        self->timer  = 0;
    }
    else if (self->attackingArmID) {
        self->fullRotation += 0x200;
        if (self->fullRotation > 0)
            self->fullRotation = 0;
    }
    else {
        self->fullRotation -= 0x200;
        if (self->fullRotation < 0)
            self->fullRotation = 0;
    }
}

void EggJanken_State1_Unknown17(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 30) {
        self->timer = 0;
        EggJanken_Unknown18();
    }
}

void EggJanken_State1_JankenLose(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 60) {
        self->eyeFrames[0] = 3;
        self->eyeFrames[1] = 4;
    }
    if (self->timer == 120) {
        self->state1   = EggJanken_State1_Unknown19;
        self->isMoving = false;
        self->timer    = 0;
    }
}

void EggJanken_State1_Unknown19(void)
{
    RSDK_THIS(EggJanken);

    self->fullRotation += 0x800;
    if ((self->fullRotation & 0x1FFFF) == 0x10000) {
        for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
            self->armRadiusSpeed[self->armID] = 0x40000;
            for (int s = 0; s < EggJanken_SegmentCount; ++s) self->jointAngles[EggJanken_SegmentCount * self->armID + s] = 0xC0;
            self->stateArm[self->armID]                                 = EggJanken_StateArm_Unknown3;
        }

        self->state1 = EggJanken_State1_Unknown20;
    }
}

void EggJanken_State1_Unknown21(void)
{
    RSDK_THIS(EggJanken);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (abs(self->position.x - player1->position.x) >= 0xC0000) {
        if (self->position.x >= player1->position.x)
            self->startPos.x -= 0x10000;
        else
            self->startPos.x += 0x10000;
    }
    else {
        for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
            for (self->armJointID = 0; self->armJointID < EggJanken_SegmentCount; ++self->armJointID) {
                self->jointAngleVels[self->armID]                                                  = 8;
                self->jointDelays[EggJanken_SegmentCount * self->armID + self->armJointID]       = 4 * self->armJointID;
                self->jointTargetAngles[EggJanken_SegmentCount * self->armID + self->armJointID] = 160;
                self->jointDirection[EggJanken_SegmentCount * self->armID + self->armJointID]    = 1;
            }
            self->stateArm[self->armID] = EggJanken_StateArm_Unknown6;
        }

        RSDK.PlaySfx(EggJanken->sfxDrop, false, 255);
        self->state1 = EggJanken_State1_Unknown22;
    }
}

void EggJanken_State1_Unknown20(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_StateArm_Unknown7)
        self->state1 = EggJanken_State1_Unknown21;
}

void EggJanken_State1_Unknown22(void)
{
    RSDK_THIS(EggJanken);

    self->startPos.y += self->velocity.y;
    self->position.y = self->startPos.y;
    self->velocity.y += 0x7000;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        for (self->armID = 0; self->armID < EggJanken_ArmCount; ++self->armID) {
            for (self->armJointID = 0; self->armJointID < EggJanken_SegmentCount; ++self->armJointID) {
                self->jointAngleVels[self->armID]                                                  = 8;
                self->jointDelays[EggJanken_SegmentCount * self->armID + self->armJointID]       = 4 * self->armJointID;
                self->jointTargetAngles[EggJanken_SegmentCount * self->armID + self->armJointID] = 200;
                self->jointDirection[EggJanken_SegmentCount * self->armID + self->armJointID]    = 0;
            }
            self->stateArm[self->armID] = EggJanken_StateArm_Unknown6;
        }

        RSDK.PlaySfx(EggJanken->sfxImpact4, false, 255);
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->shakePos.y   = 4;
        self->velocity.y   = 0;
        self->state1       = EggJanken_State1_Unknown23;
    }
}

void EggJanken_State1_Unknown23(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_StateArm_Unknown7) {
        RSDK.PlaySfx(EggJanken->sfxImpact3, false, 255);
        self->state1 = EggJanken_State1_Unknown24;
    }
}

void EggJanken_State1_Unknown24(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 60) {
        self->timer       = 0;
        self->stateArm[0] = EggJanken_StateArm_Unknown8;
        self->stateArm[1] = EggJanken_StateArm_Unknown8;
        self->state1      = EggJanken_State1_Unknown25;
    }
}

void EggJanken_State1_Unknown25(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_StateArm_Unknown7)
        self->state1 = EggJanken_State1_Unknown26;
}

void EggJanken_State1_Unknown26(void)
{
    RSDK_THIS(EggJanken);

    self->startPos.y = self->startPos.y - 0x20000;
    self->position.y = self->startPos.y - 0x20000;
    if (self->position.y < self->yCap) {
        self->startPos.y = self->yCap;
        self->position.y = self->yCap;
        self->state1     = EggJanken_State1_Unknown27;
    }
}

void EggJanken_State1_Unknown27(void)
{
    RSDK_THIS(EggJanken);

    self->fullRotation += 1024;
    if ((self->fullRotation & 0x1FFFF) == 0) {
        self->angle    = 0;
        self->isMoving = true;
        EggJanken_Unknown18();
    }
}

void EggJanken_State2_Unknown1(void)
{
    RSDK_THIS(EggJanken);

    self->timer2            = 0;
    self->eyeFrames[0]      = RSDK.Rand(0, 3);
    self->eyeFrames[1]      = RSDK.Rand(0, 3);
    self->state2            = EggJanken_State2_Unknown2;
    self->animator3.frameID = 0;
    RSDK.PlaySfx(EggJanken->sfxClick, false, 255);
}

void EggJanken_State2_Unknown2(void)
{
    RSDK_THIS(EggJanken);

    ++self->timer2;

    switch (self->health) {
        default: break;
        case 1:
            if (!(self->timer2 & 0x3F)) {
                self->eyeFrames[0] = RSDK.Rand(0, 3);
                self->eyeFrames[1] = RSDK.Rand(0, 3);
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }
            break;
        case 2:
            if (!(self->timer2 & 0x3F)) {
                self->eyeFrames[0] = (self->eyeFrames[0] + 2) % 3;
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }

            if (!(self->timer2 & 0x7F)) {
                self->eyeFrames[1] = (self->eyeFrames[1] + 1) % 3u;
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }
            break;
        case 3:
            if (self->health == 3) {
                if (!(self->timer2 & 0x3F)) {
                    self->eyeFrames[0] = (self->eyeFrames[0] + 1) % 3;
                    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
                }

                if (!(self->timer2 & 0x7F)) {
                    self->eyeFrames[1] = (self->eyeFrames[1] + 1) % 3u;
                    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
                }
            }
            break;
    }
}

void EggJanken_State2_Unknown3(void) {}

void EggJanken_Unknown40(void)
{
    RSDK_THIS(EggJanken);

    int slot = self->armJointID + EggJanken_SegmentCount * self->armID;

    self->jointAngleVels[self->armID] = 4;
    self->jointDelays[slot]             = 4 * self->armJointID;
    self->jointDirection[slot] ^= 1;
    self->jointTargetAngles[slot] = self->jointAngles[slot];
    if (self->jointDirection[slot]) {
        self->jointTargetAngles[slot] = 136 - (8 * self->armJointID);
        if (self->armJointID == EggJanken_JointCount)
            self->jointTargetAngles[slot] -= 8;
    }
    else {
        self->jointTargetAngles[slot] = 224 - (8 * self->armJointID);
        if (self->armJointID == EggJanken_JointCount)
            self->jointTargetAngles[slot] -= 8;
    }
    if (self->armJointID == EggJanken_JointCount) {
        self->jointFlags[self->armID] = 0;
    }
}

void EggJanken_StateArm_Unknown1(void) {}

void EggJanken_StateArm_Unknown7(void) {}

void EggJanken_StateArm_Unknown8(void)
{
    RSDK_THIS(EggJanken);

    if (self->armRadius[self->armID])
        self->armRadius[self->armID] -= 0x8000;
    else
        self->stateArm[self->armID] = EggJanken_StateArm_Unknown7;
}

void EggJanken_StateArm_Unknown3(void)
{
    RSDK_THIS(EggJanken);

    if (self->armRadius[self->armID] != 0x800000) {
        self->armRadius[self->armID] += 0x10000;
    }
    else {
        self->armRadiusSpeed[self->armID] = 0x8000;
        self->stateArm[self->armID]       = EggJanken_StateArm_Unknown7;
    }
}

void EggJanken_StateArm_Unknown2(void)
{
    RSDK_THIS(EggJanken);

    self->armRadius[self->armID] += self->armRadiusSpeed[self->armID];
    self->armRadiusSpeed[self->armID] -= 0x800;

    if (self->armRadius[self->armID] <= 0) {
        if (self->state1 == EggJanken_State1_Unknown9 && !self->armID) {
            RSDK.PlaySfx(EggJanken->sfxHit, false, 255);
            self->invincibilityTimer = 60;
            self->eyeFrames[0]       = 8;
            self->eyeFrames[1]       = 9;
            --self->health;
        }
        self->armRadius[self->armID] = 0;
        self->stateArm[self->armID]  = EggJanken_StateArm_Unknown7;
    }
}

void EggJanken_StateArm_Unknown4(void)
{
    RSDK_THIS(EggJanken);

    if (self->jointFlags[self->armID] == ((1 << EggJanken_SegmentCount) - 1))
        EggJanken_Unknown40();

    int slot = self->armJointID + EggJanken_SegmentCount * self->armID;

    if (self->jointDelays[slot]) {
        self->jointDelays[slot]--;
    }
    else {
        if (self->jointDirection[slot]) {
            if (self->jointAngles[slot] > self->jointTargetAngles[slot])
                self->jointAngles[slot] -= self->jointAngleVels[self->armID];
            else
                self->jointFlags[self->armID] |= (1 << self->armJointID);
        }
        else if (self->jointAngles[slot] < self->jointTargetAngles[slot])
            self->jointAngles[slot] += self->jointAngleVels[self->armID];
        else
            self->jointFlags[self->armID] |= (1 << self->armJointID);
    }
}

void EggJanken_StateArm_Unknown5(void)
{
    RSDK_THIS(EggJanken);

    int slot = self->armJointID + EggJanken_SegmentCount * self->armID;

    if (self->jointDelays[slot]) {
        self->jointDelays[slot]--;
    }
    else {
        if (self->jointDirection[slot]) {
            if (self->jointAngles[slot] > self->jointTargetAngles[slot]) {
                self->jointAngles[slot] -= self->jointAngleVels[self->armID];
            }
            else {
                self->jointFlags[self->armID] |= (1 << self->armJointID);
                self->jointAngles[slot] = self->jointTargetAngles[slot];
            }
        }
        else if (self->jointAngles[slot] < self->jointTargetAngles[slot]) {
            self->jointAngles[slot] += self->jointAngleVels[self->armID];
        }
        else {
            self->jointFlags[self->armID] |= (1 << self->armJointID);
            self->jointAngles[slot] = self->jointTargetAngles[slot];
        }
    }

    self->armRadius[self->armID] += self->armRadiusSpeed[self->armID];
    self->armRadiusSpeed[self->armID] -= 0x800;

    if (self->armRadius[self->armID] <= 0) {
        self->jointFlags[self->armID] = 0;
        self->armRadius[self->armID]  = 0;
        self->stateArm[self->armID]   = EggJanken_StateArm_Unknown7;
    }
}

void EggJanken_StateArm_Unknown6(void)
{
    RSDK_THIS(EggJanken);

    if (self->jointFlags[self->armID] == ((1 << EggJanken_SegmentCount) - 1)) {
        self->jointFlags[self->armID] = 0;
        self->stateArm[self->armID]   = EggJanken_StateArm_Unknown7;
    }

    int slot = self->armJointID + EggJanken_SegmentCount * self->armID;

    if (self->jointDelays[slot]) {
        self->jointDelays[slot]--;
    }
    else {
        if (self->jointDirection[slot]) {
            if (self->jointAngles[slot] > self->jointTargetAngles[slot]) {
                self->jointAngles[slot] -= self->jointAngleVels[self->armID];
            }
            else {
                self->jointFlags[self->armID] |= (1 << self->armJointID);
                self->jointAngles[slot] = self->jointTargetAngles[slot];
            }
        }
        else if (self->jointAngles[slot] < self->jointTargetAngles[slot]) {
            self->jointAngles[slot] += self->jointAngleVels[self->armID];
        }
        else {
            self->jointFlags[self->armID] |= (1 << self->armJointID);
            self->jointAngles[slot] = self->jointTargetAngles[slot];
        }
    }
}

void EggJanken_StateDraw_Unknown1(void)
{
    RSDK_THIS(EggJanken);

    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator4, NULL, false);
    RSDK.DrawSprite(&self->animator5, NULL, false);
}

void EggJanken_StateDraw_Unknown2(void)
{
    RSDK_THIS(EggJanken);
    Vector2 drawPos;

    self->armID = 0;
    drawPos.x     = self->radius * RSDK.Sin256(0xC0 - (self->rotation >> 1));
    drawPos.y     = self->radius * RSDK.Cos256(0xC0 - (self->rotation >> 1));
    drawPos.x += self->position.x;
    drawPos.y += self->position.y;
    EggJanken->animator.frameID = 0;
    RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);

    int radius                  = (self->armRadius[self->armID] >> 16);
    EggJanken->animator.frameID = 1;
    self->armJointID          = 0;
    for (self->armJointID = 0; self->armJointID < EggJanken_JointCount; ++self->armJointID) {
        drawPos.x +=
            32 * radius * RSDK.Sin256(self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID] + (self->rotation >> 1));
        drawPos.y -=
            32 * radius * RSDK.Cos256(self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID] + (self->rotation >> 1));
        RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);
    }

    EggJanken->animator.frameID = 2;
    drawPos.x += (radius * RSDK.Sin256(self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID] + (self->rotation >> 1)))
                 << 6;
    drawPos.y -= (radius * RSDK.Cos256(self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID] + (self->rotation >> 1)))
                 << 6;
    RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);

    self->armPos[0] = drawPos;
    self->armID     = 1;
    drawPos.x         = self->radius * RSDK.Sin256(0x40 - (self->rotation >> 1));
    drawPos.y         = self->radius * RSDK.Cos256(0x40 - (self->rotation >> 1));
    drawPos.x += self->position.x;
    drawPos.y += self->position.y;
    EggJanken->animator.frameID = 0;
    RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);

    radius                      = (self->armRadius[self->armID] >> 16);
    EggJanken->animator.frameID = 1;
    self->armJointID          = 0;
    for (self->armJointID = 0; self->armJointID < EggJanken_JointCount; ++self->armJointID) {
        drawPos.x -=
            32 * radius * RSDK.Sin256(self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID] - (self->rotation >> 1));
        drawPos.y -=
            32 * radius * RSDK.Cos256(self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID] - (self->rotation >> 1));
        RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);
    }

    EggJanken->animator.frameID = 2;
    drawPos.x -= (radius * RSDK.Sin256(self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID] - (self->rotation >> 1)))
                 << 6;
    drawPos.y -= (radius * RSDK.Cos256(self->jointAngles[EggJanken_SegmentCount * self->armID + self->armJointID] - (self->rotation >> 1)))
                 << 6;
    RSDK.DrawSprite(&EggJanken->animator, &drawPos, false);

    self->armPos[1] = drawPos;
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    if (Zone->timer & 1) {
        self->inkEffect                                                    = INK_ADD;
        self->alpha                                                        = 128;
        self->animator1.frameID                                            = self->eyeFrames[0];
        RSDK.GetFrame(EggJanken->aniFrames, 0, self->eyeFrames[0])->pivotX = -24;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->animator1.frameID                                            = self->eyeFrames[1];
        RSDK.GetFrame(EggJanken->aniFrames, 0, self->eyeFrames[1])->pivotX = 0;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->inkEffect = INK_NONE;
        self->alpha     = 512;
    }
    RSDK.DrawSprite(&self->animator4, NULL, false);
    RSDK.DrawSprite(&self->animator5, NULL, false);
}

void EggJanken_StateDraw_Unknown3(void)
{
    RSDK_THIS(EggJanken);

    if (Zone->timer & 1) {
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator4, NULL, false);
        RSDK.DrawSprite(&self->animator5, NULL, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void EggJanken_EditorDraw(void)
{
    RSDK_THIS(EggJanken);
    EggJanken_StateDraw_Unknown1();

    if (showGizmos())
        DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 0 | 4 | 8, -WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 208);
}

void EggJanken_EditorLoad(void)
{
    EggJanken->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 3, &EggJanken->animator, true, 0);
}
#endif

void EggJanken_Serialize(void) {}

// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EggJanken Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectEggJanken *EggJanken = NULL;

void EggJanken_Update(void)
{
    RSDK_THIS(EggJanken);

    self->moveOffset.x = self->position.x & 0xFFFF0000;
    self->moveOffset.y = self->position.y & 0xFFFF0000;
    self->solidPos.x   = self->position.x & 0xFFFF0000;
    self->solidPos.y   = self->position.y & 0xFFFF0000;

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
        if (!(self->invincibilityTimer & 1)) {
            if (self->invincibilityTimer & 2)
                RSDK.SetPaletteEntry(0, 128, 0xFFFFFF);
            else
                RSDK.SetPaletteEntry(0, 128, 0x000000);
        }
    }

#if RETRO_USE_MOD_LOADER
    StateMachine_Run(self->state);
#else
    // Direct call, no StateMachine for some reason
    self->state();
#endif

    self->rotation   = self->fullRotation >> 8;
    self->position.x = self->origin.x;
    self->position.y = self->origin.y;
    if (self->isMoving)
        EggJanken_HandleMovement();

    self->moveOffset.x -= self->position.x & 0xFFFF0000;
    self->moveOffset.y -= self->position.y & 0xFFFF0000;

    RSDK.ProcessAnimation(&self->bodyAnimator);
    RSDK.ProcessAnimation(&self->propellorLAnimator);
    RSDK.ProcessAnimation(&self->propellorRAnimator);

#if RETRO_USE_MOD_LOADER
    StateMachine_Run(self->stateEyes);
#else
    // Another direct call, no StateMachine for some reason
    self->stateEyes();
#endif

    for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
        for (self->armJointID = 0; self->armJointID < EGGJANKEN_SEGMENT_COUNT; ++self->armJointID) {
#if RETRO_USE_MOD_LOADER
            StateMachine_Run(self->stateArm[self->armID]);
#else
            // More direct calls, no StateMachine for some reason
            self->stateArm[self->armID]();
#endif
        }
    }

    if (self->stateDraw != EggJanken_Draw_Destroyed)
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

    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 0, &self->eyeAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 7, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 2, &self->buttonAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 4, &self->propellorLAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 5, &self->propellorRAnimator, true, 0);

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->visible       = false;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->drawFX |= FX_ROTATE | FX_FLIP;
    self->origin.x               = self->position.x;
    self->origin.y               = self->position.y;
    self->startY                 = self->position.y + 0x100000;
    self->stateEyes              = EggJanken_Eyes_None;
    self->eyeFrames[0]           = 3;
    self->eyeFrames[1]           = 4;
    self->health                 = 3;
    self->buttonAnimator.frameID = 1;

    for (int32 a = 0; a < EGGJANKEN_ARM_COUNT; ++a) self->stateArm[a] = EggJanken_Arm_None;
    self->state = EggJanken_State_SetupArena;
}

void EggJanken_StageLoad(void)
{
    EggJanken->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE);

    EggJanken->hitboxBody.left   = -32;
    EggJanken->hitboxBody.top    = -35;
    EggJanken->hitboxBody.right  = 32;
    EggJanken->hitboxBody.bottom = 29;

    EggJanken->hitboxButtonSolid.left   = -16;
    EggJanken->hitboxButtonSolid.top    = 29;
    EggJanken->hitboxButtonSolid.right  = 16;
    EggJanken->hitboxButtonSolid.bottom = 37;

    EggJanken->hitboxButton.left   = -16;
    EggJanken->hitboxButton.top    = 29;
    EggJanken->hitboxButton.right  = 16;
    EggJanken->hitboxButton.bottom = 41;

    EggJanken->hitboxArm.left   = -12;
    EggJanken->hitboxArm.top    = -12;
    EggJanken->hitboxArm.right  = 12;
    EggJanken->hitboxArm.bottom = 12;

    EggJanken->stateJankenResult[0] = EggJanken_Result_PlayerDraws;
    EggJanken->stateJankenResult[1] = EggJanken_Result_PlayerWins;
    EggJanken->stateJankenResult[2] = EggJanken_Result_PlayerLoses;
    EggJanken->stateJankenResult[3] = EggJanken_Result_PlayerLoses;
    EggJanken->stateJankenResult[4] = EggJanken_Result_PlayerDraws;
    EggJanken->stateJankenResult[5] = EggJanken_Result_PlayerWins;
    EggJanken->stateJankenResult[6] = EggJanken_Result_PlayerWins;
    EggJanken->stateJankenResult[7] = EggJanken_Result_PlayerLoses;
    EggJanken->stateJankenResult[8] = EggJanken_Result_PlayerDraws;

    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 3, &EggJanken->armAnimator, true, 0);

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

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        self->position.x = self->solidPos.x;
        self->position.y = self->solidPos.y;

        switch (Player_CheckCollisionBox(player, self, &EggJanken->hitboxBody)) {
            default: break;

            case C_TOP:
                player->position.x -= self->moveOffset.x;
                player->position.y -= self->moveOffset.y;
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
            Player_CheckCollisionBox(player, self, &EggJanken->hitboxButtonSolid);

            if (Player_CheckCollisionTouch(player, self, &EggJanken->hitboxButton)) {
                if (player->velocity.y < 0 && !self->buttonAnimator.frameID) {
                    self->buttonAnimator.frameID = 1;
                    if (self->state != EggJanken_State_AwaitButtonPress) {
                        RSDK.StopSfx(EggJanken->sfxBeep3);
                        RSDK.PlaySfx(EggJanken->sfxClick, false, 255);

                        self->state = EggJanken_State_ButtonPressed;
                        for (int32 a = 0; a < EGGJANKEN_ARM_COUNT; ++a) self->stateArm[a] = EggJanken_Arm_RetractArm;
                        self->stateEyes = EggJanken_Eyes_None;

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

            for (int32 i = 0; i < EGGJANKEN_ARM_COUNT; ++i) {
                self->position.x = self->armPos[i].x;
                self->position.y = self->armPos[i].y;

                if (Player_CheckCollisionTouch(player, self, &EggJanken->hitboxArm)) {
                    // This object goes unused so it wasn't updated for plus, but if it was there'd likely be a Plaer_CheckMightyUnspin call here
                    Player_Hurt(player, self);
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
        if (self->origin.x >= (ScreenInfo->position.x + ScreenInfo->size.x - 48) << 16)
            self->velocity.x = -self->velocity.x;
    }
    else if (self->origin.x <= (ScreenInfo->position.x + 48) << 16)
        self->velocity.x = -self->velocity.x;

    self->origin.x += self->velocity.x;

    // this is not the same code as BadnikHelpers_Oscillate, maybe this is an older variant?
    self->position.y += RSDK.Sin256(self->angle) << 10;
    self->angle = (self->angle + 4) & 0xFF;
}

void EggJanken_Explode(void)
{
    RSDK_THIS(EggJanken);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(EggJanken->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int32 x                                                                 = self->position.x + (RSDK.Rand(-24, 24) << 16);
            int32 y                                                                 = self->position.y + (RSDK.Rand(-24, 24) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void EggJanken_ResetStates(void)
{
    RSDK_THIS(EggJanken);

    self->state = EggJanken_State_RaiseArms;
    for (int32 a = 0; a < EGGJANKEN_ARM_COUNT; ++a) self->stateArm[a] = EggJanken_Arm_ExtendArm;

    for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
        self->jointFlags[self->armID]     = 0;
        self->jointAngleVels[self->armID] = 4;

        for (self->armJointID = 0; self->armJointID < EGGJANKEN_SEGMENT_COUNT; ++self->armJointID) {
            self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]       = 0;
            self->jointTargetAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] = 0;
            self->jointDirection[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]    = 0;
            self->jointDelays[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]       = 4 * self->armJointID;
        }
    }

    self->eyeFrames[0] = 3;
    self->eyeFrames[1] = 4;
}

void EggJanken_SwapArmSwingDir(void)
{
    RSDK_THIS(EggJanken);

    int32 slot = self->armJointID + EGGJANKEN_SEGMENT_COUNT * self->armID;

    self->jointAngleVels[self->armID] = 4;
    self->jointDelays[slot]           = 4 * self->armJointID;
    self->jointDirection[slot] ^= 1;
    self->jointTargetAngles[slot] = self->jointAngles[slot];

    if (self->jointDirection[slot]) {
        self->jointTargetAngles[slot] = 136 - (8 * self->armJointID);

        if (self->armJointID == EGGJANKEN_JOINT_COUNT)
            self->jointTargetAngles[slot] -= 8;
    }
    else {
        self->jointTargetAngles[slot] = 224 - (8 * self->armJointID);

        if (self->armJointID == EGGJANKEN_JOINT_COUNT)
            self->jointTargetAngles[slot] -= 8;
    }

    if (self->armJointID == EGGJANKEN_JOINT_COUNT) {
        self->jointFlags[self->armID] = 0;
    }
}

void EggJanken_Result_PlayerWins(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxBeep4, false, 255);
    self->state = EggJanken_State_ResultPlayerWinner;
}

void EggJanken_Result_PlayerLoses(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxFail, false, 255);
    self->state = EggJanken_State_ResultPlayerLoser;
}

void EggJanken_Result_PlayerDraws(void)
{
    RSDK_THIS(EggJanken);

    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
    self->state = EggJanken_State_ResultPlayerDraw;
}

void EggJanken_State_SetupArena(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer >= 2) {
        self->timer = 0;

        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 208;

        self->origin.y -= 0x1000000;
        self->active = ACTIVE_NORMAL;
        self->state  = EggJanken_State_StartFight;
    }
}

void EggJanken_State_StartFight(void)
{
    RSDK_THIS(EggJanken);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        self->visible               = true;
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x;

        self->state     = EggJanken_State_EnterJanken;
        self->stateDraw = EggJanken_Draw_Closed;
        self->timer     = 272;
    }
}

void EggJanken_State_EnterJanken(void)
{
    RSDK_THIS(EggJanken);

    if (!--self->timer) {
        self->isMoving               = true;
        self->buttonAnimator.frameID = 0;
        RSDK.PlaySfx(EggJanken->sfxClick, false, 255);
        self->state = EggJanken_State_AwaitButtonPress;
    }
    else {
        self->origin.y += 0x10000;
    }
}

void EggJanken_State_AwaitButtonPress(void)
{
    RSDK_THIS(EggJanken);

    if (self->buttonAnimator.frameID == 1) {
        RSDK.SetSpriteAnimation(EggJanken->aniFrames, 8, &self->bodyAnimator, true, 0);
        self->state = EggJanken_State_Opened;

        EntityEggJankenPart *part = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_FACEPLATE_TL), self->position.x, self->position.y);
        part->velocity.x          = -0x20000;
        part->velocity.y          = -0x20000;
        part->angle               = -1;

        part             = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_FACEPLATE_TR), self->position.x, self->position.y);
        part->velocity.x = 0x20000;
        part->velocity.y = -0x20000;
        part->angle      = 1;

        part             = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_FACEPLATE_BL), self->position.x, self->position.y);
        part->velocity.x = -0x10000;
        part->velocity.y = -0x10000;
        part->angle      = -1;

        part             = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_FACEPLATE_BR), self->position.x, self->position.y);
        part->velocity.x = 0x10000;
        part->velocity.y = -0x10000;
        part->angle      = 1;

        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
    }
}

void EggJanken_State_Opened(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer >= 120) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(EggJanken->aniFrames, 1, &self->bodyAnimator, true, 0);
        self->state     = EggJanken_State_InitialArmExtend;
        self->stateDraw = EggJanken_Draw_Active;

        for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
            self->armRadiusSpeed[self->armID] = 0x40000;
            for (int32 s = 0; s < EGGJANKEN_SEGMENT_COUNT; ++s) self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + s] = 0xC0;
            self->stateArm[self->armID] = EggJanken_Arm_StretchRetractArm;
        }

        EntityEggJankenPart *part = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_SIDE_L), self->position.x, self->position.y);
        part->velocity.x          = -0x40000;
        part->velocity.y          = -0x20000;
        part->angle               = -3;

        part             = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_SIDE_R), self->position.x, self->position.y);
        part->velocity.x = -0x30000;
        part->velocity.y = -0x10000;
        part->angle      = -3;

        part             = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_SIDE_L), self->position.x, self->position.y);
        part->direction  = FLIP_X;
        part->velocity.x = 0x40000;
        part->velocity.y = -0x20000;
        part->angle      = 3;

        part             = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_SIDE_R), self->position.x, self->position.y);
        part->direction  = FLIP_X;
        part->velocity.x = 0x30000;
        part->velocity.y = -0x10000;
        part->angle      = 3;

        RSDK.PlaySfx(EggJanken->sfxHit, false, 0xFF);
    }
}

void EggJanken_State_InitialArmExtend(void)
{
    RSDK_THIS(EggJanken);

    if (self->radius < 0x2300)
        self->radius += 0x100;

    if (self->stateArm[0] == EggJanken_Arm_Idle) {
        for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
            self->jointFlags[self->armID]     = 0;
            self->jointAngleVels[self->armID] = 4;
            for (self->armJointID = 0; self->armJointID < EGGJANKEN_SEGMENT_COUNT; ++self->armJointID) {
                self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]       = 0;
                self->jointTargetAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] = 0;
                self->jointDirection[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]    = 0;
                self->jointDelays[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]       = 4 * self->armJointID;
            }
        }

        for (int32 a = 0; a < EGGJANKEN_ARM_COUNT; ++a) self->stateArm[a] = EggJanken_Arm_ExtendArm;
        self->state = EggJanken_State_InitialArmRaise;
    }
}

void EggJanken_State_InitialArmRaise(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_Arm_Idle) {
        for (int32 a = 0; a < EGGJANKEN_ARM_COUNT; ++a) self->stateArm[a] = EggJanken_Arm_SwingArm;
    }

    if ((self->jointAngles[0] & 0xF8) == 0x88 && (self->jointAngles[1] & 0xF8) == 0x80) {
        self->velocity.x = 0x10000;
        self->state      = EggJanken_State_None;
        self->stateEyes  = EggJanken_Eyes_Setup;
    }
}

void EggJanken_State_None(void)
{
    // we just chillin tbh
}

void EggJanken_State_Destroyed(void)
{
    RSDK_THIS(EggJanken);

    EggJanken_Explode();

    EntityEggJankenPart *part = NULL;
    switch (++self->timer) {
        case 60:
            part                         = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_BUTTON), self->position.x, self->position.y);
            self->buttonAnimator.frameID = 2;
            break;

        case 90:
            self->stateDraw = EggJanken_Draw_Closed;
            self->armID     = 0;

            for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
                part = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_BALL_TL), self->armPos[self->armID].x, self->armPos[self->armID].y);
                part->velocity.x = -0x20000;
                part->velocity.y = -0x20000;

                part = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_BALL_TR), self->armPos[self->armID].x, self->armPos[self->armID].y);
                part->velocity.x = 0x20000;
                part->velocity.y = -0x20000;

                part = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_BALL_BL), self->armPos[self->armID].x, self->armPos[self->armID].y);
                part->velocity.x = -0x10000;
                part->velocity.y = -0x10000;

                part = CREATE_ENTITY(EggJankenPart, INT_TO_VOID(EGGJANKENPART_BALL_BR), self->armPos[self->armID].x, self->armPos[self->armID].y);
                part->velocity.x = 0x10000;
                part->velocity.y = -0x10000;

                self->armPos[self->armID].x = 0;
                self->armPos[self->armID].y = 0;
            }
            break;

        case 120:
            self->stateDraw = EggJanken_Draw_Destroyed;
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
            break;

        case 180: {
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);

            EntitySignPost *signPost = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, SignPost);
            signPost->position.x     = self->position.x;
            signPost->state          = SignPost_State_Falling;

            self->state = EggJanken_State_None;
            break;
        }

        default: break;
    }

    if (self->timer > 120) {
        self->origin.y += self->velocity.y;
        self->position.y = self->origin.y;
        self->velocity.y += 0x3800;
    }
}

void EggJanken_State_ButtonPressed(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_Arm_Idle && ++self->timer == 30) {
        int32 choice1      = self->jankenResult2[0];
        int32 choice2      = self->jankenResult2[1];
        self->eyeFrames[0] = choice1;
        self->eyeFrames[1] = choice2;

        EggJanken->stateJankenResult[3 * choice1 + choice2]();

        self->timer = 0;
    }
}

void EggJanken_State_ResultPlayerWinner(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 60) {
        self->eyeFrames[0] = 5;
        self->eyeFrames[1] = 6;
    }

    if (self->timer == 90) {
        for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
            self->armRadiusSpeed[self->armID] = 0x40000;
            for (int32 s = 0; s < EGGJANKEN_SEGMENT_COUNT; ++s) self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + s] = 0xC0;
            self->stateArm[self->armID] = EggJanken_Arm_StretchRetractArm;
        }

        self->state = EggJanken_State_HitShake;
        self->timer = 0;
    }

    if (self->timer > 60 && (Zone->timer & 1)) {
        if (self->fullRotation == 0x800)
            self->fullRotation = 0x1F800;
        else
            self->fullRotation = 0x800;
    }
}

void EggJanken_State_HitShake(void)
{
    RSDK_THIS(EggJanken);

    if (Zone->timer & 1) {
        if (self->fullRotation == 0x800)
            self->fullRotation = 0x1F800;
        else
            self->fullRotation = 0x800;
    }

    if (self->stateArm[0] == EggJanken_Arm_Idle) {
        self->fullRotation = 0;
        self->timer        = 0;

        if (!self->health) {
            self->isMoving         = false;
            SceneInfo->timeEnabled = false;
            self->state            = EggJanken_State_Destroyed;

            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            if (player1->superState == SUPERSTATE_SUPER)
                player1->superState = SUPERSTATE_FADEOUT;
            Player_GiveScore(player1, 1000);

            self->invincibilityTimer = 0;
        }
        else {
            self->state = EggJanken_State_FinishedBeingHit;
        }
    }
}

void EggJanken_State_FinishedBeingHit(void)
{
    RSDK_THIS(EggJanken);

    if (!self->invincibilityTimer && ++self->timer == 30) {
        EggJanken_ResetStates();
        self->timer = 0;
    }
}

void EggJanken_State_RaiseArms(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_Arm_Idle) {
        self->state = EggJanken_State_SwingDropArms;
        for (int32 a = 0; a < EGGJANKEN_ARM_COUNT; ++a) self->stateArm[a] = EggJanken_Arm_SwingArm;
    }
}

void EggJanken_State_SwingDropArms(void)
{
    RSDK_THIS(EggJanken);

    if ((self->jointAngles[0] & 0xF8) == 0x88 && (self->jointAngles[1] & 0xF8) == 0x80) {
        self->velocity.x = self->storedXVel;
        self->state      = EggJanken_State_None;
        self->stateEyes  = EggJanken_Eyes_Setup;
    }
}

void EggJanken_State_ResultPlayerDraw(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 60) {
        self->eyeFrames[0] = 10;
        self->eyeFrames[1] = 10;
    }

    if (self->timer == 120) {
        self->state = EggJanken_State_WaitForArmAttackExtend;

        self->attackingArmID                                                  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;
        self->armRadiusSpeed[self->attackingArmID]                            = 0x40000;
        self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->attackingArmID + 3] = 0xC0;
        self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->attackingArmID + 2] = 0xC0;
        self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->attackingArmID + 1] = 0xC0;
        self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->attackingArmID + 0] = 0xC0;
        self->stateArm[self->attackingArmID]                                  = EggJanken_Arm_ExtendArm;

        self->timer = 0;
    }
}

void EggJanken_State_WaitForArmAttackExtend(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[self->attackingArmID] == EggJanken_Arm_Idle) {
        self->fullRotation = 0;

        self->jointAngleVels[self->attackingArmID] = 8;
        for (self->armJointID = 0; self->armJointID < EGGJANKEN_SEGMENT_COUNT; ++self->armJointID) {
            int32 slot                    = EGGJANKEN_SEGMENT_COUNT * self->attackingArmID + self->armJointID;
            self->jointDelays[slot]       = 4 * self->armJointID;
            self->jointTargetAngles[slot] = 240;
            self->jointDirection[slot]    = 0;
        }

        self->stateArm[self->attackingArmID] = EggJanken_Arm_ArmAttack;
        self->state                          = EggJanken_State_PrepareArmAttack;
    }
}

void EggJanken_State_PrepareArmAttack(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[self->attackingArmID] == EggJanken_Arm_Idle) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        self->jointAngleVels[self->attackingArmID] = 8;
        for (self->armJointID = 0; self->armJointID < EGGJANKEN_SEGMENT_COUNT; ++self->armJointID) {
            int32 slot = EGGJANKEN_SEGMENT_COUNT * self->attackingArmID + self->armJointID;

            self->jointDelays[slot] = 4 * self->armJointID;
            int32 distX             = self->position.x - player1->position.x;
            int32 distY             = (self->position.y - player1->position.y) >> 16;
            if (self->attackingArmID)
                self->jointTargetAngles[slot] = (uint8)(0x40 - RSDK.ATan2((distX + 0x280000) >> 16, distY));
            else
                self->jointTargetAngles[slot] = (uint8)(RSDK.ATan2((distX - 0x280000) >> 16, distY) - 0x40);

            self->jointDirection[slot] = FLIP_X;
        }

        self->stateArm[self->attackingArmID]       = EggJanken_Arm_ArmAttack;
        self->armRadiusSpeed[self->attackingArmID] = 0x38000;

        if (self->attackingArmID) {
            int32 distX = abs((self->position.x - player1->position.x + 0x280000) >> 16);
            int32 distY = abs((self->position.y - player1->position.y) >> 16);
            self->armRadiusSpeed[self->attackingArmID] += 0x180 * (distX + distY - 120);
        }
        else {
            int32 distX = abs((self->position.x - player1->position.x - 0x280000) >> 16);
            int32 distY = abs((self->position.y - player1->position.y) >> 16);
            self->armRadiusSpeed[self->attackingArmID] += 0x180 * (distY + distX - 120);
        }

        self->state = EggJanken_State_ArmAttack;
    }
    else if (self->attackingArmID) {
        self->fullRotation -= 0x200;
    }
    else {
        self->fullRotation += 0x200;
    }
}

void EggJanken_State_ArmAttack(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[self->attackingArmID] == EggJanken_Arm_Idle) {
        self->state = EggJanken_State_FinishedArmAttack;
        self->timer = 0;
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

void EggJanken_State_FinishedArmAttack(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 30) {
        self->timer = 0;
        EggJanken_ResetStates();
    }
}

void EggJanken_State_ResultPlayerLoser(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 60) {
        self->eyeFrames[0] = 3;
        self->eyeFrames[1] = 4;
    }
    if (self->timer == 120) {
        self->state    = EggJanken_State_FlipOver;
        self->isMoving = false;
        self->timer    = 0;
    }
}

void EggJanken_State_FlipOver(void)
{
    RSDK_THIS(EggJanken);

    self->fullRotation += 0x800;

    if ((self->fullRotation & 0x1FFFF) == 0x10000) {
        for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
            self->armRadiusSpeed[self->armID] = 0x40000;
            for (int32 s = 0; s < EGGJANKEN_SEGMENT_COUNT; ++s) self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + s] = 0xC0;
            self->stateArm[self->armID] = EggJanken_Arm_ExtendArm;
        }

        self->state = EggJanken_State_ExtendDropArms;
    }
}

void EggJanken_State_ExtendDropArms(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_Arm_Idle)
        self->state = EggJanken_State_DropTarget;
}

void EggJanken_State_DropTarget(void)
{
    RSDK_THIS(EggJanken);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (abs(self->position.x - player1->position.x) >= 0xC0000) {
        if (self->position.x >= player1->position.x)
            self->origin.x -= 0x10000;
        else
            self->origin.x += 0x10000;
    }
    else {
        for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
            for (self->armJointID = 0; self->armJointID < EGGJANKEN_SEGMENT_COUNT; ++self->armJointID) {
                self->jointAngleVels[self->armID]                                                 = 8;
                self->jointDelays[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]       = 4 * self->armJointID;
                self->jointTargetAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] = 160;
                self->jointDirection[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]    = 1;
            }

            self->stateArm[self->armID] = EggJanken_Arm_Dropping;
        }

        RSDK.PlaySfx(EggJanken->sfxDrop, false, 0xFF);
        self->state = EggJanken_State_Drop;
    }
}

void EggJanken_State_Drop(void)
{
    RSDK_THIS(EggJanken);

    self->origin.y += self->velocity.y;
    self->position.y = self->origin.y;
    self->velocity.y += 0x7000;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        for (self->armID = 0; self->armID < EGGJANKEN_ARM_COUNT; ++self->armID) {
            for (self->armJointID = 0; self->armJointID < EGGJANKEN_SEGMENT_COUNT; ++self->armJointID) {
                self->jointAngleVels[self->armID]                                                 = 8;
                self->jointDelays[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]       = 4 * self->armJointID;
                self->jointTargetAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] = 200;
                self->jointDirection[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID]    = 0;
            }

            self->stateArm[self->armID] = EggJanken_Arm_Dropping;
        }

        RSDK.PlaySfx(EggJanken->sfxImpact4, false, 255);
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->shakePos.y   = 4;
        self->velocity.y     = 0;
        self->state          = EggJanken_State_DropArms;
    }
}

void EggJanken_State_DropArms(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_Arm_Idle) {
        RSDK.PlaySfx(EggJanken->sfxImpact3, false, 255);
        self->state = EggJanken_State_Dropped;
    }
}

void EggJanken_State_Dropped(void)
{
    RSDK_THIS(EggJanken);

    if (++self->timer == 60) {
        self->timer = 0;
        for (int32 a = 0; a < EGGJANKEN_ARM_COUNT; ++a) self->stateArm[a] = EggJanken_Arm_RetractArm;
        self->state = EggJanken_State_RetractDropArms;
    }
}

void EggJanken_State_RetractDropArms(void)
{
    RSDK_THIS(EggJanken);

    if (self->stateArm[0] == EggJanken_Arm_Idle)
        self->state = EggJanken_State_RiseUp;
}

void EggJanken_State_RiseUp(void)
{
    RSDK_THIS(EggJanken);

    self->origin.y -= 0x20000;
    self->position.y = self->origin.y;

    if (self->position.y < self->startY) {
        self->origin.y   = self->startY;
        self->position.y = self->startY;
        self->state      = EggJanken_State_FlipBackOver;
    }
}

void EggJanken_State_FlipBackOver(void)
{
    RSDK_THIS(EggJanken);

    self->fullRotation += 0x400;

    if ((self->fullRotation & 0x1FFFF) == 0) {
        self->angle    = 0;
        self->isMoving = true;
        EggJanken_ResetStates();
    }
}

void EggJanken_Eyes_Setup(void)
{
    RSDK_THIS(EggJanken);

    self->slotTimer    = 0;
    self->eyeFrames[0] = RSDK.Rand(0, 3);
    self->eyeFrames[1] = RSDK.Rand(0, 3);
    self->stateEyes    = EggJanken_Eyes_ChangeSlots;

    self->buttonAnimator.frameID = 0;
    RSDK.PlaySfx(EggJanken->sfxClick, false, 255);
}

void EggJanken_Eyes_ChangeSlots(void)
{
    RSDK_THIS(EggJanken);

    ++self->slotTimer;

    switch (self->health) {
        default: break;

        case 1:
            if (!(self->slotTimer & 0x3F)) {
                self->eyeFrames[0] = RSDK.Rand(0, 3);
                self->eyeFrames[1] = RSDK.Rand(0, 3);
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }
            break;

        case 2:
            if (!(self->slotTimer & 0x3F)) {
                self->eyeFrames[0] = (self->eyeFrames[0] + 2) % 3;
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }

            if (!(self->slotTimer & 0x7F)) {
                self->eyeFrames[1] = (self->eyeFrames[1] + 1) % 3;
                RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
            }
            break;

        case 3:
            if (self->health == 3) {
                if (!(self->slotTimer & 0x3F)) {
                    self->eyeFrames[0] = (self->eyeFrames[0] + 1) % 3;
                    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
                }

                if (!(self->slotTimer & 0x7F)) {
                    self->eyeFrames[1] = (self->eyeFrames[1] + 1) % 3;
                    RSDK.PlaySfx(EggJanken->sfxBeep3, false, 255);
                }
            }
            break;
    }
}

void EggJanken_Eyes_None(void)
{
    // we are NOT playing Janken rn..
}

void EggJanken_Arm_None(void)
{
    // no arm movement
}

void EggJanken_Arm_Idle(void)
{
    // although this state is the same as EggJanken_Arm_None
    // its used to determine if the boss is done with its state rather than not active at all
}

void EggJanken_Arm_RetractArm(void)
{
    RSDK_THIS(EggJanken);

    if (self->armRadius[self->armID])
        self->armRadius[self->armID] -= 0x8000;
    else
        self->stateArm[self->armID] = EggJanken_Arm_Idle;
}

void EggJanken_Arm_ExtendArm(void)
{
    RSDK_THIS(EggJanken);

    if (self->armRadius[self->armID] != 0x800000) {
        self->armRadius[self->armID] += 0x10000;
    }
    else {
        self->armRadiusSpeed[self->armID] = 0x8000;
        self->stateArm[self->armID]       = EggJanken_Arm_Idle;
    }
}

void EggJanken_Arm_StretchRetractArm(void)
{
    RSDK_THIS(EggJanken);

    self->armRadius[self->armID] += self->armRadiusSpeed[self->armID];
    self->armRadiusSpeed[self->armID] -= 0x800;

    if (self->armRadius[self->armID] <= 0) {
        if (self->state == EggJanken_State_HitShake && !self->armID) {
            RSDK.PlaySfx(EggJanken->sfxHit, false, 255);
            self->invincibilityTimer = 60;
            self->eyeFrames[0]       = 8;
            self->eyeFrames[1]       = 9;
            --self->health;
        }

        self->armRadius[self->armID] = 0;
        self->stateArm[self->armID]  = EggJanken_Arm_Idle;
    }
}

void EggJanken_Arm_SwingArm(void)
{
    RSDK_THIS(EggJanken);

    if (self->jointFlags[self->armID] == ((1 << EGGJANKEN_SEGMENT_COUNT) - 1))
        EggJanken_SwapArmSwingDir();

    int32 slot = self->armJointID + EGGJANKEN_SEGMENT_COUNT * self->armID;

    if (self->jointDelays[slot]) {
        self->jointDelays[slot]--;
    }
    else {
        if (self->jointDirection[slot]) {
            if (self->jointAngles[slot] > self->jointTargetAngles[slot])
                self->jointAngles[slot] -= self->jointAngleVels[self->armID];
            else
                self->jointFlags[self->armID] |= 1 << self->armJointID;
        }
        else {
            if (self->jointAngles[slot] < self->jointTargetAngles[slot])
                self->jointAngles[slot] += self->jointAngleVels[self->armID];
            else
                self->jointFlags[self->armID] |= 1 << self->armJointID;
        }
    }
}

void EggJanken_Arm_ArmAttack(void)
{
    RSDK_THIS(EggJanken);

    int32 slot = self->armJointID + EGGJANKEN_SEGMENT_COUNT * self->armID;

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
        else {
            if (self->jointAngles[slot] < self->jointTargetAngles[slot]) {
                self->jointAngles[slot] += self->jointAngleVels[self->armID];
            }
            else {
                self->jointFlags[self->armID] |= (1 << self->armJointID);
                self->jointAngles[slot] = self->jointTargetAngles[slot];
            }
        }
    }

    self->armRadius[self->armID] += self->armRadiusSpeed[self->armID];
    self->armRadiusSpeed[self->armID] -= 0x800;

    if (self->armRadius[self->armID] <= 0) {
        self->jointFlags[self->armID] = 0;
        self->armRadius[self->armID]  = 0;
        self->stateArm[self->armID]   = EggJanken_Arm_Idle;
    }
}

void EggJanken_Arm_Dropping(void)
{
    RSDK_THIS(EggJanken);

    if (self->jointFlags[self->armID] == ((1 << EGGJANKEN_SEGMENT_COUNT) - 1)) {
        self->jointFlags[self->armID] = 0;
        self->stateArm[self->armID]   = EggJanken_Arm_Idle;
    }

    int32 slot = self->armJointID + EGGJANKEN_SEGMENT_COUNT * self->armID;

    if (self->jointDelays[slot]) {
        self->jointDelays[slot]--;
    }
    else {
        if (self->jointDirection[slot]) {
            if (self->jointAngles[slot] > self->jointTargetAngles[slot]) {
                self->jointAngles[slot] -= self->jointAngleVels[self->armID];
            }
            else {
                self->jointFlags[self->armID] |= 1 << self->armJointID;
                self->jointAngles[slot] = self->jointTargetAngles[slot];
            }
        }
        else {
            if (self->jointAngles[slot] < self->jointTargetAngles[slot]) {
                self->jointAngles[slot] += self->jointAngleVels[self->armID];
            }
            else {
                self->jointFlags[self->armID] |= 1 << self->armJointID;
                self->jointAngles[slot] = self->jointTargetAngles[slot];
            }
        }
    }
}

void EggJanken_Draw_Closed(void)
{
    RSDK_THIS(EggJanken);

    RSDK.DrawSprite(&self->buttonAnimator, NULL, false);
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
    RSDK.DrawSprite(&self->propellorLAnimator, NULL, false);
    RSDK.DrawSprite(&self->propellorRAnimator, NULL, false);
}

void EggJanken_Draw_Active(void)
{
    RSDK_THIS(EggJanken);
    Vector2 drawPos;

    self->armID = 0;
    drawPos.x   = self->radius * RSDK.Sin256(0xC0 - (self->rotation >> 1));
    drawPos.y   = self->radius * RSDK.Cos256(0xC0 - (self->rotation >> 1));
    drawPos.x += self->position.x;
    drawPos.y += self->position.y;
    EggJanken->armAnimator.frameID = 0;
    RSDK.DrawSprite(&EggJanken->armAnimator, &drawPos, false);

    int32 radius                   = (self->armRadius[self->armID] >> 16);
    EggJanken->armAnimator.frameID = 1;
    self->armJointID               = 0;
    for (self->armJointID = 0; self->armJointID < EGGJANKEN_JOINT_COUNT; ++self->armJointID) {
        drawPos.x += 32 * radius * RSDK.Sin256(self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] + (self->rotation >> 1));
        drawPos.y -= 32 * radius * RSDK.Cos256(self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] + (self->rotation >> 1));
        RSDK.DrawSprite(&EggJanken->armAnimator, &drawPos, false);
    }

    EggJanken->armAnimator.frameID = 2;
    drawPos.x += (radius * RSDK.Sin256(self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] + (self->rotation >> 1))) << 6;
    drawPos.y -= (radius * RSDK.Cos256(self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] + (self->rotation >> 1))) << 6;
    RSDK.DrawSprite(&EggJanken->armAnimator, &drawPos, false);

    self->armPos[0] = drawPos;
    self->armID     = 1;
    drawPos.x       = self->radius * RSDK.Sin256(0x40 - (self->rotation >> 1));
    drawPos.y       = self->radius * RSDK.Cos256(0x40 - (self->rotation >> 1));
    drawPos.x += self->position.x;
    drawPos.y += self->position.y;
    EggJanken->armAnimator.frameID = 0;
    RSDK.DrawSprite(&EggJanken->armAnimator, &drawPos, false);

    radius                         = (self->armRadius[self->armID] >> 16);
    EggJanken->armAnimator.frameID = 1;
    self->armJointID               = 0;
    for (self->armJointID = 0; self->armJointID < EGGJANKEN_JOINT_COUNT; ++self->armJointID) {
        drawPos.x -= 32 * radius * RSDK.Sin256(self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] - (self->rotation >> 1));
        drawPos.y -= 32 * radius * RSDK.Cos256(self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] - (self->rotation >> 1));
        RSDK.DrawSprite(&EggJanken->armAnimator, &drawPos, false);
    }

    EggJanken->armAnimator.frameID = 2;
    drawPos.x -= (radius * RSDK.Sin256(self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] - (self->rotation >> 1))) << 6;
    drawPos.y -= (radius * RSDK.Cos256(self->jointAngles[EGGJANKEN_SEGMENT_COUNT * self->armID + self->armJointID] - (self->rotation >> 1))) << 6;
    RSDK.DrawSprite(&EggJanken->armAnimator, &drawPos, false);

    self->armPos[1] = drawPos;
    RSDK.DrawSprite(&self->buttonAnimator, NULL, false);
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    if (Zone->timer & 1) {
        self->inkEffect                                                    = INK_ADD;
        self->alpha                                                        = 128;
        self->eyeAnimator.frameID                                          = self->eyeFrames[0];
        RSDK.GetFrame(EggJanken->aniFrames, 0, self->eyeFrames[0])->pivotX = -24;
        RSDK.DrawSprite(&self->eyeAnimator, NULL, false);

        self->eyeAnimator.frameID                                          = self->eyeFrames[1];
        RSDK.GetFrame(EggJanken->aniFrames, 0, self->eyeFrames[1])->pivotX = 0;
        RSDK.DrawSprite(&self->eyeAnimator, NULL, false);

        self->inkEffect = INK_NONE;
        self->alpha     = 0x200;
    }

    RSDK.DrawSprite(&self->propellorLAnimator, NULL, false);
    RSDK.DrawSprite(&self->propellorRAnimator, NULL, false);
}

void EggJanken_Draw_Destroyed(void)
{
    RSDK_THIS(EggJanken);

    if (Zone->timer & 1) {
        RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
        RSDK.DrawSprite(&self->propellorLAnimator, NULL, false);
        RSDK.DrawSprite(&self->propellorRAnimator, NULL, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void EggJanken_EditorDraw(void)
{
    RSDK_THIS(EggJanken);

    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 0, &self->eyeAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 7, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 2, &self->buttonAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 4, &self->propellorLAnimator, true, 0);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 5, &self->propellorRAnimator, true, 0);

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->visible       = false;
    self->drawFX |= FX_ROTATE | FX_FLIP;
    self->origin.x               = self->position.x;
    self->origin.y               = self->position.y;
    self->startY                 = self->position.y + 0x100000;
    self->eyeFrames[0]           = 3;
    self->eyeFrames[1]           = 4;
    self->health                 = 3;
    self->buttonAnimator.frameID = 1;

    EggJanken_Draw_Closed();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 208, 1 | 0 | 4 | 8, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void EggJanken_EditorLoad(void)
{
    EggJanken->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EggJanken->aniFrames, 3, &EggJanken->armAnimator, true, 0);
}
#endif

void EggJanken_Serialize(void) {}

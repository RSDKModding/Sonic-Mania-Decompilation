// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LottoMachine Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLottoMachine *LottoMachine;

void LottoMachine_Update(void)
{
    RSDK_THIS(LottoMachine);

    StateMachine_Run(self->state);

    if (self->chutePos >= self->chuteTargetPos) {
        if (self->chutePos > self->chuteTargetPos) {
            self->chutePos -= self->chuteVel;
            if (self->chutePos < 0)
                self->chuteAnimator.frameID = 1;
        }
    }
    else {
        self->chutePos += self->chuteVel;
        if (self->chutePos > 0)
            self->chuteAnimator.frameID = 2;
    }

    self->angle = (self->angle + self->spinSpeed) & 0x1FF0000;
}

void LottoMachine_LateUpdate(void) {}

void LottoMachine_StaticUpdate(void)
{
    LottoMachine->shineAnimator.frameID = SPZ2Setup->stageLightsFrame;

    foreach_all(LottoMachine, lottoMachine)
    {
        int32 slot = RSDK.GetEntitySlot(lottoMachine);
        RSDK.AddDrawListRef(Zone->objectDrawGroup[0], slot);
        RSDK.AddDrawListRef(lottoMachine->drawGroupHigh, slot);
    }
}

void LottoMachine_Draw(void)
{
    RSDK_THIS(LottoMachine);

    Vector2 drawPos;
    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        self->direction = FLIP_NONE;
        self->rotation  = self->angle >> 16;
        RSDK.DrawSprite(&self->supportAnimator, NULL, false);

        self->rotation += 0x80;
        RSDK.DrawSprite(&self->supportAnimator, NULL, false);

        self->rotation += 0x80;
        RSDK.DrawSprite(&self->supportAnimator, NULL, false);

        self->rotation += 0x80;
        RSDK.DrawSprite(&self->supportAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->motorAnimator, NULL, false);

        self->machineAnimator.frameID = 7;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->glassAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->glassAnimator, NULL, false);

        self->machineAnimator.frameID = 0;
        self->inkEffect               = INK_ADD;
        self->direction               = FLIP_NONE;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&LottoMachine->shineAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&LottoMachine->shineAnimator, NULL, false);

        self->inkEffect = INK_NONE;
        for (int32 i = 0; i < 3; ++i) {
            self->direction               = FLIP_NONE;
            self->machineAnimator.frameID = i + 2;
            RSDK.DrawSprite(&self->machineAnimator, NULL, false);
            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->machineAnimator, NULL, false);
        }
    }
    else if (SceneInfo->currentDrawGroup == self->drawGroupHigh) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x6C0000 + self->chutePos;
        RSDK.DrawSprite(&self->chuteAnimator, &drawPos, false);
    }
    else {
        self->machineAnimator.frameID = 8;
        self->direction               = FLIP_NONE;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);

        self->direction = FLIP_Y;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);

        self->direction = FLIP_XY;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);

        self->direction = FLIP_NONE;
    }
}

void LottoMachine_Create(void *data)
{
    RSDK_THIS(LottoMachine);

    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 0, &self->machineAnimator, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 1, &self->supportAnimator, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 2, &self->chuteAnimator, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 3, &self->motorAnimator, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 4, &self->glassAnimator, true, 0);

    if (SceneInfo->inEditor) {
        self->drawFX = FX_FLIP;
    }
    else {
        self->active              = ACTIVE_BOUNDS;
        self->updateRange.x       = 0x1000000;
        self->updateRange.y       = 0x1000000;
        self->visible             = true;
        self->drawGroup           = Zone->objectDrawGroup[1] + 1;
        self->alpha               = 160;
        self->drawFX              = FX_ROTATE | FX_FLIP;
        self->chuteTargetPos      = 0x180000;
        self->chutePos            = 0x180000;
        self->drawGroupHigh       = Zone->objectDrawGroup[1];
        self->motorAnimator.speed = 0;
        self->state               = LottoMachine_State_Startup;
    }
}

void LottoMachine_StageLoad(void)
{
    LottoMachine->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LottoMachine.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 5, &LottoMachine->shineAnimator, true, 0);

    LottoMachine->hitboxBottom.left   = -16;
    LottoMachine->hitboxBottom.top    = 124;
    LottoMachine->hitboxBottom.right  = 16;
    LottoMachine->hitboxBottom.bottom = 160;

    LottoMachine->hitboxMotor.left   = -48;
    LottoMachine->hitboxMotor.top    = 92;
    LottoMachine->hitboxMotor.right  = 48;
    LottoMachine->hitboxMotor.bottom = 124;

    LottoMachine->active = ACTIVE_ALWAYS;

    LottoMachine->sfxPimPom = RSDK.GetSfx("Stage/PimPom.wav");
    LottoMachine->sfxFail   = RSDK.GetSfx("Stage/Fail.wav");

    Zone_AddVSSwapCallback(LottoMachine_VSSwap_CheckBusy);
}

void LottoMachine_VSSwap_CheckBusy(void)
{
#if MANIA_USE_PLUS
    if ((1 << Zone->swapPlayerID) & LottoMachine->activePlayers)
        Zone->playerSwapEnabled[Zone->swapPlayerID] = false;
#else
    if (LottoMachine->activePlayers)
        Zone->playerSwapEnabled = false;
#endif
}

void LottoMachine_CheckPlayerCollisions(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (self->playerAngles[playerID] <= 0) {
            // Inside the Machine
            if (RSDK.CheckObjectCollisionTouchCircle(self, 0x600000, player, 0x100000)) {

                bool32 playerMatch = false;
                for (int32 p = 0; p < self->playerCount; ++p) {
                    EntityPlayer *playerPtr = self->playerPtrs[p];
                    if (player == playerPtr)
                        playerMatch = true;
                }

                if (!playerMatch) {
                    if ((!player->sidekick || self->playerCount) && (!self->collectedBallCount || player->sidekick)) {
                        if (self->playerCount < Player->playerCount) {
                            if (!self->playerCount)
                                self->playerDir = player->direction;

                            self->playerPtrs[self->playerCount++] = player;

                            if (Zone->cameraBoundsB[playerID] != (self->position.y >> 16) + 160) {
                                self->playerTimers[playerID]  = Zone->cameraBoundsB[playerID];
                                Zone->cameraBoundsB[playerID] = (self->position.y >> 16) + 160;
                            }

                            LottoMachine->activePlayers |= 1 << playerID;
                            if (!self->timer)
                                self->timer = 1;
                        }
                    }
                    else {
                        player->velocity.x           = 0;
                        player->velocity.y           = -0xA0000;
                        player->applyJumpCap         = false;
                        self->playerAngles[playerID] = 16;
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                        RSDK.PlaySfx(LottoMachine->sfxFail, false, 255);
                    }
                }
            }
            else if (RSDK.CheckObjectCollisionTouchCircle(self, 0x730000, player, 0x100000)) { // Outside of the machine
                int32 angle  = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
                int32 vel    = MAX((abs(player->velocity.x) + abs(player->velocity.y)) >> 8, 0x400);
                int32 radius = RSDK.Rand(512, vel);

                angle += RSDK.Rand(-6, 6);
                int32 velX = radius * RSDK.Cos256(angle);
                int32 velY = radius * RSDK.Sin256(angle);

                if ((player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_GLIDE) || player->state == Player_State_FlyCarried
#if MANIA_USE_PLUS
                    || player->state == Player_State_RayGlide
#endif
                ) {
                    if (player->state == Player_State_FlyCarried)
                        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;
                    player->state = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                }

                player->groundVel            = velX;
                player->velocity.x           = velX;
                player->velocity.y           = velY;
                player->onGround             = false;
                player->applyJumpCap         = false;
                self->playerAngles[playerID] = 8;
                RSDK.PlaySfx(LottoMachine->sfxPimPom, false, 255);
            }
        }
        else {
            --self->playerAngles[playerID];
        }
    }
}

void LottoMachine_CheckPlayerCollisions_Bottom(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(Player, player)
    {
        if (!player->sidekick || self->playerCount) {
            if (Player_CheckCollisionTouch(player, self, &LottoMachine->hitboxBottom)) {
                bool32 hasPlayer = false;
                for (int32 p = 0; p < self->playerCount; ++p) {
                    EntityPlayer *playerPtr = self->playerPtrs[p];
                    if (player == playerPtr)
                        hasPlayer = true;
                }

                if (!hasPlayer) {
                    if (self->playerCount < Player->playerCount) {
                        if (!self->playerCount)
                            self->playerDir = player->direction;

                        self->playerPtrs[self->playerCount++] = player;
                        player->state                         = Player_State_Static;
                        player->position.x                    = self->position.x;
                        player->position.y                    = self->position.y + 0x980000;
                        player->velocity.x                    = 0;
                        player->velocity.y                    = 0;

                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                        RSDK.PlaySfx(Player->sfxRoll, false, 255);
                        LottoMachine->activePlayers |= 1 << player->playerID;

                        if (!self->timer)
                            self->timer = 1;
                    }
                }
            }
        }
    }
}

void LottoMachine_HandleMotor(void)
{
    RSDK_THIS(LottoMachine);

    int32 vel = 0;
    for (int32 p = 0; p < self->playerCount; ++p) {
        EntityPlayer *player = self->playerPtrs[p];
        if (player) {
            if (player->state != Player_State_Death) {
                player->position.x = self->position.x;
                Player_CheckCollisionPlatform(player, self, &LottoMachine->hitboxMotor);
                player->direction        = self->playerDir;
                player->jumpAbilityState = 0;

                if (player->onGround) {
                    vel += player->groundVel;

                    if (self->playerDir) {
                        if (player->groundVel > -0x10000)
                            player->groundVel = -0x10000;
                    }
                    else if (player->groundVel < 0x10000) {
                        player->groundVel = 0x10000;
                    }
                }
            }
        }
    }

    if (self->playerDir) {
        if (self->spinSpeed < -0x10000) {
            self->spinSpeed += 0x800;
            if (self->spinSpeed > -0x10000)
                self->spinSpeed = -0x10000;
        }

        if (vel < self->spinSpeed)
            self->spinSpeed = vel;

        self->motorAnimator.speed = -self->spinSpeed >> 11;
    }
    else {
        if (self->spinSpeed > 0x10000) {
            self->spinSpeed -= 0x800;
            if (self->spinSpeed < 0x10000)
                self->spinSpeed = 0x10000;
        }

        if (vel > self->spinSpeed)
            self->spinSpeed = vel;

        self->motorAnimator.speed = self->spinSpeed >> 11;
    }

    if (self->motorAnimator.speed > 0x80)
        self->motorAnimator.speed = 0x80;
}

void LottoMachine_SetupBalls(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(LottoBall, ball)
    {
        if (abs(ball->position.x - self->position.x) < 0x900000 && abs(ball->position.y - self->position.y) < 0x900000) {
            ball->state  = LottoBall_State_FallIntoMachine;
            ball->parent = self;
            ball->timer  = 16;
        }
    }
}

void LottoMachine_SetupUIBalls(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(LottoBall, ball)
    {
        if (ball->isUIBall) {
            if (ball->isVisible[RSDK.GetEntitySlot(self->playerPtrs[0])])
                ball->state = LottoBall_State_SetupUIBall;
        }
    }
}

void LottoMachine_GiveRings(void)
{
    RSDK_THIS(LottoMachine);

    int32 multiplier = 1;
    int32 bonus      = 0;

    int32 playerCount = 0;
    for (int32 p = 0; p < PLAYER_COUNT; ++p) {
        uint8 reward = self->collectedBallTypes[p] & 0xFF;
        switch (self->collectedBallTypes[p] >> 8) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW: bonus += reward; break;
            case LOTTOBALL_MULTI: multiplier *= reward; break;
            case LOTTOBALL_EGGMAN: ++playerCount; break;
            default: break;
        }
    }
    int32 reward = bonus * multiplier;

    for (int32 p = 0; p < self->playerCount; ++p) {
        EntityPlayer *player = self->playerPtrs[p];
        if (player->state != Player_State_Death && !player->sidekick) {
            int32 rings = 0;
            int32 count = reward;

            if (playerCount == 4) {
                rings = player->rings;
            }
            else if (playerCount > 0) {
                rings = player->rings - (player->rings >> playerCount);
                if (!player->rings)
                    count = reward >> playerCount;
            }

            int32 ringCount = count - rings * multiplier;
            Player_GiveRings(player, ringCount, false);

            if (ringCount > 0) {
                RSDK.PlaySfx(Ring->sfxRing, false, 0xFF);
            }
            else if (!ringCount) {
                RSDK.PlaySfx(LottoMachine->sfxFail, false, 0xFF);
            }
            else if (ringCount < 0) {
                Vector2 pos;
                pos.x = self->position.x;
                pos.y = self->position.y + 0x540000;
                Ring_FakeLoseRings(&pos, -ringCount, self->drawGroup);
                RSDK.PlaySfx(Player->sfxLoseRings, false, 255);
                player->state = Player_State_Hurt;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                player->velocity.y      = -0x40000;
                player->onGround        = false;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
            }

            EntityLottoBall *ball             = CREATE_ENTITY(LottoBall, NULL, ScreenInfo->center.x << 16, (ScreenInfo->size.y - 48) << 16);
            ball->type                        = LOTTOBALL_BIG;
            ball->isUIBall                    = true;
            ball->ringCount                   = ringCount;
            ball->drawGroup                   = Zone->hudDrawGroup;
            ball->active                      = ACTIVE_NORMAL;
            ball->drawFX                      = FX_SCALE;
            ball->state                       = LottoBall_State_ShowUIBall;
            ball->isVisible[player->playerID] = true;
            if (ringCount <= 0)
                ball->angle = 0x800000;

            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 6, &ball->ballAnimator, true, 0);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 6, &ball->leftNumAnimator, true, 1);
        }
    }
}

void LottoMachine_State_Startup(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_CheckPlayerCollisions_Bottom();
    LottoMachine_HandleMotor();

    if (self->timer > 0) {
        self->timer++;
        if (self->timer > 60) {
            self->timer = 0;
            RSDK.PlaySfx(Player->sfxRelease, false, 0xFF);

            for (int32 p = 0; p < self->playerCount; ++p) {
                EntityPlayer *player = self->playerPtrs[p];

                if (player && player->state != Player_State_Death) {
                    int32 playerID = RSDK.GetEntitySlot(player);
                    if (player->state == Player_State_Static) {
                        player->state                = Player_State_Air;
                        player->velocity.x           = 0;
                        player->velocity.y           = -0x98000;
                        player->applyJumpCap         = false;
                        self->playerAngles[playerID] = 32;
                    }

                    if (Zone->cameraBoundsB[playerID] != (self->position.y >> 16) + 160) {
                        self->playerTimers[playerID]  = Zone->cameraBoundsB[playerID];
                        Zone->cameraBoundsB[playerID] = ((self->position.y >> 16) + 160);
                    }
                }
            }

            LottoMachine_SetupBalls();
            self->chuteVel       = 0x10000;
            self->spinSpeed      = !self->playerDir ? 0x10000 : -0x10000;
            self->chuteTargetPos = 0x80000;
            self->state          = LottoMachine_State_HandleBallCollect;
        }
    }
}

void LottoMachine_State_HandleBallCollect(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_HandleMotor();

    RSDK.ProcessAnimation(&self->motorAnimator);
    self->glassAnimator.frameID = self->motorAnimator.frameID;

    switch (self->collectedBallCount) {
        case 0:
            if (++self->timer == 240) {
                self->timer          = 0;
                self->chuteVel       = 0x20000;
                self->chuteTargetPos = -0x180000;
                self->drawGroupHigh  = Zone->objectDrawGroup[0] + 1;
                self->state          = LottoMachine_State_CollectBall;
            }
            break;

        case 1:
        case 2:
        case 3:
            if (++self->timer == 32) {
                self->timer          = 0;
                self->chuteVel       = 0x20000;
                self->chuteTargetPos = -0x180000;
                self->state          = LottoMachine_State_CollectBall;
            }
            break;

        case 4:
            if (++self->timer == 96) {
                self->timer              = 0;
                self->collectedBallCount = 5;
                LottoMachine_SetupUIBalls();
            }
            break;

        case 5:
            if (++self->timer == 32) {
                self->timer              = 0;
                self->collectedBallCount = 6;
                LottoMachine_GiveRings();
            }
            break;

        case 6:
            if (++self->timer == 64) {
                self->timer          = 0;
                self->chuteVel       = 0x20000;
                self->chuteTargetPos = -0x180000;
                self->drawGroupHigh  = Zone->objectDrawGroup[1];
                self->state          = LottoMachine_State_DropPlayers;
            }
            break;

        default: break;
    }
}

void LottoMachine_State_CollectBall(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_HandleMotor();

    RSDK.ProcessAnimation(&self->motorAnimator);
    self->glassAnimator.frameID = self->motorAnimator.frameID;

    if (self->timer == 20) {
        int32 dist               = 10000;
        EntityLottoBall *ballPtr = NULL;

        foreach_active(LottoBall, ball)
        {
            if (abs(ball->position.x - self->position.x) < 0x900000) {
                if (abs(ball->position.y - self->position.y) < 0x900000) {
                    int32 rx = (self->position.x - ball->position.x) >> 16;
                    int32 ry = (self->position.y - ball->position.y + 0x540000) >> 16;
                    if (rx * rx + ry * ry < dist) {
                        dist    = rx * rx + ry * ry;
                        ballPtr = ball;
                    }
                }
            }
        }

        if (ballPtr) {
            ballPtr->position.x   = self->position.x;
            ballPtr->position.y   = self->position.y + 0x540000;
            ballPtr->velocity.x   = 0;
            ballPtr->velocity.y   = 0;
            ballPtr->angle        = 0;
            ballPtr->angleVel     = 0;
            ballPtr->bounds.x     = (ScreenInfo->center.x - (self->collectedBallCount << 6) + 96) << 16;
            ballPtr->bounds.y     = (ScreenInfo->size.y - 48) << 16;
            ballPtr->timer        = 0;
            ballPtr->isVisible[0] = false;

            for (int32 p = 0; p < self->playerCount; ++p) {
                EntityPlayer *player = self->playerPtrs[p];
                if (player)
                    ballPtr->isVisible[RSDK.GetEntitySlot(player)] = true;
            }

            ballPtr->state                                       = LottoBall_State_Collected;
            self->collectedBallTypes[self->collectedBallCount++] = ballPtr->lottoNum + (ballPtr->type << 8);
        }
    }

    if (++self->timer == 32) {
        self->timer          = 0;
        self->chuteVel       = 0x40000;
        self->chuteTargetPos = 0x180000;
        self->state          = LottoMachine_State_HandleBallCollected;
    }
}

void LottoMachine_State_HandleBallCollected(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_HandleMotor();

    RSDK.ProcessAnimation(&self->motorAnimator);
    self->glassAnimator.frameID = self->motorAnimator.frameID;

    if (self->chutePos == self->chuteTargetPos) {
        self->chuteVel       = 0x10000;
        self->chuteTargetPos = 0x80000;
        self->state          = LottoMachine_State_HandleBallCollect;
    }
}

void LottoMachine_State_DropPlayers(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_HandleMotor();

    RSDK.ProcessAnimation(&self->motorAnimator);
    self->glassAnimator.frameID = self->motorAnimator.frameID;

    if (self->chutePos == self->chuteTargetPos) {
        bool32 onGround = true;
        for (int32 p = 0; p < self->playerCount; ++p) {
            EntityPlayer *player = self->playerPtrs[p];
            if (player && !player->onGround)
                onGround = false;
        }

        if (onGround) {
            for (int32 p = 0; p < self->playerCount; ++p) {
                EntityPlayer *player = self->playerPtrs[p];

                if (player && player->state != Player_State_Death) {
                    player->state        = Player_State_Static;
                    player->nextAirState = StateMachine_None;
                    player->velocity.x   = 0;
                    player->velocity.y   = 0;
                    player->groundVel    = 0;
                    player->onGround     = false;
                    player->applyJumpCap = false;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                    int32 playerID                = RSDK.GetEntitySlot(player);
                    Zone->cameraBoundsB[playerID] = self->playerTimers[playerID];
                }
            }

            self->state = LottoMachine_State_ReleasePlayers;
        }
    }
}

void LottoMachine_State_ReleasePlayers(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();

    self->spinSpeed -= self->spinSpeed >> 4;
    self->motorAnimator.speed -= self->motorAnimator.speed >> 4;

    RSDK.ProcessAnimation(&self->motorAnimator);
    self->glassAnimator.frameID = self->motorAnimator.frameID;

    if (++self->timer == 60) {
        self->timer          = 0;
        self->chuteVel       = 0x40000;
        self->chuteTargetPos = 0x180000;
        self->state          = LottoMachine_State_StopSpinning;

        for (int32 p = 0; p < self->playerCount; ++p) {
            EntityPlayer *player = self->playerPtrs[p];

            if (player && player->state != Player_State_Death) {
                player->state      = Player_State_Air;
                player->velocity.y = 0x40000;
                LottoMachine->activePlayers &= ~(1 << RSDK.GetEntitySlot(player));
                RSDK.PlaySfx(Player->sfxRelease, false, 0xFF);
                self->playerAngles[RSDK.GetEntitySlot(player)] = 32;
            }
        }
    }
}

void LottoMachine_State_StopSpinning(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    ++self->timer;

    self->spinSpeed -= (self->spinSpeed >> 4);
    self->motorAnimator.speed -= self->motorAnimator.speed >> 4;

    if (self->timer == 30) {
        for (int32 p = 0; p < PLAYER_COUNT; ++p) self->playerPtrs[p] = NULL;
        self->playerCount           = 0;
        self->collectedBallCount    = 0;
        self->chuteAnimator.frameID = 0;
        self->timer                 = 0;
        self->spinSpeed             = 0;
        self->state                 = LottoMachine_State_Startup;
    }
}

#if GAME_INCLUDE_EDITOR
void LottoMachine_EditorDraw(void)
{
    RSDK_THIS(LottoMachine);
    Vector2 drawPos;

    self->machineAnimator.frameID = 8;
    self->direction               = FLIP_NONE;
    RSDK.DrawSprite(&self->machineAnimator, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->machineAnimator, NULL, false);

    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->machineAnimator, NULL, false);

    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->machineAnimator, NULL, false);

    self->direction = FLIP_NONE;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + 0x6C0000 + self->chutePos;
    RSDK.DrawSprite(&self->chuteAnimator, &drawPos, false);

    self->direction = FLIP_NONE;
    self->rotation  = self->angle >> 16;
    RSDK.DrawSprite(&self->supportAnimator, NULL, false);

    self->rotation += 0x80;
    RSDK.DrawSprite(&self->supportAnimator, NULL, false);

    self->rotation += 0x80;
    RSDK.DrawSprite(&self->supportAnimator, NULL, false);

    self->rotation += 0x80;
    RSDK.DrawSprite(&self->supportAnimator, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->motorAnimator, NULL, false);

    self->machineAnimator.frameID = 7;
    RSDK.DrawSprite(&self->machineAnimator, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->glassAnimator, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->glassAnimator, NULL, false);

    self->machineAnimator.frameID = 0;
    self->inkEffect               = INK_ADD;
    self->direction               = FLIP_NONE;
    RSDK.DrawSprite(&self->machineAnimator, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->machineAnimator, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&LottoMachine->shineAnimator, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&LottoMachine->shineAnimator, NULL, false);

    self->inkEffect = INK_NONE;
    for (int32 i = 0; i < 3; ++i) {
        self->direction               = FLIP_NONE;
        self->machineAnimator.frameID = i + 2;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->machineAnimator, NULL, false);
    }
}

void LottoMachine_EditorLoad(void) { LottoMachine->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LottoMachine.bin", SCOPE_STAGE); }
#endif

void LottoMachine_Serialize(void) {}

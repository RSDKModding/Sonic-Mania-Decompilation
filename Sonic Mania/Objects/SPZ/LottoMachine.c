#include "SonicMania.h"

ObjectLottoMachine *LottoMachine;

void LottoMachine_Update(void)
{
    RSDK_THIS(LottoMachine);
    StateMachine_Run(self->state);

    if (self->field_A0 >= self->field_A4) {
        if (self->field_A0 > self->field_A4) {
            self->field_A0 -= self->field_A8;
            if (self->field_A0 < 0)
                self->animator3.frameID = 1;
        }
    }
    else {
        self->field_A0 += self->field_A8;
        if (self->field_A0 > 0) {
            self->animator3.frameID = 2;
        }
    }
    self->angle = (self->angle + self->field_74) & 0x1FF0000;
}

void LottoMachine_LateUpdate(void) {}

void LottoMachine_StaticUpdate(void)
{
    LottoMachine->animator.frameID = SPZ2Setup->frameD;
    foreach_all(LottoMachine, lottoMachine)
    {
        int slot = RSDK.GetEntityID(lottoMachine);
        RSDK.AddDrawListRef(Zone->drawOrderLow, slot);
        RSDK.AddDrawListRef(lottoMachine->drawOrderHigh, slot);
    }
}

void LottoMachine_Draw(void)
{
    RSDK_THIS(LottoMachine);
    Vector2 drawPos;

    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        self->direction = FLIP_NONE;
        self->rotation  = self->angle >> 16;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->rotation += 0x80;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->rotation += 0x80;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->rotation += 0x80;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator4, NULL, false);

        self->animator1.frameID = 7;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator5, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator5, NULL, false);

        self->animator1.frameID = 0;
        self->inkEffect         = INK_ADD;
        self->direction         = FLIP_NONE;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&LottoMachine->animator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&LottoMachine->animator, NULL, false);

        self->inkEffect = INK_NONE;
        for (int i = 0; i < 3; ++i) {
            self->direction         = FLIP_NONE;
            self->animator1.frameID = i + 2;
            RSDK.DrawSprite(&self->animator1, NULL, false);
            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->animator1, NULL, false);
        }
    }
    else if (SceneInfo->currentDrawGroup == self->drawOrderHigh) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x6C0000 + self->field_A0;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);
    }
    else {
        self->animator1.frameID = 8;
        self->direction         = FLIP_NONE;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_Y;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_XY;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_NONE;
    }
}

void LottoMachine_Create(void *data)
{
    RSDK_THIS(LottoMachine);

    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 2, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 3, &self->animator4, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 4, &self->animator5, true, 0);
    if (SceneInfo->inEditor) {
        self->drawFX = FX_FLIP;
    }
    else {
        self->active                   = ACTIVE_BOUNDS;
        self->updateRange.x            = 0x1000000;
        self->updateRange.y            = 0x1000000;
        self->visible                  = true;
        self->drawOrder                = Zone->drawOrderHigh + 1;
        self->alpha                    = 160;
        self->drawFX                   = FX_ROTATE | FX_FLIP;
        self->field_A4                 = 0x180000;
        self->field_A0                 = 0x180000;
        self->drawOrderHigh            = Zone->drawOrderHigh;
        self->animator4.speed = 0;
        self->state                    = LottoMachine_State_Unknown1;
    }
}

void LottoMachine_StageLoad(void)
{
    LottoMachine->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LottoMachine.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 5, &LottoMachine->animator, true, 0);
    LottoMachine->hitbox1.left   = -16;
    LottoMachine->hitbox1.top    = 124;
    LottoMachine->hitbox1.right  = 16;
    LottoMachine->hitbox1.bottom = 160;
    LottoMachine->hitbox2.left   = -48;
    LottoMachine->hitbox2.top    = 92;
    LottoMachine->hitbox2.right  = 48;
    LottoMachine->hitbox2.bottom = 124;
    LottoMachine->active         = ACTIVE_ALWAYS;
    LottoMachine->sfxPimPom      = RSDK.GetSfx("Stage/PimPom.wav");
    LottoMachine->sfxFail        = RSDK.GetSfx("Stage/Fail.wav");
    Zone_AddVSSwapCallback(LottoMachine_ZoneCB);
}

void LottoMachine_ZoneCB(void)
{
#if RETRO_USE_PLUS
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
        int playerID = RSDK.GetEntityID(player);
        if (self->playerAngles[playerID] <= 0) {
            if (RSDK.CheckObjectCollisionTouchCircle(self, 0x600000, player, 0x100000)) {

                bool32 flag = false;
                for (int p = 0; p < self->playerCount; ++p) {
                    EntityPlayer *playerPtr = (EntityPlayer *)self->playerPtrs[p];
                    if (player == playerPtr) {
                        flag = true;
                    }
                }

                if (!flag) {
                    if ((!player->sidekick || self->playerCount) && (!self->field_8C || player->sidekick)) {
                        if (self->playerCount < Player->playerCount) {
                            if (!self->playerCount)
                                self->field_78 = player->direction;
                            self->playerPtrs[self->playerCount++] = (Entity *)player;

                            if (Zone->cameraBoundsB[playerID] != (self->position.y >> 16) + 160) {
                                self->playerTimers[playerID] = Zone->cameraBoundsB[playerID];
                                Zone->cameraBoundsB[playerID] = ((self->position.y >> 16) + 160);
                            }

                            LottoMachine->activePlayers |= (1 << playerID);
                            if (!self->timer)
                                self->timer = 1;
                        }
                    }
                    else {
                        player->velocity.x             = 0;
                        player->velocity.y             = -0xA0000;
                        player->jumpAbility            = 0;
                        self->playerAngles[playerID] = 16;
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                        RSDK.PlaySfx(LottoMachine->sfxFail, false, 255);
                    }
                }
            }
            else if (RSDK.CheckObjectCollisionTouchCircle(self, 0x730000, player, 0x100000)) {
                int angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
                int vel   = (abs(player->velocity.x) + abs(player->velocity.y)) >> 8;
                if (vel < 0x400)
                    vel = 0x400;
                int radius = RSDK.Rand(512, vel);
                angle += RSDK.Rand(-6, 6);
                int velX = radius * RSDK.Cos256(angle);
                int velY = radius * RSDK.Sin256(angle);
                if ((player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_FLY) || player->state == Player_State_FlyCarried
#if RETRO_USE_PLUS
                    || player->state == Player_State_RayGlide
#endif
                ) {
                    if (player->state == Player_State_FlyCarried)
                        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;
                    player->state = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                }
                player->velocity.x             = velX;
                player->groundVel              = velX;
                player->velocity.y             = velY;
                player->onGround               = false;
                player->jumpAbility            = 0;
                self->playerAngles[playerID] = 8;
                RSDK.PlaySfx(LottoMachine->sfxPimPom, false, 255);
            }
        }
        else {
            --self->playerAngles[playerID];
        }
    }
}

void LottoMachine_CheckPlayerCollisions2(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(Player, player)
    {
        if (!player->sidekick || self->playerCount) {
            if (Player_CheckCollisionTouch(player, self, &LottoMachine->hitbox1)) {
                bool32 flag = false;
                for (int p = 0; p < self->playerCount; ++p) {
                    EntityPlayer *playerPtr = (EntityPlayer *)self->playerPtrs[p];
                    if (player == playerPtr) {
                        flag = true;
                    }
                }

                if (!flag) {
                    if (self->playerCount < Player->playerCount) {
                        if (!self->playerCount)
                            self->field_78 = player->direction;
                        self->playerPtrs[self->playerCount++] = (Entity *)player;
                        player->state      = Player_State_None;
                        player->position.x = self->position.x;
                        player->position.y = self->position.y + 0x980000;
                        player->velocity.x = 0;
                        player->velocity.y = 0;
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

void LottoMachine_Unknown4(void)
{
    RSDK_THIS(LottoMachine);

    int vel = 0;
    for (int p = 0; p < self->playerCount; ++p) {
        EntityPlayer *player = (EntityPlayer *)self->playerPtrs[p];
        if (player) {
            if (player->state != Player_State_Die) {
                player->position.x = self->position.x;
                Player_CheckCollisionPlatform(player, self, &LottoMachine->hitbox2);
                player->direction        = self->field_78;
                player->jumpAbilityTimer = 0;
                if (player->onGround) {
                    vel += player->groundVel;
                    if (self->field_78) {
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

    if (self->field_78) {
        if (self->field_74 < -0x10000) {
            self->field_74 += 0x800;
            if (self->field_74 > -0x10000)
                self->field_74 = -0x10000;
        }
        if (vel < self->field_74)
            self->field_74 = vel;
        self->animator4.speed = -self->field_74 >> 11;
    }
    else {
        if (self->field_74 > 0x10000) {
            self->field_74 -= 0x800;
            if (self->field_74 < 0x10000)
                self->field_74 = 0x10000;
        }
        if (vel > self->field_74)
            self->field_74 = vel;
        self->animator4.speed = self->field_74 >> 11;
    }
    if (self->animator4.speed > 0x80)
        self->animator4.speed = 0x80;
}

void LottoMachine_Unknown5(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(LottoBall, ball)
    {
        if (abs(ball->position.x - self->position.x) < 0x900000 && abs(ball->position.y - self->position.y) < 0x900000) {
            ball->state  = LottoBall_Unknown1;
            ball->parent = (Entity *)self;
            ball->timer  = 16;
        }
    }
}

void LottoMachine_Unknown6(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(LottoBall, ball)
    {
        if (ball->screenRelative) {
            if (ball->screenFlags[RSDK.GetEntityID(self->playerPtrs[0])])
                ball->state = LottoBall_Unknown6;
        }
    }
}

void LottoMachine_GiveRings(void)
{
    RSDK_THIS(LottoMachine);

    int multiplier = 1;
    int bonus      = 0;

    int playerCount = 0;
    for (int p = 0; p < PLAYER_MAX; ++p) {
        uint8 reward = self->field_90[p] & 0xFF;
        switch (self->field_90[p] >> 8) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW: bonus += reward; break;
            case LOTTOBALL_MULTI: multiplier *= reward; break;
            case LOTTOBALL_EGGMAN: ++playerCount; break;
            default: break;
        }
    }
    int reward = bonus * multiplier;

    for (int p = 0; p < self->playerCount; ++p) {
        EntityPlayer *player = (EntityPlayer *)self->playerPtrs[p];
        if (player->state != Player_State_Die && !player->sidekick) {
            int rings = 0;
            int count = reward;
            if (playerCount == 4) {
                rings = player->rings;
            }
            else if (playerCount > 0) {
                rings = player->rings - (player->rings >> playerCount);
                if (!player->rings)
                    count = reward >> playerCount;
            }

            int ringCount = count - rings * multiplier;
            Player_GiveRings(ringCount, player, false);
            if (ringCount > 0) {
                RSDK.PlaySfx(Ring->sfxRing, false, 255);
            }
            else if (!ringCount) {
                RSDK.PlaySfx(LottoMachine->sfxFail, false, 255);
            }
            else if (ringCount < 0) {
                Vector2 pos;
                pos.x = self->position.x;
                pos.y = self->position.y + 0x540000;
                Ring_FakeLoseRings(&pos, -ringCount, self->drawOrder);
                RSDK.PlaySfx(Player->sfxLoseRings, false, 255);
                player->state = Player_State_Hit;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                player->velocity.y      = -0x40000;
                player->onGround        = false;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
            }

            EntityLottoBall *ball               = CREATE_ENTITY(LottoBall, NULL, ScreenInfo->centerX << 16, (ScreenInfo->height - 48) << 16);
            ball->type                          = LOTTOBALL_BIG;
            ball->screenRelative                = true;
            ball->ringCount                     = ringCount;
            ball->drawOrder                     = Zone->hudDrawOrder;
            ball->active                        = ACTIVE_NORMAL;
            ball->drawFX                        = FX_SCALE;
            ball->state                         = LottoBall_Unknown8;
            ball->screenFlags[player->playerID] = true;
            if (ringCount <= 0)
                ball->angle = 0x800000;
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 6, &ball->animator1, true, 0);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 6, &ball->animator2, true, 1);
        }
    }
}

void LottoMachine_State_Unknown1(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_CheckPlayerCollisions2();
    LottoMachine_Unknown4();

    if (self->timer > 0) {
        self->timer++;
        if (self->timer > 60) {
            self->timer = 0;
            RSDK.PlaySfx(Player->sfxRelease, false, 255);
            for (int p = 0; p < self->playerCount; ++p) {
                EntityPlayer *player = (EntityPlayer *)self->playerPtrs[p];
                if (player) {
                    if (player->state != Player_State_Die) {
                        int playerID = RSDK.GetEntityID(player);
                        if (player->state == Player_State_None) {
                            player->state                  = Player_State_Air;
                            player->velocity.x             = 0;
                            player->velocity.y             = -0x98000;
                            player->jumpAbility            = 0;
                            self->playerAngles[playerID] = 32;
                        }

                        if (Zone->cameraBoundsB[playerID] != (self->position.y >> 16) + 160) {
                            self->playerTimers[playerID] = Zone->cameraBoundsB[playerID];
                            Zone->cameraBoundsB[playerID] = ((self->position.y >> 16) + 160);
                        }
                    }
                }
            }

            LottoMachine_Unknown5();
            self->field_A8 = 0x10000;
            if (!self->field_78)
                self->field_74 = 0x10000;
            else
                self->field_74 = -0x10000;
            self->field_A4 = 0x80000;
            self->state    = LottoMachine_State_Unknown2;
        }
    }
}

void LottoMachine_State_Unknown2(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_Unknown4();
    RSDK.ProcessAnimation(&self->animator4);
    self->animator5.frameID = self->animator4.frameID;

    switch (self->field_8C) {
        case 0:
            if (++self->timer == 240) {
                self->timer         = 0;
                self->field_A8      = 0x20000;
                self->field_A4      = -0x180000;
                self->drawOrderHigh = Zone->drawOrderLow + 1;
                self->state         = LottoMachine_State_Unknown3;
            }
            break;
        case 1:
        case 2:
        case 3:
            if (++self->timer == 32) {
                self->timer    = 0;
                self->field_A8 = 0x20000;
                self->field_A4 = -0x180000;
                self->state    = LottoMachine_State_Unknown3;
            }
            break;
        case 4:
            if (++self->timer == 96) {
                self->timer    = 0;
                self->field_8C = 5;
                LottoMachine_Unknown6();
            }
            break;
        case 5:
            if (++self->timer == 32) {
                self->timer    = 0;
                self->field_8C = 6;
                LottoMachine_GiveRings();
            }
            break;
        case 6:
            if (++self->timer == 64) {
                self->timer         = 0;
                self->field_A8      = 0x20000;
                self->field_A4      = -0x180000;
                self->drawOrderHigh = Zone->drawOrderHigh;
                self->state         = LottoMachine_State_Unknown5;
            }
            break;
        default: break;
    }
}

void LottoMachine_State_Unknown3(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_Unknown4();
    RSDK.ProcessAnimation(&self->animator4);
    self->animator5.frameID = self->animator4.frameID;
    if (self->timer == 20) {
        int dist                 = 10000;
        EntityLottoBall *ballPtr = NULL;
        foreach_active(LottoBall, ball)
        {
            if (abs(ball->position.x - self->position.x) < 0x900000) {
                if (abs(ball->position.y - self->position.y) < 0x900000) {
                    int rx = (self->position.x - ball->position.x) >> 16;
                    int ry = (self->position.y - ball->position.y + 0x540000) >> 16;
                    if (rx * rx + ry * ry < dist) {
                        dist    = rx * rx + ry * ry;
                        ballPtr = ball;
                    }
                }
            }
        }

        if (ballPtr) {
            ballPtr->position.x     = self->position.x;
            ballPtr->position.y     = self->position.y + 0x540000;
            ballPtr->velocity.x     = 0;
            ballPtr->velocity.y     = 0;
            ballPtr->angle          = 0;
            ballPtr->angleVel       = 0;
            ballPtr->field_70       = (ScreenInfo->centerX - (self->field_8C << 6) + 96) << 16;
            ballPtr->field_74       = (ScreenInfo->height - 48) << 16;
            ballPtr->timer          = 0;
            ballPtr->screenFlags[0] = false;
            for (int p = 0; p < self->playerCount; ++p) {
                EntityPlayer *player = (EntityPlayer *)self->playerPtrs[p];
                if (player) {
                    ballPtr->screenFlags[RSDK.GetEntityID(player)] = true;
                }
            }

            ballPtr->state                     = LottoBall_Unknown4;
            self->field_90[self->field_8C] = ballPtr->lottoNum + (ballPtr->type << 8);
            ++self->field_8C;
        }
    }

    if (++self->timer == 32) {
        self->timer    = 0;
        self->field_A8 = 0x40000;
        self->field_A4 = 0x180000;
        self->state    = LottoMachine_State_Unknown4;
    }
}

void LottoMachine_State_Unknown4(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_Unknown4();
    RSDK.ProcessAnimation(&self->animator4);
    self->animator5.frameID = self->animator4.frameID;
    if (self->field_A0 == self->field_A4) {
        self->field_A8 = 0x10000;
        self->field_A4 = 0x80000;
        self->state    = LottoMachine_State_Unknown2;
    }
}

void LottoMachine_State_Unknown5(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_Unknown4();
    RSDK.ProcessAnimation(&self->animator4);
    self->animator5.frameID = self->animator4.frameID;

    if (self->field_A0 == self->field_A4) {
        bool32 flag = true;
        for (int p = 0; p < self->playerCount; ++p) {
            EntityPlayer *player = (EntityPlayer *)self->playerPtrs[p];
            if (player && !player->onGround) {
                flag = false;
            }
        }

        if (flag) {
            for (int p = 0; p < self->playerCount; ++p) {
                EntityPlayer *player = (EntityPlayer *)self->playerPtrs[p];
                if (player) {
                    if (player->state != Player_State_Die) {
                        player->state        = Player_State_None;
                        player->nextAirState = StateMachine_None;
                        player->velocity.x   = 0;
                        player->velocity.y   = 0;
                        player->groundVel    = 0;
                        player->onGround     = false;
                        player->jumpAbility  = 0;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                        int playerID                   = RSDK.GetEntityID(player);
                        Zone->cameraBoundsB[playerID] = self->playerTimers[playerID];
                    }
                }
            }
            self->state = LottoMachine_State_Unknown6;
        }
    }
}

void LottoMachine_State_Unknown6(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    self->field_74 -= self->field_74 >> 4;
    self->animator4.speed -= self->animator4.speed >> 4;
    RSDK.ProcessAnimation(&self->animator4);
    self->animator5.frameID = self->animator4.frameID;
    if (++self->timer == 60) {
        self->timer    = 0;
        self->field_A8 = 0x40000;
        self->field_A4 = 0x180000;
        self->state    = LottoMachine_State_Unknown7;

        for (int p = 0; p < self->playerCount; ++p) {
            EntityPlayer *player = (EntityPlayer *)self->playerPtrs[p];
            if (player) {
                if (player->state != Player_State_Die) {
                    player->state      = Player_State_Air;
                    player->velocity.y = 0x40000;
                    LottoMachine->activePlayers &= ~(1 << RSDK.GetEntityID(player));
                    RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    self->playerAngles[RSDK.GetEntityID(player)] = 32;
                }
            }
        }
    }
}

void LottoMachine_State_Unknown7(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    ++self->timer;
    self->field_74 -= (self->field_74 >> 4);
    self->animator4.speed -= self->animator4.speed >> 4;
    if (self->timer == 30) {
        self->playerPtrs[0]     = NULL;
        self->playerPtrs[1]     = NULL;
        self->playerPtrs[2]     = NULL;
        self->playerPtrs[3]     = NULL;
        self->playerCount       = 0;
        self->field_8C          = 0;
        self->animator3.frameID = 0;
        self->timer             = 0;
        self->field_74          = 0;
        self->state             = LottoMachine_State_Unknown1;
    }
}

#if RETRO_INCLUDE_EDITOR
void LottoMachine_EditorDraw(void)
{
    RSDK_THIS(LottoMachine);
    Vector2 drawPos;

    self->animator1.frameID = 8;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_Y;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_XY;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_NONE;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + 0x6C0000 + self->field_A0;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    self->direction = FLIP_NONE;
    self->rotation  = self->angle >> 16;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->rotation += 0x80;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->rotation += 0x80;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->rotation += 0x80;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    self->animator1.frameID = 7;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator5, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator5, NULL, false);

    self->animator1.frameID = 0;
    self->inkEffect         = INK_ADD;
    self->direction         = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&LottoMachine->animator, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&LottoMachine->animator, NULL, false);

    self->inkEffect = INK_NONE;
    for (int i = 0; i < 3; ++i) {
        self->direction         = FLIP_NONE;
        self->animator1.frameID = i + 2;
        RSDK.DrawSprite(&self->animator1, NULL, false);
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void LottoMachine_EditorLoad(void) { LottoMachine->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LottoMachine.bin", SCOPE_STAGE); }
#endif

void LottoMachine_Serialize(void) {}

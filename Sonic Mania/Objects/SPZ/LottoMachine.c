#include "SonicMania.h"

ObjectLottoMachine *LottoMachine;

void LottoMachine_Update(void)
{
    RSDK_THIS(LottoMachine);
    StateMachine_Run(entity->state);

    if (entity->field_A0 >= entity->field_A4) {
        if (entity->field_A0 > entity->field_A4) {
            entity->field_A0 -= entity->field_A8;
            if (entity->field_A0 < 0)
                entity->animator3.frameID = 1;
        }
    }
    else {
        entity->field_A0 += entity->field_A8;
        if (entity->field_A0 > 0) {
            entity->animator3.frameID = 2;
        }
    }
    entity->angle = (entity->angle + entity->field_74) & 0x1FF0000;
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

    if (RSDK_sceneInfo->currentDrawGroup == RSDK_sceneInfo->entity->drawOrder) {
        entity->direction = FLIP_NONE;
        entity->rotation  = entity->angle >> 16;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->rotation += 0x80;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->rotation += 0x80;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->rotation += 0x80;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator4, NULL, false);

        entity->animator1.frameID = 7;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator5, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator5, NULL, false);

        entity->animator1.frameID = 0;
        entity->inkEffect         = INK_ADD;
        entity->direction         = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&LottoMachine->animator, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&LottoMachine->animator, NULL, false);

        entity->inkEffect = INK_NONE;
        for (int i = 0; i < 3; ++i) {
            entity->direction         = FLIP_NONE;
            entity->animator1.frameID = i + 2;
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->animator1, NULL, false);
        }
    }
    else if (RSDK_sceneInfo->currentDrawGroup == entity->drawOrderHigh) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y + 0x6C0000 + entity->field_A0;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    }
    else {
        entity->animator1.frameID = 8;
        entity->direction         = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_Y;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_XY;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = FLIP_NONE;
    }
}

void LottoMachine_Create(void *data)
{
    RSDK_THIS(LottoMachine);

    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 2, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 3, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(LottoMachine->aniFrames, 4, &entity->animator5, true, 0);
    if (RSDK_sceneInfo->inEditor) {
        entity->drawFX = FX_FLIP;
    }
    else {
        entity->active                   = ACTIVE_BOUNDS;
        entity->updateRange.x            = 0x1000000;
        entity->updateRange.y            = 0x1000000;
        entity->visible                  = true;
        entity->drawOrder                = Zone->drawOrderHigh + 1;
        entity->alpha                    = 160;
        entity->drawFX                   = FX_ROTATE | FX_FLIP;
        entity->field_A4                 = 0x180000;
        entity->field_A0                 = 0x180000;
        entity->drawOrderHigh            = Zone->drawOrderHigh;
        entity->animator4.animationSpeed = 0;
        entity->state                    = LottoMachine_State_Unknown1;
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
    LottoMachine->sfxPimPom      = RSDK.GetSFX("Stage/PimPom.wav");
    LottoMachine->sfxFail        = RSDK.GetSFX("Stage/Fail.wav");
    Zone_AddCallback(LottoMachine_ZoneCB);
}

void LottoMachine_ZoneCB(void)
{
#if RETRO_USE_PLUS
    if ((1 << Zone->playerID) & LottoMachine->activePlayers)
        Zone->playerFlags[Zone->playerID] = 0;
#else
    if (LottoMachine->activePlayers)
        Zone->playerFlags = 0;
#endif
}

void LottoMachine_CheckPlayerCollisions(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (entity->playerAngles[playerID] <= 0) {
            if (RSDK.CheckObjectCollisionTouchCircle(entity, 0x600000, player, 0x100000)) {

                bool32 flag = false;
                for (int p = 0; p < entity->playerCount; ++p) {
                    EntityPlayer *playerPtr = (EntityPlayer *)entity->playerPtrs[p];
                    if (player == playerPtr) {
                        flag = true;
                    }
                }

                if (!flag) {
                    if ((!player->sidekick || entity->playerCount) && (!entity->field_8C || player->sidekick)) {
                        if (entity->playerCount < Player->playerCount) {
                            if (!entity->playerCount)
                                entity->field_78 = player->direction;
                            entity->playerPtrs[entity->playerCount++] = (Entity *)player;

                            if (Zone->screenBoundsB1[playerID] != (entity->position.y >> 16) + 160) {
                                entity->playerTimers[playerID] = Zone->screenBoundsB1[playerID];
                                Zone->screenBoundsB1[playerID] = ((entity->position.y >> 16) + 160);
                            }

                            LottoMachine->activePlayers |= (1 << playerID);
                            if (!entity->timer)
                                entity->timer = 1;
                        }
                    }
                    else {
                        player->velocity.x             = 0;
                        player->velocity.y             = -0xA0000;
                        player->jumpAbility            = 0;
                        entity->playerAngles[playerID] = 16;
                        RSDK.PlaySfx(Player->sfx_Release, false, 255);
                        RSDK.PlaySfx(LottoMachine->sfxFail, false, 255);
                    }
                }
            }
            else if (RSDK.CheckObjectCollisionTouchCircle(entity, 0x730000, player, 0x100000)) {
                int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                int vel   = (abs(player->velocity.x) + abs(player->velocity.y)) >> 8;
                if (vel < 0x400)
                    vel = 0x400;
                int radius = RSDK.Rand(512, vel);
                angle += RSDK.Rand(-6, 6);
                int velX = radius * RSDK.Cos256(angle);
                int velY = radius * RSDK.Sin256(angle);
                if ((player->characterID == ID_KNUCKLES && player->playerAnimator.animationID == ANI_FLY) || player->state == Player_State_FlyCarried
#if RETRO_USE_PLUS
                    || player->state == Player_State_RayGlide
#endif
                ) {
                    if (player->state == Player_State_FlyCarried)
                        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;
                    player->state = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
                }
                player->velocity.x             = velX;
                player->groundVel              = velX;
                player->velocity.y             = velY;
                player->onGround               = false;
                player->jumpAbility            = 0;
                entity->playerAngles[playerID] = 8;
                RSDK.PlaySfx(LottoMachine->sfxPimPom, false, 255);
            }
        }
        else {
            --entity->playerAngles[playerID];
        }
    }
}

void LottoMachine_CheckPlayerCollisions2(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(Player, player)
    {
        if (!player->sidekick || entity->playerCount) {
            if (Player_CheckCollisionTouch(player, entity, &LottoMachine->hitbox1)) {
                bool32 flag = false;
                for (int p = 0; p < entity->playerCount; ++p) {
                    EntityPlayer *playerPtr = (EntityPlayer *)entity->playerPtrs[p];
                    if (player == playerPtr) {
                        flag = true;
                    }
                }

                if (!flag) {
                    if (entity->playerCount < Player->playerCount) {
                        if (!entity->playerCount)
                            entity->field_78 = player->direction;
                        entity->playerPtrs[entity->playerCount++] = (Entity *)player;
                        player->state      = Player_State_None;
                        player->position.x = entity->position.x;
                        player->position.y = entity->position.y + 0x980000;
                        player->velocity.x = 0;
                        player->velocity.y = 0;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
                        RSDK.PlaySfx(Player->sfx_Roll, false, 255);
                        LottoMachine->activePlayers |= 1 << player->playerID;
                        if (!entity->timer)
                            entity->timer = 1;
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
    for (int p = 0; p < entity->playerCount; ++p) {
        EntityPlayer *player = (EntityPlayer *)entity->playerPtrs[p];
        if (player) {
            if (player->state != Player_State_Die) {
                player->position.x = entity->position.x;
                Player_CheckCollisionPlatform(player, entity, &LottoMachine->hitbox2);
                player->direction        = entity->field_78;
                player->jumpAbilityTimer = 0;
                if (player->onGround) {
                    vel += player->groundVel;
                    if (entity->field_78) {
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

    if (entity->field_78) {
        if (entity->field_74 < -0x10000) {
            entity->field_74 += 0x800;
            if (entity->field_74 > -0x10000)
                entity->field_74 = -0x10000;
        }
        if (vel < entity->field_74)
            entity->field_74 = vel;
        entity->animator4.animationSpeed = -entity->field_74 >> 11;
    }
    else {
        if (entity->field_74 > 0x10000) {
            entity->field_74 -= 0x800;
            if (entity->field_74 < 0x10000)
                entity->field_74 = 0x10000;
        }
        if (vel > entity->field_74)
            entity->field_74 = vel;
        entity->animator4.animationSpeed = entity->field_74 >> 11;
    }
    if (entity->animator4.animationSpeed > 0x80)
        entity->animator4.animationSpeed = 0x80;
}

void LottoMachine_Unknown5(void)
{
    RSDK_THIS(LottoMachine);

    foreach_active(LottoBall, ball)
    {
        if (abs(ball->position.x - entity->position.x) < 0x900000 && abs(ball->position.y - entity->position.y) < 0x900000) {
            ball->state  = LottoBall_Unknown1;
            ball->parent = (Entity *)entity;
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
            if (ball->screenFlags[RSDK.GetEntityID(entity->playerPtrs[0])])
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
        uint8 reward = entity->field_90[p] & 0xFF;
        switch (entity->field_90[p] >> 8) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW: bonus += reward; break;
            case LOTTOBALL_MULTI: multiplier *= reward; break;
            case LOTTOBALL_EGGMAN: ++playerCount; break;
            default: break;
        }
    }
    int reward = bonus * multiplier;

    for (int p = 0; p < entity->playerCount; ++p) {
        EntityPlayer *player = (EntityPlayer *)entity->playerPtrs[p];
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
                RSDK.PlaySfx(Ring->sfx_Ring, false, 255);
            }
            else if (!ringCount) {
                RSDK.PlaySfx(LottoMachine->sfxFail, false, 255);
            }
            else if (ringCount < 0) {
                Vector2 pos;
                pos.x = entity->position.x;
                pos.y = entity->position.y + 0x540000;
                Ring_FakeLoseRings(&pos, -ringCount, entity->drawOrder);
                RSDK.PlaySfx(Player->sfx_LoseRings, false, 255);
                player->state = Player_State_Hit;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
                player->velocity.y      = -0x40000;
                player->onGround        = false;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
            }

            EntityLottoBall *ball               = CREATE_ENTITY(LottoBall, NULL, RSDK_screens->centerX << 16, (RSDK_screens->height - 48) << 16);
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

    if (entity->timer > 0) {
        entity->timer++;
        if (entity->timer > 60) {
            entity->timer = 0;
            RSDK.PlaySfx(Player->sfx_Release, false, 255);
            for (int p = 0; p < entity->playerCount; ++p) {
                EntityPlayer *player = (EntityPlayer *)entity->playerPtrs[p];
                if (player) {
                    if (player->state != Player_State_Die) {
                        int playerID = RSDK.GetEntityID(player);
                        if (player->state == Player_State_None) {
                            player->state                  = Player_State_Air;
                            player->velocity.x             = 0;
                            player->velocity.y             = -0x98000;
                            player->jumpAbility            = 0;
                            entity->playerAngles[playerID] = 32;
                        }

                        if (Zone->screenBoundsB1[playerID] != (entity->position.y >> 16) + 160) {
                            entity->playerTimers[playerID] = Zone->screenBoundsB1[playerID];
                            Zone->screenBoundsB1[playerID] = ((entity->position.y >> 16) + 160);
                        }
                    }
                }
            }

            LottoMachine_Unknown5();
            entity->field_A8 = 0x10000;
            if (!entity->field_78)
                entity->field_74 = 0x10000;
            else
                entity->field_74 = -0x10000;
            entity->field_A4 = 0x80000;
            entity->state    = LottoMachine_State_Unknown2;
        }
    }
}

void LottoMachine_State_Unknown2(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_Unknown4();
    RSDK.ProcessAnimation(&entity->animator4);
    entity->animator5.frameID = entity->animator4.frameID;

    switch (entity->field_8C) {
        case 0:
            if (++entity->timer == 240) {
                entity->timer         = 0;
                entity->field_A8      = 0x20000;
                entity->field_A4      = -0x180000;
                entity->drawOrderHigh = Zone->drawOrderLow + 1;
                entity->state         = LottoMachine_State_Unknown3;
            }
            break;
        case 1:
        case 2:
        case 3:
            if (++entity->timer == 32) {
                entity->timer    = 0;
                entity->field_A8 = 0x20000;
                entity->field_A4 = -0x180000;
                entity->state    = LottoMachine_State_Unknown3;
            }
            break;
        case 4:
            if (++entity->timer == 96) {
                entity->timer    = 0;
                entity->field_8C = 5;
                LottoMachine_Unknown6();
            }
            break;
        case 5:
            if (++entity->timer == 32) {
                entity->timer    = 0;
                entity->field_8C = 6;
                LottoMachine_GiveRings();
            }
            break;
        case 6:
            if (++entity->timer == 64) {
                entity->timer         = 0;
                entity->field_A8      = 0x20000;
                entity->field_A4      = -0x180000;
                entity->drawOrderHigh = Zone->drawOrderHigh;
                entity->state         = LottoMachine_State_Unknown5;
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
    RSDK.ProcessAnimation(&entity->animator4);
    entity->animator5.frameID = entity->animator4.frameID;
    if (entity->timer == 20) {
        int dist                 = 10000;
        EntityLottoBall *ballPtr = NULL;
        foreach_active(LottoBall, ball)
        {
            if (abs(ball->position.x - entity->position.x) < 0x900000) {
                if (abs(ball->position.y - entity->position.y) < 0x900000) {
                    int rx = (entity->position.x - ball->position.x) >> 16;
                    int ry = (entity->position.y - ball->position.y + 0x540000) >> 16;
                    if (rx * rx + ry * ry < dist) {
                        dist    = rx * rx + ry * ry;
                        ballPtr = ball;
                    }
                }
            }
        }

        if (ballPtr) {
            ballPtr->position.x     = entity->position.x;
            ballPtr->position.y     = entity->position.y + 0x540000;
            ballPtr->velocity.x     = 0;
            ballPtr->velocity.y     = 0;
            ballPtr->angle          = 0;
            ballPtr->angleVel       = 0;
            ballPtr->field_70       = (RSDK_screens->centerX - (entity->field_8C << 6) + 96) << 16;
            ballPtr->field_74       = (RSDK_screens->height - 48) << 16;
            ballPtr->timer          = 0;
            ballPtr->screenFlags[0] = false;
            for (int p = 0; p < entity->playerCount; ++p) {
                EntityPlayer *player = (EntityPlayer *)entity->playerPtrs[p];
                if (player) {
                    ballPtr->screenFlags[RSDK.GetEntityID(player)] = true;
                }
            }

            ballPtr->state                     = LottoBall_Unknown4;
            entity->field_90[entity->field_8C] = ballPtr->lottoNum + (ballPtr->type << 8);
            ++entity->field_8C;
        }
    }

    if (++entity->timer == 32) {
        entity->timer    = 0;
        entity->field_A8 = 0x40000;
        entity->field_A4 = 0x180000;
        entity->state    = LottoMachine_State_Unknown4;
    }
}

void LottoMachine_State_Unknown4(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_Unknown4();
    RSDK.ProcessAnimation(&entity->animator4);
    entity->animator5.frameID = entity->animator4.frameID;
    if (entity->field_A0 == entity->field_A4) {
        entity->field_A8 = 0x10000;
        entity->field_A4 = 0x80000;
        entity->state    = LottoMachine_State_Unknown2;
    }
}

void LottoMachine_State_Unknown5(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    LottoMachine_Unknown4();
    RSDK.ProcessAnimation(&entity->animator4);
    entity->animator5.frameID = entity->animator4.frameID;

    if (entity->field_A0 == entity->field_A4) {
        bool32 flag = true;
        for (int p = 0; p < entity->playerCount; ++p) {
            EntityPlayer *player = (EntityPlayer *)entity->playerPtrs[p];
            if (player && !player->onGround) {
                flag = false;
            }
        }

        if (flag) {
            for (int p = 0; p < entity->playerCount; ++p) {
                EntityPlayer *player = (EntityPlayer *)entity->playerPtrs[p];
                if (player) {
                    if (player->state != Player_State_Die) {
                        player->state        = Player_State_None;
                        player->nextAirState = StateMachine_None;
                        player->velocity.x   = 0;
                        player->velocity.y   = 0;
                        player->groundVel    = 0;
                        player->onGround     = false;
                        player->jumpAbility  = 0;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
                        int playerID                   = RSDK.GetEntityID(player);
                        Zone->screenBoundsB1[playerID] = entity->playerTimers[playerID];
                    }
                }
            }
            entity->state = LottoMachine_State_Unknown6;
        }
    }
}

void LottoMachine_State_Unknown6(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    entity->field_74 -= entity->field_74 >> 4;
    entity->animator4.animationSpeed -= entity->animator4.animationSpeed >> 4;
    RSDK.ProcessAnimation(&entity->animator4);
    entity->animator5.frameID = entity->animator4.frameID;
    if (++entity->timer == 60) {
        entity->timer    = 0;
        entity->field_A8 = 0x40000;
        entity->field_A4 = 0x180000;
        entity->state    = LottoMachine_State_Unknown7;

        for (int p = 0; p < entity->playerCount; ++p) {
            EntityPlayer *player = (EntityPlayer *)entity->playerPtrs[p];
            if (player) {
                if (player->state != Player_State_Die) {
                    player->state      = Player_State_Air;
                    player->velocity.y = 0x40000;
                    LottoMachine->activePlayers &= ~(1 << RSDK.GetEntityID(player));
                    RSDK.PlaySfx(Player->sfx_Release, false, 255);
                    entity->playerAngles[RSDK.GetEntityID(player)] = 32;
                }
            }
        }
    }
}

void LottoMachine_State_Unknown7(void)
{
    RSDK_THIS(LottoMachine);

    LottoMachine_CheckPlayerCollisions();
    ++entity->timer;
    entity->field_74 -= (entity->field_74 >> 4);
    entity->animator4.animationSpeed -= entity->animator4.animationSpeed >> 4;
    if (entity->timer == 30) {
        entity->playerPtrs[0]     = NULL;
        entity->playerPtrs[1]     = NULL;
        entity->playerPtrs[2]     = NULL;
        entity->playerPtrs[3]     = NULL;
        entity->playerCount       = 0;
        entity->field_8C          = 0;
        entity->animator3.frameID = 0;
        entity->timer             = 0;
        entity->field_74          = 0;
        entity->state             = LottoMachine_State_Unknown1;
    }
}

#if RETRO_INCLUDE_EDITOR
void LottoMachine_EditorDraw(void)
{
    RSDK_THIS(LottoMachine);
    Vector2 drawPos;

    entity->animator1.frameID = 8;
    entity->direction         = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_NONE;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y + 0x6C0000 + entity->field_A0;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    entity->direction = FLIP_NONE;
    entity->rotation  = entity->angle >> 16;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->rotation += 0x80;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->rotation += 0x80;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->rotation += 0x80;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    entity->animator1.frameID = 7;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator5, NULL, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator5, NULL, false);

    entity->animator1.frameID = 0;
    entity->inkEffect         = INK_ADD;
    entity->direction         = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&LottoMachine->animator, NULL, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&LottoMachine->animator, NULL, false);

    entity->inkEffect = INK_NONE;
    for (int i = 0; i < 3; ++i) {
        entity->direction         = FLIP_NONE;
        entity->animator1.frameID = i + 2;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void LottoMachine_EditorLoad(void) { LottoMachine->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LottoMachine.bin", SCOPE_STAGE); }
#endif

void LottoMachine_Serialize(void) {}

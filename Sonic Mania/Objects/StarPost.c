#include "../SonicMania.h"

ObjectStarPost *StarPost;

void StarPost_Update(void)
{
    RSDK_THIS(StarPost);
    StateMachine_Run(entity->state);
}

void StarPost_LateUpdate(void) {}

void StarPost_StaticUpdate(void) {}

void StarPost_Draw(void)
{
    RSDK_THIS(StarPost);
    RSDK.DrawSprite(&entity->poleData, &entity->position, false);

    entity->ballPos.x = entity->position.x - 640 * RSDK.Cos1024(entity->angle);
    entity->ballPos.y = entity->position.y - 640 * RSDK.Sin1024(entity->angle) - 0xE0000;
    RSDK.DrawSprite(&entity->ballData, &entity->ballPos, false);

    Vector2 drawPos;
    if (entity->starFlag > 0) {
        int angle2 = entity->starAngle2;
        int angle  = 3 * RSDK.Sin512(entity->starAngle);
        for (int i = 0; i < 4; ++i) {
            drawPos.x = entity->position.x + ((RSDK.Sin512(angle2) << 12) * entity->starOffset >> 7);
            drawPos.y = (((angle * RSDK.Sin512(angle2)) + (RSDK.Cos512(angle2) << 10)) * entity->starOffset >> 7) + entity->position.y - 0x320000;
            RSDK.DrawSprite(&entity->starData, &drawPos, false);
            angle2 += 128;
        }
    }
}

void StarPost_Create(void *data)
{
    RSDK_THIS(StarPost);
    if (globals->gameMode == MODE_TIMEATTACK || (globals->gameMode == MODE_COMPETITION && entity->vsRemove)) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else {
        if (!RSDK_sceneInfo->inEditor) {
            entity->visible       = true;
            entity->drawOrder     = Zone->drawOrderLow;
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            entity->state         = StarPost_State_Idle;
            entity->angle         = 256;
        }

        RSDK.SetSpriteAnimation(StarPost->spriteIndex, 0, &entity->poleData, true, 0);
        if (entity->activated) {
            RSDK.SetSpriteAnimation(StarPost->spriteIndex, 2, &entity->ballData, true, 0);
            entity->ballData.animationSpeed = 64;
        }
        else {
            RSDK.SetSpriteAnimation(StarPost->spriteIndex, 1, &entity->ballData, true, 0);
        }
        RSDK.SetSpriteAnimation(StarPost->spriteIndex, 3, &entity->starData, true, 0);
        entity->ballPos.x = entity->position.x;
        entity->ballPos.y = entity->position.y - 0x180000;
    }
}

void StarPost_StageLoad(void)
{
    StarPost->spriteIndex   = RSDK.LoadSpriteAnimation("Global/StarPost.bin", SCOPE_STAGE);
    StarPost->hitbox.left   = -8;
    StarPost->hitbox.top    = -44;
    StarPost->hitbox.right  = 8;
    StarPost->hitbox.bottom = 20;
    StarPost->activePlayers = (1 << Player->playerCount) - 1;

    if (DebugMode->itemCount < 256) {
        DebugMode->objectIDs[DebugMode->itemCount] = StarPost->objectID;
        DebugMode->spawn[DebugMode->itemCount]     = StarPost_DebugSpawn;
        DebugMode->draw[DebugMode->itemCount++]    = StarPost_DebugDraw;
    }

    for (int i = 0; i < Player->playerCount; ++i) {
        if (StarPost->postIDs[i]) {
            EntityPlayer *player          = RSDK_GET_ENTITY(i, Player);
            EntityStarPost *savedStarPost = RSDK_GET_ENTITY(StarPost->postIDs[i], StarPost);
            if (!TMZ2Setup) {
                foreach_all(StarPost, starPost) {
                    if (starPost->id < savedStarPost->id && !starPost->activated) {
                        starPost->activated = StarPost->activePlayers;
                        RSDK.SetSpriteAnimation(StarPost->spriteIndex, 2, &starPost->ballData, true, 0);
                    }
                }
            }

            if (!globals->specialRingID) {
                if (globals->gameMode < MODE_TIMEATTACK) {
                    int ms = RSDK_sceneInfo->milliseconds;
                    int s  = RSDK_sceneInfo->minutes;
                    int m  = RSDK_sceneInfo->seconds;
                    if ((RSDK_sceneInfo->milliseconds || RSDK_sceneInfo->seconds || RSDK_sceneInfo->minutes) || ms != globals->tempMilliseconds
                        || s != globals->tempSeconds || m != globals->tempMinutes) {
                        RSDK_sceneInfo->milliseconds = StarPost->storedMS;
                        RSDK_sceneInfo->seconds      = StarPost->storedSeconds;
                        RSDK_sceneInfo->minutes      = StarPost->storedMinutes;
                    }
                }

                player->position.x = StarPost->playerPositions[i].x;
                player->position.y = StarPost->playerPositions[i].y;
                player->position.y += 0x100000;
                player->direction = StarPost->playerDirections[i];
                if (!i) {
                    EntityPlayer *sideKick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                    if (globals->gameMode != MODE_COMPETITION) {
                        sideKick->position.x = player->position.x;
                        sideKick->position.y = player->position.y;
                        sideKick->direction  = player->direction;
                        if (player->direction)
                            sideKick->position.x += 0x100000;
                        else
                            sideKick->position.x -= 0x100000;

                        for (int p = 0; p < 0x10; ++p) {
                            Player->flyCarryPositions[p].x = player->position.x;
                            Player->flyCarryPositions[p].y = player->position.y;
                        }
                    }
                }
            }
            savedStarPost->activated = StarPost->activePlayers;
        }

#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_COMPETITION || globals->gameMode == MODE_ENCORE) {
#else
        if (globals->gameMode == MODE_COMPETITION) {
#endif
            EntityPlayer *player           = RSDK_GET_ENTITY(i, Player);
            StarPost->playerPositions[i].x = player->position.x;
            StarPost->playerPositions[i].y = player->position.y;
            StarPost->playerPositions[i].y -= 0x100000;
            StarPost->playerDirections[i] = player->direction;
        }
    }

    StarPost->sfx_StarPost = RSDK.GetSFX("Global/StarPost.wav");
    StarPost->sfx_Warp     = RSDK.GetSFX("Global/SpecialWarp.wav");
}

void StarPost_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(StarPost->spriteIndex, 0, &DebugMode->debugData, true, 1);
    RSDK.DrawSprite(&DebugMode->debugData, NULL, false);
}
void StarPost_DebugSpawn(void) { RSDK.CreateEntity(StarPost->objectID, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y); }
void StarPost_ResetStarPosts(void)
{
    for (int i = 0; i < Player->playerCount; ++i) StarPost->postIDs[i] = 0;
    StarPost->storedMS      = 0;
    StarPost->storedSeconds = 0;
    StarPost->storedMinutes = 0;
}
void StarPost_CheckBonusStageEntry(void)
{
    RSDK_THIS(StarPost);
    entity->starAngle += 4;
    entity->starAngle &= 0x1FF;
    entity->starAngle2 += 18;
    entity->starAngle2 &= 0x1FF;

    if (entity->starTimer > 472) {
        --entity->starOffset;
    }
    else if (entity->starTimer < 0x80) {
        ++entity->starOffset;
    }

    if (++entity->starTimer == 600) {
        entity->starTimer = 0;
        entity->starFlag  = 0;
        entity->active    = ACTIVE_BOUNDS;
    }
    entity->starData.frameID  = (entity->starAngle >> 3) & 3;
    entity->starHitbox.left   = -(entity->starOffset >> 2);
    entity->starHitbox.top    = -48;
    entity->starHitbox.right  = entity->starOffset >> 2;
    entity->starHitbox.bottom = -40;
    if (entity->starTimer >= 60) {
        if (!globals->recallEntities) {
            if (Player_CheckCollisionTouch(RSDK.GetEntityByID(SLOT_PLAYER1), entity, &entity->starHitbox)) {
                SaveGame_SaveGameState();
                RSDK.PlaySFX(StarPost->sfx_Warp, 0, 0xFE);
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
                int *saveRAM = SaveGame_GetGlobalData();
#if RETRO_USE_PLUS
                if ((User.CheckDLC(DLC_PLUS) && saveRAM && saveRAM[30]) || globals->gameMode == MODE_ENCORE) {
                    SaveGame->saveRAM[30] = RSDK_sceneInfo->listPos;
                    RSDK.LoadScene("Pinball", "");
                    Zone_StartFadeOut(10, 0xF0F0F0);
                    RSDK.StopChannel(Music->slotID);
                }
                else {
#endif
                    SaveGame->saveRAM[30] = RSDK_sceneInfo->listPos;
                    RSDK.LoadScene("Blue Spheres", "");
                    RSDK_sceneInfo->listPos += globals->blueSpheresID;
                    Zone_StartFadeOut(10, 0xF0F0F0);
                    RSDK.StopChannel(Music->slotID);
#if RETRO_USE_PLUS
                }
#endif
            }
        }
    }
}
void StarPost_CheckCollisions(void)
{
    RSDK_THIS(StarPost);
    foreach_active(Player, player) {
        int playerSlot = RSDK.GetEntityID(player);
        if (!((1 << playerSlot) & entity->activated) && !player->sidekick) {
            if (Player_CheckCollisionTouch(player, entity, &StarPost->hitbox)) {
                entity->state = StarPost_State_BallSpin;
                if (!TMZ2Setup) {
                    foreach_all(StarPost, starPost) {
                        if (starPost->id < entity->id && !starPost->activated) {
                            starPost->activated = 1 << playerSlot;
                            RSDK.SetSpriteAnimation(StarPost->spriteIndex, 2, &starPost->ballData, true, 0);
                        }
                    }
                }

                StarPost->postIDs[playerSlot]           = RSDK_sceneInfo->entitySlot;
                StarPost->playerPositions[playerSlot].x = entity->position.x;
                StarPost->playerPositions[playerSlot].y = entity->position.y;
                StarPost->playerDirections[playerSlot]  = entity->direction;
                if (globals->gameMode < MODE_TIMEATTACK) {
                    StarPost->storedMS      = RSDK_sceneInfo->milliseconds;
                    StarPost->storedSeconds = RSDK_sceneInfo->seconds;
                    StarPost->storedMinutes = RSDK_sceneInfo->minutes;
                }

                int speed = 0;
                if (player->onGround)
                    speed = -12 * (player->groundVel >> 17);
                else
                    speed = -12 * (player->velocity.x >> 17);

                if (speed >= 0)
                    speed += 32;
                else
                    speed -= 32;

                int ballSpeed = entity->ballSpeed;
                if (!ballSpeed) {
                    entity->ballSpeed = speed;
                }
                else if (ballSpeed <= 0) {
                    if (speed < ballSpeed) {
                        entity->ballSpeed = speed;
                    }
                    else if (speed > 0) {
                        speed += ballSpeed;
                        entity->ballSpeed = speed;
                    }
                }
                else {
                    if (speed > ballSpeed) {
                        entity->ballSpeed = speed;
                    }
                    else if (speed < 0) {
                        speed += ballSpeed;
                        entity->ballSpeed = speed;
                    }
                }

                entity->timer = 0;
                if (globals->gameMode < MODE_TIMEATTACK) {
                    int quota = 50;
#if RETRO_USE_PLUS
                    if (globals->gameMode != MODE_ENCORE)
                        quota = 25;
#endif
                    if (player->rings >= quota) {
                        entity->starTimer  = 0;
                        entity->starAngle  = 0;
                        entity->starAngle2 = 0;
                        entity->starOffset = 0;
                        entity->starFlag   = (player->rings - 20) % 3 + 1;
                    }
                }

                if (!entity->activated) {
                    RSDK.SetSpriteAnimation(StarPost->spriteIndex, 2, &entity->ballData, true, 0);
                    entity->ballData.animationSpeed = 0;
                }
                entity->activated |= 1 << playerSlot;
                entity->active = ACTIVE_NORMAL;
                RSDK.PlaySFX(StarPost->sfx_StarPost, 0, 255);
            }
        }
    }
}
void StarPost_State_Idle(void)
{
    RSDK_THIS(StarPost);
    if (entity->activated < StarPost->activePlayers)
        StarPost_CheckCollisions();
    if (entity->starFlag > 0)
        StarPost_CheckBonusStageEntry();
    RSDK.ProcessAnimation(&entity->ballData);
}
void StarPost_State_BallSpin(void)
{
    RSDK_THIS(StarPost);
    if (entity->activated < StarPost->activePlayers)
        StarPost_CheckCollisions();

    entity->angle += entity->ballSpeed;
    if (!StarPost->hasAchievement && entity->timer == 10) {
#if RETRO_USE_PLUS
        User.UnlockAchievement("ACH_STARPOST");
#else
        APICallback_UnlockAchievement("ACH_STARPOST");
#endif
        StarPost->hasAchievement = true;
    }

    bool32 flag = false;
    if (entity->ballSpeed <= 0) {
        if (entity->angle <= -768) {
            ++entity->timer;
            entity->angle += 1024;
            if (entity->ballSpeed + 8 > -32)
                entity->ballSpeed = -32;
            else
                entity->ballSpeed += 8;
            flag = entity->ballSpeed == -32;
        }
    }
    else {
        if (entity->angle >= 1280) {
            ++entity->timer;
            entity->angle -= 0x400;
            if (entity->ballSpeed - 8 < 32)
                entity->ballSpeed = 32;
            else
                entity->ballSpeed -= 8;
            flag = entity->ballSpeed == 32;
        }
    }

    if (flag) {
        entity->state                   = StarPost_State_Idle;
        entity->ballData.animationSpeed = 64;
        entity->ballSpeed               = 0;
        entity->angle                   = 256;
        if (entity->starFlag == 0)
            entity->active = ACTIVE_BOUNDS;
    }

    if (entity->starFlag > 0)
        StarPost_CheckBonusStageEntry();
    RSDK.ProcessAnimation(&entity->ballData);
}

void StarPost_EditorDraw(void) {}

void StarPost_EditorLoad(void) {}

void StarPost_Serialize(void)
{
    RSDK_EDITABLE_VAR(StarPost, VAR_ENUM, id);
    RSDK_EDITABLE_VAR(StarPost, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(StarPost, VAR_BOOL, vsRemove);
}

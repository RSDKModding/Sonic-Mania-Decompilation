#include "../SonicMania.h"

ObjectStarPost *StarPost;

void StarPost_Update()
{
    EntityStarPost *entity = (EntityStarPost *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void StarPost_LateUpdate() {}

void StarPost_StaticUpdate() {}

void StarPost_Draw()
{
    EntityStarPost *entity = (EntityStarPost *)RSDK_sceneInfo->entity;
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
    EntityStarPost *entity = (EntityStarPost *)RSDK_sceneInfo->entity;
    if (globals->gameMode == MODE_TIMEATTACK || (globals->gameMode == MODE_COMPETITION && entity->vsRemove)) {
        RSDK.ResetEntityPtr(entity, 0, 0);
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

void StarPost_StageLoad()
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
            EntityPlayer *player          = (EntityPlayer *)RSDK.GetEntityByID(i);
            EntityStarPost *savedStarPost = (EntityStarPost *)RSDK.GetEntityByID(StarPost->postIDs[i]);
            if (!TMZ2Setup) {
                EntityStarPost *starPost = NULL;
                while (RSDK.GetEntities(StarPost->objectID, (Entity **)&starPost)) {
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
                    EntityPlayer *sideKick = (EntityPlayer *)RSDK.GetEntityByID(1);
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
            EntityPlayer *player           = (EntityPlayer *)RSDK.GetEntityByID(i);
            StarPost->playerPositions[i].x = player->position.x;
            StarPost->playerPositions[i].y = player->position.y;
            StarPost->playerPositions[i].y -= 0x100000;
            StarPost->playerDirections[i] = player->direction;
        }
    }

    StarPost->sfx_StarPost = RSDK.GetSFX("Global/StarPost.wav");
    StarPost->sfx_Warp     = RSDK.GetSFX("Global/SpecialWarp.wav");
}

void StarPost_DebugDraw()
{
    RSDK.SetSpriteAnimation(StarPost->spriteIndex, 0, &DebugMode->debugData, true, 1);
    RSDK.DrawSprite(&DebugMode->debugData, NULL, 0);
}
void StarPost_DebugSpawn() { RSDK.CreateEntity(StarPost->objectID, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y); }
void StarPost_ResetStarPosts()
{
    for (int i = 0; i < Player->playerCount; ++i) StarPost->postIDs[i] = 0;
    StarPost->storedMS      = 0;
    StarPost->storedSeconds = 0;
    StarPost->storedMinutes = 0;
}
void StarPost_CheckBonusStageEntry()
{
    EntityStarPost *entity = (EntityStarPost *)RSDK_sceneInfo->entity;
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
                RSDK.PlaySFX(StarPost->sfx_Warp, 0, 254);
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
                int *saveRAM = SaveGame_GetGlobalData();
                if ((User.CheckDLC(DLC_PLUS) && saveRAM && saveRAM[30]) || globals->gameMode == MODE_ENCORE) {
                    SaveGame->saveRAM[30] = RSDK_sceneInfo->listPos;
                    RSDK.LoadScene("Pinball", "");
                    EntityZone *entityZone = (EntityZone *)RSDK.GetEntityByID(SLOT_ZONE);
                    entityZone->screenID   = 4;
                    entityZone->timer      = 0;
                    entityZone->fadeTimer  = 10;
                    entityZone->fadeColour = 0xF0F0F0;
                    entityZone->state      = Zone_Unknown13;
                    entityZone->stateDraw  = Zone_Unknown12;
                    entityZone->visible    = true;
                    entityZone->drawOrder  = 15;
                    RSDK.SoundUnknown1(Music->slotID);
                }
                else {
                    SaveGame->saveRAM[30] = RSDK_sceneInfo->listPos;
                    RSDK.LoadScene("Blue Spheres", "");
                    RSDK_sceneInfo->listPos += globals->blueSpheresID;
                    EntityZone *entityZone = (EntityZone *)RSDK.GetEntityByID(SLOT_ZONE);
                    entityZone->screenID   = 4;
                    entityZone->timer      = 0;
                    entityZone->fadeTimer  = 10;
                    entityZone->fadeColour = 0xF0F0F0;
                    entityZone->state      = Zone_Unknown13;
                    entityZone->stateDraw  = Zone_Unknown12;
                    entityZone->visible    = true;
                    entityZone->drawOrder  = 15;
                    RSDK.SoundUnknown1(Music->slotID);
                }
            }
        }
    }
}
void StarPost_CheckCollisions()
{
    EntityPlayer *player   = 0;
    EntityStarPost *entity = (EntityStarPost *)RSDK_sceneInfo->entity;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        int playerSlot = RSDK.GetEntityID(player);
        if (!((1 << playerSlot) & entity->activated) && !player->sidekick) {
            if (Player_CheckCollisionTouch(player, entity, &StarPost->hitbox)) {
                entity->state = StarPost_State_BallSpin;
                if (!TMZ2Setup) {
                    EntityStarPost *starPost = 0;
                    while (RSDK.GetEntities(StarPost->objectID, (Entity **)&starPost)) {
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
void StarPost_State_Idle()
{
    EntityStarPost *entity = (EntityStarPost *)RSDK_sceneInfo->entity;
    if (entity->activated < StarPost->activePlayers)
        StarPost_CheckCollisions();
    if (entity->starFlag > 0)
        StarPost_CheckBonusStageEntry();
    RSDK.ProcessAnimation(&entity->ballData);
}
void StarPost_State_BallSpin()
{
    EntityStarPost *entity = (EntityStarPost *)RSDK_sceneInfo->entity;
    if (entity->activated < StarPost->activePlayers) // activated
        StarPost_CheckCollisions();

    entity->angle += entity->ballSpeed;
    if (!StarPost->hasAchievement && entity->timer == 10) {
#if RETRO_USE_PLUS
        User.UnlockAchievement("ACH_STARPOST");
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

void StarPost_EditorDraw() {}

void StarPost_EditorLoad() {}

void StarPost_Serialize()
{
    RSDK_EDITABLE_VAR(StarPost, VAR_ENUM, id);
    RSDK_EDITABLE_VAR(StarPost, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(StarPost, VAR_BOOL, vsRemove);
}
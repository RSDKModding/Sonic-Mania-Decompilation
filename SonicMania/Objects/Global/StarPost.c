// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: StarPost Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectStarPost *StarPost;

void StarPost_Update(void)
{
    RSDK_THIS(StarPost);

    StateMachine_Run(self->state);
}

void StarPost_LateUpdate(void) {}

void StarPost_StaticUpdate(void) {}

void StarPost_Draw(void)
{
    RSDK_THIS(StarPost);

    RSDK.DrawSprite(&self->poleAnimator, &self->position, false);

    self->ballPos.x = self->position.x - 0x280 * RSDK.Cos1024(self->angle);
    self->ballPos.y = self->position.y - 0x280 * RSDK.Sin1024(self->angle) - TO_FIXED(14);
    RSDK.DrawSprite(&self->ballAnimator, &self->ballPos, false);

    Vector2 drawPos;
    if (self->bonusStageID > 0) {
        int32 angleX    = self->starAngleX;
        int32 amplitude = 3 * RSDK.Sin512(self->starAngleY);
        for (int32 i = 0; i < 4; ++i) {
            drawPos.x = self->position.x + ((RSDK.Sin512(angleX) << 12) * self->starRadius >> 7);
            drawPos.y = (((amplitude * RSDK.Sin512(angleX)) + (RSDK.Cos512(angleX) << 10)) * self->starRadius >> 7) + self->position.y - TO_FIXED(50);
            RSDK.DrawSprite(&self->starAnimator, &drawPos, false);
            angleX += 128;
        }
    }
}

void StarPost_Create(void *data)
{
    RSDK_THIS(StarPost);

    if (globals->gameMode == MODE_TIMEATTACK || (globals->gameMode == MODE_COMPETITION && self->vsRemove)) {
        destroyEntity(self);
    }
    else {
        if (!SceneInfo->inEditor) {
            self->visible       = true;
            self->drawGroup     = Zone->objectDrawGroup[0];
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = TO_FIXED(64);
            self->updateRange.y = TO_FIXED(64);
            self->state         = StarPost_State_Idle;
            self->angle         = 256;
        }

        RSDK.SetSpriteAnimation(StarPost->aniFrames, 0, &self->poleAnimator, true, 0);
        if (self->interactedPlayers) {
            RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &self->ballAnimator, true, 0);
            self->ballAnimator.speed = 64;
        }
        else {
            RSDK.SetSpriteAnimation(StarPost->aniFrames, 1, &self->ballAnimator, true, 0);
        }
        RSDK.SetSpriteAnimation(StarPost->aniFrames, 3, &self->starAnimator, true, 0);
        self->ballPos.x = self->position.x;
        self->ballPos.y = self->position.y - TO_FIXED(24);
    }
}

void StarPost_StageLoad(void)
{
    StarPost->aniFrames = RSDK.LoadSpriteAnimation("Global/StarPost.bin", SCOPE_STAGE);

    StarPost->hitbox.left   = -8;
    StarPost->hitbox.top    = -44;
    StarPost->hitbox.right  = 8;
    StarPost->hitbox.bottom = 20;

    StarPost->interactablePlayers = (1 << Player->playerCount) - 1;

    DEBUGMODE_ADD_OBJ(StarPost);

    for (int32 p = 0; p < Player->playerCount; ++p) {
        if (StarPost->postIDs[p]) {
            EntityPlayer *player          = RSDK_GET_ENTITY(p, Player);
            EntityStarPost *savedStarPost = RSDK_GET_ENTITY(StarPost->postIDs[p], StarPost);

            // Disable id-based auto activation in TMZ2 due to its non-linear nature
            if (!TMZ2Setup) {
                foreach_all(StarPost, starPost)
                {
                    if (starPost->id < savedStarPost->id && !starPost->interactedPlayers) {
                        starPost->interactedPlayers = StarPost->interactablePlayers;
                        RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &starPost->ballAnimator, true, 0);
                    }
                }
            }

            if (!globals->specialRingID) {
                if (globals->gameMode < MODE_TIMEATTACK) {
                    int32 ms = SceneInfo->milliseconds;
                    int32 s  = SceneInfo->seconds;
                    int32 m  = SceneInfo->minutes;
                    if (!(ms || s || m) || ms != globals->tempMilliseconds || s != globals->tempSeconds || m != globals->tempMinutes) {
                        SceneInfo->milliseconds = StarPost->storedMS;
                        SceneInfo->seconds      = StarPost->storedSeconds;
                        SceneInfo->minutes      = StarPost->storedMinutes;
                    }
                }

                player->position.x = StarPost->playerPositions[p].x;
                player->position.y = StarPost->playerPositions[p].y + TO_FIXED(16);
                player->direction  = StarPost->playerDirections[p];

                if (!p) {
                    EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                    if (globals->gameMode != MODE_COMPETITION) {
                        sidekick->position.x = player->position.x;
                        sidekick->position.y = player->position.y;
                        sidekick->direction  = player->direction;
                        if (player->direction)
                            sidekick->position.x += TO_FIXED(16);
                        else
                            sidekick->position.x -= TO_FIXED(16);

                        for (int32 i = 0; i < 0x10; ++i) {
                            Player->leaderPositionBuffer[i].x = player->position.x;
                            Player->leaderPositionBuffer[i].y = player->position.y;
                        }
                    }
                }
            }
            savedStarPost->interactedPlayers = StarPost->interactablePlayers;
        }

#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_COMPETITION || globals->gameMode == MODE_ENCORE) {
#else
        if (globals->gameMode == MODE_COMPETITION) {
#endif
            EntityPlayer *player           = RSDK_GET_ENTITY(p, Player);
            StarPost->playerPositions[p].x = player->position.x;
            StarPost->playerPositions[p].y = player->position.y - TO_FIXED(16);
            StarPost->playerDirections[p]  = player->direction;
        }
    }

    StarPost->sfxStarPost = RSDK.GetSfx("Global/StarPost.wav");
    StarPost->sfxWarp     = RSDK.GetSfx("Global/SpecialWarp.wav");
}

void StarPost_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(StarPost->aniFrames, 0, &DebugMode->animator, true, 1);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}
void StarPost_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(StarPost, NULL, self->position.x, self->position.y);
}
void StarPost_ResetStarPosts(void)
{
    for (int32 i = 0; i < Player->playerCount; ++i) StarPost->postIDs[i] = 0;
    StarPost->storedMS      = 0;
    StarPost->storedSeconds = 0;
    StarPost->storedMinutes = 0;
}
void StarPost_CheckBonusStageEntry(void)
{
    RSDK_THIS(StarPost);

    self->starAngleY += 4;
    self->starAngleY &= 0x1FF;
    self->starAngleX += 18;
    self->starAngleX &= 0x1FF;

    if (self->starTimer > 472)
        --self->starRadius;
    else if (self->starTimer < 0x80)
        ++self->starRadius;

    if (++self->starTimer == 600) {
        self->starTimer    = 0;
        self->bonusStageID = 0;
        self->active       = ACTIVE_BOUNDS;
    }

    self->starAnimator.frameID = (self->starAngleY >> 3) & 3;

    self->hitboxStars.left   = -(self->starRadius >> 2);
    self->hitboxStars.top    = -48;
    self->hitboxStars.right  = self->starRadius >> 2;
    self->hitboxStars.bottom = -40;

    if (self->starTimer >= 60) {
        if (!globals->recallEntities) {
            if (Player_CheckCollisionTouch(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), self, &self->hitboxStars)) {
                SaveGame_SaveGameState();
                RSDK.PlaySfx(StarPost->sfxWarp, false, 0xFE);
                RSDK.SetEngineState(ENGINESTATE_FROZEN);

#if MANIA_USE_PLUS
                ProgressRAM *progress = GameProgress_GetProgressRAM();
                if ((API.CheckDLC(DLC_PLUS) && progress && progress->allGoldMedals) || globals->gameMode == MODE_ENCORE) {
                    SaveGame_GetSaveRAM()->storedStageID = SceneInfo->listPos;
                    RSDK.SetScene("Pinball", "");
                    Zone_StartFadeOut(10, 0xF0F0F0);
                    Music_Stop();
                }
                else {
#endif
                    SaveGame_GetSaveRAM()->storedStageID = SceneInfo->listPos;
                    RSDK.SetScene("Blue Spheres", "");
                    SceneInfo->listPos += globals->blueSpheresID;
                    Zone_StartFadeOut(10, 0xF0F0F0);
                    Music_Stop();
#if MANIA_USE_PLUS
                }
#endif
            }
        }
    }
}
void StarPost_CheckCollisions(void)
{
    RSDK_THIS(StarPost);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (!((1 << playerID) & self->interactedPlayers) && !player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &StarPost->hitbox)) {
                self->state = StarPost_State_Spinning;
                if (!TMZ2Setup) {
                    foreach_all(StarPost, starPost)
                    {
                        if (starPost->id < self->id && !starPost->interactedPlayers) {
                            starPost->interactedPlayers = 1 << playerID;
                            RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &starPost->ballAnimator, true, 0);
                        }
                    }
                }

                StarPost->postIDs[playerID]           = SceneInfo->entitySlot;
                StarPost->playerPositions[playerID].x = self->position.x;
                StarPost->playerPositions[playerID].y = self->position.y;
                StarPost->playerDirections[playerID]  = self->direction;
                if (globals->gameMode < MODE_TIMEATTACK) {
                    StarPost->storedMS      = SceneInfo->milliseconds;
                    StarPost->storedSeconds = SceneInfo->seconds;
                    StarPost->storedMinutes = SceneInfo->minutes;
                }

                int32 playerVelocity = player->onGround ? player->groundVel : player->velocity.x;
                int32 ballSpeed      = -12 * (playerVelocity >> 17);

                if (ballSpeed >= 0)
                    ballSpeed += 32;
                else
                    ballSpeed -= 32;

                if (!self->ballSpeed) {
                    self->ballSpeed = ballSpeed;
                }
                else if (self->ballSpeed <= 0) {
                    if (ballSpeed < self->ballSpeed) {
                        self->ballSpeed = ballSpeed;
                    }
                    else if (ballSpeed > 0) {
                        ballSpeed += self->ballSpeed;
                        self->ballSpeed = ballSpeed;
                    }
                }
                else {
                    if (ballSpeed > self->ballSpeed) {
                        self->ballSpeed = ballSpeed;
                    }
                    else if (ballSpeed < 0) {
                        ballSpeed += self->ballSpeed;
                        self->ballSpeed = ballSpeed;
                    }
                }

                self->timer = 0;
                if (globals->gameMode < MODE_TIMEATTACK) {
                    int32 quota = 25;
#if MANIA_USE_PLUS
                    if (globals->gameMode == MODE_ENCORE)
                        quota = 50;
#endif

                    if (player->rings >= quota) {
                        self->starTimer  = 0;
                        self->starAngleY = 0;
                        self->starAngleX = 0;
                        self->starRadius = 0;
                        // This is the calculation for the bonus stages from sonic 3
                        // I mean it works, sure but it's just more proof this was prolly based off S3 '14
                        self->bonusStageID = (player->rings - 20) % 3 + 1;
                    }
                }

                if (!self->interactedPlayers) {
                    RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &self->ballAnimator, true, 0);
                    self->ballAnimator.speed = 0;
                }

                self->interactedPlayers |= 1 << playerID;
                self->active = ACTIVE_NORMAL;
                RSDK.PlaySfx(StarPost->sfxStarPost, false, 255);
            }
        }
    }
}
void StarPost_State_Idle(void)
{
    RSDK_THIS(StarPost);

    if (self->interactedPlayers < StarPost->interactablePlayers)
        StarPost_CheckCollisions();

    if (self->bonusStageID > 0)
        StarPost_CheckBonusStageEntry();

    RSDK.ProcessAnimation(&self->ballAnimator);
}
void StarPost_State_Spinning(void)
{
    RSDK_THIS(StarPost);

    if (self->interactedPlayers < StarPost->interactablePlayers)
        StarPost_CheckCollisions();

    self->angle += self->ballSpeed;
    if (!StarPost->hasAchievement && self->timer == 10) {
        API_UnlockAchievement(&achievementList[ACH_STARPOST]);
        StarPost->hasAchievement = true;
    }

    bool32 isIdle = false;
    if (self->ballSpeed <= 0) {
        if (self->angle <= -0x300) {
            ++self->timer;
            self->angle += 0x400;

            self->ballSpeed += 8;
            if (self->ballSpeed > -32)
                self->ballSpeed = -32;

            isIdle = self->ballSpeed == -32;
        }
    }
    else {
        if (self->angle >= 0x500) {
            ++self->timer;
            self->angle -= 0x400;

            self->ballSpeed -= 8;
            if (self->ballSpeed < 32)
                self->ballSpeed = 32;

            isIdle = self->ballSpeed == 32;
        }
    }

    if (isIdle) {
        self->state              = StarPost_State_Idle;
        self->ballAnimator.speed = 64;
        self->ballSpeed          = 0;
        self->angle              = 0x100;

        if (!self->bonusStageID)
            self->active = ACTIVE_BOUNDS;
    }

    if (self->bonusStageID > 0)
        StarPost_CheckBonusStageEntry();

    RSDK.ProcessAnimation(&self->ballAnimator);
}

#if GAME_INCLUDE_EDITOR
void StarPost_EditorDraw(void) { StarPost_DebugDraw(); }

void StarPost_EditorLoad(void)
{
    StarPost->aniFrames = RSDK.LoadSpriteAnimation("Global/StarPost.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(StarPost, direction);
    RSDK_ENUM_VAR("Respawn Facing Right", FLIP_NONE);
    RSDK_ENUM_VAR("Respawn Facing Left", FLIP_X);
}
#endif

void StarPost_Serialize(void)
{
    RSDK_EDITABLE_VAR(StarPost, VAR_ENUM, id);
    RSDK_EDITABLE_VAR(StarPost, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(StarPost, VAR_BOOL, vsRemove);
}

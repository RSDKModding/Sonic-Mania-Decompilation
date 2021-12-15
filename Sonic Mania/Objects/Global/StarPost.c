// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: StarPost Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    self->ballPos.y = self->position.y - 0x280 * RSDK.Sin1024(self->angle) - 0xE0000;
    RSDK.DrawSprite(&self->ballAnimator, &self->ballPos, false);

    Vector2 drawPos;
    if (self->starFlag > 0) {
        int32 angle2 = self->starAngle2;
        int32 angle  = 3 * RSDK.Sin512(self->starAngle);
        for (int32 i = 0; i < 4; ++i) {
            drawPos.x = self->position.x + ((RSDK.Sin512(angle2) << 12) * self->starOffset >> 7);
            drawPos.y = (((angle * RSDK.Sin512(angle2)) + (RSDK.Cos512(angle2) << 10)) * self->starOffset >> 7) + self->position.y - 0x320000;
            RSDK.DrawSprite(&self->starAnimator, &drawPos, false);
            angle2 += 128;
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
            self->drawOrder     = Zone->drawOrderLow;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            self->state         = StarPost_State_Idle;
            self->angle         = 256;
        }

        RSDK.SetSpriteAnimation(StarPost->aniFrames, 0, &self->poleAnimator, true, 0);
        if (self->activated) {
            RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &self->ballAnimator, true, 0);
            self->ballAnimator.speed = 64;
        }
        else {
            RSDK.SetSpriteAnimation(StarPost->aniFrames, 1, &self->ballAnimator, true, 0);
        }
        RSDK.SetSpriteAnimation(StarPost->aniFrames, 3, &self->starAnimator, true, 0);
        self->ballPos.x = self->position.x;
        self->ballPos.y = self->position.y - 0x180000;
    }
}

void StarPost_StageLoad(void)
{
    StarPost->aniFrames     = RSDK.LoadSpriteAnimation("Global/StarPost.bin", SCOPE_STAGE);
    StarPost->hitbox.left   = -8;
    StarPost->hitbox.top    = -44;
    StarPost->hitbox.right  = 8;
    StarPost->hitbox.bottom = 20;
    StarPost->activePlayers = (1 << Player->playerCount) - 1;

    DEBUGMODE_ADD_OBJ(StarPost);

    for (int32 i = 0; i < Player->playerCount; ++i) {
        if (StarPost->postIDs[i]) {
            EntityPlayer *player          = RSDK_GET_ENTITY(i, Player);
            EntityStarPost *savedStarPost = RSDK_GET_ENTITY(StarPost->postIDs[i], StarPost);
            if (!TMZ2Setup) {
                foreach_all(StarPost, starPost)
                {
                    if (starPost->id < savedStarPost->id && !starPost->activated) {
                        starPost->activated = StarPost->activePlayers;
                        RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &starPost->ballAnimator, true, 0);
                    }
                }
            }

            if (!globals->specialRingID) {
                if (globals->gameMode < MODE_TIMEATTACK) {
                    int32 ms = SceneInfo->milliseconds;
                    int32 s  = SceneInfo->minutes;
                    int32 m  = SceneInfo->seconds;
                    if ((SceneInfo->milliseconds || SceneInfo->seconds || SceneInfo->minutes) || ms != globals->tempMilliseconds
                        || s != globals->tempSeconds || m != globals->tempMinutes) {
                        SceneInfo->milliseconds = StarPost->storedMS;
                        SceneInfo->seconds      = StarPost->storedSeconds;
                        SceneInfo->minutes      = StarPost->storedMinutes;
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

                        for (int32 p = 0; p < 0x10; ++p) {
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
    self->starAngle += 4;
    self->starAngle &= 0x1FF;
    self->starAngle2 += 18;
    self->starAngle2 &= 0x1FF;

    if (self->starTimer > 472) {
        --self->starOffset;
    }
    else if (self->starTimer < 0x80) {
        ++self->starOffset;
    }

    if (++self->starTimer == 600) {
        self->starTimer = 0;
        self->starFlag  = 0;
        self->active    = ACTIVE_BOUNDS;
    }
    self->starAnimator.frameID = (self->starAngle >> 3) & 3;
    self->starHitbox.left      = -(self->starOffset >> 2);
    self->starHitbox.top       = -48;
    self->starHitbox.right     = self->starOffset >> 2;
    self->starHitbox.bottom    = -40;
    if (self->starTimer >= 60) {
        if (!globals->recallEntities) {
            if (Player_CheckCollisionTouch(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), self, &self->starHitbox)) {
                SaveGame_SaveGameState();
                RSDK.PlaySfx(StarPost->sfxWarp, false, 0xFE);
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
#if RETRO_USE_PLUS
                EntityGameProgress *progress = GameProgress_GetGameProgress();
                if ((API.CheckDLC(DLC_PLUS) && progress && progress->allGoldMedals) || globals->gameMode == MODE_ENCORE) {
                    SaveGame->saveRAM->storedStageID = SceneInfo->listPos;
                    RSDK.SetScene("Pinball", "");
                    Zone_StartFadeOut(10, 0xF0F0F0);
                    RSDK.StopChannel(Music->channelID);
                }
                else {
#endif
                    SaveGame->saveRAM->storedStageID = SceneInfo->listPos;
                    RSDK.SetScene("Blue Spheres", "");
                    SceneInfo->listPos += globals->blueSpheresID;
                    Zone_StartFadeOut(10, 0xF0F0F0);
                    RSDK.StopChannel(Music->channelID);
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
    foreach_active(Player, player)
    {
        int32 playerSlot = RSDK.GetEntityID(player);
        if (!((1 << playerSlot) & self->activated) && !player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &StarPost->hitbox)) {
                self->state = StarPost_State_BallSpin;
                if (!TMZ2Setup) {
                    foreach_all(StarPost, starPost)
                    {
                        if (starPost->id < self->id && !starPost->activated) {
                            starPost->activated = 1 << playerSlot;
                            RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &starPost->ballAnimator, true, 0);
                        }
                    }
                }

                StarPost->postIDs[playerSlot]           = SceneInfo->entitySlot;
                StarPost->playerPositions[playerSlot].x = self->position.x;
                StarPost->playerPositions[playerSlot].y = self->position.y;
                StarPost->playerDirections[playerSlot]  = self->direction;
                if (globals->gameMode < MODE_TIMEATTACK) {
                    StarPost->storedMS      = SceneInfo->milliseconds;
                    StarPost->storedSeconds = SceneInfo->seconds;
                    StarPost->storedMinutes = SceneInfo->minutes;
                }

                int32 speed = 0;
                if (player->onGround)
                    speed = -12 * (player->groundVel >> 17);
                else
                    speed = -12 * (player->velocity.x >> 17);

                if (speed >= 0)
                    speed += 32;
                else
                    speed -= 32;

                int32 ballSpeed = self->ballSpeed;
                if (!ballSpeed) {
                    self->ballSpeed = speed;
                }
                else if (ballSpeed <= 0) {
                    if (speed < ballSpeed) {
                        self->ballSpeed = speed;
                    }
                    else if (speed > 0) {
                        speed += ballSpeed;
                        self->ballSpeed = speed;
                    }
                }
                else {
                    if (speed > ballSpeed) {
                        self->ballSpeed = speed;
                    }
                    else if (speed < 0) {
                        speed += ballSpeed;
                        self->ballSpeed = speed;
                    }
                }

                self->timer = 0;
                if (globals->gameMode < MODE_TIMEATTACK) {
                    int32 quota = 50;
#if RETRO_USE_PLUS
                    if (globals->gameMode != MODE_ENCORE)
                        quota = 25;
#endif
                    if (player->rings >= quota) {
                        self->starTimer  = 0;
                        self->starAngle  = 0;
                        self->starAngle2 = 0;
                        self->starOffset = 0;
                        self->starFlag   = (player->rings - 20) % 3 + 1;
                    }
                }

                if (!self->activated) {
                    RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &self->ballAnimator, true, 0);
                    self->ballAnimator.speed = 0;
                }
                self->activated |= 1 << playerSlot;
                self->active = ACTIVE_NORMAL;
                RSDK.PlaySfx(StarPost->sfxStarPost, false, 255);
            }
        }
    }
}
void StarPost_State_Idle(void)
{
    RSDK_THIS(StarPost);
    if (self->activated < StarPost->activePlayers)
        StarPost_CheckCollisions();
    if (self->starFlag > 0)
        StarPost_CheckBonusStageEntry();
    RSDK.ProcessAnimation(&self->ballAnimator);
}
void StarPost_State_BallSpin(void)
{
    RSDK_THIS(StarPost);
    if (self->activated < StarPost->activePlayers)
        StarPost_CheckCollisions();

    self->angle += self->ballSpeed;
    if (!StarPost->hasAchievement && self->timer == 10) {
        API_UnlockAchievement("ACH_STARPOST");
        StarPost->hasAchievement = true;
    }

    bool32 flag = false;
    if (self->ballSpeed <= 0) {
        if (self->angle <= -768) {
            ++self->timer;
            self->angle += 1024;
            if (self->ballSpeed + 8 > -32)
                self->ballSpeed = -32;
            else
                self->ballSpeed += 8;
            flag = self->ballSpeed == -32;
        }
    }
    else {
        if (self->angle >= 1280) {
            ++self->timer;
            self->angle -= 0x400;
            if (self->ballSpeed - 8 < 32)
                self->ballSpeed = 32;
            else
                self->ballSpeed -= 8;
            flag = self->ballSpeed == 32;
        }
    }

    if (flag) {
        self->state                       = StarPost_State_Idle;
        self->ballAnimator.speed = 64;
        self->ballSpeed                   = 0;
        self->angle                       = 256;
        if (!self->starFlag)
            self->active = ACTIVE_BOUNDS;
    }

    if (self->starFlag > 0)
        StarPost_CheckBonusStageEntry();
    RSDK.ProcessAnimation(&self->ballAnimator);
}

#if RETRO_INCLUDE_EDITOR
void StarPost_EditorDraw(void) { StarPost_DebugDraw(); }

void StarPost_EditorLoad(void) { StarPost->aniFrames = RSDK.LoadSpriteAnimation("Global/StarPost.bin", SCOPE_STAGE); }
#endif

void StarPost_Serialize(void)
{
    RSDK_EDITABLE_VAR(StarPost, VAR_ENUM, id);
    RSDK_EDITABLE_VAR(StarPost, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(StarPost, VAR_BOOL, vsRemove);
}

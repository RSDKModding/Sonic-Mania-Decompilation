// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DBTower Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDBTower *DBTower;

void DBTower_Update(void)
{
    RSDK_THIS(DBTower);

    StateMachine_Run(self->state);
}

void DBTower_LateUpdate(void) {}

void DBTower_StaticUpdate(void) {}

void DBTower_Draw(void)
{
    RSDK_THIS(DBTower);

    if (self->connectedSegmentCount <= 0) {
        self->bodyAnimator.frameID = self->rotation >> 5;
        RSDK.DrawSprite(self->segmentAnimators[0], &self->bodyPositions[0], false);
    }
    else {
        self->direction ^= FLIP_X;
        for (int32 i = self->connectedSegmentCount; i > 0; --i) {
            self->rotation             = (2 * self->bodyAngles[i] - 15) & 0x1E;
            self->bodyAnimator.frameID = self->bodyAngles[i] >> 4;
            RSDK.DrawSprite(self->segmentAnimators[i], &self->bodyPositions[i], false);
        }

        self->direction ^= FLIP_X;
        self->rotation = 0;

        if (self->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 160, 0xE0E0E0);

        RSDK.DrawSprite(self->segmentAnimators[0], &self->bodyPositions[0], false);
        RSDK.SetPaletteEntry(0, 160, 0x200000);
    }
}

void DBTower_Create(void *data)
{
    RSDK_THIS(DBTower);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->drawFX        = FX_ROTATE | FX_FLIP;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            self->visible       = true;

            if (data) {
                self->active    = ACTIVE_NORMAL;
                self->drawGroup = Zone->objectDrawGroup[0];
                RSDK.SetSpriteAnimation(DBTower->aniFrames, 2, &self->bodyAnimator, true, 0);
                self->segmentAnimators[0] = &self->bodyAnimator;
                self->bodyPositions[0].x  = self->position.x;
                self->bodyPositions[0].y  = self->position.y;
                self->state               = (Type_StateMachine)data;
            }
            else {
                self->active    = ACTIVE_BOUNDS;
                self->drawGroup = Zone->objectDrawGroup[0] + 1;
                self->timer     = 0;
                self->direction = FLIP_X;

                for (int32 i = 0; i < DBTOWER_SEGMENT_COUNT; ++i) {
                    self->segmentAnimators[i] = &self->bodyAnimator;
                    self->segmentUnused3[i]   = 0;
                    self->bodyAngles[i]       = 0xC0;
                }
                self->segmentAnimators[0] = &self->headAnimator;
                self->bodyPositions[0].x  = self->position.x + ((ScreenInfo->center.x - 64) << 16);
                self->bodyPositions[0].y  = self->position.y + 0xA20000;

                self->originPos             = self->position;
                self->health                = 6;
                self->wobbleAngleVel        = 640;
                self->xOffsetAngle          = 64;
                self->connectedSegmentCount = 0;

                RSDK.SetSpriteAnimation(DBTower->aniFrames, 0, &self->headAnimator, true, 0);
                RSDK.SetSpriteAnimation(DBTower->aniFrames, 2, &self->bodyAnimator, true, 0);

                self->state = DBTower_State_SetupArena;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void DBTower_StageLoad(void)
{
    DBTower->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE);

    DBTower->hitboxSegment.left   = -27;
    DBTower->hitboxSegment.top    = -27;
    DBTower->hitboxSegment.right  = 27;
    DBTower->hitboxSegment.bottom = 27;

    DBTower->defeated = false;
    DBTower->active   = ACTIVE_ALWAYS;

    DBTower->sfxRocketJet    = RSDK.GetSfx("Stage/RocketJet.wav");
    DBTower->sfxHit          = RSDK.GetSfx("Stage/BossHit.wav");
    DBTower->sfxExplosion2   = RSDK.GetSfx("Stage/Explosion2.wav");
    DBTower->sfxExplosion3   = RSDK.GetSfx("Stage/Explosion3.wav");
    DBTower->sfxBumper3      = RSDK.GetSfx("Stage/Bumper3.wav");
    DBTower->sfxAssemble     = RSDK.GetSfx("Stage/Assemble.wav");
    DBTower->sfxRocketJet2   = RSDK.GetSfx("Stage/RocketJet.wav");
    DBTower->sfxRockemSockem = RSDK.GetSfx("Stage/RockemSockem.wav");
}

void DBTower_CheckPlayerCollisions_Head(void)
{
    RSDK_THIS(DBTower);

    if (self->invincibilityTimer > 0) {
        self->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (self->playerTimers[playerID]) {
                --self->playerTimers[playerID];
            }
            else {
                self->position.x = self->bodyPositions[0].x;
                self->position.y = self->bodyPositions[0].y;

                if (Player_CheckBadnikTouch(player, self, &DBTower->hitboxSegment) && Player_CheckBossHit(player, self)) {
                    self->wobbleAngleVel = 2048;

                    if (--self->health <= 0) {
                        SceneInfo->timeEnabled = false;
                        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                        RSDK.PlaySfx(DBTower->sfxExplosion2, false, 255);

                        self->timer = 120;
                        self->state = DBTower_State_Destroyed;
                    }
                    else {
                        self->invincibilityTimer = 48;
                        RSDK.PlaySfx(DBTower->sfxHit, false, 255);
                    }

                    foreach_break;
                }
                else {
                    for (int32 i = 1; i <= self->connectedSegmentCount; ++i) {
                        self->position.x = self->bodyPositions[i].x;
                        self->position.y = self->bodyPositions[i].y;

                        uint8 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y) - self->bodyAngles[i];
                        if (Player_CheckBadnikTouch(player, self, &DBTower->hitboxSegment)) {
                            if (angle >= 0x80) {
#if MANIA_USE_PLUS
                                if (!Player_CheckMightyUnspin(player, 0x300, true, &player->uncurlTimer))
#endif
                                    Player_Hurt(player, self);
                            }
                            else if (Player_CheckBossHit(player, self)) {
                                RSDK.PlaySfx(DBTower->sfxBumper3, false, 255);
                                self->playerTimers[playerID] = 30;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    self->position.x = self->bodyPositions[0].x;
    self->position.y = self->bodyPositions[0].y;
}

void DBTower_Explode(void)
{
    RSDK_THIS(DBTower);

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(UberCaterkiller->sfxExplosion2, false, 255);

        if (!(Zone->timer & 7)) {
            int32 x                    = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void DBTower_State_SetupArena(void)
{
    RSDK_THIS(DBTower);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->center.x;

        self->active = ACTIVE_NORMAL;
        self->timer  = 0;
        self->position.x += (ScreenInfo->center.x - 64) << 16;

        self->originPos.x = self->position.x;
        self->originPos.y += 0xA20000;

        self->bodyPositions[0].x = self->position.x;
        self->bodyPositions[0].y = self->originPos.y;
        for (int32 i = 1; i < DBTOWER_SEGMENT_COUNT; ++i) {
            self->bodyPositions[i].x = self->originPos.x;
            self->bodyPositions[i].y = 0x7FFF0000;
        }

        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);

#if MANIA_USE_PLUS
        if (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) {
            self->timer = 60;
            self->state = DBTower_State_Setup_Encore;
        }
        else {
#endif
            RSDK.PlaySfx(DBTower->sfxAssemble, false, 255);
            self->bodyPositions[++self->connectedSegmentCount].y = 0x7FFF0000;
            self->segmentOffsetY += 0x360000;
            self->timer = 0;
            self->state = DBTower_State_HandleBoss;
#if MANIA_USE_PLUS
        }
#endif
    }
}

#if MANIA_USE_PLUS
void DBTower_State_Setup_Encore(void)
{
    RSDK_THIS(DBTower);

    if (--self->timer <= 0) {
        RSDK.PlaySfx(DBTower->sfxAssemble, false, 255);
        self->bodyPositions[++self->connectedSegmentCount].y = 0x7FFF0000;
        self->segmentOffsetY += 0x360000;
        self->timer = 0;
        self->state = DBTower_State_HandleBoss;
    }
}
#endif

void DBTower_State_HandleBoss(void)
{
    RSDK_THIS(DBTower);

    ++self->xOffsetAngle;
    self->angle = (RSDK.Sin256(self->wobbleAngle >> 8) >> 5) + 0xC0;

    if (self->headAnimator.frameID != 0)
        RSDK.ProcessAnimation(&self->headAnimator);

    int32 x = 0x3600 * ((DBTOWER_SEGMENT_COUNT - 1) - self->connectedSegmentCount) * RSDK.Cos256(self->angle);
    self->bodyPositions[self->connectedSegmentCount].x = self->originPos.x + x + (RSDK.Cos256(self->xOffsetAngle) << 12);
    self->bodyPositions[self->connectedSegmentCount].y = self->originPos.y + self->segmentOffsetY;

    for (int32 i = self->connectedSegmentCount - 1; i >= 0; --i) {
        self->bodyPositions[i].x = 0x3600 * RSDK.Cos256(self->angle) + self->bodyPositions[i + 1].x;
        self->bodyPositions[i].y = 0x3600 * RSDK.Sin256(self->angle) + self->bodyPositions[i + 1].y;
    }

    self->wobbleAngle += self->wobbleAngleVel;

    if (self->wobbleAngleVel > 640) {
        self->wobbleAngleVel -= 8;
        RSDK.ProcessAnimation(&self->headAnimator);
    }

    if (self->segmentOffsetY < 0) {
        self->segmentOffsetY += self->velocity.y;
        if (self->segmentOffsetY < 0) {
            self->velocity.y += 0x2800;
        }
        else {
            self->segmentOffsetY = 0;
            self->velocity.y     = 0;
        }
    }
    else if (self->segmentOffsetY <= 0) {
        if (++self->timer >= 120 || !self->connectedSegmentCount) {
            self->timer = 0;
            if (self->connectedSegmentCount > 0) {
                RSDK.PlaySfx(DBTower->sfxRocketJet2, false, 255);

                EntityDBTower *child = NULL;
                if (!(self->connectedSegmentCount & 1)) {
                    child = CREATE_ENTITY(DBTower, DBTower_State_BodyRolling, self->bodyPositions[self->connectedSegmentCount].x,
                                          self->bodyPositions[self->connectedSegmentCount].y);
                }
                else {
                    child             = CREATE_ENTITY(DBTower, DBTower_State_BodyBouncing, self->bodyPositions[self->connectedSegmentCount].x,
                                          self->bodyPositions[self->connectedSegmentCount].y);
                    child->velocity.x = -0x20000;
                    child->velocity.y = -0x80000;
                }
                child->bodyAngles[0] = self->bodyAngles[self->connectedSegmentCount];
                child->rotation      = 2 * self->bodyAngles[self->connectedSegmentCount];
            }

            if (!(--self->connectedSegmentCount & 0x80)) {
                self->segmentOffsetY -= 0x360000;
            }
            else {
                RSDK.PlaySfx(DBTower->sfxAssemble, false, 255);
                self->connectedSegmentCount += 2;
                self->bodyPositions[self->connectedSegmentCount].y = 0x7FFF0000;
                self->segmentOffsetY += 0x360000;
            }
        }
    }
    else {
        self->segmentOffsetY -= 0x10000;

        if (self->segmentOffsetY <= 0) {
            if (self->connectedSegmentCount < DBTOWER_SEGMENT_COUNT - 1) {
                self->connectedSegmentCount++;
                self->bodyPositions[self->connectedSegmentCount].y = 0x7FFF0000;
                self->segmentOffsetY += 0x360000;
            }
        }
    }
    DBTower_CheckPlayerCollisions_Head();
}

void DBTower_State_Destroyed(void)
{
    RSDK_THIS(DBTower);

    DBTower_Explode();

    if (--self->timer <= 0) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 32;
        RSDK.PlaySfx(DBTower->sfxExplosion3, false, 255);

        for (int32 i = 1; i < DBTOWER_SEGMENT_COUNT; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->bodyPositions[i].x, self->bodyPositions[i].y);
            RSDK.SetSpriteAnimation(DBTower->aniFrames, self->segmentAnimators[i]->animationID, &debris->animator, true,
                                    self->segmentAnimators[i]->frameID);
            debris->velocity.x       = 4 * RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y       = 4 * RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength  = 0x4800;
            debris->drawGroup        = Zone->objectDrawGroup[1];
            debris->updateRange.x    = 0x400000;
            debris->updateRange.y    = 0x400000;
            self->bodyPositions[i].x = -0x800000;
            self->bodyPositions[i].y = -0x800000;
        }

        self->velocity.y = -0x40000;
        self->state      = DBTower_State_Finish;
    }
}

void DBTower_State_Finish(void)
{
    RSDK_THIS(DBTower);

    DBTower_Explode();

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->bodyPositions[0].y = self->position.y;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        DBTower->defeated = true;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

#if MANIA_USE_PLUS
        if (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE)) {
            self->timer = 0;
            self->state = DBTower_State_SpawnSignPost;
        }
        else {
#endif
            Zone->cameraBoundsR[0] += 848;
            Zone->playerBoundActiveR[0] = false;
            Zone->cameraBoundsB[0]      = 304;
            destroyEntity(self);
#if MANIA_USE_PLUS
        }
#endif
    }
}

#if MANIA_USE_PLUS
void DBTower_State_SpawnSignPost(void)
{
    RSDK_THIS(DBTower);

    if (++self->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->state      = SignPost_State_Falling;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }
        destroyEntity(self);
    }
}
#endif

void DBTower_CheckPlayerCollisions_Body(void)
{
    RSDK_THIS(DBTower);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (self->playerTimers[playerID]) {
            self->playerTimers[playerID]--;
        }
        else {
            uint8 angle = (RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y) - self->bodyAngles[0]);
            if (Player_CheckBadnikTouch(player, self, &DBTower->hitboxSegment)) {
                if (angle >= 0x80) {
#if MANIA_USE_PLUS
                    if (Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer))
                        self->playerTimers[playerID] = 30;
                    else
#endif
                        Player_Hurt(player, self);
                }
                else if (
#if MANIA_USE_PLUS
                    (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_CROUCH) ||
#endif
                    Player_CheckBossHit(player, self)) {
                    RSDK.PlaySfx(DBTower->sfxBumper3, false, 255);
                    self->playerTimers[playerID] = 30;
                    foreach_break;
                }
            }
        }
    }
}

void DBTower_State_BodyBouncing(void)
{
    RSDK_THIS(DBTower);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x1B0000, true)) {
        RSDK.PlaySfx(DBTower->sfxRockemSockem, false, 255);
        self->velocity.y = -0x80000;
    }

    self->bodyPositions[0] = self->position;
    self->bodyAngles[0]    = (self->bodyAngles[0] - 8) & 0xFF;
    self->rotation         = self->bodyAngles[0] << 1;

    if (RSDK.CheckOnScreen(self, &self->updateRange))
        DBTower_CheckPlayerCollisions_Body();
    else
        destroyEntity(self);
}

void DBTower_State_BodyRolling(void)
{
    RSDK_THIS(DBTower);

    self->position.x += self->velocity.x;
    self->velocity.x -= 0x2800;

    self->bodyPositions[0] = self->position;
    self->bodyPositions[0] = self->position;
    self->bodyAngles[0]    = (self->bodyAngles[0] - 8) & 0xFF;
    self->rotation         = self->bodyAngles[0] << 1;

    if (RSDK.CheckOnScreen(self, &self->updateRange))
        DBTower_CheckPlayerCollisions_Body();
    else
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void DBTower_EditorDraw(void)
{
    RSDK_THIS(DBTower);

    RSDK.SetSpriteAnimation(DBTower->aniFrames, 0, &self->headAnimator, true, 0);
    RSDK.DrawSprite(&self->headAnimator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YCENTER, WIDE_SCR_XCENTER, SCREEN_YCENTER, 1 | 0 | 4 | 0, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void DBTower_EditorLoad(void) { DBTower->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE); }
#endif

void DBTower_Serialize(void) {}

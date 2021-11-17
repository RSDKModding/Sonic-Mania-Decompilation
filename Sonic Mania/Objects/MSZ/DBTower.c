#include "SonicMania.h"

ObjectDBTower *DBTower;

void DBTower_Update(void)
{
    RSDK_THIS(DBTower);
    StateMachine_Run(entity->state);
}

void DBTower_LateUpdate(void) {}

void DBTower_StaticUpdate(void) {}

void DBTower_Draw(void)
{
    RSDK_THIS(DBTower);

    if (entity->segmentCount <= 0) {
        entity->animator2.frameID = entity->rotation >> 5;
        RSDK.DrawSprite(entity->animatorPtrs[0], &entity->bodyPositions[0], false);
    }
    else {
        entity->direction ^= FLIP_X;
        for (int i = entity->segmentCount; i > 0; --i) {
            entity->rotation          = (2 * entity->bodyAngles[i] - 15) & 0x1E;
            entity->animator2.frameID = entity->bodyAngles[i] >> 4;
            RSDK.DrawSprite(entity->animatorPtrs[i], &entity->bodyPositions[i], false);
        }

        entity->direction ^= FLIP_X;
        entity->rotation = 0;
        if (entity->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 160, 0xE0E0E0);
        RSDK.DrawSprite(entity->animatorPtrs[0], &entity->bodyPositions[0], false);
        RSDK.SetPaletteEntry(0, 160, 0x200000);
    }
}

void DBTower_Create(void *data)
{
    RSDK_THIS(DBTower);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->drawFX        = FX_ROTATE | FX_FLIP;
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            entity->visible       = true;

            if (data) {
                entity->active    = ACTIVE_NORMAL;
                entity->drawOrder = Zone->drawOrderLow;
                RSDK.SetSpriteAnimation(DBTower->aniFrames, 2, &entity->animator2, true, 0);
                entity->animatorPtrs[0]    = &entity->animator2;
                entity->bodyPositions[0].x = entity->position.x;
                entity->bodyPositions[0].y = entity->position.y;
                entity->state              = (Type_StateMachine)data;
            }
            else {
                entity->active    = ACTIVE_BOUNDS;
                entity->drawOrder = Zone->drawOrderLow + 1;
                entity->timer     = 0;
                entity->direction = FLIP_X;

                for (int i = 0; i < DBTower_SegmentCount; ++i) {
                    entity->animatorPtrs[i] = &entity->animator2;
                    entity->field_B0[i]     = 0;
                    entity->bodyAngles[i]   = 0xC0;
                }
                entity->animatorPtrs[0]    = &entity->animator1;
                entity->bodyPositions[0].x = entity->position.x + ((RSDK_screens->centerX - 64) << 16);
                entity->bodyPositions[0].y = entity->position.y + 0xA20000;

                entity->field_68.x  = entity->position.x;
                entity->field_68.y  = entity->position.y;
                entity->health      = 6;
                entity->field_D0[1] = 640;
                entity->field_D0[3] = 64;
                entity->segmentCount    = 0;
                RSDK.SetSpriteAnimation(DBTower->aniFrames, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(DBTower->aniFrames, 2, &entity->animator2, true, 0);
                entity->state = DBTower_State_SetupArena;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void DBTower_StageLoad(void)
{
    DBTower->aniFrames       = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE);
    DBTower->hitbox.left     = -27;
    DBTower->hitbox.top      = -27;
    DBTower->hitbox.right    = 27;
    DBTower->hitbox.bottom   = 27;
    DBTower->defeated        = false;
    DBTower->active          = ACTIVE_ALWAYS;
    DBTower->sfxRocketJet    = RSDK.GetSFX("Stage/RocketJet.wav");
    DBTower->sfxHit          = RSDK.GetSFX("Stage/BossHit.wav");
    DBTower->sfxExplosion2   = RSDK.GetSFX("Stage/Explosion2.wav");
    DBTower->sfxExplosion3   = RSDK.GetSFX("Stage/Explosion3.wav");
    DBTower->sfxBumper3      = RSDK.GetSFX("Stage/Bumper3.wav");
    DBTower->sfxAssemble     = RSDK.GetSFX("Stage/Assemble.wav");
    DBTower->sfxRocketJet2   = RSDK.GetSFX("Stage/RocketJet.wav");
    DBTower->sfxRockemSockem = RSDK.GetSFX("Stage/RockemSockem.wav");
}

void DBTower_CheckPlayerCollisions(void)
{
    RSDK_THIS(DBTower);

    if (entity->invincibilityTimer > 0) {
        entity->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            uint8 *timer = &entity->playerTimers[RSDK.GetEntityID(player)];
            if (*timer) {
                --*timer;
            }
            else {
                entity->position.x = entity->bodyPositions[0].x;
                entity->position.y = entity->bodyPositions[0].y;

                if (Player_CheckBadnikTouch(player, entity, &DBTower->hitbox) && Player_CheckBossHit(player, entity)) {
                    entity->field_D0[1] = 2048;
                    if (--entity->health <= 0) {
                        RSDK_sceneInfo->timeEnabled = false;
                        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                        RSDK.PlaySfx(DBTower->sfxExplosion2, false, 255);
                        entity->timer = 120;
                        entity->state = DBTower_State_Destroyed;
                    }
                    else {
                        entity->invincibilityTimer = 48;
                        RSDK.PlaySfx(DBTower->sfxHit, false, 255);
                    }
                    foreach_break;
                }
                else {
                    for (int i = 1; i < DBTower_SegmentCount; ++i) {
                        entity->position.x = entity->bodyPositions[i].x;
                        entity->position.y = entity->bodyPositions[i].y;

                        uint8 angle =
                            RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y) - entity->bodyAngles[i];
                        if (Player_CheckBadnikTouch(player, entity, &DBTower->hitbox)) {
                            if (angle >= 0x80) {
#if RETRO_USE_PLUS
                                if (!Player_CheckMightyUnspin(0x300, player, true, &player->uncurlTimer))
#endif
                                    Player_CheckHit(player, entity);
                            }
                            else if (Player_CheckBossHit(player, entity)) {
                                RSDK.PlaySfx(DBTower->sfxBumper3, false, 255);
                                *timer = 30;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    entity->position.x = entity->bodyPositions[0].x;
    entity->position.y = entity->bodyPositions[0].y;
}

void DBTower_Explode(void)
{
    RSDK_THIS(DBTower);
    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(UberCaterkiller->sfxExplosion2, false, 255);

        if (!(Zone->timer & 7)) {
            int x = RSDK.Rand(-19, 20) << 16;
            int y = RSDK.Rand(-24, 25) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x + entity->position.x, y + entity->position.y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void DBTower_State_SetupArena(void)
{
    RSDK_THIS(DBTower);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
        entity->active              = ACTIVE_NORMAL;
        entity->timer               = 0;
        entity->field_68.y += 0xA20000;
        entity->position.x += (RSDK_screens->centerX - 64) << 16;

        entity->field_68.x         = entity->position.x;
        entity->bodyPositions[0].x = entity->position.x;
        entity->bodyPositions[0].y = entity->field_68.y;
        for (int i = 1; i < DBTower_SegmentCount; ++i) {
            entity->bodyPositions[i].x = entity->field_68.x;
            entity->bodyPositions[i].y = 0x7FFF0000;
        }

        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);

#if RETRO_USE_PLUS
        if (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) {
            entity->timer = 60;
            entity->state = DBTower_State_Setup_Encore;
        }
        else {
#endif
            RSDK.PlaySfx(DBTower->sfxAssemble, false, 255);
            entity->bodyPositions[++entity->segmentCount].y = 0x7FFF0000;
            entity->field_D0[2] += 0x360000;
            entity->timer = 0;
            entity->state = DBTower_State_HandleBoss;
#if RETRO_USE_PLUS
        }
#endif
    }
}

void DBTower_State_Setup_Encore(void)
{
    RSDK_THIS(DBTower);
    if (--entity->timer <= 0) {
        RSDK.PlaySfx(DBTower->sfxAssemble, false, 255);
        entity->bodyPositions[++entity->segmentCount].y = 0x7FFF0000;
        entity->field_D0[2] += 0x360000;
        entity->timer = 0;
        entity->state = DBTower_State_HandleBoss;
    }
}

void DBTower_State_HandleBoss(void)
{
    RSDK_THIS(DBTower);

    ++entity->field_D0[3];
    entity->angle = (RSDK.Sin256(entity->field_D0[0] >> 8) >> 5) + 0xC0;
    if (entity->animator1.frameID != 0)
        RSDK.ProcessAnimation(&entity->animator1);

    int x                                         = 0x3600 * ((DBTower_SegmentCount - 1) - entity->segmentCount) * RSDK.Cos256(entity->angle);
    entity->bodyPositions[entity->segmentCount].x = x + (RSDK.Cos256(entity->field_D0[3]) << 12) + entity->field_68.x;
    entity->bodyPositions[entity->segmentCount].y = entity->field_D0[2] + entity->field_68.y;

    for (int i = entity->segmentCount - 1; i >= 0; --i) {
        entity->bodyPositions[i].x = 0x3600 * RSDK.Cos256(entity->angle) + entity->bodyPositions[i + 1].x;
        entity->bodyPositions[i].y = 0x3600 * RSDK.Sin256(entity->angle) + entity->bodyPositions[i + 1].y;
    }

    entity->field_D0[0] += entity->field_D0[1];
    if (entity->field_D0[1] > 640) {
        entity->field_D0[1] -= 8;
        RSDK.ProcessAnimation(&entity->animator1);
    }

    if (entity->field_D0[2] < 0) {
        entity->field_D0[2] += entity->velocity.y;
        if (entity->field_D0[2] < 0) {
            entity->velocity.y += 0x2800;
        }
        else {
            entity->field_D0[2] = 0;
            entity->velocity.y  = 0;
        }
    }
    else if (entity->field_D0[2] <= 0) {
        if (++entity->timer >= 120 || !entity->segmentCount) {
            entity->timer = 0;
            if (entity->segmentCount > 0) {
                RSDK.PlaySfx(DBTower->sfxRocketJet2, false, 255);

                EntityDBTower *child = NULL;
                if (!(entity->segmentCount & 1)) {
                    child = CREATE_ENTITY(DBTower, DBTower_State_BodyRolling, entity->bodyPositions[entity->segmentCount].x,
                                          entity->bodyPositions[entity->segmentCount].y);
                }
                else {
                    child             = CREATE_ENTITY(DBTower, DBTower_State_BodyBouncing, entity->bodyPositions[entity->segmentCount].x,
                                          entity->bodyPositions[entity->segmentCount].y);
                    child->velocity.x = -0x20000;
                    child->velocity.y = -0x80000;
                }
                child->bodyAngles[0] = entity->bodyAngles[entity->segmentCount];
                child->rotation      = 2 * entity->bodyAngles[entity->segmentCount];
            }

            if (!(--entity->segmentCount & 0x80)) {
                entity->field_D0[2] -= 0x360000;
            }
            else {
                RSDK.PlaySfx(DBTower->sfxAssemble, false, 255);
                entity->segmentCount += 2;
                entity->bodyPositions[entity->segmentCount].y = 0x7FFF0000;
                entity->field_D0[2] += 0x360000;
            }
        }
    }
    else {
        entity->field_D0[2] -= 0x10000;
        if (entity->field_D0[2] <= 0) {
            if (entity->segmentCount < DBTower_SegmentCount - 1) {
                entity->segmentCount++;
                entity->bodyPositions[entity->segmentCount].y = 0x7FFF0000;
                entity->field_D0[2] += 0x360000;
            }
        }
    }
    DBTower_CheckPlayerCollisions();
}

void DBTower_State_Destroyed(void)
{
    RSDK_THIS(DBTower);
    DBTower_Explode();

    if (--entity->timer <= 0) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 32;
        RSDK.PlaySfx(DBTower->sfxExplosion3, false, 255);

        for (int i = 1; i < DBTower_SegmentCount; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->bodyPositions[i].x, entity->bodyPositions[i].y);
            RSDK.SetSpriteAnimation(DBTower->aniFrames, entity->animatorPtrs[i]->animationID, &debris->animator, true,
                                    entity->animatorPtrs[i]->frameID);
            debris->velocity.x         = 4 * RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y         = 4 * RSDK.Rand(-0x20000, -0x10000);
            debris->gravity            = 0x4800;
            debris->drawOrder          = Zone->drawOrderHigh;
            debris->updateRange.x      = 0x400000;
            debris->updateRange.y      = 0x400000;
            entity->bodyPositions[i].x = -0x800000;
            entity->bodyPositions[i].y = -0x800000;
        }

        entity->velocity.y = -0x40000;
        entity->state      = DBTower_State_Finish;
    }
}

void DBTower_State_Finish(void)
{
    RSDK_THIS(DBTower);

    DBTower_Explode();
    entity->position.y += entity->velocity.y;
    entity->bodyPositions[0].y = entity->position.y;
    entity->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        DBTower->defeated = true;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

#if RETRO_USE_PLUS
        if (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) {
            entity->timer = 0;
            entity->state = DBTower_State_SpawnSignPost;
        }
        else {
#endif
            Zone->screenBoundsR1[0] += 848;
            Zone->playerBoundActiveR[0] = false;
            Zone->screenBoundsB1[0]     = 304;
            destroyEntity(entity);
#if RETRO_USE_PLUS
        }
#endif
    }
}

#if RETRO_USE_PLUS
void DBTower_State_SpawnSignPost(void)
{
    RSDK_THIS(DBTower);

    if (++entity->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = entity->position.x;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }
        destroyEntity(entity);
    }
}
#endif

void DBTower_CheckPlayerCollisions2(void)
{
    RSDK_THIS(DBTower);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (entity->playerTimers[playerID]) {
            entity->playerTimers[playerID]--;
        }
        else {
            uint8 angle = (RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y) - entity->bodyAngles[0]);
            if (Player_CheckBadnikTouch(player, entity, &DBTower->hitbox)) {
                if (angle >= 0x80) {
#if RETRO_USE_PLUS
                    if (Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
                        entity->playerTimers[playerID] = 30;
                    else
#endif
                        Player_CheckHit(player, entity);
                }
                else if (
#if RETRO_USE_PLUS
                    player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_CROUCH ||
#endif
                    Player_CheckBossHit(player, entity)) {
                    RSDK.PlaySfx(DBTower->sfxBumper3, false, 255);
                    entity->playerTimers[playerID] = 30;
                    foreach_break;
                }
            }
        }
    }
}

void DBTower_State_BodyBouncing(void)
{
    RSDK_THIS(DBTower);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x1B0000, true)) {
        RSDK.PlaySfx(DBTower->sfxRockemSockem, false, 255);
        entity->velocity.y = -0x80000;
    }
    entity->bodyPositions[0].x = entity->position.x;
    entity->bodyPositions[0].y = entity->position.y;
    entity->bodyAngles[0]      = (entity->bodyAngles[0] - 8) & 0xFF;
    entity->rotation           = 2 * entity->bodyAngles[0];
    if (RSDK.CheckOnScreen(entity, &entity->updateRange))
        DBTower_CheckPlayerCollisions2();
    else
        destroyEntity(entity);
}

void DBTower_State_BodyRolling(void)
{
    RSDK_THIS(DBTower);

    entity->position.x += entity->velocity.x;
    entity->velocity.x -= 0x2800;
    entity->bodyPositions[0].x = entity->position.x;
    entity->bodyPositions[0].y = entity->position.y;
    entity->bodyPositions[0].x = entity->position.x;
    entity->bodyPositions[0].y = entity->position.y;
    entity->bodyAngles[0]      = (entity->bodyAngles[0] - 8) & 0xFF;
    entity->rotation           = 2 * entity->bodyAngles[0];
    if (RSDK.CheckOnScreen(entity, &entity->updateRange))
        DBTower_CheckPlayerCollisions2();
    else
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void DBTower_EditorDraw(void)
{
    RSDK_THIS(DBTower);
    RSDK.SetSpriteAnimation(DBTower->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void DBTower_EditorLoad(void) { DBTower->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE); }
#endif

void DBTower_Serialize(void) {}

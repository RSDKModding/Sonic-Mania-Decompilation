#include "SonicMania.h"

ObjectSignPost *SignPost;

void SignPost_Update(void)
{
    RSDK_THIS(SignPost);
    StateMachine_Run(entity->state);
}

void SignPost_LateUpdate(void) {}

void SignPost_StaticUpdate(void) {}

void SignPost_Draw(void)
{
    RSDK_THIS(SignPost);
    Vector2 drawPos;
    if (entity->state) {
        entity->drawFX = FX_SCALE;
        drawPos.y      = entity->position.y;

        Animator *face = &entity->facePlateAnim;
        if (entity->rotation <= 128 || entity->rotation >= 384)
            face = &entity->eggPlateAnim;

        if (RSDK.Cos512(entity->rotation) >= 0)
            entity->scale.x = RSDK.Cos512(entity->rotation);
        else
            entity->scale.x = -RSDK.Cos512(entity->rotation);

        int32 angle = 0;
        if (RSDK.Sin512(entity->rotation) >= 0)
            angle = RSDK.Sin512(entity->rotation);
        else
            angle = -RSDK.Sin512(entity->rotation);

        switch (entity->rotation >> 7) {
            case 0:
            case 2:
                drawPos.x = entity->position.x + (angle << 9);
                RSDK.DrawSprite(face, &drawPos, 0);
                drawPos.x += -0xC00 * entity->scale.x - (angle << 9);
                break;
            case 1:
            case 3:
                drawPos.x = entity->position.x - (angle << 9);
                RSDK.DrawSprite(face, &drawPos, 0);
                drawPos.x += (angle + 2 * (3 * entity->scale.x - 32)) << 9;
                break;
            default: break;
        }
        entity->scale.x = angle;
        RSDK.DrawSprite(&entity->sidebarData, &drawPos, false);
        entity->drawFX = FX_NONE;
        RSDK.DrawSprite(&entity->postTopData, NULL, false);
        RSDK.DrawSprite(&entity->standData, NULL, false);
    }
}

void SignPost_Create(void *data)
{
    RSDK_THIS(SignPost);

    if (!entity->vsBoundsSize.x)
        entity->vsBoundsSize.x = 0x2000000;
    if (!entity->vsBoundsSize.y)
        entity->vsBoundsSize.y = 0xF00000;
    if (!entity->vsExtendTop)
        entity->vsExtendTop = 120;
    if (!entity->vsExtendBottom)
        entity->vsExtendBottom = 120;
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode != MODE_TIMEATTACK) {
            RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_EGGMAN, &entity->eggPlateAnim, true, 0);
            switch (globals->playerID & 0xFF) {
                case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_TAILS, &entity->facePlateAnim, true, 0); break;
                case ID_KNUCKLES: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_KNUX, &entity->facePlateAnim, true, 0); break;
#if RETRO_USE_PLUS
                case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_MIGHTY, &entity->facePlateAnim, true, 0); break;
                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_RAY, &entity->facePlateAnim, true, 0); break;
#endif
                default: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_SONIC, &entity->facePlateAnim, true, 0); break;
            }
            RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_POST, &entity->postTopData, true, 0);
            RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_POST, &entity->sidebarData, true, 1);
            RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_POST, &entity->standData, true, 2);
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            if (globals->gameMode == MODE_COMPETITION) {
                int32 checkA = entity->vsBoundsOffset.y + (entity->vsBoundsSize.y >> 1);
                int32 checkB = entity->vsBoundsOffset.y - (entity->vsBoundsSize.y >> 1);

                if ((-0x10000 * entity->vsExtendTop) < entity->vsBoundsOffset.y - (entity->vsBoundsSize.y >> 1))
                    checkB = (-0x10000 * entity->vsExtendTop);

                if (entity->vsExtendBottom << 16 < checkA)
                    checkA = entity->vsExtendBottom << 16;

                if (abs(entity->vsBoundsOffset.x - (entity->vsBoundsSize.x >> 1)) > checkB)
                    entity->updateRange.x = abs(entity->vsBoundsOffset.x - (entity->vsBoundsSize.x >> 1));
                else
                    entity->updateRange.x = ((entity->vsBoundsSize.x >> 1) + entity->vsBoundsOffset.x) + 0x400000;

                if (abs(checkB) > checkA)
                    entity->updateRange.y = abs(checkB) + 0x400000;
                else
                    entity->updateRange.y = checkA + 0x400000;
            }
            entity->visible   = true;
            entity->drawOrder = Zone->drawOrderLow;
            entity->spinSpeed = 0x3000;
            entity->spinCount = 8;
            entity->maxAngle  = 0x10000;
            entity->scale.y   = 512;

            switch (entity->type) {
                default: destroyEntity(entity); break;
                case 0:
                    if (globals->gameMode != MODE_COMPETITION) {
                        entity->active = ACTIVE_BOUNDS;
                        entity->state  = SignPost_State_SetupCompetition;
                    }
                    else {
                        destroyEntity(entity);
                    }
                    break;
                case 1:
                    if (globals->gameMode != MODE_COMPETITION) {
                        entity->active = ACTIVE_XBOUNDS;
                        entity->state  = StateMachine_None;
                    }
                    else {
                        destroyEntity(entity);
                    }
                    break;
                case 2:
                    if (globals->gameMode == MODE_COMPETITION) {
                        entity->active = ACTIVE_BOUNDS;
                        entity->state  = SignPost_State_SetupCompetition;
                    }
                    else {
                        destroyEntity(entity);
                    }
                    break;
                case 3:
                    if (globals->gameMode != MODE_COMPETITION) {
                        entity->active = ACTIVE_BOUNDS;
                        entity->state  = SignPost_State_Finish;
                    }
                    else {
                        destroyEntity(entity);
                    }
                    break;
                case 4:
                    if (globals->gameMode != MODE_COMPETITION) {
                        entity->active = ACTIVE_BOUNDS;
                        entity->state  = SignPost_State_SetupCompetition;
                    }
                    else {
                        destroyEntity(entity);
                    }
                    break;
            }
        }
    }
}

void SignPost_StageLoad(void)
{
    SignPost->spriteIndex          = RSDK.LoadSpriteAnimation("Global/SignPost.bin", SCOPE_STAGE);
    SignPost->hitbox.left          = -24;
    SignPost->hitbox.top           = -22;
    SignPost->hitbox.right         = 24;
    SignPost->hitbox.bottom        = 8;
    SignPost->itemBoxHitbox.left   = -8;
    SignPost->itemBoxHitbox.top    = 20;
    SignPost->itemBoxHitbox.right  = 8;
    SignPost->itemBoxHitbox.bottom = 24;
    SignPost->maxPlayerCount       = (1 << Player->playerCount) - 1;

    DEBUGMODE_ADD_OBJ(SignPost);
    SignPost->sfx_SignPost     = RSDK.GetSFX("Global/SignPost.wav");
    SignPost->sfx_SignPost2P   = RSDK.GetSFX("Global/SignPost2p.wav");
    SignPost->sfx_Twinkle      = RSDK.GetSFX("Global/Twinkle.wav");
    SignPost->sfx_BubbleBounce = RSDK.GetSFX("Global/BubbleBounce.wav");
    SignPost->sfx_Slide        = RSDK.GetSFX("Global/Slide.wav");
}

void SignPost_DebugSpawn(void)
{
    RSDK_THIS(SignPost);
    EntitySignPost *signpost = CREATE_ENTITY(SignPost, NULL, entity->position.x, entity->position.y);
    signpost->debugObj       = true;
}
void SignPost_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_POST, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
    RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_EGGMAN, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
    RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_POST, &DebugMode->animator, true, 2);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}
void SignPost_SpinSpeed(void)
{
    RSDK_THIS(SignPost);
    entity->angle += entity->spinSpeed;

    if (entity->angle >= entity->maxAngle) {
        entity->maxAngle += 0x20000;
        int32 speed = 0x600 * (entity->spinCount);
        if (speed > 0x3000)
            speed = 0x3000;
        entity->spinSpeed = speed;
        --entity->spinCount;
        if (!entity->spinCount) {
            entity->spinSpeed                    = 0;
            entity->angle                        = 0x10000;
            entity->facePlateAnim.animationSpeed = 1;
        }
    }
    entity->rotation = (entity->angle >> 8) & 0x1FF;
}
void SignPost_SpawnSparkle(void)
{
    RSDK_THIS(SignPost);
    if (!(Zone->timer & 3)) {
        EntityRing *ring = (EntityRing *)RSDK.CreateEntity(Ring->objectID, 0, entity->position.x + RSDK.Rand(-0x180000, 0x180000),
                                                           entity->position.y + RSDK.Rand(-0x200000, 0x80000));
        ring->state      = Ring_State_Sparkle;
        ring->stateDraw  = Ring_StateDraw_Sparkle;
        ring->active     = ACTIVE_NORMAL;
        ring->visible    = false;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, entity->sparkleType + 2, &ring->animator, true, 0);
        int32 cnt = ring->animator.frameCount;
        if (ring->animator.animationID == 2) {
            ring->alpha = 224;
            cnt >>= 1;
        }
        ring->maxFrameCount           = cnt - 1;
        ring->animator.animationSpeed = 6;
        entity->sparkleType           = (entity->sparkleType + 1) % 3;
    }
}
void SignPost_State_SetupCompetition(void)
{
    RSDK_THIS(SignPost);
    entity->state = SignPost_State_Competition;
}
void SignPost_State_Competition(void)
{
    SignPost_HandleCompetition();
    SignPost_CheckTouch();
}
void SignPost_State_Land(void)
{
    RSDK_THIS(SignPost);
    SignPost_SpinSpeed();
    SignPost_SpawnSparkle();
    RSDK.ProcessAnimation(&entity->facePlateAnim);
    if (!entity->spinCount) {
        entity->type  = 3;
        entity->state = SignPost_State_Finish;
        Music_PlayTrack(TRACK_ACTCLEAR);
        RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->objectID, NULL);
    }
}
void SignPost_State_CompetitionFinish(void)
{
    RSDK_THIS(SignPost);
    SignPost_SpinSpeed();
    SignPost_SpawnSparkle();
    RSDK.ProcessAnimation(&entity->facePlateAnim);
    SignPost_CheckTouch();

    if (!entity->spinCount) {
        if (entity->activePlayers >= SignPost->maxPlayerCount) {
            entity->type                = 3;
            entity->state               = SignPost_State_Finish;
            RSDK_sceneInfo->timeEnabled = false;
            EntityZone *zone            = (EntityZone *)RSDK.GetEntityByID(SLOT_ZONE);
            zone->screenID              = 4;
            zone->timer                 = 0;
            zone->fadeSpeed             = 10;
            zone->fadeColour            = 0;
            zone->state                 = Zone_State_Fadeout_Unknown;
            zone->stateDraw             = Zone_StateDraw_Fadeout;
            zone->visible               = 1;
            zone->drawOrder             = 15;
        }
        else {
            entity->spinSpeed = 0x3000;
            entity->spinCount = 8;
            entity->maxAngle  = 0x10000;
            entity->state     = SignPost_State_Competition;
        }
    }
}
void SignPost_State_Launched(void)
{
    RSDK_THIS(SignPost);
    SignPost_SpinSpeed();
    SignPost_SpawnSparkle();
    RSDK.ProcessAnimation(&entity->facePlateAnim);
    entity->spinCount = 16;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->gravityStrength;
    if (entity->velocity.y < 0)
        entity->state = SignPost_State_Fall;
}
void SignPost_State_Fall(void)
{
    RSDK_THIS(SignPost);
    entity->active              = ACTIVE_NORMAL;
    RSDK_sceneInfo->timeEnabled = false;
    if (entity->type == 1) {
        entity->type = 0;
        if (globals->gameMode < MODE_COMPETITION) {
            switch (globals->playerID & 0xFF) {
                case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_TAILS, &entity->facePlateAnim, true, 0); break;
                case ID_KNUCKLES: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_KNUX, &entity->facePlateAnim, true, 0); break;
#if RETRO_USE_PLUS
                case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_MIGHTY, &entity->facePlateAnim, true, 0); break;
                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_RAY, &entity->facePlateAnim, true, 0); break;
#endif
                default: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_SONIC, &entity->facePlateAnim, true, 0); break;
            }
        }
    }

    foreach_active(Player, player) {
        if (entity->velocity.y >= 0) {
            if (player->velocity.y < 0 && player->playerAnimator.animationID == ANI_JUMP && !player->onGround) {
                if (Player_CheckCollisionTouch(player, entity, &SignPost->hitbox)) {
                    entity->velocity.x = (entity->position.x - player->position.x) >> 4;
                    entity->velocity.y = -0x20000;
                    RSDK.PlaySfx(SignPost->sfx_Twinkle, 0, 255);
                    EntityScoreBonus *scoreBonus = CREATE_ENTITY(ScoreBonus, NULL, entity->position.x, entity->position.y);
                    scoreBonus->drawOrder    = Zone->drawOrderHigh;
                    scoreBonus->animator.frameID = 0;
                    Player_GiveScore(player, 100);
                }
            }
        }
    }

    SignPost_SpinSpeed();
    SignPost_SpawnSparkle();
    RSDK.ProcessAnimation(&entity->facePlateAnim);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->spinCount = 16;

    if (entity->velocity.x >= 0) {
        if (entity->position.x > (RSDK_screens->position.x + RSDK_screens->width - 32) << 16) {
            entity->velocity.x = -entity->velocity.x;
        }
        else if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x180000, 0, true)) {
            entity->velocity.x = -entity->velocity.x;
        }
    }
    else {
        if (entity->position.x < (RSDK_screens->position.x + 32) << 16) {
            entity->velocity.x = -entity->velocity.x;
        }
        else if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x180000, 0, true)) {
            entity->velocity.x = -entity->velocity.x;
        }
    }

    entity->velocity.y += 0xC00;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x180000, true)) {
        foreach_active(ItemBox, itemBox) {
            if (itemBox->hidden) {
                if (RSDK.CheckObjectCollisionTouchBox(itemBox, &ItemBox->hiddenHitbox, entity, &SignPost->itemBoxHitbox)) {
                    RSDK.PlaySfx(SignPost->sfx_BubbleBounce, 0, 255);
                    itemBox->velocity.y = -0x50000;
                    itemBox->hidden     = 0;
                    itemBox->state      = ItemBox_State_Falling;
                    entity->itemBounceCount++;
                    entity->velocity.y = -0x20000;
#if RETRO_USE_PLUS
                    if (entity->itemBounceCount == 2)
                        API.UnlockAchievement("ACH_SIGNPOST");
#else
                    if (entity->itemBounceCount == 2)
                        APICallback_UnlockAchievement("ACH_SIGNPOST");
#endif
                }
            }
        }
        if (entity->velocity.y >= 0) {
            RSDK.PlaySfx(SignPost->sfx_Slide, 0, 255);
            entity->spinCount  = 4;
            entity->velocity.y = 0;
            Music_FadeOut(0.025);
            entity->state = SignPost_State_Land;
        }
    }
}
void SignPost_State_Finish(void)
{
    RSDK_THIS(SignPost);
    RSDK.ProcessAnimation(&entity->facePlateAnim);
    if (entity->debugObj) {
        Zone->stageFinishCallback  = NULL;
        globals->atlEnabled        = false;
        globals->enableIntro       = false;
        globals->suppressTitlecard = false;
        globals->suppressAutoMusic = false;
    }
}
void SignPost_CheckTouch(void)
{
    RSDK_THIS(SignPost);

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (entity->activePlayers && RSDK_GET_ENTITY(p + Player->playerCount, Player)->objectID == GameOver->objectID) {
            entity->activePlayers |= 1 << p;
        }
        else if (!p || globals->gameMode == MODE_COMPETITION) {
            if (!((1 << p) & entity->activePlayers)) {
                bool32 flag = false;
                if (globals->gameMode != MODE_COMPETITION) {
                    flag = player->position.x > entity->position.x;
                }
                else if (entity->playerPosStore[p].x && entity->playerPosStore[p].y) {
                    flag = MathHelpers_Unknown12(player->position.x, player->position.y, entity->playerPosStore[p].x, entity->playerPosStore[p].y,
                                                 entity->position.x, entity->position.y - (entity->vsExtendTop << 16), entity->position.x,
                                                 (entity->vsExtendBottom << 16) + entity->position.y);
                }

                if (flag) {
                    if (!((1 << p) & entity->activePlayers) && globals->gameMode == MODE_COMPETITION)
                        Announcer_AnnounceGoal(player->camera->screenID);
                    RSDK.PlaySfx(SignPost->sfx_SignPost, 0, 255);
                    entity->active = ACTIVE_NORMAL;
                    if (player->superState == SUPERSTATE_SUPER)
                        player->superState = SUPERSTATE_FADEOUT;

                    int32 vel = 0;
                    if (player->onGround)
                        vel = player->groundVel;
                    else
                        vel = player->velocity.x;

                    entity->velocity.y      = -(vel >> 1);
                    entity->gravityStrength = vel / 96;
                    if (globals->gameMode == MODE_COMPETITION) {
                        entity->active = ACTIVE_NORMAL;
                        if (!entity->activePlayers) {
                            switch (player->characterID) {
                                case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_TAILS, &entity->facePlateAnim, true, 0); break;
                                case ID_KNUCKLES:
                                    RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_KNUX, &entity->facePlateAnim, true, 0);
                                    break;
#if RETRO_USE_PLUS
                                case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_MIGHTY, &entity->facePlateAnim, true, 0); break;
                                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_RAY, &entity->facePlateAnim, true, 0); break;
#endif
                                default: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_SONIC, &entity->facePlateAnim, true, 0); break;
                            }
                            RSDK.PlaySfx(SignPost->sfx_SignPost2P, 0, 255);
                        }

                        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
#if RETRO_USE_PLUS
                        EntityCompetition *manager = (EntityCompetition *)Competition->activeEntity;
                        if (!manager) {
                            Competition->activeEntity = (Entity *)CREATE_ENTITY(Competition, NULL, entity->position.x, entity->position.y);
                        }
                        manager->playerFlags[player->playerID] = true;
#endif
                        session->rings[player->playerID]             = player->rings;
                        session->time[player->playerID].minutes      = RSDK_sceneInfo->minutes;
                        session->time[player->playerID].seconds      = RSDK_sceneInfo->seconds;
                        session->time[player->playerID].milliseconds = RSDK_sceneInfo->milliseconds;
                        session->score[player->playerID]             = player->score;
                        session->lives[player->playerID]             = player->lives;
#if RETRO_USE_PLUS
                        Competition_CalculateScore(player->playerID, 2);
#else
                        CompetitionSession_DeriveWinner(player->playerID, 2);
#endif

                        entity->activePlayers |= (1 << p);
                        if (entity->activePlayers == SignPost->maxPlayerCount)
                            Music_FadeOut(0.025);
                        entity->state = SignPost_State_CompetitionFinish;
                    }
                    else {
#if RETRO_USE_PLUS
                        if (globals->gameMode == MODE_ENCORE) {
                            switch (globals->playerID & 0xFF) {
                                case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_TAILS, &entity->facePlateAnim, true, 0); break;
                                case ID_KNUCKLES:
                                    RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_KNUX, &entity->facePlateAnim, true, 0);
                                    break;
                                case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_MIGHTY, &entity->facePlateAnim, true, 0); break;
                                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_RAY, &entity->facePlateAnim, true, 0); break;
                                default: RSDK.SetSpriteAnimation(SignPost->spriteIndex, SIGNPOSTANI_SONIC, &entity->facePlateAnim, true, 0); break;
                            }
                        }
#endif

                        RSDK_sceneInfo->timeEnabled = false;
                        if (vel >= 0x40000) {
                            entity->state = SignPost_State_Launched;
                        }
                        else {
                            Music_FadeOut(0.025);
                            entity->state = SignPost_State_Land;
                        }
                    }
                }
                entity->playerPosStore[p].x = player->position.x;
                entity->playerPosStore[p].y = player->position.y;
            }
        }
    }
}
void SignPost_HandleCompetition(void)
{
    RSDK_THIS(SignPost);
    int32 x = entity->vsBoundsOffset.x + entity->position.x;
    int32 y = entity->vsBoundsOffset.y + entity->position.y;

    Hitbox hitbox;
    hitbox.left   = -entity->vsBoundsSize.x >> 17;
    hitbox.top    = -entity->vsBoundsSize.y >> 17;
    hitbox.right  = entity->vsBoundsSize.x >> 17;
    hitbox.bottom = entity->vsBoundsSize.y >> 17;

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (player->objectID == Player->objectID && !player->sidekick) {
            if (globals->gameMode == MODE_COMPETITION) {
                int32 ex             = entity->position.x;
                int32 ey             = entity->position.y;
                entity->position.x = x;
                entity->position.y = y;
                if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                    entity->position.x      = ex;
                    entity->position.y      = ey;
                    Zone->screenBoundsL1[p] = (entity->position.x >> 0x10) - RSDK_screens[p].centerX;
                    Zone->screenBoundsR1[p] = RSDK_screens[p].centerX + (entity->position.x >> 0x10);
                    if (globals->gameMode == MODE_COMPETITION)
                        Zone->playerBoundActiveR[p] = true;
                }
                else {
                    entity->position.x = ex;
                    entity->position.y = ey;
                }
            }
            else {
                if (entity->position.x - player->position.x < 0x1000000 || entity->position.x - (Zone->screenBoundsR1[p] << 16) < 0x1000000) {
                    Zone->screenBoundsL1[p] = (entity->position.x >> 0x10) - RSDK_screens[p].centerX;
                    Zone->screenBoundsR1[p] = RSDK_screens[p].centerX + (entity->position.x >> 0x10);
                    if (globals->gameMode == MODE_COMPETITION)
                        Zone->playerBoundActiveR[p] = true;
                }
            }
        }
    }
}

void SignPost_EditorDraw(void) {}

void SignPost_EditorLoad(void) {}

void SignPost_Serialize(void)
{
    RSDK_EDITABLE_VAR(SignPost, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SignPost, VAR_VECTOR2, vsBoundsSize);
    RSDK_EDITABLE_VAR(SignPost, VAR_VECTOR2, vsBoundsOffset);
    RSDK_EDITABLE_VAR(SignPost, VAR_ENUM, vsExtendTop);
    RSDK_EDITABLE_VAR(SignPost, VAR_ENUM, vsExtendBottom);
}

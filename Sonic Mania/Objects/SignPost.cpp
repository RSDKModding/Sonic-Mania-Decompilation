#include "../SonicMania.hpp"

ObjectSignPost *SignPost;

void SignPost_Update()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void SignPost_LateUpdate()
{

}

void SignPost_StaticUpdate()
{

}

void SignPost_Draw()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    Vector2 drawPos;
    if (entity->state) {
        entity->drawFX = FX_SCALE;
        drawPos.y           = entity->position.y;

        EntityAnimationData *face = &entity->eggPlateAnim;
        if (entity->rotation <= 128 || entity->rotation >= 384)
            face = &entity->facePlateAnim;

        if (RSDK.Cos512(entity->rotation) >= 0)
            entity->scale.x = RSDK.Cos512(entity->rotation);
        else
            entity->scale.x = -RSDK.Cos512(entity->rotation);

        int angle = 0;
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
        RSDK.DrawSprite(&entity->sidebarData, &drawPos, 0);
        entity->drawFX = FX_NONE;
        RSDK.DrawSprite(&entity->postTopData, 0, 0);
        RSDK.DrawSprite(&entity->standData, 0, 0);
    }
}

void SignPost_Create(void* data)
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;

    if (!entity->vsBoundsSize.x)
        entity->vsBoundsSize.x = 0x2000000;
    if (!entity->vsBoundsSize.y)
        entity->vsBoundsSize.y = 0xF00000;
    if (!entity->vsExtendTop)
        entity->vsExtendTop = 120;
    if (!entity->vsExtendBottom)
        entity->vsExtendBottom = 120;
    if (!RSDK_sceneInfo->inEditor) {
        if (options->gameMode != MODE_TIMEATTACK) {
            RSDK.SetSpriteAnimation(SignPost->spriteIndex, 5, &entity->eggPlateAnim, true, 0);
            switch (options->playerID & 0xFF) {
                case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 1, &entity->facePlateAnim, true, 0); break;
                case ID_KNUCKLES: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 2, &entity->facePlateAnim, true, 0); break;
                case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 3, &entity->facePlateAnim, true, 0); break;
                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 4, &entity->facePlateAnim, true, 0); break;
                default: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 0, &entity->facePlateAnim, true, 0); break;
            }
            RSDK.SetSpriteAnimation(SignPost->spriteIndex, 6, &entity->postTopData, true, 0);
            RSDK.SetSpriteAnimation(SignPost->spriteIndex, 6, &entity->sidebarData, true, 1);
            RSDK.SetSpriteAnimation(SignPost->spriteIndex, 6, &entity->standData, true, 2);
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            if (options->gameMode == MODE_COMPETITION) {
                int checkA = entity->vsBoundsOffset.y + (entity->vsBoundsSize.y >> 1);
                int checkB = entity->vsBoundsOffset.y - (entity->vsBoundsSize.y >> 1);

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
            entity->spinSpeed      = 0x3000;
            entity->spinCount      = 8;
            entity->maxAngle       = 0x10000;
            entity->scale.y   = 512;

            switch (entity->type) {
                default: RSDK.ResetEntityPtr(entity, 0, 0); break;
                case 0:
                    if (options->gameMode != MODE_COMPETITION) {
                        entity->active = ACTIVE_BOUNDS;
                        entity->state  = SignPost_State_SetupCompetition;
                    }
                    else {
                        RSDK.ResetEntityPtr(entity, 0, 0);
                    }
                    break;
                case 1:
                    if (options->gameMode != MODE_COMPETITION) {
                        entity->active = ACTIVE_XBOUNDS;
                        entity->state  = NULL;
                    }
                    else {
                        RSDK.ResetEntityPtr(entity, 0, 0);
                    }
                    break;
                case 2:
                    if (options->gameMode == MODE_COMPETITION) {
                        entity->active = ACTIVE_BOUNDS;
                        entity->state  = SignPost_State_SetupCompetition;
                    }
                    else {
                        RSDK.ResetEntityPtr(entity, 0, 0);
                    }
                    break;
                case 3:
                    entity->active = ACTIVE_BOUNDS;
                    entity->state  = SignPost_State_Finish;
                    break;
                case 4:
                    if (options->gameMode != MODE_COMPETITION) {
                        entity->active = ACTIVE_BOUNDS;
                        entity->state  = SignPost_State_SetupCompetition;
                    }
                    else {
                        RSDK.ResetEntityPtr(entity, 0, 0);
                    }
                    break;
            }
        }
    }
}

void SignPost_StageLoad()
{
    SignPost->spriteIndex          = RSDK.LoadAnimation("Global/SignPost.bin", SCOPE_STAGE);
    SignPost->hitbox.left          = -24;
    SignPost->hitbox.top           = -22;
    SignPost->hitbox.right         = 24;
    SignPost->hitbox.bottom        = 8;
    SignPost->itemBoxHitbox.left   = -8;
    SignPost->itemBoxHitbox.top    = 20;
    SignPost->itemBoxHitbox.right  = 8;
    SignPost->itemBoxHitbox.bottom = 24;
    SignPost->maxPlayerCount       = (1 << Player->playerCount) - 1;

    DebugMode_AddObject(SignPost->objectID, SignPost_DebugDraw, SignPost_DebugSpawn);
    SignPost->sfx_SignPost     = RSDK.GetSFX("Global/SignPost.wav");
    SignPost->sfx_SignPost2P   = RSDK.GetSFX("Global/SignPost2p.wav");
    SignPost->sfx_Twinkle      = RSDK.GetSFX("Global/Twinkle.wav");
    SignPost->sfx_BubbleBounce = RSDK.GetSFX("Global/BubbleBounce.wav");
    SignPost->sfx_Slide        = RSDK.GetSFX("Global/Slide.wav");
}

void SignPost_DebugSpawn()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK.CreateEntity(SignPost->objectID, 0, RSDK_sceneInfo->entity->position.x,
                                                RSDK_sceneInfo->entity->position.y);
    entity->debugObj = true;
}
void SignPost_DebugDraw()
{
    RSDK.SetSpriteAnimation(SignPost->spriteIndex, 6, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
    RSDK.SetSpriteAnimation(SignPost->spriteIndex, 5, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
    RSDK.SetSpriteAnimation(SignPost->spriteIndex, 6, &DebugMode->debugData, true, 2);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
}
void SignPost_SpinSpeed()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    entity->angle += entity->spinSpeed;

    if (entity->angle >= entity->maxAngle) {
        entity->maxAngle += 0x20000;
        int speed = 0x600 * (entity->spinCount);
        if (speed > 0x3000)
            speed = 0x3000;
        entity->spinSpeed = speed;
        --entity->spinCount;
        if (!entity->spinCount) {
            entity->spinSpeed                    = 0;
            entity->angle                   = 0x10000;
            entity->facePlateAnim.animationSpeed = 1;
        }
    }
    entity->rotation = (entity->angle >> 8) & 0x1FF;
}
void SignPost_SpawnSparkle()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    if (!(Zone->timer & 3)) {
        EntityRing *ring   = (EntityRing *)RSDK.CreateEntity(Ring->objectID, 0, entity->position.x + RSDK.Rand(-0x180000, 0x180000),
                                              entity->position.y + RSDK.Rand(-0x200000, 0x80000));
        ring->state        = Ring_State_Sparkle;
        ring->stateDraw    = Ring_StateDraw_Sparkle;
        ring->active  = ACTIVE_NORMAL;
        ring->visible = false;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, entity->sparkleType + 2, &ring->animData, true, 0);
        int cnt = ring->animData.frameCount;
        if (ring->animData.animationID == 2) {
            ring->alpha = 224;
            cnt >>= 1;
        }
        ring->maxFrameCount       = cnt - 1;
        ring->animData.animationSpeed = 6;
        entity->sparkleType       = (entity->sparkleType + 1) % 3;
    }
}
void SignPost_State_SetupCompetition()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    entity->state          = SignPost_State_Competition;
}
void SignPost_State_Competition()
{
    SignPost_HandleCompetition();
    SignPost_CheckTouch();
}
void SignPost_State_Land()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    SignPost_SpinSpeed();
    SignPost_SpawnSparkle();
    RSDK.ProcessAnimation(&entity->facePlateAnim);
    if (!entity->spinCount) {
        entity->type  = 3;
        entity->state = SignPost_State_Finish;
        Music_PlayTrack(TRACK_ACTCLEAR);
        RSDK.ResetEntitySlot(16, ActClear->objectID, 0);
    }
}
void SignPost_State_CompetitionFinish()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
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
            zone->fade1                 = 10;
            zone->fade2                 = 0;
            zone->state                 = Zone_Unknown15;
            zone->stateDraw             = Zone_Unknown12;
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
void SignPost_State_Launched()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    SignPost_SpinSpeed();
    SignPost_SpawnSparkle();
    RSDK.ProcessAnimation(&entity->facePlateAnim);
    entity->spinCount = 16;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->gravityStrength;
    if (entity->velocity.y < 0)
        entity->state = SignPost_State_Fall;
}
void SignPost_State_Fall()
{
    EntitySignPost *entity              = (EntitySignPost *)RSDK_sceneInfo->entity;
    entity->active = ACTIVE_NORMAL;
    RSDK_sceneInfo->timeEnabled = false;
    if (entity->type == 1) {
        entity->type = 0;
        if (options->gameMode < MODE_COMPETITION) {
            switch (options->playerID & 0xFF) {
                case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 1, &entity->facePlateAnim, true, 0); break;
                case ID_KNUCKLES: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 2, &entity->facePlateAnim, true, 0); break;
                case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 3, &entity->facePlateAnim, true, 0); break;
                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 4, &entity->facePlateAnim, true, 0); break;
                default: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 0, &entity->facePlateAnim, true, 0); break;
            }
        }
    }
    
    EntityPlayer *player = NULL;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        if (entity->velocity.y >= 0) {
            if (player->velocity.y < 0 && player->playerAnimData.animationID == ANI_JUMP && !player->onGround) {
                if (Player_CheckCollisionTouch(player, entity, &SignPost->hitbox)) {
                    entity->velocity.x = (entity->position.x - player->position.x) >> 4;
                    entity->velocity.y = -131072;
                    RSDK.PlaySFX(SignPost->sfx_Twinkle, 0, 255);
                    EntityScoreBonus *scoreBonus =
                        (EntityScoreBonus *)RSDK.CreateEntity(ScoreBonus->objectID, 0, entity->position.x, entity->position.y);
                    scoreBonus->drawOrder    = Zone->drawOrderHigh;
                    scoreBonus->data.frameID = 0;
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
    RSDK.GetEntityByID(SLOT_CAMERA1);
    if (entity->velocity.x >= 0) {
        if (entity->position.x > (RSDK_screens->position.x + RSDK_screens->width - 32) << 16) {
            entity->velocity.x = -entity->velocity.x;
        }
        else if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 1, 0, 0x180000, 0, 1)) {
            entity->velocity.x = -entity->velocity.x;
        }
    }
    else {
        if (entity->position.x < (RSDK_screens->position.x + 32) << 16) {
            entity->velocity.x = -entity->velocity.x;
        }
        else if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 1, 0, 0x180000, 0, 1)) { entity->velocity.x = -entity->velocity.x; }
    }

    entity->velocity.y += 0xC00;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, 0, 0x180000, true)) {
        EntityItemBox *itemBox = NULL;
        while (RSDK.GetActiveEntities(ItemBox->objectID, (Entity **)&itemBox)) {
            if (itemBox->hidden) {
                if (RSDK.CheckObjectCollisionTouchBox(itemBox, &ItemBox->hiddenHitbox, entity, &SignPost->itemBoxHitbox)) {
                    RSDK.PlaySFX(SignPost->sfx_BubbleBounce, 0, 255);
                    itemBox->velocity.y      = -0x50000;
                    itemBox->hidden          = 0;
                    itemBox->state           = ItemBox_State_Falling;
                    entity->itemBounceCount++;
                    entity->velocity.y  = -0x20000;
                    if (entity->itemBounceCount == 2)
                        User.UnlockAchievement("ACH_SIGNPOST");
                }
            }
        }
        if (entity->velocity.y >= 0) {
            RSDK.PlaySFX(SignPost->sfx_Slide, 0, 255);
            entity->spinCount       = 4;
            entity->velocity.y = 0;
            Music_Unknown10(0.025);
            entity->state           = SignPost_State_Land;
        }
    }
}
void SignPost_State_Finish()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->facePlateAnim);
    if (entity->debugObj) {
        Zone->stageFinishCallback  = NULL;
        options->atlEnabled        = false;
        options->enableIntro       = false;
        options->suppressTitlecard = false;
        options->suppressAutoMusic = false;
    }
}
void SignPost_CheckTouch()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    int ty1                = RSDK_sceneInfo->entity->position.y - (entity->vsExtendTop << 16);
    int ty2                = (entity->vsExtendBottom << 16) + RSDK_sceneInfo->entity->position.y;
    int tx                 = entity->position.x;

    int p = 0;
    for (; p < Player->playerCount; ++p) {
        EntityPlayer *player = (EntityPlayer *)RSDK.GetEntityByID(p);
        if (!entity->activePlayers || RSDK.GetEntityByID(p + Player->playerCount)->objectID != GameOver->objectID) {
            if (!p || options->gameMode == MODE_COMPETITION) {
                if (!((1 << p) & entity->activePlayers)) {
                    bool32 flag = false;
                    if (options->gameMode != MODE_COMPETITION) {
                        flag = player->position.x > entity->position.x;
                    }
                    else if (entity->playerPosStore[p].x && entity->playerPosStore[p].y) {
                        flag = Game_Unknown20(player->position.x, player->position.y, entity->playerPosStore[p].x, entity->playerPosStore[p].y, tx,
                                              ty1, tx, ty2);
                        entity->activePlayers |= 1 << p;
                    }

                    
                    if (flag) {
                        // if (!((1 << p) & entity->activePlayers) && options->gameMode == MODE_COMPETITION)
                        //    Announcer_Unknown2(player->camera->screenID);
                        RSDK.PlaySFX(SignPost->sfx_SignPost, 0, 255);
                        entity->active = ACTIVE_NORMAL;
                        if (player->superState == 2)
                            player->superState = 3;

                        int vel = 0;
                        if (player->onGround)
                            vel = player->groundVel;
                        else
                            vel = player->velocity.x;

                        entity->velocity.y      = -(vel >> 1);
                        entity->gravityStrength = vel / 96;
                        if (options->gameMode == MODE_COMPETITION) {
                            entity->active = ACTIVE_NORMAL;
                            if (!entity->activePlayers) {
                                switch (player->characterID) {
                                    case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 1, &entity->facePlateAnim, true, 0); break;
                                    case ID_KNUCKLES: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 2, &entity->facePlateAnim, true, 0); break;
                                    case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 3, &entity->facePlateAnim, true, 0); break;
                                    case ID_RAY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 4, &entity->facePlateAnim, true, 0); break;
                                    default: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 0, &entity->facePlateAnim, true, 0); break;
                                }
                                RSDK.PlaySFX(SignPost->sfx_SignPost2P, 0, 255);
                            }

                            // if (!Competition->activeEntity) {
                            //    Competition->activeEntity = (EntityCompetition *)RSDK.CreateEntity(Competition->objectID, 0, entity->position.x,
                            //    entity->position.y);
                            //}
                            // v12->playerFlags[player->playerID]    = 1;
                            int pos                               = 3 * player->playerID;
                            options->competitionSession[pos + 47] = player->rings;
                            options->competitionSession[pos + 59] = RSDK_sceneInfo->minutes;
                            options->competitionSession[pos + 60] = RSDK_sceneInfo->seconds;
                            options->competitionSession[pos + 61] = RSDK_sceneInfo->milliseconds;
                            options->competitionSession[pos + 51] = player->score;
                            options->competitionSession[pos + 80] = player->lives;
                            // Competition_Unknown4(v13, 2);

                            entity->activePlayers = entity->activePlayers | (1 << p);
                            if (entity->activePlayers == SignPost->maxPlayerCount)
                                Music_Unknown10(0.025);
                            entity->state = SignPost_State_CompetitionFinish;
                        }
                        else {
                            if (options->gameMode == MODE_ENCORE) {
                                switch (options->playerID & 0xFF) {
                                    case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 1, &entity->facePlateAnim, true, 0); break;
                                    case ID_KNUCKLES: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 2, &entity->facePlateAnim, true, 0); break;
                                    case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 3, &entity->facePlateAnim, true, 0); break;
                                    case ID_RAY: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 4, &entity->facePlateAnim, true, 0); break;
                                    default: RSDK.SetSpriteAnimation(SignPost->spriteIndex, 0, &entity->facePlateAnim, true, 0); break;
                                }
                            }

                            RSDK_sceneInfo->timeEnabled = false;
                            if (vel >= 0x40000) {
                                entity->state = SignPost_State_Launched;
                            }
                            else {
                                Music_Unknown10(0.025);
                                entity->state = SignPost_State_Land;
                            }
                        }
                    }
                    entity->playerPosStore[p].x = player->position.x;
                    entity->playerPosStore[p].y = player->position.y;
                }
            }
        }
        else {
            entity->activePlayers |= 1 << p;
        }
    }
}
void SignPost_HandleCompetition()
{
    EntitySignPost *entity = (EntitySignPost *)RSDK_sceneInfo->entity;
    int x                  = entity->vsBoundsOffset.y + RSDK_sceneInfo->entity->position.y;
    int y                  = entity->vsBoundsOffset.x + RSDK_sceneInfo->entity->position.x;

    Hitbox hitbox;
    hitbox.left   = -entity->vsBoundsSize.x >> 17;
    hitbox.right  = entity->vsBoundsSize.x >> 17;
    hitbox.top    = -entity->vsBoundsSize.y >> 17;
    hitbox.bottom = entity->vsBoundsSize.y >> 17;

    for (int p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = (EntityPlayer *)RSDK.GetEntityByID(p);
        if (player->objectID == Player->objectID && !player->sidekick) {
            if (options->gameMode == MODE_COMPETITION) {
                int ex                 = entity->position.x;
                int ey = entity->position.y;
                entity->position.x = x;
                entity->position.y = y;
                if (!player->isGhost) {
                    Hitbox *otherHitbox = Player_GetHitbox(player);
                    bool32 flag         = RSDK.CheckObjectCollisionTouchBox(entity, &hitbox, player, otherHitbox);
                    entity->position.x  = ex;
                    entity->position.y  = ey;

                    if (flag) {
                        Zone->screenBoundsL1[p]  = (entity->position.x >> 0x10) - RSDK_screens[p].centerX;
                        Zone->screenBoundsR1[p] = RSDK_screens[p].centerX + (entity->position.x >> 0x10);
                        if (options->gameMode == MODE_COMPETITION)
                            Zone->screenUnknownC[p] = 1;
                    }
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
                    if (options->gameMode == MODE_COMPETITION)
                        Zone->screenUnknownC[p] = 1;
                }
            }
        }
    }
}

void SignPost_EditorDraw()
{

}

void SignPost_EditorLoad()
{

}

void SignPost_Serialize()
{
    RSDK_EDITABLE_VAR(SignPost, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SignPost, VAR_VECTOR2, vsBoundsSize);
    RSDK_EDITABLE_VAR(SignPost, VAR_VECTOR2, vsBoundsOffset);
    RSDK_EDITABLE_VAR(SignPost, VAR_ENUM, vsExtendTop);
    RSDK_EDITABLE_VAR(SignPost, VAR_ENUM, vsExtendBottom);
}


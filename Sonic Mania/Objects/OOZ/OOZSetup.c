#include "SonicMania.h"

ObjectOOZSetup *OOZSetup;

void OOZSetup_Update(void) {}

void OOZSetup_LateUpdate(void) {}

void OOZSetup_StaticUpdate(void)
{
    foreach_all(OOZSetup, setup) { RSDK.AddDrawListRef(Zone->playerDrawLow + 1, RSDK.GetEntityID(setup)); }

    OOZSetup->palTimer += 128;
    if (OOZSetup->palTimer > 255) {
        OOZSetup->palTimer -= 256;
        RSDK.RotatePalette(3, 216, 223, false);
        RSDK.RotatePalette(4, 216, 223, false);
        RSDK.RotatePalette(5, 216, 223, false);
        RSDK.RotatePalette(6, 216, 223, false);
    }

    RSDK.SetLimitedFade(1, 3, 4, OOZSetup->palTimer, 216, 223);
    RSDK.SetLimitedFade(2, 5, 6, OOZSetup->palTimer, 216, 223);
    RSDK.CopyPalette(1, 216, 0, 216, 8);

    if (!(Zone->timer & 3)) {
        ++OOZSetup->bgPtr->deformationOffset;
        OOZSetup->aniTilesTimer += 16;
        OOZSetup->aniTilesTimer %= 160;
        RSDK.DrawAniTiles(OOZSetup->aniTiles, 760, 0, OOZSetup->aniTilesTimer, 128, 16);
        RSDK.DrawAniTiles(OOZSetup->aniTiles, 757, 136, OOZSetup->aniTilesTimer, 16, 16);
        RSDK.DrawAniTiles(OOZSetup->aniTiles, 758, 128, OOZSetup->aniTilesTimer, 32, 16);
    }
    if (--OOZSetup->aniTilesDelay < 1) {
        ++OOZSetup->aniTilesFrame;
        OOZSetup->aniTilesFrame &= 7;
        OOZSetup->aniTilesDelay = OOZSetup->aniTileDelays[OOZSetup->aniTilesFrame];
        RSDK.DrawAniTiles(OOZSetup->aniTiles, 756, 16 * OOZSetup->aniTilesFrame, 160, 16, 16);
    }

    OOZSetup->value9 = 0;
    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (player->state != Player_State_None) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            ushort tile =
                RSDK.GetTileInfo(Zone->fgLow, player->position.x >> 20, ((playerHitbox->bottom << 16) + player->position.y - 0x10000) >> 20);
            if (tile == 0xFFFF)
                tile = RSDK.GetTileInfo(Zone->fgHigh, player->position.x >> 20, ((playerHitbox->bottom << 16) + player->position.y - 0x10000) >> 20);
            int behaviour = RSDK.GetTileBehaviour(tile, player->collisionPlane);

            if (behaviour) {
                if (player->shield == SHIELD_FIRE && player->superState != 2 && behaviour != 4) {
                    int tx = (player->position.x & 0xFFF00000) + 0x70000;
                    int ty = ((playerHitbox->bottom + 8) << 16) + player->position.y;
                    if (behaviour == 1) {
                        if (OOZSetup_Unknown6((ty & 0xFFF00000) - 0xC0000, tx, player->angle)) {
                            EntitySol *sol  = CREATE_ENTITY(Sol, intToVoid(1), tx - 0x10000, (ty & 0xFFF00000) - 0xC0000);
                            sol->velocity.x = -0x40000;
                            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->animator1, true, 0);
                            sol->state = Sol_Unknown12;

                            sol             = CREATE_ENTITY(Sol, intToVoid(1), tx + 0x10000, (ty & 0xFFF00000) - 0xC0000);
                            sol->velocity.x = 0x40000;
                            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->animator1, true, 0);
                            sol->state = Sol_Unknown12;
                        }
                    }
                    else if (player->onGround) {
                        if (OOZSetup_Unknown6(ty & 0xFFFF0000, tx, player->angle)) {
                            EntitySol *sol  = CREATE_ENTITY(Sol, intToVoid(1), tx - 0x10000, (ty & 0xFFFF0000) - 0x80000);
                            sol->velocity.x = -0x40000;
                            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->animator1, true, 0);
                            sol->state = Sol_Unknown11;

                            sol             = CREATE_ENTITY(Sol, intToVoid(1), tx + 0x10000, (ty & 0xFFFF0000) - 0x80000);
                            sol->velocity.x = 0x40000;
                            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->animator1, true, 0);
                            sol->state = Sol_Unknown11;
                        }
                    }
                }

                switch (behaviour) {
                    default: OOZSetup->activePlayers &= ~(1 << playerID); break;
                    case 1:
                        if (!player->sidekick)
                            OOZSetup->value9++;
                        OOZSetup->activePlayers &= ~(1 << playerID);
                        if (player->velocity.y < 0) {
                            player->velocity.y += 0x3800;
                        }
                        else {
                            player->interaction    = true;
                            player->tileCollisions = true;
                            player->state          = OOZSetup_PlayerState_OilPool;
                        }
                        break;
                    case 2:
                        if (player->state != Player_State_BubbleBounce
#if RETRO_USE_PLUS
                            && player->state != Player_State_MightyHammerDrop
#endif
                            ) {
                            OOZSetup->activePlayers &= ~(1 << playerID);
                            if (player->onGround == true) {
                                player->interaction    = true;
                                player->tileCollisions = true;
                                player->state          = OOZSetup_PlayerState_OilStrip;
                            }
                        }
                        break;
                    case 3:
                        if (player->state != Player_State_BubbleBounce
#if RETRO_USE_PLUS
                            && player->state != Player_State_MightyHammerDrop
#endif
                            ) {
                            OOZSetup->activePlayers &= ~(1 << playerID);
                            if (player->onGround) {
                                player->interaction    = true;
                                player->tileCollisions = true;
                                if (!player->angle)
                                    player->state = OOZSetup_PlayerState_OilStrip;
                                else
                                    player->state = OOZSetup_PlayerState_OilSlide;
                            }
                        }
                        break;
                    case 4:
                        if (!player->sidekick)
                            OOZSetup->value9++;
                        OOZSetup->activePlayers |= 1 << playerID;
                        player->interaction    = true;
                        player->tileCollisions = true;
                        if (player->velocity.y < 0)
                            player->velocity.y += 49152;
                        else
                            player->state = OOZSetup_PlayerState_OilFall;
                        break;
                }
            }
            else {
                OOZSetup->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (OOZSetup->flags && RSDK_sceneInfo->timeEnabled) {
        if (OOZSetup->fadeTimer < 0x10000)
            OOZSetup->fadeTimer++;
    }
    else {
        if (OOZSetup->fadeTimer > 0)
            OOZSetup->fadeTimer -= 32;
    }

#if RETRO_USE_PLUS
    foreach_active(Ring, ring)
    {
        if (ring->state == Ring_State_Bounce) {
            ushort tile = RSDK.GetTileInfo(Zone->fgLow, ring->position.x >> 20, (ring->position.y + 0xE0000) >> 20);
            if (tile == 0xFFFF)
                tile = RSDK.GetTileInfo(Zone->fgHigh, ring->position.x >> 20, (ring->position.y + 0xE0000) >> 20);
            if (RSDK.GetTileBehaviour(tile, ring->collisionPlane) == 1) {
                ring->velocity.x -= ring->velocity.x >> 4;
                ring->velocity.y = 0x2800;
                ring->drawOrder  = Zone->drawOrderHigh;
                if (ring->alpha > 64) {
                    ring->alpha     = 64;
                    ring->inkEffect = INK_ALPHA;
                }
            }
        }
    }
#endif
    OOZSetup_Unknown5();
#if RETRO_USE_PLUS
    RSDK.ProcessAnimation(&OOZSetup->animator);
#endif
}

void OOZSetup_Draw(void)
{
    RSDK_THIS(OOZSetup);
#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->currentDrawGroup != entity->drawOrder) {
        foreach_active(Player, player)
        {
            if ((1 << RSDK.GetEntityID(player)) & OOZSetup->activePlayers)
                RSDK.DrawSprite(&OOZSetup->animator, &player->position, false);
        }
    }
#endif
    if (entity->type)
        RSDK.FillScreen(0xC0C0E8, OOZSetup->fadeTimer >> 5, OOZSetup->fadeTimer >> 5, OOZSetup->fadeTimer >> 4);
    else
        OOZSetup_Unknown4();
}

void OOZSetup_Create(void *data)
{
    RSDK_THIS(OOZSetup);

    entity->active  = ACTIVE_ALWAYS;
    entity->visible = true;
    entity->drawFX  = FX_ROTATE;
    if (!entity->type)
        entity->drawOrder = Zone->drawOrderLow;
    else
        entity->drawOrder = 14;
}

void OOZSetup_StageLoad(void)
{
    OOZSetup->aniTiles = RSDK.LoadSpriteSheet("OOZ/AniTiles.gif", SCOPE_STAGE);

    OOZSetup->bgPtr = RSDK.GetSceneLayer(0);
    for (int i = 0; i < 0x400; ++i) {
        OOZSetup->bgPtr->deformationData[i] = OOZSetup->deformData[i & 0x3F];
    }
    OOZSetup->fadeTimer     = 0;
    OOZSetup->flags         = 1;
    OOZSetup->value9        = 0;
    Animals->animalTypes[0] = ANIMAL_ROCKY;
    Animals->animalTypes[1] = ANIMAL_PECKY;
    memset(OOZSetup->flameTimers, 0, sizeof(OOZSetup->flameTimers));
    memset(OOZSetup->flameTimerPtrs, 0, sizeof(OOZSetup->flameTimerPtrs));
    OOZSetup->flameCount = 0;
    OOZSetup->solFrames  = RSDK.LoadSpriteAnimation("OOZ/Sol.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(OOZSetup->solFrames, 3, &OOZSetup->flameAnimator, true, 0);
#if RETRO_USE_PLUS
    OOZSetup->splashFrames = RSDK.LoadSpriteAnimation("OOZ/Splash.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(OOZSetup->splashFrames, 0, &OOZSetup->animator, true, 0);
#endif
    if (RSDK.CheckStageFolder("OOZ2")) {
        foreach_all(OOZ1Outro, cutscene)
        {
            OOZSetup->cutscenePtr = (Entity *)cutscene;
            foreach_break;
        }

        GenericTrigger->callbacks[0] = OOZSetup_GenericTriggerCB;
        if (!StarPost->postIDs[0])
            Zone->screenBoundsB1[0] = 1600;

        if (isMainGameMode() && globals->enableIntro) {
            if (!PlayerHelpers_CheckStageReload()) {
                SaveGame_LoadPlayerState();
                globals->suppressAutoMusic = true;
                globals->suppressTitlecard = true;
            }
        }

#if RETRO_USE_PLUS
        if (isMainGameMode() && PlayerHelpers_CheckAct2())
            Zone->stageFinishCallback = OOZ2Outro_StageFinishCB_Act2;

        if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
            RSDK.LoadPalette(0, "EncoreOOZ2.act", 255);
            RSDK.LoadPalette(2, "EncoreOOZSmog.act", 255);
            RSDK.CopyPalette(0, 128, 1, 128, 128);
            RSDK.CopyPalette(0, 128, 3, 128, 128);
            RSDK.CopyPalette(0, 128, 4, 128, 128);
            RSDK.RotatePalette(4, 215, 223, false);
            RSDK.CopyPalette(2, 128, 5, 128, 128);
            RSDK.CopyPalette(2, 128, 6, 128, 128);
            RSDK.RotatePalette(6, 215, 223, false);
        }
#endif
    }
#if RETRO_USE_PLUS
    else if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreOOZ1.act", 255);
        RSDK.CopyPalette(0, 128, 1, 128, 80);
        RSDK.CopyPalette(0, 128, 3, 128, 80);
    }
#endif

    int id = Soundboard_LoadSFX("OOZ/Slide.wav", 12382, OOZSetup_CheckCB_Slide, NULL);
    if (id >= 0)
        Soundboard->sfxUnknown8[id] = 30;

    id = Soundboard_LoadSFX("OOZ/OilSwim.wav", true, OOZSetup_CheckCB_Swim, NULL);
    if (id >= 0)
        Soundboard->sfxUnknown8[id] = 30;

    id = Soundboard_LoadSFX("Stage/Flame2.wav", true, OOZSetup_CheckCB_Flame, NULL);
    if (id >= 0)
        Soundboard->sfxUnknown8[id] = 30;
}

bool32 OOZSetup_CheckCB_Flame(void)
{
    int count = 0;
    foreach_active(Sol, sol)
    {
        if (sol->isFlameFX)
            count++;
    }

    for (int i = 0; i < OOZSetup->flameCount; ++i) {
        if (OOZSetup->flameTimerPtrs[i])
            count++;
    }
    return count > 0;
}

bool32 OOZSetup_CheckCB_Slide(void)
{
    int count = 0;
    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            if (player->state == OOZSetup_PlayerState_OilSlide || player->state == OOZSetup_PlayerState_OilStrip)
                ++count;
        }
    }
    return count > 0;
}

bool32 OOZSetup_CheckCB_Swim(void) { return OOZSetup->value9 > 0; }

void OOZSetup_Unknown4(void)
{
    RSDK_THIS(OOZSetup);
    for (int i = 0; i < OOZSetup->flameCount; ++i) {
        if (OOZSetup->flameTimerPtrs[i]) {
            entity->rotation                = 2 * (OOZSetup->flamePositions[i].x & 0xFF);
            OOZSetup->flameAnimator.frameID = OOZSetup->flamePositions[i].y & 0xFF;
            RSDK.DrawSprite(&OOZSetup->flameAnimator, &OOZSetup->flamePositions[i], false);
        }
    }
}

void OOZSetup_Unknown5(void)
{
    RSDK_THIS(OOZSetup);
    for (int i = 0; i < OOZSetup->flameCount; ++i) {
        if (OOZSetup->flameTimerPtrs[i]) {
            --(*OOZSetup->flameTimerPtrs[i]);
            if (!*OOZSetup->flameTimerPtrs[i]) {
                OOZSetup->flameTimerPtrs[i] = NULL;
                EntitySol *sol              = CREATE_ENTITY(Sol, intToVoid(1), OOZSetup->flamePositions[i].x, OOZSetup->flamePositions[i].y);
                sol->isFlameFX              = true;
                sol->rotation               = 2 * (OOZSetup->flamePositions[i].x & 0xFF);
                RSDK.SetSpriteAnimation(Sol->aniFrames, 2, &sol->animator1, true, 0);
                sol->state = Sol_Unknown10;
            }
            else {
                int frame      = OOZSetup->flamePositions[i].y & 0xFF;
                int frameTimer = (OOZSetup->flamePositions[i].y >> 8) & 0xFF;
                if (frameTimer >= 3) {
                    frame++;
                    frameTimer = 0;
                    if (frame > 10)
                        frame = 1;
                }
                else {
                    ++frameTimer;
                }
                OOZSetup->flamePositions[i].y = frame | (frameTimer << 8) | (OOZSetup->flamePositions[i].y & 0xFFFF0000);
            }

            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, entity, &Sol->hitbox1)) {
                    if (player->shield != SHIELD_FIRE)
                        Player_CheckHit(player, entity);
                }
            }
        }
    }
}

bool32 OOZSetup_Unknown6(int posY, int posX, int angle)
{
    RSDK_THIS(OOZSetup);
    int pos = (posX >> 20) + (posY >> 20 << 10);
    if (pos <= 0x1FFFF) {
        if (!OOZSetup->flameTimers[pos]) {
            int i = 0;
            for (; i < 399; ++i) {
                if (!OOZSetup->flameTimerPtrs[i])
                    break;
            }

            OOZSetup->flameTimerPtrs[i]   = &OOZSetup->flameTimers[pos];
            OOZSetup->flamePositions[i].x = posX;
            OOZSetup->flamePositions[i].y = posY;
            OOZSetup->flamePositions[i].x &= 0xFFFF0000;
            OOZSetup->flamePositions[i].y &= 0xFFFF0000;
            OOZSetup->flamePositions[i].x |= angle;

            if (i + 1 > OOZSetup->flameCount) {
                OOZSetup->flameCount = i + 1;
            }
            OOZSetup->flameTimers[pos]                                                                          = 0xF0;
            CREATE_ENTITY(Explosion, intToVoid(2), entity->position.x, entity->position.y - 0x60000)->drawOrder = entity->drawOrder;
            return true;
        }
    }
    return false;
}

void OOZSetup_GenericTriggerCB(void)
{
    if (!OOZSetup->hasAchievement) {
#if RETRO_USE_PLUS
        API.UnlockAchievement("ACH_OOZ");
#else
        APICallback_UnlockAchievement("ACH_OOZ");
#endif
        OOZSetup->hasAchievement = true;
    }
}

void OOZSetup_PlayerState_OilPool(void)
{
    RSDK_THIS(Player);
    int top              = entity->topSpeed;
    int acc              = entity->acceleration;
    entity->topSpeed     = (entity->topSpeed >> 1) + (entity->topSpeed >> 3);
    entity->acceleration = (entity->acceleration >> 1) + (entity->acceleration >> 3);

    if (entity->groundVel <= 0) {
        if (entity->groundVel < -entity->topSpeed) {
            entity->groundVel += 0x3800;
        }
    }
    else if (entity->groundVel > entity->topSpeed) {
        entity->groundVel -= 0x3800;
    }

    entity->position.y += 0x10000;
    entity->up       = false;
    entity->down     = false;
    entity->onGround = true;
    Player_State_Ground();
    entity->topSpeed     = top;
    entity->acceleration = acc;
}

void OOZSetup_PlayerState_OilStrip(void)
{
    RSDK_THIS(Player);
    int acc  = entity->acceleration;
    int top  = entity->topSpeed;
    int skid = entity->skidSpeed;
    int dec  = entity->deceleration;

    Animator *animator = &entity->playerAnimator;
    entity->position.y += 0x10000;
    entity->skidSpeed    = entity->skidSpeed >> 3;
    entity->topSpeed     = (entity->topSpeed >> 1) + (entity->topSpeed >> 2);
    entity->acceleration = 0x400;
    entity->deceleration = entity->deceleration >> 3;
    entity->up           = false;
    entity->down         = false;
    entity->skidding     = 0;
    entity->onGround     = true;
    Player_State_Ground();

#if RETRO_USE_PLUS
    if ((animator->animationID == ANI_HURT || animator->animationID == ANI_FLUME) && (entity->groundedStore == true) & entity->onGround) {
#else
    if (animator->animationID == ANI_HURT && (entity->groundedStore == true) & entity->onGround) {
#endif
        if (abs(entity->groundVel) >= 0x20000) {
            memcpy(&entity->playerAnimator, animator, sizeof(Animator));
            if (entity->playerAnimator.animationTimer >= 3)
                entity->playerAnimator.animationTimer = 256;

            if (entity->angle == 64 || entity->angle == 192) {
                entity->onGround = false;
                entity->state    = Player_State_Air;
            }
        }
    }
    entity->acceleration = acc;
    entity->topSpeed     = top;
    entity->skidSpeed    = skid;
    entity->deceleration = dec;
}

void OOZSetup_PlayerState_OilSlide(void)
{
    RSDK_THIS(Player);
    if (!entity->onGround) {
        entity->state = Player_State_Air;
        Player_HandleAirMovement();
    }
    else {
        if (entity->camera)
            entity->camera->field_94 = 0;
        entity->jumpAbilityTimer = 0;
        if (entity->angle) {
            if (entity->angle <= 128) {
                if (entity->groundVel < 0x80000) {
                    entity->groundVel += 0x4000;
                }
            }
            else if (entity->groundVel > -0x80000) {
                entity->groundVel -= 0x4000;
            }
        }

        entity->groundVel += RSDK.Sin256(entity->angle) << 13 >> 8;
        entity->controlLock = 30;
        entity->direction   = (RSDK.Sin256(entity->angle) << 13 >> 8) + entity->groundVel < 0;
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLUME, &entity->playerAnimator, false, 0);
#else
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_HURT, &entity->playerAnimator, false, 2);
#endif
    }

    if (entity->jumpPress) {
        Player_StartJump(entity);
        if (entity->angle <= 128) {
            if (entity->velocity.x < 0)
                entity->velocity.x += ((entity->jumpStrength + (entity->jumpStrength >> 1)) * RSDK.Sin256(entity->angle)) >> 8;
        }
        else if (entity->velocity.x > 0) {
            entity->velocity.x += ((entity->jumpStrength + (entity->jumpStrength >> 1)) * RSDK.Sin256(entity->angle)) >> 8;
        }
    }
}

void OOZSetup_PlayerState_OilFall(void)
{
    RSDK_THIS(Player);
    int top              = entity->topSpeed;
    int acc              = entity->acceleration;
    entity->topSpeed     = (entity->topSpeed >> 2) + (entity->topSpeed >> 3);
    entity->acceleration = (entity->acceleration >> 2) + (entity->acceleration >> 3);

    if (entity->groundVel <= 0) {
        if (entity->groundVel < -entity->topSpeed)
            entity->groundVel += 0x3800;
    }
    else {
        if (entity->groundVel > entity->topSpeed)
            entity->groundVel -= 0x3800;
    }

    entity->up            = false;
    entity->down          = false;
    entity->groundedStore = false;
    entity->onGround      = false;
    entity->velocity.y    = 0x10000;
    if (abs(entity->velocity.x) > 0x20000) {
        if (entity->velocity.x <= 0)
            entity->velocity.x += 0xC000;
        else
            entity->velocity.x -= 0xC000;
    }
    if (!entity->left && !entity->right)
        entity->velocity.x -= entity->velocity.x >> 4;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
    Player_HandleGroundMovement();

    if (entity->camera)
        entity->camera->field_94 = 0;
    entity->jumpAbilityTimer = 0;
    entity->nextAirState     = Player_State_Air;
    if (entity->jumpPress) {
        Player_StartJump(entity);
        entity->jumpAbilityTimer = 0;
        entity->timer            = 0;
    }
    entity->topSpeed     = top;
    entity->acceleration = acc;
}

void OOZSetup_EditorDraw(void) {}

void OOZSetup_EditorLoad(void) {}

void OOZSetup_Serialize(void) { RSDK_EDITABLE_VAR(OOZSetup, VAR_UINT8, type); }

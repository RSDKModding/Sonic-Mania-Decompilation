#include "SonicMania.h"

ObjectPlayer *Player;

void Player_Update(void)
{
    RSDK_THIS(Player);
#if RETRO_USE_PLUS
    if (!API.CheckDLC(DLC_PLUS) && entity->characterID > ID_KNUCKLES)
        Player_ChangeCharacter(entity, ID_SONIC);
#endif

    StateMachine_Run(entity->stateInputReplay);
    StateMachine_Run(entity->stateInput);

    if (entity->objectID == Player->objectID) {
        if (entity->camera) {
            if (entity->scrollDelay > 0) {
                entity->scrollDelay--;
                if (!entity->scrollDelay)
                    entity->camera->state = Camera_State_Follow;
            }
            else if (entity->state != Player_State_LookUp && entity->state != Player_State_Crouch) {
                if (entity->camera->lookPos.y > 0) {
                    entity->camera->lookPos.y -= 2;
                }
                else if (entity->camera->lookPos.y < 0) {
                    entity->camera->lookPos.y += 2;
                }
            }
        }

        if (entity->invincibleTimer) {
            if (entity->invincibleTimer <= 0) {
                entity->invincibleTimer++;
            }
            else {
                entity->invincibleTimer--;
                if (!entity->invincibleTimer) {
                    Player_ApplyShieldEffect(entity);
#if RETRO_USE_PLUS
                    if (globals->gameMode != MODE_ENCORE || !entity->sidekick) {
#else
                    if (!entity->sidekick) {
#endif
                        bool32 flag = true;
                        foreach_active(Player, player)
                        {
                            if (player->invincibleTimer > 0)
                                flag = false;
                        }
                        if (flag)
                            Music_ResumePrevTrack(TRACK_INVINCIBLE, true);
                    }
                }
            }
        }

        if (entity->speedShoesTimer > 0) {
            entity->speedShoesTimer--;
            if (!entity->speedShoesTimer) {
                Player_ChangePhysicsState(entity);
                bool32 flag = true;
                foreach_active(Player, player)
                {
                    if (player->speedShoesTimer > 0)
                        flag = false;
                }
                if (flag)
                    Music_ResumePrevTrack(TRACK_SNEAKERS, true);
            }
        }

        if (entity->state != Player_State_Hit) {
            if (entity->blinkTimer > 0) {
                entity->blinkTimer--;
                entity->visible = !(entity->blinkTimer & 4);
            }
        }

#if RETRO_USE_PLUS
        if (entity->characterID == ID_RAY && entity->state != Player_State_RayGlide && !entity->isGhost) {
            Player->raySwoopTimer = 0;
            Player->rayDiveTimer  = 0;
        }

        if (entity->uncurlTimer > 0)
            entity->uncurlTimer--;
#endif

        Player_HandleSuperForm();
        if (entity->characterID == ID_TAILS && entity->state != Player_State_TailsFlight && entity->abilitySpeed)
            entity->abilitySpeed = 0;

        // Hurt Player if we're touching T/B or L/R sides at same time
        if (entity->collisionFlagH == 3 || entity->collisionFlagV == 3)
            entity->hurtFlag = HURTFLAG_HURT;

        entity->collisionFlagH = 0;
        entity->collisionFlagV = 0;
        if (entity->collisionLayers & Zone->moveID) {
            TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
            move->position.x = -entity->moveOffset.x >> 16;
            move->position.y = -entity->moveOffset.y >> 16;
        }

        if (entity->forceJumpIn)
            entity->state = Player_State_StartJumpIn;
        if (entity->forceTransform)
            entity->state = Player_State_Transform;

        StateMachine_Run(entity->state);

        if (entity->objectID == Player->objectID) {
            entity->flailing      = false;
            entity->invertGravity = false;
            if (entity->outerbox) {
                entity->groundedStore = entity->onGround;
                RSDK.ProcessTileCollisions(entity, entity->outerbox, entity->innerbox);
            }
            else {
                entity->outerbox      = RSDK.GetHitbox(&entity->playerAnimator, 0);
                entity->innerbox      = RSDK.GetHitbox(&entity->playerAnimator, 1);
                entity->groundedStore = entity->onGround;
                RSDK.ProcessTileCollisions(entity, entity->outerbox, entity->innerbox);
                entity->outerbox = NULL;
            }
            entity->collisionLayers &= ~Zone->moveID;
            if (entity->onGround && !entity->collisionMode)
                entity->collisionFlagV |= 1;
        }
    }
}

void Player_LateUpdate(void)
{
    RSDK_THIS(Player);
    if (entity->superState == SUPERSTATE_FADEIN && entity->state != Player_State_Transform)
        Player_CheckGoSuper(entity, 0x7F);
    if (entity->state == Player_State_FlyCarried) {
        entity->leaderPos.x = entity->position.x >> 0x10 << 0x10;
        entity->leaderPos.y = entity->position.y >> 0x10 << 0x10;
    }

    if (entity->hurtFlag) {
        entity->abilityValues[0] = 0;
#if RETRO_USE_PLUS
        if (!entity->sidekick)
            RSDK.CopyEntity(Zone->entityData[1], entity, false);
#endif

        bool32 flag = entity->sidekick || globals->gameMode == MODE_COMPETITION;
#if RETRO_USE_PLUS
        flag |= globals->gameMode == MODE_ENCORE;
#endif

        if (flag) {
            if (entity->invincibleTimer > 1)
                entity->invincibleTimer = 1;
            if (entity->speedShoesTimer > 1)
                entity->speedShoesTimer = 1;
        }
        else {
            entity->invincibleTimer = 0;
            entity->speedShoesTimer = 0;
        }
        if (entity->gravityStrength <= 1)
            entity->direction |= FLIP_Y;
        else
            entity->direction &= ~FLIP_Y;
        entity->visible         = true;
        entity->onGround        = false;
        entity->groundVel       = 0;
        entity->velocity.x      = 0;
        entity->nextGroundState = StateMachine_None;
        entity->nextAirState    = StateMachine_None;
        entity->interaction     = false;
        entity->tileCollisions  = false;
        if (globals->gameMode != MODE_COMPETITION)
            entity->active = ACTIVE_ALWAYS;
        entity->shield         = SHIELD_NONE;
        entity->collisionFlagH = 0;
        entity->collisionFlagV = 0;
        entity->underwater     = false;
        Player_ChangePhysicsState(entity);
        destroyEntity(RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(entity), Shield));

        switch (entity->hurtFlag) {
            default: break;
            case HURTFLAG_HURT: RSDK.PlaySfx(Player->sfx_Hurt, false, 255);
            case HURTFLAG_DIE:
                entity->hurtFlag   = HURTFLAG_NONE;
                entity->velocity.y = -0x68000;
                entity->state      = Player_State_Die;
                if (!(entity->drawFX & FX_SCALE) || entity->scale.x == 0x200)
                    entity->drawOrder = Zone->playerDrawHigh;
                if (entity->sidekick || globals->gameMode == MODE_COMPETITION) {
                    if (entity->camera) {
                        entity->scrollDelay   = 2;
                        entity->camera->state = StateMachine_None;
                    }
                }
#if RETRO_USE_PLUS
                else if (globals->gameMode == MODE_ENCORE) {
                    EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                    if (!globals->stock && !sidekick->objectID) {
                        RSDK.SetGameMode(ENGINESTATE_FROZEN);
                        RSDK_sceneInfo->timeEnabled = false;
                    }

                    if (entity->camera) {
                        entity->scrollDelay   = 2;
                        entity->camera->state = StateMachine_None;
                    }
                }
#endif
                else {
                    RSDK.SetGameMode(ENGINESTATE_FROZEN);
                    RSDK_sceneInfo->timeEnabled = false;
                    if (entity->camera) {
                        entity->scrollDelay   = 2;
                        entity->camera->state = StateMachine_None;
                    }
                }
                break;
            case HURTFLAG_DROWN:
                entity->hurtFlag        = HURTFLAG_NONE;
                entity->gravityStrength = 0x1000;
                entity->velocity.y      = 0;
                RSDK.PlaySfx(Water->sfx_Drown, false, 255);
                entity->state = Player_State_Drown;
                if (!entity->sidekick) {
                    if (globals->gameMode == MODE_COMPETITION) {
                        Music_ResumePrevTrack(TRACK_DROWNING, false);
                    }
#if RETRO_USE_PLUS
                    else if (globals->gameMode != MODE_ENCORE) {
#else
                    else {
#endif
                        RSDK_sceneInfo->timeEnabled = false;

                        if (entity->camera) {
                            entity->scrollDelay   = 2;
                            entity->camera->state = StateMachine_None;
                        }
                    }
#if RETRO_USE_PLUS
                    else {
                        EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                        if (globals->stock == 0 && !sidekick->objectID) {
                            RSDK_sceneInfo->timeEnabled = false;
                        }

                        if (entity->camera) {
                            entity->scrollDelay   = 2;
                            entity->camera->state = StateMachine_None;
                        }
                    }
#endif
                }
                if (entity->camera) {
                    entity->scrollDelay   = 2;
                    entity->camera->state = StateMachine_None;
                }
                break;
        }
    }

    if (entity->onGround) {
        if (entity->nextGroundState) {
            entity->state           = entity->nextGroundState;
            entity->nextGroundState = StateMachine_None;
            if (entity->sidekick) {
                EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                player->scoreBonus   = 0;
            }
            else {
                EntityPlayer *player = entity;
                player->scoreBonus   = 0;
            }
        }

        if (entity->camera) {
            if (entity->playerAnimator.animationID == ANI_JUMP)
                entity->camera->adjustY = entity->cameraOffset;
            else
                entity->camera->adjustY = 0;
        }
    }
    else {
        if (entity->nextAirState) {
            entity->state        = entity->nextAirState;
            entity->nextAirState = StateMachine_None;
        }
    }

    if (entity->tailSpriteIndex != 0xFFFF) {
        switch (entity->playerAnimator.animationID) {
            case ANI_IDLE:
            case ANI_BORED1:
            case ANI_LOOKUP:
            case ANI_CROUCH:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 0, &entity->tailAnimator, false, 0);
                entity->tailDirection = entity->direction;
                entity->tailRotation  = entity->rotation;
                break;
            case ANI_JUMP:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 1, &entity->tailAnimator, false, 0);
                if (entity->onGround) {
                    entity->tailRotation  = entity->rotation;
                    entity->tailDirection = entity->groundVel <= 0;
                }
                else {
                    entity->tailRotation = RSDK.ATan2(entity->velocity.x, entity->velocity.y) << 1;
                    if (entity->direction == FLIP_X)
                        entity->tailRotation = (entity->tailRotation - 256) & 0x1FF;
                    entity->tailDirection = entity->direction;
                }
                break;
            case ANI_SKID:
            case ANI_PUSH:
            case ANI_HANG:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 3, &entity->tailAnimator, false, 0);
                entity->tailDirection = entity->direction;
                break;
            case ANI_SPINDASH:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 2, &entity->tailAnimator, false, 0);
                entity->tailDirection = entity->direction;
                break;
            case ANI_HANGMOVE:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 4, &entity->tailAnimator, false, 0);
                entity->tailDirection = entity->direction;
                break;
            case ANI_PULLEYHOLD:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 5, &entity->tailAnimator, false, 0);
                entity->tailDirection = entity->direction;
                if (entity->tailDirection)
                    entity->tailRotation = entity->rotation + 32;
                else
                    entity->tailRotation = entity->rotation - 32;
                break;
            default: RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->tailAnimator, false, 0); break;
        }
        RSDK.ProcessAnimation(&entity->tailAnimator);
    }
    RSDK.ProcessAnimation(&entity->playerAnimator);
}

void Player_StaticUpdate(void)
{
#if RETRO_USE_PLUS
    if (Player->superDashCooldown > 0) {
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        foreach_all(HUD, hud)
        {
            if (hud)
                hud->enableRingFlash = true;
            foreach_break;
        }
        --Player->superDashCooldown;
    }
#endif

    bool32 flying = false;
    bool32 tired  = false;
    if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
        foreach_active(Player, player)
        {
            if (player->characterID == ID_TAILS) {
                int32 anim = player->playerAnimator.animationID;
                if (anim == ANI_FLY || anim == ANI_FLYLIFT)
                    flying = true;
                if (anim == ANI_FLYTIRED || anim == ANI_FLYLIFTTIRED)
                    tired = true;
            }
        }

        if (flying) {
            if (!Player->playingFlySFX) {
                RSDK.PlaySfx(Player->sfx_Flying, true, 255);
                Player->playingFlySFX = true;
            }
        }

        if (tired) {
            if (!Player->playingTiredSFX) {
                RSDK.PlaySfx(Player->sfx_Tired, true, 255);
                Player->playingTiredSFX = true;
            }
        }
    }

    if (!flying && Player->playingFlySFX) {
        RSDK.StopSFX(Player->sfx_Flying);
        Player->playingFlySFX = false;
    }

    if (!tired && Player->playingTiredSFX) {
        RSDK.StopSFX(Player->sfx_Tired);
        Player->playingTiredSFX = false;
    }

#if RETRO_USE_PLUS
    Player->raySwoopTimer -= 8;
    if (Player->raySwoopTimer < 0)
        Player->raySwoopTimer = 0;

    Player->rayDiveTimer -= 16;
    if (Player->rayDiveTimer < 0)
        Player->rayDiveTimer = 0;
#endif
}

void Player_Draw(void)
{
    RSDK_THIS(Player);
#if RETRO_USE_PLUS
    if (entity->isGhost) {
        entity->inkEffect = INK_BLEND;
        entity->alpha     = 0x7F;
    }
#endif
    Entity *parent   = entity->abilityPtrs[0];
    Vector2 posStore = entity->position;
    if (entity->state == Player_State_FlyIn && parent) {
        entity->position.x = parent->position.x;
        entity->position.y = parent->position.y;
    }
    if (entity->tailSpriteIndex != 0xFFFF) {
        int32 rotStore    = entity->rotation;
        int32 dirStore    = entity->direction;
        entity->rotation  = entity->tailRotation;
        entity->direction = entity->tailDirection;
        RSDK.DrawSprite(&entity->tailAnimator, NULL, false);
        entity->rotation  = rotStore;
        entity->direction = dirStore;
    }

    if (entity->playerID != 1 || (globals->playerID & 0xFF) != ((globals->playerID >> 8) & 0xFF) || globals->gameMode == MODE_TIMEATTACK) {
        RSDK.DrawSprite(&entity->playerAnimator, NULL, false);
    }
    else {
        uint32 colourStore[6];
        switch (entity->characterID) {
            case ID_SONIC:
                for (int32 c = 0; c < 6; ++c) {
                    colourStore[c] = RSDK.GetPaletteEntry(0, c + 0x40);
                    RSDK.SetPaletteEntry(0, c + 0x40, Player->superPalette_Sonic[c]);
                }

                RSDK.DrawSprite(&entity->playerAnimator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 0x40, colourStore[c]);
                break;
            case ID_TAILS:
                for (int32 c = 0; c < 6; ++c) {
                    colourStore[c] = RSDK.GetPaletteEntry(0, c + 0x46);
                    RSDK.SetPaletteEntry(0, c + 0x46, Player->superPalette_Tails[c]);
                }

                RSDK.DrawSprite(&entity->playerAnimator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 0x46, colourStore[c]);
                break;
            case ID_KNUCKLES:
                for (int32 c = 0; c < 6; ++c) {
                    colourStore[c] = RSDK.GetPaletteEntry(0, c + 0x50);
                    RSDK.SetPaletteEntry(0, c + 0x50, Player->superPalette_Knux[c]);
                }

                RSDK.DrawSprite(&entity->playerAnimator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 0x50, colourStore[c]);
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                for (int32 c = 0; c < 6; ++c) {
                    colourStore[c] = RSDK.GetPaletteEntry(0, c + 96);
                    RSDK.SetPaletteEntry(0, c + 96, Player->superPalette_Mighty[c]);
                }

                RSDK.DrawSprite(&entity->playerAnimator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 96, colourStore[c]);
                break;
            case ID_RAY:
                for (int32 c = 0; c < 6; ++c) {
                    colourStore[c] = RSDK.GetPaletteEntry(0, c + 113);
                    RSDK.SetPaletteEntry(0, c + 113, Player->superPalette_Ray[c]);
                }

                RSDK.DrawSprite(&entity->playerAnimator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 113, colourStore[c]);
                break;
#endif
        }
    }

    if (entity->state == Player_State_FlyIn) {
        if (parent) {
            entity->position.x = posStore.x;
            entity->position.y = posStore.y;
        }
    }
}

void Player_Create(void *data)
{
    RSDK_THIS(Player);
    if (RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Player->sonicSpriteIndex, ANI_IDLE, &entity->playerAnimator, true, 0);
        // entity->characterID = ID_SONIC;
    }
    else {
        entity->playerID = RSDK.GetEntityID(entity);
        switch (entity->characterID) {
            case ID_TAILS:
                entity->spriteIndex     = Player->tailsSpriteIndex;
                entity->tailSpriteIndex = Player->tailsTailsSpriteIndex;
                entity->cameraOffset    = 0;
                entity->movesetState    = Player_TailsJumpAbility;
                entity->sensorY         = 0x100000;
                break;
            case ID_KNUCKLES:
                entity->spriteIndex     = Player->knuxSpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                entity->movesetState    = Player_KnuxJumpAbility;
                entity->sensorY         = 0x140000;
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                entity->spriteIndex     = Player->mightySpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                entity->movesetState    = Player_MightyJumpAbility;
                entity->sensorY         = 0x140000;
                break;
            case ID_RAY:
                entity->spriteIndex     = Player->raySpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                entity->movesetState    = Player_RayJumpAbility;
                entity->sensorY         = 0x140000;
                break;
#endif
            default:
                entity->spriteIndex     = Player->sonicSpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                entity->movesetState    = Player_SonicJumpAbility;
                entity->sensorY         = 0x140000;
                if (globals->medalMods & getMod(MEDAL_PEELOUT)) {
                    entity->peeloutState = Player_StartPeelout;
                    for (int32 f = 0; f < 4; ++f) {
                        SpriteFrame *dst = RSDK.GetFrame(entity->spriteIndex, ANI_DASH, f);
                        SpriteFrame *src = RSDK.GetFrame(entity->spriteIndex, ANI_FLY, f);

                        *dst = *src;
                    }
                }
                break;
        }
        entity->sensorX[0]     = 0xA0000;
        entity->sensorX[1]     = 0x50000;
        entity->sensorX[2]     = 0;
        entity->sensorX[3]     = -0x50000;
        entity->sensorX[4]     = -0xA0000;
        entity->active         = ACTIVE_NORMAL;
        entity->tileCollisions = true;
        entity->visible        = true;
        entity->drawOrder      = Zone->playerDrawLow;
        entity->scale.x        = 0x200;
        entity->scale.y        = 0x200;
        entity->controllerID   = entity->playerID + 1;
        entity->state          = Player_State_Ground;

        if (!RSDK_sceneInfo->entitySlot || globals->gameMode == MODE_COMPETITION) {
            entity->stateInput = Player_ProcessP1Input;
        }
#if RETRO_USE_PLUS
        else if (RSDK_sceneInfo->entitySlot == 1 && globals->gameMode == MODE_TIMEATTACK) {
            StateMachine_Run(Player->configureGhostCB);
        }
#endif
        else {
#if RETRO_USE_PLUS
            RSDK.AssignControllerID(entity->controllerID, CONT_AUTOASSIGN);
#else
            APICallback_AssignControllerID(entity->controllerID, CONT_AUTOASSIGN);
#endif
            entity->stateInput = Player_ProcessP2Input_AI;
            entity->sidekick   = true;
        }

        RSDK_stickL[entity->controllerID].deadzone = 0.3;
        entity->rings                              = Player->rings;
        entity->ringExtraLife                      = Player->ringExtraLife;
        Player->rings                              = 0;
        Player->ringExtraLife                      = 100;
        entity->hyperRing                          = (Player->powerups >> 6) & 1;
        Player->powerups &= ~0x40;
        if (Player->powerups >= 0x80) {
            entity->state = Player_ForceSuperTransform;
            Player->powerups &= ~0x80;
        }
        if (Player->powerups) {
            entity->shield = Player->powerups;
            Player_ApplyShieldEffect(entity);
        }

        Player->powerups                  = 0;
        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        if (globals->gameMode == MODE_COMPETITION) {
            entity->lives    = session->lives[entity->playerID];
            entity->score    = 0;
            entity->score1UP = 50000;
        }
        else if (globals->gameMode == MODE_TIMEATTACK) {
            entity->lives    = 1;
            entity->score    = 0;
            entity->score1UP = 50000;
        }
        else {
            entity->lives    = Player->savedLives;
            entity->score    = Player->savedScore;
            entity->score1UP = Player->savedScore1UP;
        }

        int32 target = entity->score1UP;
        while (entity->score1UP <= target) entity->score1UP += 50000;

        entity->collisionLayers = Zone->fgLayers;
        entity->drawFX          = FX_ROTATE | FX_FLIP;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimator, true, 0);
        Player_ChangePhysicsState(entity);
        entity->maxWalkSpeed = 0x40000;
        entity->maxJogSpeed  = 0x60000;
        entity->maxRunSpeed  = 0xC0000;
    }
}

void Player_StageLoad(void)
{
    if (!globals->playerID)
        globals->playerID = RSDK.CheckStageFolder("MSZCutscene") ? ID_KNUCKLES : ID_DEFAULT_PLAYER;

    RSDK_sceneInfo->debugMode = globals->medalMods & getMod(MEDAL_DEBUGMODE);
    RSDK_sceneInfo->debugMode = true; // TEMP
#if RETRO_USE_PLUS
    RSDK.SetDebugValue("Debug Mode", &RSDK_sceneInfo->debugMode, DTYPE_BOOL, false, true);
#endif
    if (globals->medalMods & getMod(MEDAL_ANDKNUCKLES)) {
        globals->playerID &= 0xFF;
        globals->playerID |= ID_KNUCKLES_ASSIST;
    }
    Player->playerCount = 0;
    Player->active      = ACTIVE_ALWAYS;
    if (globals->gameMode == MODE_COMPETITION)
        Player_LoadSpritesVS();
    else
        Player_LoadSprites();
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE) {
        Player->playerCount    = 2;
        EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        sidekick->playerID     = 1;
    }
    else {
        Player->playerCount = RSDK.GetEntityCount(Player->objectID, 0);
    }
#else
    Player->playerCount = RSDK.GetEntityCount(Player->objectID, 0);
#endif
    Player->flyCarryPosA = 1;
    Player->flyCarryPosB = 0;
#if RETRO_USE_PLUS
    Player->disableP2KeyCheck = false;
#endif
    Player->upState         = false;
    Player->downState       = false;
    Player->leftState       = false;
    Player->rightState      = false;
    Player->jumpPressState  = false;
    Player->jumpHoldState   = false;

    //Sfx Loading
    Player->sfx_Jump        = RSDK.GetSFX("Global/Jump.wav");
    Player->sfx_LoseRings   = RSDK.GetSFX("Global/LoseRings.wav");
    Player->sfx_Hurt        = RSDK.GetSFX("Global/Hurt.wav");
    Player->sfx_Roll        = RSDK.GetSFX("Global/Roll.wav");
    Player->sfx_Charge      = RSDK.GetSFX("Global/Charge.wav");
    Player->sfx_Release     = RSDK.GetSFX("Global/Release.wav");
    Player->sfx_PeelCharge  = RSDK.GetSFX("Global/PeelCharge.wav");
    Player->sfx_PeelRelease = RSDK.GetSFX("Global/PeelRelease.wav");
    Player->sfx_Dropdash    = RSDK.GetSFX("Global/DropDash.wav");
    Player->sfx_Skidding    = RSDK.GetSFX("Global/Skidding.wav");
    Player->sfx_Grab        = RSDK.GetSFX("Global/Grab.wav");
    Player->sfx_Flying      = RSDK.GetSFX("Global/Flying.wav");
    Player->sfx_Tired       = RSDK.GetSFX("Global/Tired.wav");
    Player->sfx_Land        = RSDK.GetSFX("Global/Land.wav");
    Player->sfx_Slide       = RSDK.GetSFX("Global/Slide.wav");
    Player->sfx_Outtahere   = RSDK.GetSFX("Global/OuttaHere.wav");
    Player->sfx_Transform2  = RSDK.GetSFX("Stage/Transform2.wav");
#if RETRO_USE_PLUS
    Player->sfx_PimPom        = RSDK.GetSFX("Stage/PimPom.wav");
    Player->sfx_Swap          = RSDK.GetSFX("Global/Swap.wav");
    Player->sfx_SwapFail      = RSDK.GetSFX("Global/SwapFail.wav");
    Player->sfx_MightyDeflect = RSDK.GetSFX("Global/MightyDeflect.wav");
    Player->sfx_MightyDrill   = RSDK.GetSFX("Global/MightyDrill.wav");
    Player->sfx_MightyLand    = RSDK.GetSFX("Global/MightyLand.wav");
    Player->sfx_MightyUnspin  = RSDK.GetSFX("Global/MightyUnspin.wav");
#endif

#if RETRO_USE_PLUS
    Soundboard_LoadSFX("Global/RaySwoop.wav", 41417, Player_CheckRaySwooping, Player_UpdateRaySwoopSFX);
    Soundboard_LoadSFX("Global/RayDive.wav", 72323, Player_CheckRayDiving, Player_UpdateRayDiveSFX);
#endif

    for (int32 p = 0; p < PLAYER_MAX; ++p) Player->gotHit[p] = false;
}

// Extra Entity Functions
void Player_LoadSprites(void)
{
    foreach_all(Player, entity)
    {
        int32 pID = globals->playerID & 0xFF;
#if RETRO_USE_PLUS
        if (pID == ID_MIGHTY || pID == ID_RAY)
            pID = ID_SONIC;
#endif

        if (pID & entity->characterID) {
            entity->characterID = globals->playerID & 0xFF;
            switch (entity->characterID) {
                case ID_TAILS:
                    Player->tailsSpriteIndex      = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
                    Player->tailsTailsSpriteIndex = RSDK.LoadSpriteAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                    break;
                case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
                case ID_MIGHTY: Player->mightySpriteIndex = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
                case ID_RAY: Player->raySpriteIndex = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE); break;
#endif
                default:
                    Player->sonicSpriteIndex = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
                    Player->superSpriteIndex = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                    break;
            }
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            RSDK.CopyEntity(player1, entity, true);
            player1->camera = Camera_SetTargetEntity(0, (Entity *)player1);
            RSDK.AddCamera(&player1->position, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, true);
        }
        else {
            destroyEntity(entity);
        }
    }

    if (((globals->playerID >> 8) & 0xFF) > 0) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

        for (int32 i = 0; i < 0x10; ++i) {
            Player->flyCarryPositions[i] = player1->position;
        }
        player2->objectID   = Player->objectID;
        player2->position.x = player1->position.x;
        player2->position.y = player1->position.y;

        if (globals->gameMode != MODE_TIMEATTACK) {
            RSDK.AddCamera(&player2->position, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, true);
            player2->position.x -= 0x100000;
        }

        player2->characterID = globals->playerID >> 8;
        switch (player2->characterID) {
            case ID_TAILS:
                Player->tailsSpriteIndex      = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
                Player->tailsTailsSpriteIndex = RSDK.LoadSpriteAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                break;
            case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: Player->mightySpriteIndex = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
            case ID_RAY: Player->raySpriteIndex = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE); break;
#endif
            default:
                player2->characterID     = ID_SONIC;
                Player->sonicSpriteIndex = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
                Player->superSpriteIndex = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                break;
        }
    }
}
void Player_LoadSpritesVS(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    foreach_all(Player, entity)
    {
        if (entity->characterID & 1) {
            int32 slotID = 0;
            for (int32 i = 0; i < session->playerCount; ++i, ++slotID) {
                EntityPlayer *player = RSDK_GET_ENTITY(slotID, Player);
                RSDK.CopyEntity(player, entity, false);
                player->characterID = (globals->playerID >> (8 * i)) & 0xFF;
                switch (player->characterID) {
                    case ID_TAILS:
                        Player->tailsSpriteIndex      = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
                        Player->tailsTailsSpriteIndex = RSDK.LoadSpriteAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                        break;
                    case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
                    case ID_MIGHTY: Player->mightySpriteIndex = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
                    case ID_RAY: Player->raySpriteIndex = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE); break;
#endif
                    default:
                        player->characterID      = ID_SONIC;
                        Player->sonicSpriteIndex = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
                        Player->superSpriteIndex = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                        break;
                }
                player->controllerID = i + 1;
                player->camera       = Camera_SetTargetEntity(i, (Entity *)player);
            }
        }
        destroyEntity(entity);
    }
}
void Player_SaveValues(void)
{
    EntityPlayer *entity     = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Player->savedLives       = entity->lives;
    globals->restartLives[0] = Player->savedLives;
    Player->savedScore       = entity->score;
    globals->restartScore    = Player->savedScore;
    Player->savedScore1UP    = entity->score1UP;
    globals->restartScore1UP = Player->savedScore1UP;
}
void Player_GiveScore(EntityPlayer *player, int32 score)
{
    if (player->sidekick)
        player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player->score += score;
    if (player->score > 9999999)
        player->score = 9999999;

    if (player->score >= player->score1UP) {
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_TIMEATTACK && globals->gameMode != MODE_ENCORE) {
#else
        if (globals->gameMode != MODE_TIMEATTACK) {
#endif
            if (player->lives < 99)
                player->lives++;
            Music_PlayMusicTrack(TRACK_1UP);
        }

        while (player->score1UP <= player->score) player->score1UP += 50000;
    }
}
void Player_GiveRings(int32 amount, EntityPlayer *player, bool32 playSFX)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (globals->gameMode == MODE_COMPETITION)
        session->totalRings[player->playerID] += amount;
    player->rings += amount;

    if (player->rings < 0) {
        player->rings = 0;
    }
    else if (player->rings >= 999) {
        player->rings = 999;
    }

    if (player->rings >= player->ringExtraLife) {
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_TIMEATTACK && globals->gameMode != MODE_ENCORE) {
#else
        if (globals->gameMode != MODE_TIMEATTACK) {
#endif
            if (player->lives < 99)
                player->lives++;
            Music_PlayMusicTrack(TRACK_1UP);
        }
        player->ringExtraLife += 100;
        if (player->ringExtraLife > 300)
            player->ringExtraLife = 1000;
    }

    if (playSFX) {
        if (Ring->pan) {
            int32 channel = RSDK.PlaySfx(Ring->sfx_Ring, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
            Ring->pan = 0;
        }
        else {
            int32 channel = RSDK.PlaySfx(Ring->sfx_Ring, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
            Ring->pan = 1;
        }
    }
}
void Player_GiveLife(EntityPlayer *entity)
{
#if RETRO_USE_PLUS
    if (globals->gameMode != MODE_TIMEATTACK && globals->gameMode != MODE_ENCORE) {
#else
    if (globals->gameMode != MODE_TIMEATTACK) {
#endif
        if (entity->lives < 99)
            entity->lives++;
        Music_PlayMusicTrack(TRACK_1UP);
    }
}
void Player_ApplyShieldEffect(EntityPlayer *player)
{
    if (player->shield && player->superState != SUPERSTATE_SUPER && player->invincibleTimer <= 0) {
        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
        RSDK.ResetEntityPtr(shield, Shield->objectID, player);
    }
}
void Player_ChangeCharacter(EntityPlayer *entity, int32 character)
{
    uint16 playerID     = entity->playerID;
    entity->characterID = character;
    globals->playerID &= ~(255 << 8 * playerID);
    globals->playerID |= character << 8 * entity->playerID;
    switch (entity->characterID) {
        case ID_TAILS:
            Player->tailsSpriteIndex      = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
            Player->tailsTailsSpriteIndex = RSDK.LoadSpriteAnimation("Players/TailSprite.bin", SCOPE_STAGE);
            if (SizeLaser) {
                SizeLaser->tailsIndex      = RSDK.LoadSpriteAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
                SizeLaser->tailSpriteIndex = RSDK.LoadSpriteAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            }

            if (entity->isChibi) {
                entity->spriteIndex     = SizeLaser->tailsIndex;
                entity->tailSpriteIndex = SizeLaser->tailSpriteIndex;
                entity->cameraOffset    = 0x40000;
            }
            else {
                entity->spriteIndex     = Player->tailsSpriteIndex;
                entity->tailSpriteIndex = Player->tailsTailsSpriteIndex;
                entity->cameraOffset    = 0;
            }
            entity->movesetState = Player_TailsJumpAbility;
            entity->sensorY      = 0x100000;
            break;
        case ID_KNUCKLES:
            Player->knuxSpriteIndex = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE);
            if (SizeLaser) {
                SizeLaser->knuxIndex = RSDK.LoadSpriteAnimation("Players/ChibiKnux.bin", SCOPE_STAGE);
            }

            if (entity->isChibi) {
                entity->spriteIndex  = SizeLaser->knuxIndex;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex  = Player->knuxSpriteIndex;
                entity->cameraOffset = 0x50000;
            }
            entity->tailSpriteIndex = -1;
            entity->movesetState    = Player_KnuxJumpAbility;
            entity->sensorY         = 0x140000;
            break;
#if RETRO_USE_PLUS
        case ID_MIGHTY:
            Player->mightySpriteIndex = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE);
            if (SizeLaser) {
                SizeLaser->mightyIndex = RSDK.LoadSpriteAnimation("Players/ChibiMighty.bin", SCOPE_STAGE);
            }

            if (entity->isChibi) {
                entity->spriteIndex  = SizeLaser->mightyIndex;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex  = Player->mightySpriteIndex;
                entity->cameraOffset = 0x50000;
            }
            entity->tailSpriteIndex = -1;
            entity->movesetState    = Player_MightyJumpAbility;
            entity->sensorY         = 0x140000;
            break;
        case ID_RAY:
            Player->raySpriteIndex = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE);
            if (SizeLaser) {
                SizeLaser->rayIndex = RSDK.LoadSpriteAnimation("Players/ChibiRay.bin", SCOPE_STAGE);
            }

            if (entity->isChibi) {
                entity->spriteIndex  = SizeLaser->rayIndex;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex  = Player->raySpriteIndex;
                entity->cameraOffset = 0x50000;
            }
            entity->tailSpriteIndex = -1;
            entity->movesetState    = Player_RayJumpAbility;
            entity->sensorY         = 0x140000;
            break;
#endif
        default:
            Player->sonicSpriteIndex = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
            Player->superSpriteIndex = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
            if (SizeLaser) {
                SizeLaser->sonicIndex = RSDK.LoadSpriteAnimation("Players/ChibiSonic.bin", SCOPE_STAGE);
            }

            if (entity->isChibi) {
                entity->spriteIndex  = SizeLaser->sonicIndex;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex  = Player->sonicSpriteIndex;
                entity->cameraOffset = 0x50000;
            }
            entity->tailSpriteIndex = -1;
            entity->movesetState    = Player_SonicJumpAbility;
            entity->sensorY         = 0x140000;
            if (globals->medalMods & getMod(MEDAL_PEELOUT)) {
                entity->peeloutState = Player_StartPeelout;
                for (int32 f = 0; f < 4; ++f) {
                    SpriteFrame *dst = RSDK.GetFrame(entity->spriteIndex, ANI_DASH, f);
                    SpriteFrame *src = RSDK.GetFrame(entity->spriteIndex, ANI_FLY, f);

                    *dst = *src;
                }
            }
            break;
    }
    entity->sensorX[0] = 0xA0000;
    entity->sensorX[1] = 0x50000;
    entity->sensorX[2] = 0;
    entity->sensorX[3] = -0x50000;
    entity->sensorX[4] = -0xA0000;

    if (entity->state == Player_State_KnuxWallClimb || entity->state == Player_State_DropDash || entity->state == Player_State_TailsFlight
        || entity->state == Player_State_KnuxGlideDrop || entity->state == Player_State_KnuxGlideLeft || entity->state == Player_State_KnuxGlideRight
        || entity->state == Player_State_GlideSlide || entity->state == Player_State_KnuxLedgePullUp
#if RETRO_USE_PLUS
        || entity->state == Player_State_MightyHammerDrop || entity->state == Player_State_RayGlide || entity->state == Player_State_MightyUnspin
#endif
    ) {
        entity->state = Player_State_Air;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
    }
    else {
        RSDK.SetSpriteAnimation(entity->spriteIndex, entity->playerAnimator.animationID, &entity->playerAnimator, false, 0);
    }

    if (entity->superState != SUPERSTATE_NONE) {
        for (int32 i = 0; i < 6; ++i) {
            RSDK.SetPaletteEntry(0, i + 64, Player->superPalette_Sonic[i]);
            RSDK.SetPaletteEntry(0, i + 70, Player->superPalette_Tails[i]);
            RSDK.SetPaletteEntry(0, i + 80, Player->superPalette_Knux[i]);
#if RETRO_USE_PLUS
            RSDK.SetPaletteEntry(0, i + 96, Player->superPalette_Mighty[i]);
            RSDK.SetPaletteEntry(0, i + 113, Player->superPalette_Ray[i]);
#endif

            if (HCZSetup) {
                RSDK.SetPaletteEntry(1, i + 64, Player->superPalette_Sonic_HCZ[i]);
                RSDK.SetPaletteEntry(1, i + 70, Player->superPalette_Tails_HCZ[i]);
                RSDK.SetPaletteEntry(1, i + 80, Player->superPalette_Knux_HCZ[i]);
#if RETRO_USE_PLUS
                RSDK.SetPaletteEntry(1, i + 96, Player->superPalette_Mighty_HCZ[i]);
                RSDK.SetPaletteEntry(1, i + 113, Player->superPalette_Ray_HCZ[i]);
#endif
            }
            else if (CPZSetup) {
                RSDK.SetPaletteEntry(2, i + 64, Player->superPalette_Sonic_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 70, Player->superPalette_Tails_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 80, Player->superPalette_Knux_CPZ[i]);
#if RETRO_USE_PLUS
                RSDK.SetPaletteEntry(2, i + 96, Player->superPalette_Mighty_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 113, Player->superPalette_Ray_CPZ[i]);
#endif
            }
        }

        if (FarPlane) {
            RSDK.CopyPalette(0, 64, 3, 64, 6);
            RSDK.CopyPalette(0, 70, 3, 70, 6);
            RSDK.CopyPalette(0, 80, 3, 80, 6);
            RSDK.CopyPalette(0, 96, 3, 96, 6);
            RSDK.CopyPalette(0, 113, 3, 113, 6);
        }
        else if (OOZSetup) {
            RSDK.CopyPalette(0, 64, 1, 64, 6);
            RSDK.CopyPalette(0, 64, 2, 64, 6);
            RSDK.CopyPalette(0, 70, 1, 70, 6);
            RSDK.CopyPalette(0, 70, 2, 70, 6);
            RSDK.CopyPalette(0, 80, 1, 80, 6);
            RSDK.CopyPalette(0, 80, 2, 80, 6);
            RSDK.CopyPalette(0, 96, 1, 96, 6);
            RSDK.CopyPalette(0, 96, 2, 96, 6);
            RSDK.CopyPalette(0, 113, 1, 113, 6);
            RSDK.CopyPalette(0, 113, 2, 113, 6);
        }
        if (entity->superState == SUPERSTATE_SUPER)
            Player_CheckGoSuper(entity, 0xFF);
    }
    Player_ChangePhysicsState(entity);
}
bool32 Player_CheckGoSuper(EntityPlayer *player, uint8 emeraldflags)
{
    uint8 emeralds = 127;
    // if (emeraldflags != 255)
    //    emeralds = emeraldflags;
    if (!RSDK_sceneInfo->timeEnabled && !ERZStart && (!PhantomEgg || PhantomEgg->superFlag)) {
        return false;
    }

#if RETRO_USE_PLUS
    if (Player->canSuperCB) {
        if (!Player->canSuperCB(false))
            return false;
    }
#endif
    if ((player->superState >= SUPERSTATE_SUPER || emeralds != 0x7F || player->rings < 50) && emeraldflags != 0xFF)
        return false;

#if RETRO_USE_PLUS
    RSDK.StopSFX(Player->sfx_SwapFail);
    if (globals->medalMods & getMod(SECRET_SUPERDASH))
        player->movesetState = ERZStart_Player_StartSuperFly;
#endif

    switch (player->characterID) {
        case ID_SONIC:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 0x40, Player->superPalette_Sonic[c]);
                RSDK.SetPaletteEntry(7, c + 0x40, Player->superPalette_Sonic[c + 6]);
            }
            break;
        case ID_TAILS:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 70, Player->superPalette_Tails[c]);
                RSDK.SetPaletteEntry(7, c + 70, Player->superPalette_Tails[c + 12]);
            }
            break;
        case ID_KNUCKLES:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 0x50, Player->superPalette_Knux[c]);
                RSDK.SetPaletteEntry(7, c + 0x50, Player->superPalette_Knux[c + 12]);
            }
            break;
#if RETRO_USE_PLUS
        case ID_MIGHTY:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 0x60, Player->superPalette_Mighty[c]);
                RSDK.SetPaletteEntry(7, c + 0x60, Player->superPalette_Mighty[c + 12]);
            }
            break;
        case ID_RAY:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 113, Player->superPalette_Ray[c]);
                RSDK.SetPaletteEntry(7, c + 113, Player->superPalette_Ray[c + 12]);
            }
            break;
#endif
        default: break;
    }

    if (player->characterID == ID_SONIC && !player->isChibi)
        player->spriteIndex = Player->superSpriteIndex;
    if (emeraldflags == 0xFF) {
        player->superState = SUPERSTATE_SUPER;
        Player_ChangePhysicsState(player);
    }
    else {
        if (player->isChibi)
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_TRANSFORM, &player->playerAnimator, true, 0);
        player->invincibleTimer = 60;
        player->velocity.x      = 0;
        player->velocity.y      = 0;
        player->groundVel       = 0;
        player->onGround        = false;
        player->nextAirState    = StateMachine_None;
        player->nextGroundState = StateMachine_None;
        player->interaction     = false;
        player->state           = Player_State_Transform;
        player->forceTransform  = true;
#if RETRO_USE_PLUS
        if (!ERZStart && globals->superMusicEnabled)
#else
        if (!ERZStart)
#endif
            Music_FadeOut(0.8);

        player->jumpAbilityTimer = 0;
        player->superState       = SUPERSTATE_FADEIN;
    }
    player->superBlendAmount = 0;
    player->superBlendState  = 0;
    player->superSecondTimer = 60;
    player->timer            = 0;
    return true;
}
void Player_BlendSuperSonicColours(int32 bankID)
{
    RSDK_THIS(Player);
    if (entity->superState == SUPERSTATE_SUPER) {
        if (entity->superBlendState & 1) {
            if (entity->superBlendAmount <= 0)
                entity->superBlendState &= 2;
            else
                entity->superBlendAmount -= 4;
        }
        else if (entity->superBlendAmount >= 256) {
            entity->superBlendState |= 1;
            if (entity->superBlendState == 1) {
                for (int32 i = 0; i < 6; ++i) {
                    RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic[i + 6]);
                    RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic[i + 12]);
                }
                entity->superBlendState  = 2;
                entity->superBlendAmount = 0;
            }
        }
        else {
            entity->superBlendAmount += 4;
        }
    }
    else {
        if (entity->superBlendAmount <= 0)
            entity->superState = SUPERSTATE_NONE;
        else
            entity->superBlendAmount -= 4;
    }
    RSDK.SetLimitedFade(bankID, 6, 7, entity->superBlendAmount, 64, 69);
}
void Player_BlendSuperTailsColours(int32 bankID)
{
    RSDK_THIS(Player);
    if (entity->superState != SUPERSTATE_SUPER) {
        if (entity->superBlendAmount <= 0)
            entity->superState = SUPERSTATE_NONE;
        else
            entity->superBlendAmount -= 4;
    }
    else if (entity->superBlendState) {
        if (entity->superBlendAmount <= 0)
            entity->superBlendState = 0;
        else
            entity->superBlendAmount -= 4;
    }
    else {
        if (entity->superBlendAmount >= 256)
            entity->superBlendState = 1;
        else
            entity->superBlendAmount += 4;
    }
    RSDK.SetLimitedFade(bankID, 6, 7, entity->superBlendAmount, 70, 76);
}
void Player_BlendSuperKnuxColours(int32 bankID)
{
    RSDK_THIS(Player);
    if (entity->superState != SUPERSTATE_SUPER) {
        if (entity->superBlendAmount <= 0)
            entity->superState = SUPERSTATE_NONE;
        else
            entity->superBlendAmount -= 4;
    }
    else if (entity->superBlendState) {
        if (entity->superBlendAmount <= 0)
            entity->superBlendState = 0;
        else
            entity->superBlendAmount -= 4;
    }
    else {
        if (entity->superBlendAmount >= 256)
            entity->superBlendState = 1;
        else
            entity->superBlendAmount += 4;
    }
    RSDK.SetLimitedFade(bankID, 6, 7, entity->superBlendAmount, 80, 86);
}
#if RETRO_USE_PLUS
void Player_BlendSuperMightyColours(int32 bankID)
{
    RSDK_THIS(Player);
    if (entity->superState != SUPERSTATE_SUPER) {
        if (entity->superBlendAmount <= 0)
            entity->superState = SUPERSTATE_NONE;
        else
            entity->superBlendAmount -= 4;
    }
    else if (entity->superBlendState) {
        if (entity->superBlendAmount <= 0)
            entity->superBlendState = 0;
        else
            entity->superBlendAmount -= 4;
    }
    else {
        if (entity->superBlendAmount >= 256)
            entity->superBlendState = 1;
        else
            entity->superBlendAmount += 4;
    }
    RSDK.SetLimitedFade(bankID, 6, 7, entity->superBlendAmount, 96, 102);
}
void Player_BlendSuperRayColours(int32 bankID)
{
    RSDK_THIS(Player);
    if (entity->superState != SUPERSTATE_SUPER) {
        if (entity->superBlendAmount <= 0)
            entity->superState = SUPERSTATE_NONE;
        else
            entity->superBlendAmount -= 4;
    }
    else if (entity->superBlendState) {
        if (entity->superBlendAmount <= 0)
            entity->superBlendState = 0;
        else
            entity->superBlendAmount -= 4;
    }
    else {
        if (entity->superBlendAmount >= 256)
            entity->superBlendState = 1;
        else
            entity->superBlendAmount += 4;
    }
    RSDK.SetLimitedFade(bankID, 6, 7, entity->superBlendAmount, 113, 119);
}
#endif
void Player_HandleSuperForm(void)
{
    RSDK_THIS(Player);
    if (entity->superState != SUPERSTATE_NONE) {
        switch (entity->characterID) {
            case ID_SONIC:
                Player_BlendSuperSonicColours(0);
                if (FarPlane) {
                    RSDK.CopyPalette(0, 64, 3, 64, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 64, 1, 64, 6);
                    RSDK.CopyPalette(0, 64, 2, 64, 6);
                }
                else if (HCZSetup) {
                    if (entity->superBlendState > 1) {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic_HCZ[i + 6]);
                            RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic_HCZ[i + 12]);
                        }
                    }
                    else {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic_HCZ[i]);
                            RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic_HCZ[i + 6]);
                        }
                    }

                    Player_BlendSuperSonicColours(1);

                    if (entity->superBlendState > 1) {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic[i + 6]);
                            RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic[i + 12]);
                        }
                    }
                    else {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic[i]);
                            RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic[i + 6]);
                        }
                    }
                }
                else if (CPZSetup) {
                    if (entity->superBlendState > 1) {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic_CPZ[i + 6]);
                            RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic_CPZ[i + 12]);
                        }
                    }
                    else {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic_CPZ[i]);
                            RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic_CPZ[i + 6]);
                        }
                    }

                    Player_BlendSuperSonicColours(2);

                    if (entity->superBlendState > 1) {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic[i + 6]);
                            RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic[i + 12]);
                        }
                    }
                    else {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic[i]);
                            RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic[i + 6]);
                        }
                    }
                }
                break;
            case ID_TAILS:
                Player_BlendSuperTailsColours(0);
                if (FarPlane) {
                    RSDK.CopyPalette(0, 70, 3, 70, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 70, 1, 70, 6);
                    RSDK.CopyPalette(0, 70, 2, 70, 6);
                }
                else if (HCZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 70, Player->superPalette_Tails_HCZ[i]);
                        RSDK.SetPaletteEntry(7, i + 70, Player->superPalette_Tails_HCZ[i + 12]);
                    }

                    Player_BlendSuperTailsColours(1);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 70, Player->superPalette_Tails[i]);
                        RSDK.SetPaletteEntry(7, i + 70, Player->superPalette_Tails[i + 12]);
                    }
                }
                else if (CPZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 70, Player->superPalette_Tails_CPZ[i]);
                        RSDK.SetPaletteEntry(7, i + 70, Player->superPalette_Tails_CPZ[i + 12]);
                    }

                    Player_BlendSuperTailsColours(2);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 70, Player->superPalette_Tails[i]);
                        RSDK.SetPaletteEntry(7, i + 70, Player->superPalette_Tails[i + 12]);
                    }
                }
                break;
            case ID_KNUCKLES:
                Player_BlendSuperKnuxColours(0);
                if (FarPlane) {
                    RSDK.CopyPalette(0, 0x50, 3, 0x50, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 0x50, 1, 0x50, 6);
                    RSDK.CopyPalette(0, 0x50, 2, 0x50, 6);
                }
                else if (HCZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 0x50, Player->superPalette_Knux_HCZ[i]);
                        RSDK.SetPaletteEntry(7, i + 0x50, Player->superPalette_Knux_HCZ[i + 12]);
                    }

                    Player_BlendSuperKnuxColours(1);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 0x50, Player->superPalette_Knux[i]);
                        RSDK.SetPaletteEntry(7, i + 0x50, Player->superPalette_Knux[i + 12]);
                    }
                }
                else if (CPZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 0x50, Player->superPalette_Knux_CPZ[i]);
                        RSDK.SetPaletteEntry(7, i + 0x50, Player->superPalette_Knux_CPZ[i + 12]);
                    }

                    Player_BlendSuperKnuxColours(2);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 0x50, Player->superPalette_Knux[i]);
                        RSDK.SetPaletteEntry(7, i + 0x50, Player->superPalette_Knux[i + 12]);
                    }
                }
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                Player_BlendSuperMightyColours(0);
                if (FarPlane) {
                    RSDK.CopyPalette(0, 0x60, 3, 0x60, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 0x60, 1, 0x60, 6);
                    RSDK.CopyPalette(0, 0x60, 2, 0x60, 6);
                }
                else if (HCZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 0x60, Player->superPalette_Mighty_HCZ[i]);
                        RSDK.SetPaletteEntry(7, i + 0x60, Player->superPalette_Mighty_HCZ[i + 12]);
                    }

                    Player_BlendSuperMightyColours(1);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 0x60, Player->superPalette_Mighty[i]);
                        RSDK.SetPaletteEntry(7, i + 0x60, Player->superPalette_Mighty[i + 12]);
                    }
                }
                else if (CPZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 0x60, Player->superPalette_Mighty_CPZ[i]);
                        RSDK.SetPaletteEntry(7, i + 0x60, Player->superPalette_Mighty_CPZ[i + 12]);
                    }

                    Player_BlendSuperMightyColours(2);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 0x60, Player->superPalette_Mighty[i]);
                        RSDK.SetPaletteEntry(7, i + 0x60, Player->superPalette_Mighty[i + 12]);
                    }
                }
                break;
            case ID_RAY:
                Player_BlendSuperRayColours(0);
                if (FarPlane) {
                    RSDK.CopyPalette(0, 113, 3, 113, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 113, 1, 113, 6);
                    RSDK.CopyPalette(0, 113, 2, 113, 6);
                }
                else if (HCZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 113, Player->superPalette_Ray_HCZ[i]);
                        RSDK.SetPaletteEntry(7, i + 113, Player->superPalette_Ray_HCZ[i + 12]);
                    }

                    Player_BlendSuperRayColours(1);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 113, Player->superPalette_Ray[i]);
                        RSDK.SetPaletteEntry(7, i + 113, Player->superPalette_Ray[i + 12]);
                    }
                }
                else if (CPZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 113, Player->superPalette_Ray_CPZ[i]);
                        RSDK.SetPaletteEntry(7, i + 113, Player->superPalette_Ray_CPZ[i + 12]);
                    }

                    Player_BlendSuperRayColours(2);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 113, Player->superPalette_Ray[i]);
                        RSDK.SetPaletteEntry(7, i + 113, Player->superPalette_Ray[i + 12]);
                    }
                }
                break;
#endif
        }
    }

    if (entity->superState == SUPERSTATE_SUPER) {
        bool32 flag = false;
        if (!RSDK_sceneInfo->timeEnabled && !ERZStart) {
            if (!PhantomEgg || PhantomEgg->superFlag) {
                entity->superState = SUPERSTATE_FADEOUT;
                flag               = true;
            }
        }

        if (!flag) {
            entity->invincibleTimer = 60;
            if (--entity->superSecondTimer <= 0) {
                entity->superSecondTimer = 60;
                if (--entity->rings <= 0) {
                    entity->rings      = 0;
                    entity->superState = SUPERSTATE_FADEOUT;
                }
            }
        }
    }

    if (entity->superState == SUPERSTATE_FADEOUT) {
        switch (entity->characterID) {
            case ID_SONIC:
                entity->movesetState = Player_SonicJumpAbility;
                for (int32 i = 0; i < 6; ++i) {
                    RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic[i]);
                    RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic[i + 6]);
                }
                entity->superBlendAmount = 256;
                entity->superBlendState  = 1;
                break;
            case ID_TAILS: entity->movesetState = Player_TailsJumpAbility; break;
            case ID_KNUCKLES: entity->movesetState = Player_KnuxJumpAbility; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: entity->movesetState = Player_MightyJumpAbility; break;
            case ID_RAY: entity->movesetState = Player_RayJumpAbility; break;
#endif
            default: break;
        }

        if (ERZStart) {
            entity->shield   = SHIELD_NONE;
            entity->rotation = 0;
            if (entity->state == ERZStart_State_PlayerSuperFly || entity->state == ERZStart_State_PlayerRebound)
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_HURT, &entity->playerAnimator, false, 0);
        }
        else {
            bool32 superActive = false;
            foreach_active(Player, player)
            {
                if (player->superState == SUPERSTATE_SUPER)
                    superActive = true;
            }

            if (superActive)
                Music_ResumePrevTrack(TRACK_SUPER, true);
            if (entity->state == ERZStart_State_PlayerSuperFly || entity->state == ERZStart_State_PlayerRebound)
                entity->state = Player_State_Air;
        }

        if (entity->characterID == ID_SONIC && !entity->isChibi) {
            entity->spriteIndex = Player->sonicSpriteIndex;
            if (!entity->playerAnimator.animationID)
                entity->playerAnimator.frameID = 0;
            RSDK.SetSpriteAnimation(entity->spriteIndex, entity->playerAnimator.animationID, &entity->playerAnimator, true,
                                    entity->playerAnimator.frameID);
        }

        entity->invincibleTimer = 0;
        if (entity->shield)
            Player_ApplyShieldEffect(entity);
        entity->superState = SUPERSTATE_DONE;
        Player_ChangePhysicsState(entity);
    }
}
bool32 Player_CheckKeyPress(void)
{
    RSDK_THIS(Player);

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        return false;
#endif

#if RETRO_USE_PLUS
    if (entity->controllerID > PLAYER_MAX || Player->disableP2KeyCheck)
        return false;
#else
    if (entity->controllerID > PLAYER_MAX)
        return false;
#endif

    return RSDK_controller[entity->controllerID].keyUp.down || RSDK_controller[entity->controllerID].keyDown.down
           || RSDK_controller[entity->controllerID].keyLeft.down || RSDK_controller[entity->controllerID].keyRight.down
           || RSDK_controller[entity->controllerID].keyA.down || RSDK_controller[entity->controllerID].keyB.down
           || RSDK_controller[entity->controllerID].keyC.down || RSDK_controller[entity->controllerID].keyX.down;
}
void Player_LoseRings(EntityPlayer *player, int32 rings, uint8 cPlane)
{
    RSDK_THIS(Player);
    int32 ringCntA = rings;
    int32 ringCntB = rings - 16;
    int32 ringCntC = rings - 32;

    ringCntA = clampVal(ringCntA, 0, 16);
    ringCntB = clampVal(ringCntB, 0, 16);
    ringCntC = clampVal(ringCntC, 0, 16);

    int32 angleStart = 0xC0 - 8 * (ringCntB & -2);
    int32 angle      = angleStart + 8;
    if (!(ringCntB & 1))
        angle = angleStart - 8;

    for (int32 i = 0; i < ringCntB; ++i) {
        EntityRing *ring              = CREATE_ENTITY(Ring, player, player->position.x, player->position.y);
        ring->velocity.x              = RSDK.Cos256(angle) << 9;
        ring->velocity.y              = RSDK.Sin256(angle) << 9;
        ring->animator.animationSpeed = 0x200;
        ring->collisionPlane          = cPlane;
        ring->inkEffect               = INK_ALPHA;
        ring->alpha                   = 0x100;
        ring->isPermanent             = true;
        ring->state                   = Ring_State_Bounce;
        ring->stateDraw               = Ring_StateDraw_Normal;
        ring->drawOrder               = entity->drawOrder;
        ring->moveType                = RING_MOVE_NORMAL;
        angle += 16;
    }

    angleStart = 0xC0 - 8 * (ringCntA & -2);
    angle      = angleStart + 8;
    if (!(ringCntA & 1))
        angle = angleStart - 8;

    for (int32 i = 0; i < ringCntA; ++i) {
        EntityRing *ring              = CREATE_ENTITY(Ring, player, player->position.x, player->position.y);
        ring->velocity.x              = RSDK.Cos256(angle) << 10;
        ring->velocity.y              = RSDK.Sin256(angle) << 10;
        ring->animator.animationSpeed = 512;
        ring->collisionPlane          = cPlane;
        ring->inkEffect               = INK_ALPHA;
        ring->alpha                   = 256;
        ring->isPermanent             = true;
        ring->state                   = Ring_State_Bounce;
        ring->stateDraw               = Ring_StateDraw_Normal;
        ring->drawOrder               = entity->drawOrder;
        angle += 16;
    }

    angleStart = 0xC0 - 8 * (ringCntC & -2);
    angle      = angleStart + 8;
    if (!(ringCntC & 1))
        angle = angleStart - 8;

    for (int32 i = 0; i < ringCntC; ++i) {
        EntityRing *ring = CREATE_ENTITY(Ring, player, player->position.x, player->position.y);
        ring->velocity.x = RSDK.Cos256(angle) << 11;
        ring->velocity.y = RSDK.Sin256(angle) << 11;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, 1, &ring->animator, true, 0);
        ring->scale.x                 = 0x100;
        ring->animator.animationSpeed = 0x200;
        ring->scale.y                 = 0x100;
        ring->drawFX                  = FX_FLIP | FX_ROTATE | FX_SCALE;
        ring->state                   = Ring_State_Grow;
        ring->stateDraw               = Ring_StateDraw_Normal;
        angle += 16;
    }
}
void Player_LoseHyperRings(EntityPlayer *player, int32 rings, uint8 cPlane)
{
    int32 ringCnt = rings >> 2;
    if (ringCnt <= 8) {
        if (ringCnt < 1)
            ringCnt = 1;
    }
    else {
        ringCnt = 8;
    }
    int32 ringValue = rings / ringCnt;

    int32 startAngle = 0xC0 - 0x10 * (ringCnt & 0xFFFFFFFE);
    int32 angle      = startAngle + 0x10;
    if (!(ringCnt & 1))
        angle = startAngle - 0x10;

    for (int32 i = 0; i < ringCnt; ++i) {
        EntityRing *ring = (EntityRing *)RSDK.CreateEntity(Ring->objectID, player, player->position.x, player->position.y);
        RSDK.SetSpriteAnimation(Ring->spriteIndex, 1, &ring->animator, true, 0);
        ring->type                    = 1;
        ring->velocity.x              = 0x300 * RSDK.Cos256(angle);
        ring->isPermanent             = true;
        ring->drawFX                  = 5;
        ring->alpha                   = 0x100;
        ring->velocity.y              = 0x300 * RSDK.Sin256(angle);
        ring->animator.animationSpeed = 0x180;
        ring->collisionPlane          = cPlane;
        ring->angle                   = i << 6;
        ring->ringAmount              = ringValue;
        ring->state                   = Ring_State_Big;
        ring->stateDraw               = Ring_StateDraw_Normal;
        angle += 32;
    }
}
EntityPlayer *Player_GetNearestPlayerX(void)
{
    int32 distance = 0x7FFFFFFF;
    RSDK_THIS(Player);
    EntityPlayer *returnPlayer = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    foreach_active(Player, player)
    {
        int32 distX = abs(player->position.x - entity->position.x);
        if (distX < distance) {
            distance     = distX;
            returnPlayer = player;
        }
    }
    return returnPlayer;
}
EntityPlayer *Player_GetNearestPlayer(void)
{
    int32 distance = 0x7FFFFFFF;
    RSDK_THIS(Player);
    EntityPlayer *returnPlayer = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    foreach_active(Player, player)
    {
        int32 distX = abs(player->position.x - entity->position.x);
        int32 distY = abs(player->position.y - entity->position.y);
        if (distX + distY < distance) {
            distance     = distX + distY;
            returnPlayer = player;
        }
    }
    return returnPlayer;
}
#if RETRO_USE_PLUS
void Player_RemoveEncoreLeader(void)
{
    EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (globals->stock) {
        Player_ChangeCharacter(sidekick, globals->stock);
        globals->playerID &= 0xFF;
        globals->playerID |= globals->stock << 8;
        globals->stock >>= 8;
    }
    else {
        globals->playerID &= 0xFF;
        destroyEntity(sidekick);
        sidekick->playerID = 1;
    }
}
#endif
void Player_ResetBoundaries(EntityPlayer *player)
{
    Vector2 size;

    int32 screen = RSDK.GetEntityID(player);
    RSDK.GetLayerSize(Zone->fgLow, &size, true);
    Zone->screenBoundsL1[screen]     = 0;
    Zone->screenBoundsR1[screen]     = size.x;
    Zone->screenBoundsT1[screen]     = 0;
    Zone->screenBoundsB1[screen]     = size.y;
    Zone->screenBoundsL2[screen]     = Zone->screenBoundsL1[screen] << 16;
    Zone->screenBoundsR2[screen]     = Zone->screenBoundsR1[screen] << 16;
    Zone->screenBoundsT2[screen]     = Zone->screenBoundsT1[screen] << 16;
    Zone->screenBoundsB2[screen]     = Zone->screenBoundsB1[screen] << 16;
    Zone->deathBoundary[screen]      = Zone->screenBoundsB1[screen] << 16;
    Zone->playerBoundActiveT[screen] = true;
    Zone->playerBoundActiveT[screen] = false;

    if (player->camera) {
        player->camera->boundsL = Zone->screenBoundsL1[screen];
        player->camera->boundsR = Zone->screenBoundsR1[screen];
        player->camera->boundsT = Zone->screenBoundsT1[screen];
        player->camera->boundsB = Zone->screenBoundsB1[screen];
    }
}
void Player_State_TransportTube(void)
{
    // Nothin
}
void Player_HandleDeath(EntityPlayer *player)
{
    if (player->sidekick) {
        Player->jumpInDelay = 0;
        EntityDust *dust    = CREATE_ENTITY(Dust, intToVoid(1), player->position.x, player->position.y);
        dust->visible       = false;
        dust->active        = ACTIVE_NEVER;
        dust->isPermanent   = true;
        dust->position.y    = (RSDK_screens->position.y - 128) << 16;
#if RETRO_USE_PLUS
        EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (globals->gameMode != MODE_ENCORE || (leader->state != Player_State_Die && leader->state != Player_State_Drown)) {
            player->angle            = 0x80;
            player->state            = Player_State_StartJumpIn;
            player->abilityPtrs[0]   = (Entity *)dust;
            player->abilityValues[0] = 0;
            player->nextAirState     = StateMachine_None;
            player->nextGroundState  = StateMachine_None;
            player->stateInput       = Player_ProcessP2InputLag;
            player->position.x       = -0x400000;
            player->position.y       = -0x400000;
            player->velocity.x       = 0;
            player->velocity.y       = 0;
            player->groundVel        = 0;
            player->tileCollisions   = false;
            player->interaction      = false;
            player->drawOrder        = Zone->playerDrawHigh;
            player->airTimer         = 0;
            player->active           = ACTIVE_NORMAL;
        }
        else {
            player->state      = Player_State_EncoreRespawn;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->position.x = -0x200000;
            player->position.y = -0x200000;
        }
#else
        player->angle = 0x80;
        player->state = Player_State_StartJumpIn;
        player->abilityPtrs[0] = (Entity *)dust;
        player->abilityValues[0] = 0;
        player->nextAirState = StateMachine_None;
        player->nextGroundState = StateMachine_None;
        player->stateInput = Player_ProcessP2InputLag;
        player->position.x = -0x400000;
        player->position.y = -0x400000;
        player->velocity.x = 0;
        player->velocity.y = 0;
        player->groundVel = 0;
        player->tileCollisions = false;
        player->interaction = false;
        player->drawOrder = Zone->playerDrawHigh;
        player->airTimer = 0;
        player->active = ACTIVE_NORMAL;
#endif
    }
    else {
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE && Player_SwapMainPlayer(false)) {
            EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
            int32 id              = -1;
            for (int32 i = player2->characterID; i > 0; ++id) i >>= 1;
            globals->characterFlags &= ~(1 << id);

            if (player->state == Player_State_FlyIn || player->state == Player_State_JumpIn || player->state == Player_State_StartJumpIn) {
                if (!EncoreIntro) {
                    Player_ResetState(player);
                    Player_ResetBoundaries(player);
                }
                Player_RemoveEncoreLeader();
            }
            else {
                player->blinkTimer   = 120;
                player->scrollDelay  = 0;
                EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                player->camera       = camera;
                camera->targetPtr    = (Entity *)player;
                camera->state        = Camera_State_Follow;
                Player_RemoveEncoreLeader();
            }
        }
        else {
#endif
            if (player->lives > 0 && globals->gameMode != MODE_TIMEATTACK)
                player->lives--;
            Player->savedLives                      = player->lives;
            Player->savedScore                      = player->score;
            Player->savedScore1UP                   = player->score1UP;
            globals->restartLives[player->playerID] = player->lives;

#if RETRO_USE_PLUS
            if (globals->gameMode != MODE_ENCORE) {
                player->rings         = 0;
                player->ringExtraLife = 0;
                globals->restartRings = 0;
                globals->restart1UP   = 100;
            }
#else
        player->rings = 0;
        player->ringExtraLife = 0;
        globals->restartRings = 0;
        globals->restart1UP = 100;
#endif
            globals->coolBonus[player->playerID] = 0;

            EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
            if (globals->gameMode == MODE_COMPETITION) {
                session->lives[player->playerID] = player->lives;
            }

#if RETRO_USE_PLUS
            if (globals->gameMode != MODE_ENCORE) {
#endif
                if (player->lives) {
                    if (Zone->gotTimeOver) {
                        player->objectID        = TYPE_BLANK;
                        EntitySaveGame *saveRAM = SaveGame->saveRAM;
                        if (globals->gameMode == MODE_COMPETITION) {
                            int32 playerID                    = RSDK.GetEntityID(player);
                            EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                            if (!session->finishFlags[playerID]) {
#if RETRO_USE_PLUS
                                Competition_CalculateScore(playerID, 1);
#else
                        CompetitionSession_DeriveWinner(playerID, 1);
#endif
                            }
#if RETRO_USE_PLUS
                            foreach_all(HUD, hud) { hud->vsStates[RSDK.GetEntityID(player)] = HUD_State_GoOffScreen; }
#endif
                        }
                        else if (saveRAM) {
                            RSDK.ResetEntitySlot(SLOT_GAMEOVER, GameOver->objectID, intToVoid(true));
                            saveRAM->lives    = player->lives;
                            saveRAM->score    = player->score;
                            saveRAM->score1UP = player->score1UP;
#if RETRO_USE_PLUS
                            saveRAM->continues = globals->continues;
                            if (globals->gameMode == MODE_ENCORE) {
                                globals->playerID &= 0xFF;
                                saveRAM->playerID = globals->playerID;
                                int32 id          = -1;
                                for (int32 i = player->characterID; i > 0; ++id) i >>= 1;
                                globals->characterFlags &= ~(1 << id);
                                saveRAM->characterFlags = globals->characterFlags;
                                saveRAM->stock          = globals->stock;
                            }
#endif

                            if (globals->saveSlotID != NO_SAVE_SLOT && !checkNoSave && SaveGame->saveRAM && globals->saveLoaded == STATUS_OK) {
                                SaveGame->saveEntityPtr = RSDK_sceneInfo->entity;
                                SaveGame->saveCallback  = NULL;
#if RETRO_USE_PLUS
                                API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
#else
                        API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB);
#endif
                            }

                            EntityGameOver *gameOver = RSDK_GET_ENTITY(SLOT_GAMEOVER, GameOver);
                            gameOver->activeScreens |= 1 << player->playerID;
                            RSDK.SetGameMode(ENGINESTATE_FROZEN);
                            RSDK_sceneInfo->timeEnabled = false;
                        }
                    }
                    else if (globals->gameMode != MODE_COMPETITION) {
                        EntitySaveGame *saveRAM = SaveGame->saveRAM;
                        if (saveRAM) {
                            saveRAM->lives    = player->lives;
                            saveRAM->score    = player->score;
                            saveRAM->score1UP = player->score1UP;
#if RETRO_USE_PLUS
                            saveRAM->continues = globals->continues;
                            if (globals->gameMode == MODE_ENCORE) {
                                globals->playerID &= 0xFF;
                                saveRAM->playerID = globals->playerID;
                                int32 id          = -1;
                                for (int32 i = player->characterID; i > 0; ++id) i >>= 1;
                                globals->characterFlags &= ~(1 << id);
                                saveRAM->characterFlags = globals->characterFlags;
                                saveRAM->stock          = globals->stock;
                            }
#endif

                            if (globals->saveSlotID != NO_SAVE_SLOT && !checkNoSave && SaveGame->saveRAM && globals->saveLoaded == STATUS_OK) {
                                SaveGame->saveEntityPtr = RSDK_sceneInfo->entity;
                                SaveGame->saveCallback  = NULL;
#if RETRO_USE_PLUS
                                API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
#else
                        API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB);
#endif
                            }
                        }
                        Music_FadeOut(0.025);
                        Zone_Unknown2();
                        player->objectID = TYPE_BLANK;
                    }
                    else {
                        Player_ResetState(player);
#if RETRO_USE_PLUS
                        if (!player->playerID && globals->gameMode == MODE_ENCORE) {
                            Player_ResetState(RSDK_GET_ENTITY(SLOT_PLAYER2, Player));
                        }
#endif
                    }
                }
                else {
                    int32 screenID   = 0;
                    player->objectID = TYPE_BLANK;
                    if (player->camera) {
                        screenID                  = player->camera->screenID;
                        player->camera->targetPtr = (Entity *)player->camera;
                    }
                    if (globals->gameMode == MODE_COMPETITION) {
                        int32 playerID                    = RSDK.GetEntityID(player);
                        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                        if (!session->finishFlags[playerID]) {
#if RETRO_USE_PLUS
                            Competition_CalculateScore(playerID, 1);
#else
                    CompetitionSession_DeriveWinner(playerID, 1);
#endif
                        }
#if RETRO_USE_PLUS
                        foreach_all(HUD, hud) { hud->vsStates[RSDK.GetEntityID(player)] = HUD_State_GoOffScreen; }
#endif
                    }
                    else {
                        EntityGameOver *gameOver = RSDK_GET_ENTITY(SLOT_GAMEOVER, GameOver);
                        RSDK.ResetEntityPtr(gameOver, GameOver->objectID, intToVoid(false));
                        gameOver->playerID = RSDK.GetEntityID(player);
                        GameOver->activeScreens |= 1 << screenID;
                        RSDK.SetGameMode(ENGINESTATE_FROZEN);
                        RSDK_sceneInfo->timeEnabled = false;
                    }
                }

                if (player->objectID == Player->objectID) {
                    player->abilityValues[0] = 0;
                    Player_ResetBoundaries(player);
                    if (BoundsMarker)
                        BoundsMarker_CheckAllBounds(player, true);
                }
#if RETRO_USE_PLUS
            }
            else {
                int32 id = -1;
                for (int32 i = player->characterID; i > 0; ++id) i >>= 1;
                globals->characterFlags &= ~(1 << id);
                if (!globals->characterFlags) {
                    int32 screenID   = 0;
                    player->objectID = TYPE_BLANK;
                    if (player->camera) {
                        screenID                  = player->camera->screenID;
                        player->camera->targetPtr = (Entity *)player->camera;
                    }
                    EntityGameOver *gameOver = RSDK_GET_ENTITY(SLOT_GAMEOVER, GameOver);
                    RSDK.ResetEntityPtr(gameOver, GameOver->objectID, intToVoid(false));
                    gameOver->playerID = RSDK.GetEntityID(player);
                    GameOver->activeScreens |= 1 << screenID;
                    RSDK.SetGameMode(ENGINESTATE_FROZEN);
                    RSDK_sceneInfo->timeEnabled = false;

                    if (player->objectID == Player->objectID) {
                        player->abilityValues[0] = 0;
                        Player_ResetBoundaries(player);
                        if (BoundsMarker)
                            BoundsMarker_CheckAllBounds(player, true);
                    }
                }
                else {
                    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                    Player->jumpInDelay   = 0;
                    Player_SwapMainPlayer(true);

                    if (globals->stock) {
                        Player_ChangeCharacter(player2, globals->stock);
                        globals->stock >>= 8;
                    }
                    else {
                        globals->playerID &= 0xFF;
                        player2->objectID = TYPE_BLANK;
                    }
                    if (!EncoreIntro) {
                        Player_ResetState(player);
                        if (!player->playerID && globals->gameMode == MODE_ENCORE) {
                            Player_ResetState(RSDK_GET_ENTITY(SLOT_PLAYER2, Player));
                        }
                        if (player->objectID == Player->objectID) {
                            player->abilityValues[0] = 0;
                            Player_ResetBoundaries(player);
                            if (BoundsMarker)
                                BoundsMarker_CheckAllBounds(player, true);
                        }
                    }
                }
            }
#endif
#if RETRO_USE_PLUS
        }
#endif
    }
}
void Player_ResetState(EntityPlayer *player)
{
    player->drawOrder = Zone->playerDrawLow;
    if (!player->sidekick)
        player->stateInput = Player_ProcessP1Input;
    else
        player->stateInput = Player_ProcessP2Input_AI;
    player->tileCollisions = true;
    player->interaction    = true;
    player->collisionPlane = 0;
    player->onGround       = false;
    player->active         = ACTIVE_NORMAL;
    player->ringExtraLife  = 100;
    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
    int32 screen = RSDK.GetEntityID(player);

    player->position.x  = StarPost->playerPositions[screen].x;
    player->position.y  = StarPost->playerPositions[screen].y;
    player->direction   = StarPost->playerDirections[screen];
    player->scrollDelay = 0;
    if (player->camera) {
        player->camera->position.x = player->position.x;
        player->camera->position.y = player->position.y;
        player->camera->state      = Camera_State_Follow;
        Zone_Unknown4(RSDK.GetEntityID(player));
    }
    player->underwater = 0;

    Player_ChangePhysicsState(player);
    player->drawFX &= ~FX_SCALE;
    player->scale.x = 0x200;
    player->scale.y = 0x200;
    player->state   = Player_State_Air;
}

// Collision
Hitbox *Player_GetHitbox(EntityPlayer *player)
{
    Hitbox *playerHitbox = RSDK.GetHitbox(&player->playerAnimator, 0);
    if (playerHitbox)
        return playerHitbox;
    else
        return &defaultHitbox;
}

bool32 Player_CheckCollisionTouch(EntityPlayer *player, void *e, Hitbox *entityHitbox)
{
    Entity *entity = (Entity *)e;
    Hitbox *playerHitbox;
    Hitbox *otherHitbox;

#if RETRO_USE_PLUS
    if (player->isGhost)
        return false;
#endif
    playerHitbox = player->outerbox;
    if (!playerHitbox)
        playerHitbox = RSDK.GetHitbox(&player->playerAnimator, 0);
    otherHitbox = &defaultHitbox;
    if (playerHitbox)
        otherHitbox = playerHitbox;
    return RSDK.CheckObjectCollisionTouchBox(entity, entityHitbox, player, otherHitbox);
}
bool32 Player_CheckCollisionBox(EntityPlayer *player, void *e, Hitbox *entityHitbox)
{
    Entity *entity = (Entity *)e;
#if RETRO_USE_PLUS
    if (player->isGhost)
        return C_NONE;
#endif
    Hitbox *playerHitbox = player->outerbox;
    if (!playerHitbox)
        playerHitbox = RSDK.GetHitbox(&player->playerAnimator, 0);
    Hitbox *otherHitbox = &defaultHitbox;
    if (playerHitbox)
        otherHitbox = playerHitbox;
    switch (RSDK.CheckObjectCollisionBox(entity, entityHitbox, player, otherHitbox, true)) {
        case C_NONE:
        default: return C_NONE;
        case C_TOP: {
            player->controlLock   = 0;
            player->collisionMode = CMODE_FLOOR;

            int32 colPos[2];
            if (entity->direction & FLIP_X) {
                colPos[0] = entity->position.x - (entityHitbox->right << 16);
                colPos[1] = entity->position.x - (entityHitbox->left << 16);
            }
            else {
                colPos[0] = entity->position.x + (entityHitbox->left << 16);
                colPos[1] = entity->position.x + (entityHitbox->right << 16);
            }

            int32 sensorX1 = player->position.x + player->sensorX[0];
            int32 sensorX3 = player->position.x + player->sensorX[2];
            int32 sensorX2 = player->position.x + player->sensorX[1];
            int32 sensorX4 = player->position.x + player->sensorX[3];
            int32 sensorX5 = player->position.x + player->sensorX[4];

            if (sensorX1 >= colPos[0] && sensorX1 <= colPos[1])
                player->flailing |= 1;
            if (sensorX2 >= colPos[0] && sensorX2 <= colPos[1])
                player->flailing |= 2;
            if (sensorX3 >= colPos[0] && sensorX3 <= colPos[1])
                player->flailing |= 4;
            if (sensorX4 >= colPos[0] && sensorX4 <= colPos[1])
                player->flailing |= 8;
            if (sensorX5 >= colPos[0] && sensorX5 <= colPos[1])
                player->flailing |= 0x10;

            if (entity->velocity.y <= 0)
                player->collisionFlagV |= 1;
            return C_TOP;
        }
        case C_LEFT:
            player->controlLock = 0;
            if (player->left && player->onGround) {
                player->groundVel = -0x8000;
                player->position.x &= 0xFFFF0000;
            }
            return C_LEFT;
        case C_RIGHT:
            player->controlLock = 0;
            if (player->right && player->onGround)
                player->groundVel = 0x8000;
            return C_RIGHT;
        case C_BOTTOM: return C_BOTTOM;
    }
}
bool32 Player_CheckCollisionPlatform(EntityPlayer *player, void *e, Hitbox *entityHitbox)
{
    Entity *entity = (Entity *)e;
#if RETRO_USE_PLUS
    if (player->isGhost)
        return false;
#endif
    Hitbox *playerHitbox = player->outerbox;
    if (!playerHitbox)
        playerHitbox = RSDK.GetHitbox(&player->playerAnimator, 0);
    Hitbox *otherHitbox = &defaultHitbox;
    if (playerHitbox)
        otherHitbox = playerHitbox;
    if (RSDK.CheckObjectCollisionPlatform(entity, entityHitbox, player, otherHitbox, true)) {
        player->controlLock   = 0;
        player->collisionMode = CMODE_FLOOR;

        int32 colPos[2];
        if (entity->direction & FLIP_X) {
            colPos[0] = entity->position.x - (entityHitbox->right << 16);
            colPos[1] = entity->position.x - (entityHitbox->left << 16);
        }
        else {
            colPos[0] = entity->position.x + (entityHitbox->left << 16);
            colPos[1] = entity->position.x + (entityHitbox->right << 16);
        }

        int32 sensorX1 = player->position.x + player->sensorX[0];
        int32 sensorX3 = player->position.x + player->sensorX[2];
        int32 sensorX2 = player->position.x + player->sensorX[1];
        int32 sensorX4 = player->position.x + player->sensorX[3];
        int32 sensorX5 = player->position.x + player->sensorX[4];

        if (sensorX1 >= colPos[0] && sensorX1 <= colPos[1])
            player->flailing |= 0x01;
        if (sensorX2 >= colPos[0] && sensorX2 <= colPos[1])
            player->flailing |= 0x02;
        if (sensorX3 >= colPos[0] && sensorX3 <= colPos[1])
            player->flailing |= 0x04;
        if (sensorX4 >= colPos[0] && sensorX4 <= colPos[1])
            player->flailing |= 0x08;
        if (sensorX5 >= colPos[0] && sensorX5 <= colPos[1])
            player->flailing |= 0x10;

        if (entity->velocity.y <= 0)
            player->collisionFlagV |= 1;
        return true;
    }
    return false;
}

bool32 Player_CheckHit(EntityPlayer *player, void *e)
{
    Entity *entity = (Entity *)e;
    if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown || player->invincibleTimer
        || player->blinkTimer > 0) {
        return false;
    }

    if (player->position.x > entity->position.x)
        player->velocity.x = 0x20000;
    else
        player->velocity.x = -0x20000;

    Player_Hit(player);
    return true;
}
bool32 Player_CheckHitFlip(EntityPlayer *player)
{
    if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown || player->invincibleTimer
        || player->blinkTimer > 0) {
        return false;
    }

    if (player->direction)
        player->velocity.x = 0x20000;
    else
        player->velocity.x = -0x20000;

    Player_Hit(player);
    return true;
}
// Hits the player if not invulnerable and the player's shield doesn't match shield, returns true if player was hit
bool32 Player_CheckElementalHit(EntityPlayer *player, void *entity, int shield)
{
    return player->shield != shield ? Player_CheckHit(player, entity) : false;
}
bool32 Player_CheckAttacking(EntityPlayer *player, void *e)
{
    Entity *entity   = (Entity *)e;
    int32 anim       = player->playerAnimator.animationID;
    int32 character  = player->characterID;
    bool32 attacking = player->invincibleTimer != 0 || anim == ANI_JUMP || anim == ANI_SPINDASH;
    switch (character) {
        case ID_SONIC: attacking |= anim == ANI_DROPDASH; break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: attacking |= anim == ANI_DROPDASH; break;
#endif
        case ID_TAILS:
            if (!attacking && entity) {
                attacking = anim == ANI_FLY || anim == ANI_FLYTIRED || anim == ANI_FLYLIFT;
                if (player->position.y <= entity->position.y)
                    return false;
            }
            break;
        case ID_KNUCKLES:
            attacking |= anim == ANI_FLY || anim == ANI_FLYLIFTTIRED;
            character = player->characterID;
            break;
        default: break;
    }

    return attacking;
}
bool32 Player_CheckBadnikTouch(EntityPlayer *player, void *e, Hitbox *entityHitbox)
{
    Entity *entity = (Entity *)e;
#if RETRO_USE_PLUS
    if (player->isGhost)
        return false;
#endif

    Hitbox playerHitbox;
#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1) {
        Hitbox *plrHitbox = RSDK.GetHitbox(&player->playerAnimator, 0);
        Hitbox *othHitbox = &defaultHitbox;
        if (plrHitbox)
            othHitbox = plrHitbox;
        if (player->isChibi) {
            playerHitbox.left  = -32;
            playerHitbox.right = 32;
        }
        else {
            playerHitbox.left  = -48;
            playerHitbox.right = 48;
        }

        playerHitbox.top    = 0;
        playerHitbox.bottom = othHitbox->bottom + 16;
        if (RSDK.CheckObjectCollisionTouchBox(entity, entityHitbox, player, &playerHitbox))
            return true;
    }
#endif

    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + player->playerID, Shield);
    Hitbox *otherHitbox  = &defaultHitbox;
    if (shield->objectID != Shield->objectID || shield->state != Shield_State_Insta) {
        Hitbox *hitbox = player->outerbox;
        if (!hitbox)
            hitbox = RSDK.GetHitbox(&player->playerAnimator, 0);

        if (hitbox)
            otherHitbox = hitbox;
    }
    else {
        Hitbox *plrHitbox = RSDK.GetHitbox(&player->playerAnimator, 0);
        if (plrHitbox)
            otherHitbox = plrHitbox;
        playerHitbox.left   = 2 * otherHitbox->left - (otherHitbox->left >> 1);
        playerHitbox.top    = 2 * otherHitbox->top - (otherHitbox->top >> 1);
        playerHitbox.right  = 2 * otherHitbox->right - (otherHitbox->right >> 1);
        playerHitbox.bottom = 2 * otherHitbox->bottom - (otherHitbox->bottom >> 1);
        otherHitbox         = &playerHitbox;
    }
    return RSDK.CheckObjectCollisionTouchBox(entity, entityHitbox, player, otherHitbox);
}
bool32 Player_CheckBadnikBreak(void *e, EntityPlayer *player, bool32 destroy)
{
    Entity *entity   = (Entity *)e;

    if (Player_CheckAttacking(player, entity)) {
        CREATE_ENTITY(Animals, intToVoid((Animals->animalTypes[(RSDK.Rand(0, 32) >> 4)]) + 1), entity->position.x, entity->position.y);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y);
        explosion->drawOrder       = Zone->drawOrderHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);

        if (Zone_GetZoneID() >= 0) {
            int32 id = 0;
            switch (player->characterID) {
                case ID_SONIC: id = 1; break;
                case ID_TAILS: id = 2; break;
                case ID_KNUCKLES: id = 3; break;
#if RETRO_USE_PLUS
                case ID_MIGHTY: id = 4; break;
#endif
                default: id = 0;
#if RETRO_USE_PLUS
                    if (player->characterID == ID_RAY)
                        id = 5;
#endif
                    break;
            }

            StatInfo info;
#if RETRO_USE_PLUS
            TimeAttackData_TrackEnemyDefeat(Zone->actID, Zone_GetZoneID(), &info, id, RSDK_sceneInfo->filter == SCN_FILTER_ENCORE,
                                            (entity->position.x >> 0x10), (entity->position.y >> 0x10));
            API.TryTrackStat(&info);
#else
            APICallback_TrackEnemyDefeat(Zone->actID, Zone_GetZoneID(), id, (entity->position.x >> 0x10), (entity->position.y >> 0x10));
#endif
        }

        int32 yVel = 0;
        if (player->velocity.y <= 0) {
            yVel = player->velocity.y + 0x10000;
        }
#if RETRO_USE_PLUS
        else if (player->position.y >= entity->position.y
                 || (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_DROPDASH)) {
            yVel = player->velocity.y - 0x10000;
        }
#else
        else if (player->position.y >= entity->position.y) {
            yVel = player->velocity.y - 0x10000;
        }
#endif
        else {
            yVel = -(player->velocity.y + 2 * player->gravityStrength);
        }
        player->velocity.y = yVel;
        if (globals->gameMode != MODE_COMPETITION)
            player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityScoreBonus *scoreBonus = CREATE_ENTITY(ScoreBonus, NULL, entity->position.x, entity->position.y);
        scoreBonus->drawOrder        = Zone->drawOrderHigh;
        scoreBonus->animator.frameID = player->scoreBonus;

        switch (player->scoreBonus) {
            case 0: Player_GiveScore(player, 100); break;
            case 1: Player_GiveScore(player, 200); break;
            case 2: Player_GiveScore(player, 500); break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:
            case 0xE: Player_GiveScore(player, 1000); break;
            case 0xF: Player_GiveScore(player, 10000); break;
            default: break;
        }

        if (player->scoreBonus < 15)
            player->scoreBonus++;
        if (destroy) {
            destroyEntity(entity);
            entity->active = ACTIVE_NEVER2;
        }
        return true;
    }
#if RETRO_USE_PLUS
    else if (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_CROUCH) {
        if (player->uncurlTimer)
            return false;
        RSDK.PlaySfx(Player->sfx_PimPom, false, 255);
        player->uncurlTimer = 30;
        if (entity->position.x > player->position.x)
            player->groundVel = -0x10000;
        else
            player->groundVel = 0x10000;
    }
#endif
    else {
#if RETRO_USE_PLUS
        if (!player->uncurlTimer) {
#endif
            Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
        }
#endif
    }
    return false;
}
bool32 Player_CheckBossHit(EntityPlayer *player, void *e)
{
    Entity *entity = (Entity *)e;
    if (Player_CheckAttacking(player, entity)) {
        player->groundVel  = -player->groundVel;
        player->velocity.x = -player->velocity.x;
        player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
        if (player->characterID == ID_KNUCKLES && player->playerAnimator.animationID == ANI_FLY) {
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLYTIRED, &player->playerAnimator, false, 0);
            player->state = Player_State_KnuxGlideDrop;
        }
        else {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop || player->state == Player_State_RayGlide) {
                player->state = Player_State_Air;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                if (player->velocity.y < -0x40000)
                    player->velocity.y = -0x40000;
            }
#endif
        }
        return true;
    }
    else {
        Player_CheckHit(player, entity);
    }
    return false;
}
bool32 Player_CheckProjectileHit(EntityPlayer *player, void *p)
{
    Entity *projectile = (Entity *)p;
    bool32 deflected      = false;
#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY) {
        int32 anim = player->playerAnimator.animationID;
        if (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)
            deflected = true;
    }
#endif

    if (player->shield > SHIELD_BLUE || deflected) {
#if RETRO_USE_PLUS
        if (deflected)
            RSDK.PlaySfx(Player->sfx_MightyDeflect, false, 0xFE);
#endif
        int32 angle             = RSDK.ATan2(player->position.x - projectile->position.x, player->position.y - projectile->position.y);
        projectile->velocity.x  = -0x800 * RSDK.Cos256(angle);
        projectile->velocity.y  = -0x800 * RSDK.Sin256(angle);
        projectile->interaction = false;
        return true;
    }

    Player_CheckHit(player, projectile);
    return false;
}
#if RETRO_USE_PLUS
bool32 Player_CheckMightyShellHit(EntityPlayer *player, void *e)
{
    Entity *entity = (Entity *)e;
    if (player->characterID == ID_MIGHTY
        && (player->playerAnimator.animationID == ANI_CROUCH || player->playerAnimator.animationID == ANI_JUMP
            || player->playerAnimator.animationID == ANI_SPINDASH || player->playerAnimator.animationID == ANI_DROPDASH)) {
        if (!player->uncurlTimer) {
            RSDK.PlaySfx(Player->sfx_PimPom, false, 255);
            player->uncurlTimer = 30;
        }
        int angle          = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
        entity->velocity.x = -0x300 * RSDK.Cos256(angle);
        entity->velocity.y = -0x400 * RSDK.Sin256(angle);
        return true;
    }
    return false;
}
#endif
bool32 Player_CheckHit2(EntityPlayer *player, void *e, bool32 flag)
{
    Entity *entity  = (Entity *)e;
    int32 anim      = player->playerAnimator.animationID;
    int32 character = player->characterID;

    if (Player_CheckAttacking(player, entity)) {
        if (player->velocity.y <= 0) {
            player->velocity.y += 0x10000;
            return true;
        }
#if RETRO_USE_PLUS
        else if (player->position.y >= entity->position.y || (checkPlayerID(ID_MIGHTY, 1) && anim == ANI_DROPDASH)) {
            player->velocity.y -= 0x10000;
            return true;
        }
#else
        else if (player->position.y >= entity->position.y) {
            player->velocity.y -= 0x10000;
            return true;
        }
#endif
        else {
            player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
            return true;
        }
    }
    else {
        if (flag) {
#if RETRO_USE_PLUS
            if (!(character == ID_MIGHTY && anim == ANI_CROUCH)) {
#endif
                Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
            }
#endif
        }
        return false;
    }
}

// Player State Helpers
void Player_ChangePhysicsState(EntityPlayer *entity)
{
    int32 *tablePtr = NULL;
    switch (entity->characterID) {
        case ID_TAILS: tablePtr = Player->tailsPhysicsTable; break;
        case ID_KNUCKLES: tablePtr = Player->knuxPhysicsTable; break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: tablePtr = Player->mightyPhysicsTable; break;
        case ID_RAY: tablePtr = Player->rayPhysicsTable; break;
#endif
        default: tablePtr = Player->sonicPhysicsTable; break;
    }

    int32 state = 0;
    if (entity->underwater) {
        entity->gravityStrength = 0x2000;
        state                   = 1;
        if (entity->speedShoesTimer >= 0)
            entity->gravityStrength = 0x1000;
    }
    else {
        entity->gravityStrength = 0x5800;
        if (entity->speedShoesTimer >= 0)
            entity->gravityStrength = 0x3800;
    }

    int32 decelShift = 0;
    if (entity->superState == SUPERSTATE_SUPER) {
        state |= 2;
        decelShift = 2;
    }
    if (entity->speedShoesTimer > 0) {
        state |= 4;
        decelShift = 1;
    }
    int32 tableID               = 8 * state;
    entity->topSpeed            = tablePtr[tableID];
    entity->acceleration        = tablePtr[tableID + 1];
    entity->deceleration        = tablePtr[tableID + 1] >> decelShift;
    entity->airAcceleration     = tablePtr[tableID + 2];
    entity->airDeceleration     = tablePtr[tableID + 3];
    entity->skidSpeed           = tablePtr[tableID + 4];
    entity->rollingFriction     = tablePtr[tableID + 5];
    entity->jumpStrength        = tablePtr[tableID + 6];
    entity->jumpCap             = tablePtr[tableID + 7];
    entity->rollingDeceleration = 0x2000;

    if (entity->speedShoesTimer < 0) {
        entity->topSpeed >>= 1;
        entity->acceleration >>= 1;
        entity->airAcceleration >>= 1;
        entity->skidSpeed >>= 1;
        entity->rollingFriction >>= 1;
        entity->airDeceleration >>= 1;
    }

    if (entity->isChibi) {
        entity->topSpeed -= entity->topSpeed >> 3;
        entity->acceleration -= entity->acceleration >> 4;
        entity->airAcceleration -= entity->airAcceleration >> 4;
        entity->jumpStrength -= entity->jumpStrength >> 3;
        entity->jumpCap -= entity->jumpCap >> 3;
    }
}
void Player_HandleGroundMovement(void)
{
    RSDK_THIS(Player);

    if (entity->controlLock > 0) {
        entity->controlLock--;
        if (!entity->invertGravity) {
            entity->groundVel += RSDK.Sin256(entity->angle) << 13 >> 8;
        }
    }
    else {
        bool32 left  = false;
        bool32 right = false;
        if (entity->invertGravity) {
            right = entity->left;
            left  = entity->right;
        }
        else {
            left  = entity->left;
            right = entity->right;
        }
        if (left) {
            if (entity->groundVel > -entity->topSpeed) {
                if (entity->groundVel <= 0) {
                    entity->groundVel -= entity->acceleration;
                }
                else {
                    if (!entity->collisionMode && entity->groundVel > 0x40000 && !Zone->autoScrollSpeed) {
                        entity->direction = FLIP_NONE;
                        entity->skidding  = 24;
                    }
                    int32 skid = entity->skidSpeed;
                    if (entity->groundVel < skid) {
                        entity->groundVel = -abs(skid);
                    }
                    else {
                        entity->groundVel -= skid;
                    }
                }
            }
            if (entity->groundVel <= 0 && entity->skidding < 1)
                entity->direction = FLIP_X;
        }
        if (right) {
            if (entity->groundVel < entity->topSpeed) {
                if (entity->groundVel >= 0) {
                    entity->groundVel += entity->acceleration;
                }
                else {
                    if (!entity->collisionMode && entity->groundVel < -0x40000 && !Zone->autoScrollSpeed) {
                        entity->direction = FLIP_X;
                        entity->skidding  = 24;
                    }

                    if (entity->groundVel > -entity->skidSpeed) {
                        entity->groundVel = abs(entity->skidSpeed);
                    }
                    else {
                        entity->groundVel += entity->skidSpeed;
                    }
                }
            }

            if (entity->groundVel >= 0 && entity->skidding < 1)
                entity->direction = FLIP_NONE;
        }

        if (entity->left || entity->right) {
            if (!entity->invertGravity) {
                entity->groundVel += RSDK.Sin256(entity->angle) << 13 >> 8;

                if (entity->right) {
                    if (!entity->left) {
                        if (entity->angle > 0xC0 && entity->angle < 0xE4) {
                            if (entity->groundVel > -0x20000 && entity->groundVel < 0x28000) {
                                entity->controlLock = 30;
                            }
                        }
                    }
                }
                else if (entity->left) {
                    if (entity->angle > 0x1C && entity->angle < 0x40) {
                        if (entity->groundVel > -0x28000 && entity->groundVel < 0x20000) {
                            entity->controlLock = 30;
                        }
                    }
                }
            }
        }
        else {
            if (entity->groundVel <= 0) {
                entity->groundVel += entity->deceleration;
                if (entity->groundVel > 0)
                    entity->groundVel = 0;
            }
            else {
                entity->groundVel -= entity->deceleration;
                if (entity->groundVel < 0)
                    entity->groundVel = 0;
            }

            if (!entity->invertGravity) {
                if (entity->groundVel > 0x2000 || entity->groundVel < -0x2000)
                    entity->groundVel += RSDK.Sin256(entity->angle) << 13 >> 8;

                if (entity->angle > 0xC0 && entity->angle < 0xE4) {
                    if (entity->groundVel > -0x10000 && entity->groundVel < 0x10000) {
                        entity->controlLock = 30;
                    }
                }
                if (entity->angle > 0x1C && entity->angle < 0x40) {
                    if (entity->groundVel > -0x10000 && entity->groundVel < 0x10000) {
                        entity->controlLock = 30;
                    }
                }
            }
        }
    }

    if (!entity->invertGravity) {
        if (entity->collisionMode && entity->collisionMode <= CMODE_RWALL) {
            if (entity->angle >= 0x40 && entity->angle <= 0xC0) {
                if (entity->groundVel > -0x20000 && entity->groundVel < 0x20000) {
                    entity->velocity.x    = entity->groundVel * RSDK.Cos256(entity->angle) >> 8;
                    entity->onGround      = false;
                    entity->angle         = 0;
                    entity->collisionMode = CMODE_FLOOR;
                    entity->velocity.y    = (entity->groundVel * RSDK.Sin256(entity->angle)) >> 8;
                }
            }
        }
    }
}
void Player_HandleGroundRotation(void)
{
    RSDK_THIS(Player);

    if (entity->angle <= 4 || entity->angle >= 252) {
        if (entity->angle <= 16 || entity->angle >= 232) {
            entity->rotation = 0;
        }
        else {
            entity->rotation <<= 1;
        }
    }
    else {
        int32 vel    = (abs(entity->groundVel) <= 0x60000) + 1;
        int32 newRot = 0;
        if (entity->angle <= 16 || entity->angle >= 232)
            newRot = 0;
        else
            newRot = entity->angle << 1;
        int32 dif = newRot - entity->rotation;

        int32 rot = 0;
        if (dif >= 0)
            rot = newRot - entity->rotation;
        else
            rot = entity->rotation - newRot;

        int32 rot2 = 0;
        if (dif + 0x200 >= 0)
            rot2 = dif + 0x200;
        else
            rot2 = entity->rotation - newRot - 0x200;

        int32 check = dif - 0x200;
        if (dif - 0x200 < 0)
            check = entity->rotation - newRot + 0x200;

        if (rot >= check) {
            if (check < rot2) {
                entity->rotation += (dif - 0x200) >> vel;
                entity->rotation &= 0x1FF;
            }
            else {
                entity->rotation += (dif + 0x200) >> vel;
                entity->rotation &= 0x1FF;
            }
        }
        else {
            if (rot < rot2) {
                entity->rotation += (dif >> vel);
                entity->rotation &= 0x1FF;
            }
            else {
                entity->rotation += (dif + 0x200) >> vel;
                entity->rotation &= 0x1FF;
            }
        }
    }
}
void Player_HandleAirMovement(void)
{
    RSDK_THIS(Player);

    if (entity->camera) {
        entity->camera->offsetYFlag = true;
        entity->camera->offset.y    = 0x200000;
    }
    entity->velocity.y += entity->gravityStrength;
    if (entity->velocity.y < entity->jumpCap && entity->playerAnimator.animationID == ANI_JUMP && !entity->jumpHold && entity->jumpAbility == 1) {
        entity->velocity.y = entity->jumpCap;
        entity->velocity.x = entity->velocity.x - (entity->velocity.x >> 5);
    }

    entity->collisionMode = CMODE_FLOOR;
    entity->pushing       = 0;
    if (entity->rotation >= 256) {
        if (entity->rotation < 512) {
            entity->rotation += 4;
        }
    }
    else if (entity->rotation > 0) {
        entity->rotation -= 4;
        entity->rotation = entity->rotation;
    }
    else
        entity->rotation = 0;
}
void Player_HandleAirFriction(void)
{
    RSDK_THIS(Player);

    if (entity->velocity.y > -0x40000 && entity->velocity.y < 0)
        entity->velocity.x -= entity->velocity.x >> 5;

    if (entity->velocity.x <= -entity->topSpeed) {
        if (entity->left) {
            entity->direction = FLIP_X;
        }
    }
    else {
        if (entity->left) {
            entity->velocity.x -= entity->airAcceleration;
            entity->direction = FLIP_X;
        }
    }

    if (entity->velocity.x >= entity->topSpeed) {
        if (entity->right)
            entity->direction = FLIP_NONE;
    }
    else if (entity->right) {
        entity->direction = 0;
        entity->velocity.x += entity->airAcceleration;
    }
}
void Player_StartJump(EntityPlayer *entity)
{
    entity->controlLock = 0;
    entity->onGround    = 0;
    if (entity->collisionMode == CMODE_FLOOR && entity->state != Player_State_Roll)
        entity->position.y += entity->cameraOffset;

    entity->velocity.x = (entity->gravityStrength + entity->jumpStrength) * RSDK.Sin256(entity->angle);
    entity->velocity.x = (entity->groundVel * RSDK.Cos256(entity->angle) + entity->velocity.x) >> 8;
    entity->velocity.y = entity->groundVel * RSDK.Sin256(entity->angle);
    entity->velocity.y = (entity->velocity.y - (entity->gravityStrength + entity->jumpStrength) * RSDK.Cos256(entity->angle)) >> 8;
    if (entity->camera) {
        entity->camera->offsetYFlag = true;
        entity->camera->offset.y    = 0x200000;
    }
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
    if (entity->characterID == ID_TAILS) {
        entity->playerAnimator.animationSpeed = 120;
    }
    else {
        entity->playerAnimator.animationSpeed = ((abs(entity->groundVel) * 0xF0) / 0x60000) + 0x30;
    }
    if (entity->playerAnimator.animationSpeed > 0xF0)
        entity->playerAnimator.animationSpeed = 0xF0;
    entity->angle            = 0;
    entity->collisionMode    = CMODE_FLOOR;
    entity->skidding         = 0;
    entity->jumpAbility      = 1;
    entity->jumpAbilityTimer = 1;
    RSDK.PlaySfx(Player->sfx_Jump, false, 255);
    entity->state = Player_State_Air;
}
void Player_StartRoll(void)
{
    RSDK_THIS(Player);
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
    entity->pushing = 0;
    entity->state   = Player_State_Roll;
    if (entity->collisionMode == CMODE_FLOOR) {
        entity->position.y += entity->cameraOffset;
    }
}
void Player_StartPeelout(void)
{
    RSDK_THIS(Player);
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, true, 0);
    entity->state          = Player_State_Peelout;
    entity->abilityTimer   = 0;
    entity->spindashCharge = 0;
    RSDK.PlaySfx(Player->sfx_PeelCharge, false, 255);
}
#if RETRO_USE_PLUS
bool32 Player_SwapMainPlayer(bool32 flag)
{
    RSDK_THIS(Player);
    EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    if (!sidekick->objectID)
        return false;

    if (!flag) {
        if (Player->cantSwap || entity->drawOrder == 2 || !RSDK_sceneInfo->timeEnabled)
            return false;
        if (Player->jumpInDelay) {
            Player->jumpInDelay = 240;
            return false;
        }

        if (entity->state == Player_State_Die || entity->state == Player_State_Drown) {
            if (sidekick->state == Player_State_Die || sidekick->state == Player_State_Drown || sidekick->state == Player_State_EncoreRespawn
                || sidekick->state == Player_State_None) {
                return false;
            }
        }
        else {
            if (entity->state != Player_State_Ground && entity->state != Player_State_Roll)
                return false;
            if (sidekick->state != Player_State_Ground && sidekick->state != Player_State_Roll)
                return false;
        }
    }
    EntityPlayer *leader             = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *sidekickCam        = sidekick->camera;
    int32 sidekickController         = sidekick->controllerID;
    void (*sidekickInputState)(void) = sidekick->stateInput;
    if (sidekick->state == Ice_State_FrozenPlayer) {
        Ice_Unknown8((Entity *)sidekick);
        Ice->playerTimers[0] = 30;
        Ice->playerTimers[1] = 0;
        foreach_all(Ice, ice)
        {
            if (ice->state == Ice_State_PlayerBlock)
                destroyEntity(ice);
        }
    }
    RSDK.CopyEntity(Zone->entityData, leader, 0);
    RSDK.CopyEntity(leader, sidekick, 0);
    RSDK.CopyEntity(sidekick, (Entity *)Zone->entityData, 0);

    sidekick->state         = Player_State_Air;
    entity->controllerID    = sidekick->controllerID;
    entity->stateInput      = sidekick->stateInput;
    entity->sidekick        = false;
    entity->score           = sidekick->score;
    entity->score1UP        = sidekick->score1UP;
    entity->rings           = sidekick->rings;
    entity->ringExtraLife   = sidekick->ringExtraLife;
    entity->shield          = sidekick->shield;
    entity->camera          = sidekick->camera;
    entity->invincibleTimer = sidekick->invincibleTimer;
    entity->speedShoesTimer = sidekick->speedShoesTimer;
    entity->hyperRing       = sidekick->hyperRing;
    entity->playerID        = sidekick->playerID;
    entity->scrollDelay     = 0;
    entity->camera->state   = Camera_State_Follow;
    Player_ChangePhysicsState(entity);
    sidekick->controllerID    = sidekickController;
    sidekick->stateInput      = sidekickInputState;
    sidekick->sidekick        = true;
    sidekick->score           = 0;
    sidekick->score1UP        = 0;
    sidekick->rings           = 0;
    sidekick->ringExtraLife   = 0;
    sidekick->shield          = SHIELD_NONE;
    sidekick->invincibleTimer = 0;
    sidekick->speedShoesTimer = 0;
    sidekick->camera          = sidekickCam;
    sidekick->hyperRing       = false;
    sidekick->playerID        = RSDK.GetEntityID(sidekick);
    Player_ChangePhysicsState(sidekick);
    if (sidekick->superState == SUPERSTATE_SUPER)
        Player_CheckGoSuper(entity, 0xFF);
    globals->playerID = entity->characterID + (sidekick->characterID << 8);
    RSDK.PlaySfx(Player->sfx_Swap, false, 255);
    HUD->swapCooldown = 60;
    return true;
}
#endif

// Player States
void Player_HandleRollDeceleration(void)
{
    RSDK_THIS(Player);
    int32 initialVel = entity->groundVel;
    if (entity->right && entity->groundVel < 0)
        entity->groundVel += entity->rollingDeceleration;

    if (entity->left && entity->groundVel > 0)
        entity->groundVel -= entity->rollingDeceleration;

    if (entity->groundVel) {
        if (entity->groundVel < 0) {
            entity->groundVel += entity->rollingFriction;

            if (RSDK.Sin256(entity->angle) >= 0)
                entity->groundVel += 0x1400 * RSDK.Sin256(entity->angle) >> 8;
            else
                entity->groundVel += 0x5000 * RSDK.Sin256(entity->angle) >> 8;

            if (entity->groundVel < -0x120000)
                entity->groundVel = -0x120000;
        }
        else {
            entity->groundVel -= entity->rollingFriction;
            if (RSDK.Sin256(entity->angle) <= 0)
                entity->groundVel += 0x1400 * RSDK.Sin256(entity->angle) >> 8;
            else
                entity->groundVel += 0x5000 * RSDK.Sin256(entity->angle) >> 8;

            if (entity->groundVel > 0x120000)
                entity->groundVel = 0x120000;
        }
    }
    else {
        entity->groundVel += 0x5000 * RSDK.Sin256(entity->angle) >> 8;
    }

    switch (entity->collisionMode) {
        case CMODE_FLOOR:
            if (entity->state == Player_State_ForceRoll_Ground) {
                if (abs(entity->groundVel) < 0x10000) {
                    if (entity->direction & FLIP_Y)
                        entity->groundVel = -0x40000;
                    else
                        entity->groundVel = 0x40000;
                }
            }
            else {
                if ((entity->groundVel >= 0 && initialVel <= 0) || (entity->groundVel <= 0 && initialVel >= 0)) {
                    entity->groundVel = 0;
                    entity->state     = Player_State_Ground;
                }
            }
            break;
        case CMODE_LWALL:
        case CMODE_RWALL:
            if (entity->angle >= 0x40 && entity->angle <= 0xC0) {
                if (abs(entity->groundVel) < 0x20000) {
                    entity->velocity.x    = entity->groundVel * RSDK.Cos256(entity->angle) >> 8;
                    entity->velocity.y    = entity->groundVel * RSDK.Sin256(entity->angle) >> 8;
                    entity->onGround      = false;
                    entity->angle         = 0;
                    entity->collisionMode = CMODE_FLOOR;
                }
            }
            break;
        case CMODE_ROOF:
            if (entity->invertGravity) {
                if ((entity->groundVel >= 0 && initialVel <= 0) || (entity->groundVel <= 0 && initialVel >= 0)) {
                    entity->groundVel = 0;
                    entity->state     = Player_State_Ground;
                }
            }
            else {
                if (entity->angle >= 0x40 && entity->angle <= 0xC0) {
                    if (abs(entity->groundVel) < 0x20000) {
                        entity->velocity.x    = entity->groundVel * RSDK.Cos256(entity->angle) >> 8;
                        entity->velocity.y    = entity->groundVel * RSDK.Sin256(entity->angle) >> 8;
                        entity->onGround      = false;
                        entity->angle         = 0;
                        entity->collisionMode = CMODE_FLOOR;
                    }
                }
            }
            break;
    }
}
void Player_Hit(EntityPlayer *player)
{
    uint8 flag = 0;
    if (player->sidekick) {
        flag = 1;
    }
    else {
        int32 entityID       = RSDK.GetEntityID(player);
        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + entityID, Shield);
        if (shield->objectID == Shield->objectID) {
            player->shield = SHIELD_NONE;
            destroyEntity(shield);
            flag = 1;
        }
        else {
            flag = (player->rings <= 0) + 2;
        }
        if (!Player->gotHit[player->playerID])
            Player->gotHit[player->playerID] = true;
    }

    player->nextAirState    = StateMachine_None;
    player->nextGroundState = StateMachine_None;
    if (globals->coolBonus[player->playerID] > 0) {
        globals->coolBonus[player->playerID] -= 1000;
    }

    switch (flag) {
        default: break;
        case 1: // Hurt, no rings (shield/p2/etc)
            player->state = Player_State_Hit;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
            player->velocity.y     = -0x40000;
            player->onGround       = 0;
            player->tileCollisions = 1;
            player->blinkTimer     = 120;
            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y = -0x20000;
            }
            RSDK.PlaySfx(Player->sfx_Hurt, false, 0xFF);
            break;
        case 2: // Hurt, lost rings
            player->state = Player_State_Hit;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
            player->velocity.y     = -0x40000;
            player->onGround       = 0;
            player->tileCollisions = 1;
            player->blinkTimer     = 120;
            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y = -0x20000;
            }

            if (player->hyperRing)
                Player_LoseHyperRings(player, player->rings, player->collisionPlane);
            else
                Player_LoseRings(player, player->rings, player->collisionPlane);
            player->hyperRing     = 0;
            player->rings         = 0;
            player->ringExtraLife = 100;
            RSDK.PlaySfx(Player->sfx_LoseRings, false, 0xFF);
            break;
        case 3: // Hurt, dies.
            player->hurtFlag = HURTFLAG_DIE;
            break;
    }
}
bool32 Player_CheckValidState(EntityPlayer *player)
{
    if (player->objectID == Player->objectID && !player->hurtFlag) {
        if (player->state != Player_State_EncoreRespawn && player->state != Player_State_Die && player->state != Player_State_Drown
            && player->state != Player_State_StartJumpIn && player->state != Player_State_FlyIn && player->state != Player_State_JumpIn
            && player->state != Player_State_Transform) {
            return true;
        }
    }
    return false;
}
void Player_CheckStartFlyCarry(EntityPlayer *player1)
{
    RSDK_THIS(Player);

    if (entity->flyCarryTimer > 0)
        entity->flyCarryTimer--;

    int32 off;
    if (player1->playerAnimator.animationID == ANI_JUMP)
        off = entity->position.y + 0x210000;
    else
        off = entity->position.y + 0x1C0000;

    if (player1->state != Player_State_FlyCarried && (!player1->onGround || entity->velocity.y < 0)) {
        bool32 flag = (player1->state == Player_State_Roll || player1->state == Player_State_LookUp || player1->state == Player_State_Crouch
                       || player1->state == Player_State_Air || player1->state == Player_State_Ground);
        if (LottoMachine) {
            flag = (!((1 << RSDK.GetEntityID(player1)) & LottoMachine->activePlayers)) && flag;
        }

        if (flag && (player1->playerAnimator.animationID != ANI_FAN)) {
            if (abs(entity->position.x - player1->position.x) < 0xC0000) {
                if (abs(off - player1->position.y) < 0xC0000 && !entity->flyCarryTimer && !player1->down && !player1->onGround) {
                    RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_HANG, &player1->playerAnimator, false, 0);
                    player1->state           = Player_State_FlyCarried;
                    player1->nextAirState    = StateMachine_None;
                    player1->nextGroundState = StateMachine_None;
                    RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                }
            }
        }
    }

    if (player1->state == Player_State_FlyCarried) {
        int32 entityXPos        = entity->position.x;
        int32 entityYPos        = entity->position.y;
        int32 entityXVel        = entity->velocity.x;
        int32 entityYVel        = entity->velocity.y;
        Hitbox *player2OuterBox = RSDK.GetHitbox(&entity->playerAnimator, 0);
        Hitbox *player2InnerBox = RSDK.GetHitbox(&entity->playerAnimator, 1);
        if (entity->collisionLayers & Zone->moveID) {
            TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
            move->position.x = -entity->moveOffset.x >> 16;
            move->position.y = -entity->moveOffset.y >> 16;
        }
        RSDK.ProcessTileCollisions(entity, player2OuterBox, player2InnerBox);
        if (entity->onGround && !entity->collisionMode)
            entity->collisionFlagV |= 1;
        player1->sidekickPos.x = entity->position.x & 0xFFFF0000;
        player1->sidekickPos.y = entity->position.y & 0xFFFF0000;
        entity->velocity.y     = entityYVel;
        entity->position.x     = entityXPos;
        entity->position.y     = entityYPos;
        entity->velocity.x     = entityXVel;
        player1->velocity.y    = entityYVel;
        player1->position.y    = entityYPos;
        player1->position.y += 0x1C0000;
        player1->position.x = entityXPos;
        player1->velocity.x = entityXVel;

        Hitbox *player1OuterBox = RSDK.GetHitbox(&player1->playerAnimator, 0);
        Hitbox *player1InnerBox = RSDK.GetHitbox(&player1->playerAnimator, 1);
        Hitbox *defHitbox       = &defaultHitbox;
        if (!player1OuterBox)
            player1OuterBox = defHitbox;
        if (!player1InnerBox)
            player1InnerBox = defHitbox;
        if (player1->collisionLayers & Zone->moveID) {
            TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
            move->position.x = -player1->moveOffset.x >> 16;
            move->position.y = -player1->moveOffset.y >> 16;
        }
        RSDK.ProcessTileCollisions(player1, player1OuterBox, player1InnerBox);
        if (player1->onGround)
            player1->collisionFlagV |= 1;
        player1->collisionPlane = entity->collisionPlane;
        player1->direction      = entity->direction;
        player1->velocity.x     = 0;
        player1->velocity.y     = 0;
    }
}
void Player_P2JumpBackIn(void)
{
    RSDK_THIS(Player);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->objectID == Player->objectID && (player1->drawOrder != 2 || entity->drawOrder != 2)) {
        int32 rx = abs((entity->position.x >> 0x10) - RSDK_screens->position.x - RSDK_screens->centerX);
        int32 ry = abs((entity->position.y >> 0x10) - RSDK_screens->position.y - RSDK_screens->centerY);
        if (rx >= RSDK_screens->centerX + 96 || ry >= RSDK_screens->centerY + 96) {
            ++Player->jumpInDelay;
        }
        else {
            Player->jumpInDelay = 0;
        }

        if (Player->jumpInDelay >= 240) {
            Player->jumpInDelay      = 0;
            entity->state            = Player_State_StartJumpIn;
            entity->forceJumpIn      = true;
            entity->position.x       = -0x400000;
            entity->position.y       = -0x400000;
            entity->abilityValues[0] = 0;
            entity->drawFX &= ~FX_SCALE;
            entity->nextAirState    = StateMachine_None;
            entity->nextGroundState = StateMachine_None;
            entity->stateInput      = Player_ProcessP2InputLag;
            entity->scale.x         = 0x200;
            entity->scale.y         = 0x200;
            entity->velocity.x      = 0;
            entity->velocity.y      = 0;
            entity->groundVel       = 0;
            entity->tileCollisions  = false;
            entity->interaction     = false;
            entity->blinkTimer      = 0;
            entity->visible         = true;
            if (player1->underwater && player1->position.y < Water->waterLevel) {
                entity->drawOrder = player1->drawOrder;
                entity->airTimer  = 0;
            }
            else {
                entity->drawOrder = Zone->playerDrawHigh;
                entity->airTimer  = 0;
            }
        }
    }
}
void Player_ForceSuperTransform(void)
{
    RSDK_THIS(Player);
    if (entity->characterID == ID_SONIC)
        entity->spriteIndex = Player->superSpriteIndex;
    entity->superState = SUPERSTATE_SUPER;
    destroyEntity(RSDK.GetEntityByID(entity->playerID + Player->playerCount));
    entity->invincibleTimer = 60;
    entity->superState      = SUPERSTATE_SUPER;
    Player_ChangePhysicsState(entity);
    RSDK.ResetEntityPtr(RSDK.GetEntityByID(entity->playerID + 2 * Player->playerCount), ImageTrail->objectID, entity);
    RSDK.ResetEntityPtr(RSDK.GetEntityByID(entity->playerID + Player->playerCount), SuperSparkle->objectID, entity);
    entity->state = Player_State_Ground;
    Player_State_Ground();
}

void Player_State_None(void)
{ /* oh yeah buddy, NOTHING!*/
}
void Player_State_Ground(void)
{
    RSDK_THIS(Player);
    if (!entity->onGround) {
        entity->state = Player_State_Air;
        Player_HandleAirMovement();
    }
    else {
        if (!entity->groundVel) {
            if (entity->left) {
                --entity->pushing;
            }
            else if (entity->right) {
                ++entity->pushing;
            }
            else {
                entity->pushing = 0;
            }
        }
        else {
            if (!entity->left && !entity->right) {
                entity->pushing = 0;
            }
            else {
                if (abs(entity->groundVel) > 0x10000)
                    entity->pushing = 0;
            }
        }

        if (entity->invertGravity) {
            if (entity->collisionMode != CMODE_ROOF) {
                entity->pushing = 0;
            }
        }
        else if (entity->collisionMode != CMODE_FLOOR) {
            entity->pushing = 0;
        }

        Player_HandleGroundRotation();
        Player_HandleGroundMovement();

        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;

        entity->nextAirState = Player_State_Air;
        if (entity->skidding <= 0) {
            if (entity->pushing > -3 && entity->pushing < 3) {
                if (entity->groundVel || (entity->angle >= 32 && entity->angle <= 224 && !entity->invertGravity)) {
                    entity->timer          = 0;
                    entity->outtaHereTimer = 0;
                    if (abs(entity->groundVel) >= entity->maxWalkSpeed) {
                        if (abs(entity->groundVel) >= entity->maxJogSpeed) {
                            if (abs(entity->groundVel) < entity->maxRunSpeed) {
                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_RUN, &entity->playerAnimator, false,
                                                        entity->playerAnimator.animationID - ANI_RUN > 1);

                                entity->playerAnimator.animationSpeed = (abs(entity->groundVel) >> 12);
                                if ((abs(entity->groundVel) >> 12) + 96 > 0x200)
                                    entity->playerAnimator.animationSpeed = 0x200;
                                entity->maxJogSpeed = 0x58000;
                                entity->maxRunSpeed = 0xC0000;
                            }
                            else {
                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DASH, &entity->playerAnimator, false,
                                                        entity->playerAnimator.animationID - ANI_RUN > 1);
                                entity->maxRunSpeed = 0xB8000;
                            }
                        }
                        else {
                            if (entity->playerAnimator.animationID != ANI_WALK || entity->playerAnimator.frameID == 3)
                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JOG, &entity->playerAnimator, false, 0);
                            entity->playerAnimator.animationSpeed = (abs(entity->groundVel) >> 12) + 64;
                            entity->maxWalkSpeed                  = 0x38000;
                            entity->maxJogSpeed                   = 0x60000;
                        }
                    }
                    else {
                        if (entity->playerAnimator.animationID == ANI_JOG) {
                            if (entity->playerAnimator.frameID == 9)
                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 9);
                        }
                        else if (entity->playerAnimator.animationID == ANI_AIRWALK) {
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, entity->playerAnimator.frameID);
                        }
                        else {
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 0);
                        }
                        entity->playerAnimator.animationSpeed = (abs(entity->groundVel) >> 12) + 48;
                        entity->maxWalkSpeed                  = 0x40000;
                    }
                }
                else {
                    entity->maxWalkSpeed = 0x40000;
                    entity->maxJogSpeed  = 0x60000;
                    entity->maxRunSpeed  = 0xC0000;
                    Vector2 posStore;
                    posStore = entity->position;
                    entity->flailing |= RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[0],
                                                            entity->sensorY, 10)
                                        << 0;
                    entity->flailing |= RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[1],
                                                            entity->sensorY, 10)
                                        << 1;
                    entity->flailing |= RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[2],
                                                            entity->sensorY, 10)
                                        << 2;
                    entity->flailing |= RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[3],
                                                            entity->sensorY, 10)
                                        << 3;
                    entity->flailing |= RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[4],
                                                            entity->sensorY, 10)
                                        << 4;

                    entity->position = posStore;
                    switch (entity->flailing - 1) {
                        case 0:
                        case 2:
                            if (entity->direction == FLIP_X || (entity->characterID == ID_SONIC && entity->superState == SUPERSTATE_SUPER)
                                || entity->isChibi) {
                                entity->direction = FLIP_X;
                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimator, false, 0);
                            }
                            else {
                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE2, &entity->playerAnimator, false, 0);
                            }
                            break;
                        case 15:
                        case 23:
                            if (entity->direction && (entity->characterID != ID_SONIC || entity->superState != SUPERSTATE_SUPER)
                                && !entity->isChibi) {
                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE2, &entity->playerAnimator, false, 0);
                            }
                            else {
                                entity->direction = FLIP_NONE;
                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimator, false, 0);
                            }
                            break;
                        case 110:
                        case 115: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimator, false, 0); break;
                        default:
                            switch (entity->characterID) {
                                case ID_SONIC:
#if !RETRO_USE_PLUS
                                    // pre-1.05 super sonic didn't have a "bored" anim
                                    if (entity->superState != SUPERSTATE_SUPER) {
#endif
                                        if (entity->timer != 720 || entity->isChibi || entity->superState == SUPERSTATE_SUPER) {
                                            if (entity->timer < 240) {
                                                entity->timer++;
                                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimator, false, 0);
                                            }
                                            else {
                                                entity->timer++;
                                                if (entity->playerAnimator.animationID == ANI_BORED1) {
                                                    if (entity->playerAnimator.frameID == 41)
                                                        entity->timer = 0;
                                                }
                                                else
                                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimator, false, 0);
                                            }
                                        }
                                        else {
                                            if (entity->playerAnimator.animationID == ANI_BORED2) {
                                                if (entity->playerAnimator.frameID == 67)
                                                    entity->timer = 0;
                                            }
                                            else
                                                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED2, &entity->playerAnimator, false, 0);
                                        }
#if !RETRO_USE_PLUS
                                    }
                                    else {
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimator, false, 0);
                                    }
#endif
                                    break;
                                case ID_TAILS:
                                    if (entity->timer < 240) {
                                        entity->timer++;
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimator, false, 0);
                                    }
                                    else if (entity->playerAnimator.animationID == ANI_BORED1) {
                                        if (entity->playerAnimator.frameID == 45)
                                            entity->timer = 0;
                                    }
                                    else
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimator, false, 0);
                                    break;
                                case ID_KNUCKLES:
                                    if (entity->timer < 240) {
                                        entity->timer++;
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimator, false, 0);
                                    }
                                    else if (entity->playerAnimator.animationID == ANI_BORED1) {
                                        if (entity->playerAnimator.frameID == 69)
                                            entity->timer = 0;
                                    }
                                    else
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimator, false, 0);
                                    break;
#if RETRO_USE_PLUS
                                case ID_MIGHTY:
                                    if (entity->timer < 240) {
                                        entity->timer++;
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimator, false, 0);
                                    }
                                    else if (entity->playerAnimator.animationID == ANI_BORED1) {
                                        if (entity->playerAnimator.frameID == 35)
                                            entity->timer = 0;
                                    }
                                    else
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimator, false, 0);
                                    break;
                                case ID_RAY:
                                    if (entity->timer < 240) {
                                        entity->timer++;
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimator, false, 0);
                                    }
                                    else if (entity->playerAnimator.animationID == ANI_BORED1) {
                                        if (entity->playerAnimator.frameID == 55)
                                            entity->timer = 0;
                                    }
                                    else {
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimator, false, 0);
                                    }
                                    break;
#endif
                                default: break;
                            }
                            break;
                    }

                    if (++entity->outtaHereTimer >= 72000000) {
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_OUTTAHERE, &entity->playerAnimator, false, 0);
                        entity->state           = Player_State_OuttaHere;
                        entity->tileCollisions  = false;
                        entity->interaction     = false;
                        entity->nextAirState    = StateMachine_None;
                        entity->nextGroundState = StateMachine_None;
                        entity->velocity.x      = 0;
                        entity->velocity.y      = 0;
                        RSDK.PlaySfx(Player->sfx_Outtahere, false, 255);
                    }
                }
            }
            else {
                if (entity->pushing < -3)
                    entity->pushing = -3;
                if (entity->pushing > 3)
                    entity->pushing = 3;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_PUSH, &entity->playerAnimator, false, 0);
            }
        }
        else {
            if (entity->playerAnimator.animationID != ANI_SKID) {
                if (entity->playerAnimator.animationID == ANI_SKIDTURN) {
                    if (entity->playerAnimator.frameID == entity->playerAnimator.frameCount - 1) {
                        entity->direction ^= FLIP_X;
                        entity->skidding = 1;
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 0);
                    }
                }
                else {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SKID, &entity->playerAnimator, false, 0);
                    if (abs(entity->groundVel) >= 0x60000) {
                        if (abs(entity->groundVel) >= 0xA0000)
                            entity->playerAnimator.animationSpeed = 64;
                        else
                            entity->playerAnimator.animationSpeed = 144;
                    }
                    else {
                        entity->skidding -= 8;
                    }
                    RSDK.PlaySfx(Player->sfx_Skidding, false, 255);
                    EntityDust *dust = CREATE_ENTITY(Dust, entity, entity->position.x, entity->position.y);
                    dust->state      = Dust_State_Skid;
                }
            }
            else {
                int32 spd = entity->playerAnimator.animationSpeed;
                if (entity->direction) {
                    if (entity->groundVel >= 0) {
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SKIDTURN, &entity->playerAnimator, false, 0);
                    }
                }
                else if (entity->groundVel <= 0) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SKIDTURN, &entity->playerAnimator, false, 0);
                }
                entity->playerAnimator.animationSpeed = spd;
            }
            --entity->skidding;
        }

        if (entity->jumpPress) {
            Player_StartJump(entity);
            entity->timer = 0;
        }
        else {
            if (entity->groundVel) {
                int32 cap = 0x8800;
                if (entity->state == Player_State_Crouch)
                    cap = 0x11000;
                if (abs(entity->groundVel) >= cap && !entity->left && !entity->right && entity->down) {
                    Player_StartRoll();
                    RSDK.PlaySfx(Player->sfx_Roll, false, 255);
                }
            }
            else {
                if (((entity->angle < 0x20 || entity->angle > 0xE0) && !entity->collisionMode) || (entity->invertGravity && entity->angle == 0x80)) {
                    if (entity->up) {
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_LOOKUP, &entity->playerAnimator, true, 1);
                        entity->timer = 0;
                        entity->state = Player_State_LookUp;
                    }
                    else if (entity->down) {
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_CROUCH, &entity->playerAnimator, true, 1);
                        entity->timer = 0;
                        entity->state = Player_State_Crouch;
                    }
                }
            }
        }
    }
}
void Player_State_Air(void)
{
    RSDK_THIS(Player);
    entity->tileCollisions = true;
    Player_HandleAirFriction();
    if (entity->onGround) {
        entity->state = Player_State_Ground;
        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;
    }
    else {
        Player_HandleAirMovement();
        entity->nextGroundState = Player_State_Ground;
        if (entity->velocity.y > 0) {
            if (entity->playerAnimator.animationID >= ANI_SPRINGTWIRL) {
                if (entity->playerAnimator.animationID <= ANI_SPRINGDIAGONAL) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, entity->storedAnim, &entity->playerAnimator, false, 0);
                }
                else if (entity->playerAnimator.animationID == ANI_SPRINGCS && !entity->playerAnimator.frameID) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimator, false, 0);
                }
            }
        }

        switch (entity->playerAnimator.animationID) {
            case ANI_IDLE:
            case ANI_WALK:
                if (entity->playerAnimator.animationSpeed < 64)
                    entity->playerAnimator.animationSpeed = 64;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimator, false, entity->playerAnimator.frameID);
                break;
            case ANI_LOOKUP:
            case ANI_CROUCH:
            case ANI_SKIDTURN: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimator, false, 0); break;
            case ANI_JOG: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimator, false, 0); break;
            case ANI_JUMP:
                if (entity->velocity.y >= entity->jumpCap)
                    StateMachine_Run(entity->movesetState);
                break;
            case ANI_SKID:
                if (entity->skidding < 1) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimator, false, entity->playerAnimator.frameID);
                }
                else {
                    entity->skidding--;
                }
                break;
            case ANI_SPINDASH: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0); break;
            default: break;
        }
    }
}
void Player_State_Roll(void)
{
    RSDK_THIS(Player);
    Player_HandleGroundRotation();
    Player_HandleRollDeceleration();
    entity->jumpAbility = 0;
    if (!entity->onGround) {
        entity->state = Player_State_Air;
        Player_HandleAirMovement();
    }
    else {
        if (entity->characterID == ID_TAILS) {
            entity->playerAnimator.animationSpeed = 120;
        }
        else {
            entity->playerAnimator.animationSpeed = ((abs(entity->groundVel) * 0xF0) / 0x60000) + 0x30;
        }
        if (entity->playerAnimator.animationSpeed > 0xF0)
            entity->playerAnimator.animationSpeed = 0xF0;
        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;
        if (entity->jumpPress)
            Player_StartJump(entity);
    }
}
void Player_State_ForceRoll_Ground(void)
{
    RSDK_THIS(Player);
    Player_HandleGroundRotation();
    bool32 left  = entity->left;
    bool32 right = entity->right;
    if (entity->controlLock > 0) {
        entity->left  = false;
        entity->right = false;
        entity->controlLock--;
    }
    Player_HandleRollDeceleration();
    entity->left        = left;
    entity->right       = right;
    entity->jumpAbility = 0;
    if (!entity->onGround) {
        entity->state = Player_State_ForceRoll_Air;
        Player_HandleAirMovement();
    }
    else {
        if (entity->characterID == ID_TAILS) {
            entity->playerAnimator.animationSpeed = 120;
        }
        else {
            entity->playerAnimator.animationSpeed = ((abs(entity->groundVel) * 0xF0) / 0x60000) + 0x30;
        }
        if (entity->playerAnimator.animationSpeed > 0xF0)
            entity->playerAnimator.animationSpeed = 0xF0;
        if (entity->camera) {
            entity->camera->offsetYFlag = false;
        }
        entity->jumpAbilityTimer = 0;
    }
}
void Player_State_ForceRoll_Air(void)
{
    RSDK_THIS(Player);
    Player_HandleGroundRotation();
    bool32 left  = entity->left;
    bool32 right = entity->right;
    if (entity->controlLock > 0) {
        entity->left  = false;
        entity->right = false;
        entity->controlLock--;
    }
    Player_HandleAirFriction();

    entity->left            = left;
    entity->right           = right;
    entity->jumpAbility     = 0;
    entity->nextGroundState = Player_State_ForceRoll_Ground;
    if (!entity->onGround) {
        Player_HandleAirMovement();
    }
    else {
        entity->state = Player_State_ForceRoll_Ground;
        if (entity->camera) {
            entity->camera->offsetYFlag = false;
        }
        entity->jumpAbilityTimer = 0;
    }
}
void Player_State_LookUp(void)
{
    RSDK_THIS(Player);
    if (entity->invertGravity)
        entity->rotation = 0x80;
    entity->left  = 0;
    entity->right = 0;
    Player_HandleGroundMovement();
    if (entity->camera)
        entity->camera->offsetYFlag = false;
    entity->jumpAbilityTimer = 0;
    entity->nextAirState     = Player_State_Air;
    if (entity->up) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_LOOKUP, &entity->playerAnimator, false, 1);
        if (entity->playerAnimator.frameID == 5)
            entity->playerAnimator.animationSpeed = 0;

        if (entity->timer < 60) {
            entity->timer++;
        }
        else if (entity->camera) {
            if (entity->invertGravity) {
                if (entity->camera->lookPos.y < 96)
                    entity->camera->lookPos.y += 2;
            }
            else {
                if (entity->camera->lookPos.y > -96)
                    entity->camera->lookPos.y -= 2;
            }
        }
        if (entity->jumpPress) {
            if (entity->peeloutState) {
                StateMachine_Run(entity->peeloutState);
            }
            else {
                Player_StartJump(entity);
            }
        }
    }
    else {
        entity->playerAnimator.animationSpeed = 64;
        if (entity->playerAnimator.frameID == 0 || entity->left || entity->right)
            entity->state = Player_State_Ground;
        if (entity->jumpPress)
            Player_StartJump(entity);
    }
}
void Player_State_Crouch(void)
{
    RSDK_THIS(Player);
    if (entity->invertGravity)
        entity->rotation = 0x80;
    entity->left  = 0;
    entity->right = 0;
    Player_HandleGroundMovement();
    if (entity->camera)
        entity->camera->offsetYFlag = false;
    entity->jumpAbilityTimer = 0;
    entity->nextAirState     = Player_State_Air;
    if (entity->down) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_CROUCH, &entity->playerAnimator, false, 1);
        if (entity->playerAnimator.frameID == 4)
            entity->playerAnimator.animationSpeed = 0;

        if (entity->timer < 60) {
            entity->timer++;
        }
        else if (entity->camera) {
            if (entity->invertGravity) {
                if (entity->camera->lookPos.y > -96)
                    entity->camera->lookPos.y -= 2;
            }
            else {
                if (entity->camera->lookPos.y < 96)
                    entity->camera->lookPos.y += 2;
            }
        }

        if (entity->jumpPress) {
            EntityDust *dust = CREATE_ENTITY(Dust, entity, entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(Dust->spriteIndex, 1, &dust->animator, true, 0);
            dust->state     = Dust_State_Spindash;
            dust->drawOrder = entity->drawOrder;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SPINDASH, &entity->playerAnimator, true, 0);
            entity->state          = Player_State_Spindash;
            entity->abilityTimer   = 0;
            entity->spindashCharge = 0;
            RSDK.PlaySfx(Player->sfx_Charge, false, 255);
        }
    }
    else {
        entity->playerAnimator.animationSpeed = 128;
        if (entity->playerAnimator.frameID == 0 || entity->left || entity->right)
            entity->state = Player_State_Ground;
        if (entity->jumpPress)
            Player_StartJump(entity);
    }
}
void Player_State_Spindash(void)
{
    RSDK_THIS(Player);
    float chargeSpeeds[13] = { 1.0,       1.0614005, 1.125531,  1.1926451, 1.2630343, 1.3370349, 1.4150375,
                               1.4974997, 1.5849625, 1.6780719, 1.7776076, 1.8845228, 2.00000000 };
    if (entity->jumpPress) {
        entity->abilityTimer += 0x20000;
        if (entity->abilityTimer > 0x90000)
            entity->abilityTimer = 0x90000;

        if (entity->spindashCharge < 12)
            entity->spindashCharge++;
        if (entity->spindashCharge < 0)
            entity->spindashCharge = 0;

        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SPINDASH, &entity->playerAnimator, true, 0);
        int32 channel = RSDK.PlaySfx(Player->sfx_Charge, false, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[entity->spindashCharge]);
    }
    else {
        entity->abilityTimer -= entity->abilityTimer >> 5;
    }

    if (!entity->down) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);

        if (entity->camera && !Zone->autoScrollSpeed) {
            entity->scrollDelay   = 15;
            entity->camera->state = Camera_State_VLock;
        }

        int32 vel = 0;
        if (entity->superState == SUPERSTATE_SUPER)
            vel = (((uint32)entity->abilityTimer >> 1) & 0x7FFF8000) + 0xB0000;
        else
            vel = (((uint32)entity->abilityTimer >> 1) & 0x7FFF8000) + 0x80000;

        if (entity->direction)
            entity->groundVel = -vel;
        else
            entity->groundVel = vel;

        RSDK.StopSFX(Player->sfx_Charge);
        RSDK.PlaySfx(Player->sfx_Release, false, 0xFF);
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
        if (!entity->collisionMode) {
            entity->position.y += entity->cameraOffset;
        }
        entity->pushing = 0;
        entity->state   = Player_State_Roll;
    }
}
void Player_State_Peelout(void)
{
    RSDK_THIS(Player);
    if (entity->gravityStrength == 0x1000) {
        if (entity->superState == SUPERSTATE_SUPER) {
            entity->abilityTimer += 0xA000;
            if (entity->abilityTimer > 0x70000)
                entity->abilityTimer = 0x70000;
        }
        else {
            entity->abilityTimer += 0x6000;
            if (entity->abilityTimer > 0x60000)
                entity->abilityTimer = 0x60000;
        }
    }
    else if (entity->superState == SUPERSTATE_SUPER) {
        entity->abilityTimer += 0xC000;
        if (entity->abilityTimer > 0xD0000)
            entity->abilityTimer = 0xD0000;
    }
    else {
        entity->abilityTimer += 0x8000;
        if (entity->abilityTimer > 0xC0000)
            entity->abilityTimer = 0xC0000;
    }

    if (entity->abilityTimer >= entity->maxWalkSpeed) {
        if (entity->abilityTimer >= entity->maxJogSpeed) {
            if (entity->abilityTimer < entity->maxRunSpeed) {
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JOG, &entity->playerAnimator, false, entity->playerAnimator.animationID - 8 > 1);
                entity->playerAnimator.animationSpeed = (entity->abilityTimer >> 12) + 96;
                if (entity->playerAnimator.animationSpeed > 0x200)
                    entity->playerAnimator.animationSpeed = 512;
                entity->maxRunSpeed = 0xC0000;
                entity->maxJogSpeed = 0x58000;
            }
            else {
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DASH, &entity->playerAnimator, false, entity->playerAnimator.animationID - 8 > 1);
                entity->maxRunSpeed = 0xB8000;
            }
        }
        else {
            if (entity->playerAnimator.animationID != ANI_WALK || entity->playerAnimator.frameID == 3)
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_RUN, &entity->playerAnimator, false, 0);
            entity->playerAnimator.animationSpeed = (entity->abilityTimer >> 12) + 64;
            entity->maxWalkSpeed                  = 0x38000;
            entity->maxJogSpeed                   = 0x60000;
        }
    }
    else {
        if (entity->playerAnimator.animationID == ANI_JOG) {
            if (entity->playerAnimator.frameID == ANI_DASH)
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 9);
        }
        else if (entity->playerAnimator.animationID == ANI_AIRWALK) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, entity->playerAnimator.frameID);
        }
        else {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 0);
        }
        entity->playerAnimator.animationSpeed = (entity->abilityTimer >> 12) + 48;
        entity->maxWalkSpeed                  = 0x40000;
    }

    if (!entity->up) {
        RSDK.StopSFX(Player->sfx_PeelCharge);
        if (entity->abilityTimer >= 0x60000) {
            if (entity->camera && !Zone->autoScrollSpeed) {
                entity->scrollDelay   = 15;
                entity->camera->state = Camera_State_VLock;
            }

            if (entity->direction)
                entity->groundVel = -entity->abilityTimer;
            else
                entity->groundVel = entity->abilityTimer;
            RSDK.PlaySfx(Player->sfx_PeelRelease, false, 0xFF);
        }
        entity->state = Player_State_Ground;
    }
}
void Player_State_OuttaHere(void)
{
    RSDK_THIS(Player);
    if (entity->onGround) {
        if (entity->playerAnimator.frameID == 14) {
            entity->jumpAbility = 0;
            if (entity->direction == FLIP_X)
                entity->velocity.x = -0x10000;
            else
                entity->velocity.x = 0x10000;
            entity->velocity.y = -0x58000;
            entity->onGround   = false;
            entity->stateInput = StateMachine_None;
            if (entity->camera)
                entity->camera->state = StateMachine_None;
        }
    }
    else {
        Player_HandleAirMovement();
        if (entity->velocity.y > 0x100000) {
            RSDK.ResetEntitySlot(SLOT_GAMEOVER, GameOver->objectID, intToVoid(false));
            destroyEntity(entity);
        }
    }
}
void Player_State_Transform(void)
{
    RSDK_THIS(Player);
    entity->position.x += Zone->autoScrollSpeed;
    entity->invincibleTimer = 60;
    RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, true);
    if (++entity->timer != 36) {
        if (!entity->isChibi) {
            if (entity->playerAnimator.frameID == entity->playerAnimator.frameCount - 1) {
                entity->forceTransform = false;
                entity->interaction    = true;
                entity->state          = Player_State_Air;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 3);

#if RETRO_USE_PLUS
                if (!ERZStart && globals->superMusicEnabled)
#else
                if (!ERZStart)
#endif
                    Music_PlayMusicTrack(TRACK_SUPER);
            }
        }
    }
    else {
        EntityImageTrail *trail = RSDK_GET_ENTITY(2 * Player->playerCount + entity->playerID, ImageTrail);
        RSDK.ResetEntityPtr(trail, ImageTrail->objectID, entity);
        if (ERZStart)
            entity->shield = SHIELD_LIGHTNING;
        RSDK.PlaySfx(Player->sfx_Transform2, false, 255);

        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + entity->playerID, Shield);
        RSDK.ResetEntityPtr(shield, SuperSparkle->objectID, entity);
        entity->superState = SUPERSTATE_SUPER;
        Player_ChangePhysicsState(entity);

        if (!entity->isChibi) {
            if (entity->playerAnimator.frameID == entity->playerAnimator.frameCount - 1) {
                entity->forceTransform = false;
                entity->interaction    = true;
                entity->state          = Player_State_Air;

#if RETRO_USE_PLUS
                if (!ERZStart && globals->superMusicEnabled)
#else
                if (!ERZStart)
#endif
                    Music_PlayMusicTrack(TRACK_SUPER);
            }
        }
        else {
            entity->forceTransform = false;
            entity->interaction    = true;
            entity->state          = Player_State_Air;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimator, false, 3);
            Music_PlayMusicTrack(TRACK_SUPER);
        }
    }
}
void Player_State_Hit(void)
{
    RSDK_THIS(Player);
    if (entity->onGround) {
        entity->state = Player_State_Ground;
        if (entity->velocity.x >= -0x20000) {
            if (entity->velocity.x <= 0x20000)
                entity->groundVel = 0;
            else
                entity->groundVel -= 0x20000;
        }
        else {
            entity->groundVel += 0x20000;
        }
        entity->pushing     = 0;
        entity->controlLock = 0;
        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;
        entity->skidding         = 0;
    }
    else {
        if (entity->camera) {
            entity->camera->offsetYFlag = true;
            entity->camera->offset.y    = 0x200000;
        }
        entity->skidding = 0;
        if (!entity->underwater)
            entity->velocity.y += 0x3000;
        else
            entity->velocity.y += 0xF00;
    }
}
void Player_State_Die(void)
{
    RSDK_THIS(Player);
    if (entity->superState == SUPERSTATE_SUPER)
        entity->superState = SUPERSTATE_FADEOUT;

    if (entity->blinkTimer) {
        entity->blinkTimer = 0;
        entity->visible    = true;
    }
    entity->velocity.y += entity->gravityStrength;
    entity->velocity.x = 0;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DIE, &entity->playerAnimator, false, 0);

    if (!entity->sidekick) {
        if (entity->camera) {
            int32 y = entity->camera->position.y - entity->camera->field_6C.y + 0x1800000;
            if (entity->position.y > y)
                entity->position.y = y;
            entity->scrollDelay = 2;
        }
    }

    if (entity->velocity.y > 0x100000)
        Player_HandleDeath(entity);
}
void Player_State_Drown(void)
{
    RSDK_THIS(Player);
    if (entity->superState == SUPERSTATE_SUPER)
        entity->superState = SUPERSTATE_FADEOUT;

    if (entity->blinkTimer) {
        entity->blinkTimer = 0;
        entity->visible    = true;
    }
    entity->velocity.y += entity->gravityStrength;
    entity->velocity.x = 0;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DROWN, &entity->playerAnimator, false, 0);
    if (!entity->sidekick) {
        if (entity->camera) {
            int32 y = entity->camera->position.y + 0x1000000;
            if (entity->position.y > y)
                entity->position.y = y;
        }
    }
    if (entity->velocity.y > 0x100000)
        Player_HandleDeath(entity);
}
void Player_State_DropDash(void)
{
    RSDK_THIS(Player);
    if (entity->onGround) {
        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;
        if (entity->right)
            entity->direction = FLIP_NONE;
        if (entity->left)
            entity->direction = FLIP_X;

        int32 vel = 0;
        int32 cap = 0;
        if (entity->superState == SUPERSTATE_SUPER) {
            vel = 0xC0000;
            cap = 0xD0000;
            if (entity->camera)
                Camera_ShakeScreen(6, entity->camera->screenID, 6);
        }
        else {
            vel = 0x80000;
            cap = 0xC0000;
        }

        if (entity->direction) {
            if (entity->velocity.x <= 0) {
                entity->groundVel = (entity->groundVel >> 2) - vel;
                if (entity->groundVel < -cap)
                    entity->groundVel = -cap;
            }
            else if (entity->angle) {
                entity->groundVel = (entity->groundVel >> 1) - vel;
            }
            else {
                entity->groundVel = -vel;
            }
        }
        else {
            if (entity->velocity.x >= 0) {
                entity->groundVel = vel + (entity->groundVel >> 2);
                if (entity->groundVel > cap)
                    entity->groundVel = cap;
            }
            else if (entity->angle) {
                entity->groundVel = vel + (entity->groundVel >> 1);
            }
            else {
                entity->groundVel = vel;
            }
        }

        if (entity->camera && !Zone->autoScrollSpeed) {
            entity->scrollDelay   = 8;
            entity->camera->state = Camera_State_VLock;
        }
        EntityDust *dust = CREATE_ENTITY(Dust, entity, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Dust->spriteIndex, 2, &dust->animator, true, 0);
        dust->state = Dust_State_DropDash;
        dust->position.y += RSDK.GetHitbox(&entity->playerAnimator, 0)->bottom << 16;
        dust->direction = entity->direction;
        dust->drawOrder = entity->drawOrder;
        RSDK.StopSFX(Player->sfx_Dropdash);
        if (entity->superState == SUPERSTATE_SUPER)
            RSDK.PlaySfx(Player->sfx_PeelRelease, false, 255);
        else
            RSDK.PlaySfx(Player->sfx_Release, false, 255);

        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
        if (!entity->collisionMode) {
            entity->position.y += entity->cameraOffset;
        }
        entity->pushing = 0;
        entity->state   = Player_State_Roll;
    }
    else if (entity->jumpHold) {
        Player_HandleAirFriction();
        Player_HandleAirMovement();
        entity->playerAnimator.animationSpeed += 8;
        if (entity->playerAnimator.animationSpeed > 0x100)
            entity->playerAnimator.animationSpeed = 0x100;
    }
    else {
        entity->jumpAbilityTimer = 0;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
        entity->state = Player_State_Air;
    }
}
void Player_State_BubbleBounce(void)
{
    RSDK_THIS(Player);
    if (entity->shield == SHIELD_BUBBLE && !entity->invincibleTimer && entity->superState != SUPERSTATE_SUPER) {
        if (!entity->onGround) {
            Player_HandleAirFriction();
            Player_HandleAirMovement();
        }
        else {
            entity->controlLock = 0;
            int32 vel           = 0;
            if (!entity->underwater)
                vel = entity->gravityStrength + 0x78000;
            else
                vel = entity->gravityStrength + 0x40000;
            entity->onGround = false;

            entity->velocity.x = (entity->groundVel * RSDK.Cos256(entity->angle) + (vel * RSDK.Sin256(entity->angle))) >> 8;
            entity->velocity.y = (entity->groundVel * RSDK.Sin256(entity->angle) - (vel * RSDK.Cos256(entity->angle))) >> 8;

            if (entity->camera) {
                entity->camera->offsetYFlag = true;
                entity->camera->offset.y    = 0x200000;
            }

            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
            entity->playerAnimator.animationSpeed = (abs(entity->groundVel) >> 12) + 48;

            EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(entity), Shield);
            RSDK.SetSpriteAnimation(Shield->spriteIndex, 9, &shield->animator2, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &shield->animator, true, 0);
            shield->state = Shield_State_BubbleAlt;
            if (entity->playerAnimator.animationSpeed > 0xF0)
                entity->playerAnimator.animationSpeed = 0xF0;
            entity->angle            = 0;
            entity->collisionMode    = CMODE_FLOOR;
            entity->jumpAbility      = 0;
            entity->jumpAbilityTimer = 1;
            RSDK.PlaySfx(Shield->sfx_BubbleBounce, false, 255);
            entity->state = Player_State_Air;
        }
    }
    else {
        entity->state = Player_State_Air;
    }
}
void Player_State_TailsFlight(void)
{
    RSDK_THIS(Player);
    Player_HandleAirFriction();
    if (entity->onGround) {
        entity->state = Player_State_Ground;
        Player_HandleAirMovement();
    }
    else {
        if (entity->velocity.y < -0x10000) {
            entity->abilitySpeed = 0x800;
        }

        if (entity->velocity.y <= 0) {
            if (entity->abilityValue >= 60)
                entity->abilitySpeed = 0x800;
            else
                entity->abilityValue++;
        }

        uint8 slot = 0;
        entity->velocity.y += entity->abilitySpeed;
        if (!entity->sidekick)
            slot = RSDK_sceneInfo->entitySlot;
        if (entity->position.y < Zone->screenBoundsT2[slot] + 0x100000 && entity->velocity.y < 0)
            entity->velocity.y = 0;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (globals->gameMode != MODE_COMPETITION && !entity->isChibi && !player1->isChibi)
            Player_CheckStartFlyCarry(player1);

        if (entity->timer >= 480) {
            if (!entity->underwater) {
                if (player1->state == Player_State_FlyCarried)
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYLIFTTIRED, &entity->playerAnimator, false, 0);
                else
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
            }
            else {
                if (player1->state == Player_State_FlyCarried)
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIMLIFT, &entity->playerAnimator, false, 0);
                else
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIMTIRED, &entity->playerAnimator, false, 0);
            }
        }
        else {
            if (entity->underwater) {
                if (player1->state == Player_State_FlyCarried) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIMLIFT, &entity->playerAnimator, false, 0);
                }
                else {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIM, &entity->playerAnimator, false, 0);
                    entity->playerAnimator.animationSpeed = 128;
                    if (entity->velocity.y >= 0)
                        entity->playerAnimator.animationSpeed = 64;
                }
            }
            else {
                if (player1->state != Player_State_FlyCarried || entity->velocity.y >= 0)
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 0);
                else
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYLIFT, &entity->playerAnimator, false, 0);

                if (entity->velocity.y >= 0)
                    entity->playerAnimator.animationSpeed = 128;
                else
                    entity->playerAnimator.animationSpeed = 256;
            }

            if (++entity->timer == 480) {
                if (!entity->underwater) {
                    if (player1->state == Player_State_FlyCarried)
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYLIFTTIRED, &entity->playerAnimator, false, 0);
                    else
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
                }
                else {
                    if (player1->state == Player_State_FlyCarried)
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIMLIFT, &entity->playerAnimator, false, 0);
                    else
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIMTIRED, &entity->playerAnimator, false, 0);
                }
            }
            else if (entity->jumpPress && (!entity->underwater || player1->state != Player_State_FlyCarried)) {
                entity->abilitySpeed = -0x2000;
                entity->abilityValue = 0;
            }
        }
    }
}
void Player_State_FlyCarried(void)
{
    RSDK_THIS(Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    if (player2->state != Player_State_TailsFlight)
        entity->state = Player_State_Air;
    if (entity->sidekickPos.x != entity->leaderPos.x)
        entity->state = Player_State_Air;
    if (entity->onGround && entity->velocity.y >= 0)
        entity->state = Player_State_Ground;
    if (entity->state == Player_State_FlyCarried) {
        if (entity->jumpPress && entity->down) {
            entity->state = Player_State_Air;
            if (!entity->underwater)
                entity->velocity.y = -0x40000;
            else
                entity->velocity.y = -0x20000;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
        }
    }
    else {
        player2->flyCarryTimer = 30;
    }
}
void Player_State_KnuxGlideLeft(void)
{
    RSDK_THIS(Player);
    if (!entity->onGround) {
        if (!entity->jumpHold) {
            entity->timer        = 0;
            entity->abilitySpeed = 0;
            entity->velocity.x >>= 2;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
            entity->state = Player_State_KnuxGlideDrop;
        }
        else {
            if (entity->timer == 256) {
                if (entity->abilitySpeed < 0x180000) {
                    entity->abilitySpeed += 0x400;
                }
            }
            else if (entity->abilitySpeed < 0x40000) {
                entity->abilitySpeed += 0x1000;
            }

            if (entity->velocity.y <= 0x8000)
                entity->velocity.y += 0x2000;
            else
                entity->velocity.y -= 0x2000;

            if (entity->timer < 256)
                entity->timer += 4;
            int32 storeX = entity->position.x;

            int32 y1, y2;
            if (entity->isChibi) {
                y1 = -0x10000;
                y2 = 0x30000;
            }
            else {
                y1 = -0x20000;
                y2 = 0xB0000;
            }

            bool32 collidedA, collidedB;
            int32 oldPos = 0, newPos = 0;
            if (entity->timer >= 128) {
                entity->position.x += entity->velocity.x;
                collidedA = RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_RWALL, entity->collisionPlane, -0xC0000, y1, true);
                oldPos    = entity->position.x;
                entity->position.x += entity->velocity.x;
                collidedB = RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_RWALL, entity->collisionPlane, -0xC0000, y2, true);
                newPos    = entity->position.x;
            }
            else {
                collidedA = false;
                collidedB = false;
            }
            int32 vx           = entity->velocity.x;
            bool32 flag        = entity->velocity.x;
            entity->velocity.x = entity->abilitySpeed * RSDK.Cos512(entity->timer) >> 9;
            if (!vx && entity->velocity.x) {
                entity->abilitySpeed = 0x40000;
                flag                 = false;
                entity->velocity.x   = entity->abilitySpeed * RSDK.Cos512(entity->timer) >> 9;
            }

            entity->position.x = storeX;
            if (entity->right) {
                entity->state                         = Player_State_KnuxGlideRight;
                entity->direction                     = 0;
                entity->playerAnimator.frameID        = 0;
                entity->playerAnimator.animationTimer = 0;
            }
            else {
                if (flag || entity->timer != 256) {
                }
                else if (collidedA || collidedB) {
                    entity->abilitySpeed = 0;
                    entity->timer        = 0;
                    if (oldPos == newPos) {
                        entity->state      = Player_State_KnuxWallClimb;
                        entity->velocity.x = 0;
                        entity->velocity.y = 0;
                        RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                    }
                    else {
                        entity->velocity.x >>= 2;
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
                        entity->state = Player_State_KnuxGlideDrop;
                    }
                }
                else if (collidedA) {
                    entity->timer        = 0;
                    entity->abilitySpeed = 0;
                    entity->velocity.x >>= 2;
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
                    entity->state = Player_State_KnuxGlideDrop;
                }
            }
        }
    }
    else {
        if (entity->collisionMode) {
            entity->state = Player_State_Ground;
            if (entity->camera)
                entity->camera->offsetYFlag = false;
            entity->jumpAbilityTimer = 0;
            entity->skidding         = 0;
        }
        else {
            entity->timer = 0;
            entity->state = Player_State_GlideSlide;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYLIFTTIRED, &entity->playerAnimator, false, 0);
            entity->groundVel = entity->velocity.x;
            EntityDust *dust  = CREATE_ENTITY(Dust, entity, entity->position.x, entity->position.y);
            dust->state       = Dust_State_GlideSlide;
        }
    }

    if (entity->position.y < Zone->screenBoundsT2[RSDK_sceneInfo->entitySlot] + 0x100000) {
        entity->velocity.x   = 0;
        entity->abilitySpeed = 0;
    }
}
void Player_State_KnuxGlideRight(void)
{
    RSDK_THIS(Player);
    if (!entity->onGround) {
        if (!entity->jumpHold) {
            entity->timer        = 0;
            entity->abilitySpeed = 0;
            entity->velocity.x >>= 2;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
            entity->state = Player_State_KnuxGlideDrop;
        }
        else {
            if (!entity->timer) {
                if (entity->abilitySpeed < 0x180000) {
                    entity->abilitySpeed += 0x400;
                }
            }
            else if (entity->abilitySpeed < 0x40000) {
                entity->abilitySpeed += 0x1000;
            }

            if (entity->velocity.y <= 0x8000)
                entity->velocity.y += 0x2000;
            else
                entity->velocity.y -= 0x2000;

            if (entity->timer > 0)
                entity->timer -= 4;
            int32 storeX = entity->position.x;

            int32 y1, y2;
            if (entity->isChibi) {
                y1 = -0x10000;
                y2 = 0x30000;
            }
            else {
                y1 = -0x20000;
                y2 = 0xB0000;
            }

            bool32 collidedA, collidedB;
            int32 oldPos = 0, newPos = 0;
            if (entity->timer < 128) {
                entity->position.x += entity->velocity.x;
                collidedA = RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_LWALL, entity->collisionPlane, 0xC0000, y1, true);
                oldPos    = entity->position.x;
                entity->position.x += entity->velocity.x;
                collidedB = RSDK.ObjectTileCollision(entity, entity->collisionLayers, CMODE_LWALL, entity->collisionPlane, 0xC0000, y2, true);
                newPos    = entity->position.x;
            }
            else {
                collidedA = false;
                collidedB = false;
            }
            int32 vx           = entity->velocity.x;
            bool32 flag        = entity->velocity.x;
            entity->velocity.x = entity->abilitySpeed * RSDK.Cos512(entity->timer) >> 9;
            if (!vx && entity->velocity.x) {
                entity->abilitySpeed = 0x40000;
                flag                 = false;
                entity->velocity.x   = entity->abilitySpeed * RSDK.Cos512(entity->timer) >> 9;
            }

            entity->position.x = storeX;
            if (entity->left) {
                entity->state                         = Player_State_KnuxGlideLeft;
                entity->direction                     = FLIP_X;
                entity->playerAnimator.frameID        = 0;
                entity->playerAnimator.animationTimer = 0;
            }
            else {
                if (!flag && !entity->timer) {
                    if (collidedA || collidedB) {
                        entity->abilitySpeed = 0;
                        entity->timer        = 0;
                        if (oldPos == newPos) {
                            entity->state      = Player_State_KnuxWallClimb;
                            entity->velocity.x = 0;
                            entity->velocity.y = 0;
                            RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                        }
                        else {
                            entity->velocity.x >>= 2;
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
                            entity->state = Player_State_KnuxGlideDrop;
                        }
                    }
                    else if (collidedA) {
                        entity->timer        = 0;
                        entity->abilitySpeed = 0;
                        entity->velocity.x >>= 2;
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
                        entity->state = Player_State_KnuxGlideDrop;
                    }
                }
            }
        }
    }
    else {
        if (entity->collisionMode) {
            entity->state = Player_State_Ground;
            if (entity->camera)
                entity->camera->offsetYFlag = false;
            entity->jumpAbilityTimer = 0;
            entity->skidding         = 0;
        }
        else {
            entity->timer = 0;
            entity->state = Player_State_GlideSlide;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYLIFTTIRED, &entity->playerAnimator, false, 0);
            entity->groundVel = entity->velocity.x;
            EntityDust *dust  = CREATE_ENTITY(Dust, entity, entity->position.x, entity->position.y);
            dust->state       = Dust_State_GlideSlide;
        }
    }

    if (entity->position.y < Zone->screenBoundsT2[RSDK_sceneInfo->entitySlot] + 0x100000) {
        entity->velocity.x   = 0;
        entity->abilitySpeed = 0;
    }
}
void Player_State_KnuxGlideDrop(void)
{
    RSDK_THIS(Player);
    if (entity->onGround) {
        if (!entity->timer)
            RSDK.PlaySfx(Player->sfx_Land, false, 255);

        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;

        if (abs(Zone->autoScrollSpeed) > 0x20000) {
            if (Zone->autoScrollSpeed <= 0) {
                entity->groundVel  = Zone->autoScrollSpeed + 0x20000;
                entity->velocity.x = Zone->autoScrollSpeed + 0x20000;
            }
            else {
                entity->groundVel  = Zone->autoScrollSpeed - 0x20000;
                entity->velocity.x = Zone->autoScrollSpeed - 0x20000;
            }
            entity->timer = 16;
        }
        else {
            entity->groundVel  = 0;
            entity->velocity.x = 0;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYLIFT, &entity->playerAnimator, false, 0);
        }

        if (entity->timer >= 16) {
            entity->state    = Player_State_Ground;
            entity->skidding = 0;
            entity->timer    = 0;
        }
        else {
            entity->timer++;
        }
    }
    else {
        Player_HandleAirFriction();
        Player_HandleAirMovement();
    }
}
void Player_State_GlideSlide(void)
{
    RSDK_THIS(Player);
    if (!RSDK_sceneInfo->entity->onGround) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
        entity->state = Player_State_KnuxGlideDrop;
    }
    else {

        if (entity->groundVel && !Zone->autoScrollSpeed) {
            if (!entity->timer)
                RSDK.PlaySfx(Player->sfx_Slide, false, 255);

            entity->timer = (entity->timer + 1) & 7;
            if (entity->playerAnimator.frameID == 2) {
                entity->playerAnimator.animationSpeed = 0;
                entity->playerAnimator.animationTimer = 0;
            }

            bool32 flag = true;
            if (entity->groundVel <= 0) {
                entity->groundVel += 0x2000;
                if (entity->groundVel < 0)
                    flag = false;
            }
            else {
                entity->groundVel -= 0x2000;
                if (entity->groundVel > 0)
                    flag = false;
            }
            if (flag) {
                entity->playerAnimator.frameID = 3;
                entity->timer                  = 0;
                entity->groundVel              = 0;
            }

            if (!entity->jumpHold) {
                entity->groundVel              = 0;
                entity->timer                  = 0;
                entity->playerAnimator.frameID = 3;
            }
        }
        else {
            if (entity->camera)
                entity->camera->offsetYFlag = false;
            entity->jumpAbilityTimer              = 0;
            entity->playerAnimator.animationSpeed = 1;

            if (entity->timer >= 16) {
                entity->state    = Player_State_Ground;
                entity->skidding = 0;
            }
            else {
                entity->timer++;
            }
        }
    }
}
void Player_State_KnuxWallClimb(void)
{
    RSDK_THIS(Player);
    if (RSDK_sceneInfo->entity->onGround) {
        entity->state = Player_State_Ground;
        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;
    }
    else {
        if (entity->up) {
            if (entity->superState == SUPERSTATE_SUPER)
                entity->velocity.y = -0x20000;
            else
                entity->velocity.y = -0x10000;

            int32 bounds = Zone->screenBoundsT2[RSDK_sceneInfo->entitySlot] + 0x140000;
            if (entity->position.y < bounds)
                entity->position.y = bounds;
        }
        else if (entity->down) {
            if (entity->superState == SUPERSTATE_SUPER)
                entity->velocity.y = 0x20000;
            else
                entity->velocity.y = 0x10000;
        }
        else {
            entity->velocity.y = 0;
        }
        if (entity->jumpPress) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
            entity->state            = Player_State_Air;
            entity->jumpAbility      = 0;
            entity->jumpAbilityTimer = 1;

            RSDK.PlaySfx(Player->sfx_Jump, false, 255);
            if (entity->direction == FLIP_X) {
                entity->velocity.x = 0x40000;
                entity->groundVel  = 0x40000;
                entity->direction  = FLIP_NONE;
            }
            else {
                entity->velocity.x = -0x40000;
                entity->groundVel  = -0x40000;
                entity->direction  = FLIP_X;
            }

            entity->velocity.y = -0x38000;
            if (entity->underwater) {
                entity->velocity.x >>= 1;
                entity->groundVel >>= 1;
                entity->velocity.y = -0x1C000;
            }
        }
        else {
            Hitbox *hitbox = Player_GetHitbox(entity);
            int32 storeX   = entity->position.x;

            int32 y1 = 0;
            int32 y2 = 0;
            int32 y3 = 0;
            if (entity->isChibi) {
                y1 = -0x40000;
                y2 = 0x40000;
                y3 = -0xC0000;
            }
            else {
                y1 = -0xA0000;
                y2 = 0xB0000;
                y3 = -0x140000;
            }

            int32 x = 0;
            bool32 collidedA, collidedB;
            if (entity->direction) {
                collidedA          = RSDK.ObjectTileGrip(entity, entity->collisionLayers, 3, entity->collisionPlane, hitbox->left << 16, y1, 8);
                int32 sx           = entity->position.x;
                entity->position.x = storeX;
                collidedB          = RSDK.ObjectTileGrip(entity, entity->collisionLayers, 3, entity->collisionPlane, hitbox->left << 16, y2, 8);
                if (entity->velocity.y < 0 && entity->position.x < sx)
                    entity->velocity.y = 0;
                x = -0x40000;
            }
            else {
                collidedA          = RSDK.ObjectTileGrip(entity, entity->collisionLayers, 1, entity->collisionPlane, hitbox->right << 16, y1, 8);
                int32 sx           = entity->position.x;
                entity->position.x = storeX;
                collidedB          = RSDK.ObjectTileGrip(entity, entity->collisionLayers, 1, entity->collisionPlane, hitbox->right << 16, y2, 8);
                if (entity->velocity.y < 0 && entity->position.x > sx)
                    entity->velocity.y = 0;
                x = 0x40000;
            }
            entity->position.y += entity->velocity.y;
            if (RSDK.ObjectTileCollision(entity, entity->collisionLayers, 2, entity->collisionPlane, x, y3, true))
                entity->velocity.y = 0;

            if (collidedA) {
                if (collidedB) {
                    if (entity->velocity.y >= 0) {
                        if (entity->velocity.y <= 0)
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIM, &entity->playerAnimator, false, 0);
                        else
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIMLIFT, &entity->playerAnimator, false, 0);
                    }
                    else {
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIMTIRED, &entity->playerAnimator, false, 0);
                    }
                    entity->velocity.y = 0;
                }
                else {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 2);
                    entity->velocity.y = 0;
                    entity->state      = Player_State_KnuxGlideDrop;
                }
            }
            else {
                entity->position.y &= 0xFFF00000;
                if (entity->isChibi)
                    entity->position.y -= 0x10000;
                else
                    entity->position.y += 0xA0000;

                entity->velocity.y     = 0;
                entity->position.x     = storeX;
                entity->state          = Player_State_KnuxLedgePullUp;
                entity->timer          = 1;
                entity->tileCollisions = 0;
                entity->velocity.y     = 0;
            }
        }
    }
}
void Player_State_KnuxLedgePullUp(void)
{
    RSDK_THIS(Player);
    entity->left  = false;
    entity->right = false;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DROPDASH, &entity->playerAnimator, false, 1);

    RSDK.ProcessAnimation(&entity->playerAnimator);
    if (entity->timer != entity->playerAnimator.frameID) {
        if (entity->playerAnimator.frameID >= 6) {
            if (entity->playerAnimator.frameID == 6) {
                entity->onGround       = true;
                entity->tileCollisions = true;
            }

            if (entity->playerAnimator.frameID == entity->playerAnimator.frameCount - 1) {
                entity->timer = 0;
                entity->state = Player_State_Ground;
            }
        }
        else {
            entity->timer = entity->playerAnimator.frameID;
            if (!entity->isChibi) {
                if (entity->direction == FLIP_X)
                    entity->position.x -= 0x50000;
                else
                    entity->position.x += 0x50000;
                entity->position.y -= 0x80000;
            }
            else {
                if (entity->direction == FLIP_X)
                    entity->position.x -= 0x40000;
                else
                    entity->position.x += 0x40000;
                entity->position.y -= 0x40000;
            }

            if (entity->playerAnimator.frameID == 6) {
                entity->onGround       = true;
                entity->tileCollisions = true;
            }

            if (entity->playerAnimator.frameID == entity->playerAnimator.frameCount - 1) {
                entity->timer = 0;
                entity->state = Player_State_Ground;
            }
        }
    }
    else {
        if (entity->playerAnimator.frameID == 6) {
            entity->onGround       = true;
            entity->tileCollisions = true;
        }

        if (entity->playerAnimator.frameID == entity->playerAnimator.frameCount - 1) {
            entity->timer = 0;
            entity->state = Player_State_Ground;
        }
    }
}
#if RETRO_USE_PLUS
void Player_State_MightyHammerDrop(void)
{
    RSDK_THIS(Player);
    if (entity->onGround) {
        entity->controlLock = 0;
        int32 vel           = 0x10000;
        if (!entity->underwater)
            vel = 0x20000;
        entity->onGround = false;

        entity->velocity.x = (entity->gravityStrength + vel) * RSDK.Sin256(entity->angle);
        entity->velocity.x = (((entity->groundVel - (entity->groundVel >> 2)) * RSDK.Cos256(entity->angle)) + entity->velocity.x) >> 8;
        entity->velocity.y = (entity->groundVel - (entity->groundVel >> 2)) * RSDK.Sin256(entity->angle);
        entity->velocity.y = (entity->velocity.y - (entity->gravityStrength + vel) * RSDK.Cos256(entity->angle)) >> 8;
        if (entity->camera) {
            entity->camera->offsetYFlag = true;
            entity->camera->offset.y    = 0x200000;
        }

        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
        entity->playerAnimator.animationSpeed = (abs(entity->groundVel) >> 12) + 48;
        if (entity->playerAnimator.animationSpeed > 0xF0)
            entity->playerAnimator.animationSpeed = 0xF0;
        RSDK.StopSFX(Player->sfx_MightyDrill);
        RSDK.PlaySfx(Player->sfx_MightyLand, false, 0xFF);
        Camera_ShakeScreen(0, RSDK.GetEntityID(entity), 4);

        Hitbox *hitbox = RSDK.GetHitbox(&entity->playerAnimator, 0);
        Player_SpawnMightyHammerdropDust(0x10000, hitbox);
        Player_SpawnMightyHammerdropDust(-0x10000, hitbox);
        Player_SpawnMightyHammerdropDust(0x18000, hitbox);
        Player_SpawnMightyHammerdropDust(-0x18000, hitbox);
        Player_SpawnMightyHammerdropDust(0x20000, hitbox);
        Player_SpawnMightyHammerdropDust(-0x20000, hitbox);

        entity->angle            = 0;
        entity->collisionMode    = CMODE_FLOOR;
        entity->jumpAbility      = 0;
        entity->jumpAbilityTimer = 3;
        if (entity->invincibleTimer > 0) {
            if (entity->invincibleTimer < 8)
                entity->invincibleTimer = 8;
            entity->state = Player_State_Air;
        }
        else {
            entity->invincibleTimer = -8;
            entity->state           = Player_State_Air;
        }
    }
    else {
        Player_HandleAirFriction();
        Player_HandleAirMovement();
        if (entity->velocity.y <= 0x10000) {
            entity->state = Player_State_Air;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
        }
    }
}
void Player_State_MightyUnspin(void)
{
    RSDK_THIS(Player);
    Player_HandleAirFriction();
    if (entity->onGround) {
        entity->state = Player_State_Ground;
        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;
    }
    else {
        ++entity->blinkTimer;
        if (entity->camera) {
            entity->camera->offsetYFlag = true;
            entity->camera->offset.y    = 0x200000;
        }
        Player_HandleAirMovement();
        entity->nextGroundState = Player_State_Ground;
    }
}
void Player_SpawnMightyHammerdropDust(int32 speed, Hitbox *hitbox)
{
    RSDK_THIS(Player);
    EntityDust *dust = CREATE_ENTITY(Dust, entity, entity->position.x, entity->position.y);
    dust->state      = Dust_State_HammerDrop;
    dust->position.y += hitbox->bottom << 16;
    dust->drawOrder       = entity->drawOrder;
    dust->collisionPlane  = entity->collisionPlane;
    dust->collisionMode   = 0;
    dust->collisionLayers = entity->collisionLayers;
    dust->tileCollisions  = 1;
    dust->animator.frameDelay += 4 * (4 - (abs(speed) >> 15));
    dust->velocity.x = dust->groundVel = entity->velocity.x * (Zone->autoScrollSpeed != 0) + (speed >> entity->isChibi);
    if (entity->isChibi) {
        dust->drawFX |= FX_SCALE;
        dust->scale.x = 256;
        dust->scale.y = 256;
    }

    if (!entity->angle)
        RSDK.ObjectTileGrip(dust, dust->collisionLayers, CMODE_FLOOR, dust->collisionPlane, 0, 0, 4);

    for (int32 i = 0; i < 0x10; ++i) {
        if (RSDK.ObjectTileGrip(dust, dust->collisionLayers, CMODE_FLOOR, dust->collisionPlane, 0, 0, 8))
            break;
        dust->position.y += 0x80000;
    }
}
bool32 Player_CheckMightyUnspin(int32 bounceDistance, EntityPlayer *player, bool32 checkHammerDrop, int32 *uncurlTimer)
{
    Entity *entity = RSDK_sceneInfo->entity;
    if (player->characterID != ID_MIGHTY)
        return false;

    if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown || player->invincibleTimer
        || player->blinkTimer > 0) {
        return false;
    }

    int32 anim = player->playerAnimator.animationID;
    if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
        int32 angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
        if (player->playerAnimator.animationID != ANI_FLY) {
            player->velocity.x = bounceDistance * RSDK.Cos256(angle);
            player->groundVel  = player->velocity.x;
        }
        player->velocity.y = bounceDistance * RSDK.Sin256(angle);

        if (checkHammerDrop) {
            if (player->state == Player_State_MightyHammerDrop) {
                player->state = Player_State_Air;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
            }
            if (player->playerAnimator.animationID == ANI_SPINDASH) {
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                if (player->state == Player_State_Spindash) {
                    if (player->onGround)
                        player->state = Player_State_Roll;
                    else
                        player->state = Player_State_Air;
                }
            }
        }
        else {
            player->velocity.y = -0x48000;
            if (!(player->direction & FLIP_X))
                player->velocity.x = -0x28000;
            else
                player->velocity.x = 0x28000;

            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y = -0x24000;
            }
            player->blinkTimer = 121;
            player->visible    = true;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLY, &player->playerAnimator, false, 0);
            RSDK.PlaySfx(Player->sfx_MightyUnspin, false, 255);
            player->onGround         = 0;
            player->jumpAbility      = 0;
            player->jumpAbilityTimer = 0;
            player->state            = Player_State_MightyUnspin;
        }
        player->jumpAbility = 0;
        if (uncurlTimer && !*uncurlTimer) {
            if (player->playerAnimator.animationID != ANI_FLY)
                RSDK.PlaySfx(Player->sfx_PimPom, false, 255);
            *uncurlTimer = 8;
        }
        return true;
    }
    if (checkHammerDrop == 2 && anim == ANI_CROUCH) {
        if (!player->uncurlTimer) {
            RSDK.PlaySfx(Player->sfx_PimPom, false, 255);
            player->uncurlTimer = 30;
            if (entity->position.x > player->position.x)
                player->groundVel = -0x10000;
            else
                player->groundVel = 0x10000;
        }
        return true;
    }
    return false;
}
void Player_State_RayGlide(void)
{
    RSDK_THIS(Player);
    if (entity->rotation) {
        if (entity->abilityValue < 112)
            entity->abilityValue += 8;
    }
    else {
        if (entity->abilityValue > 16)
            entity->abilityValue -= 8;
    }

    if (entity->abilitySpeed) {
        entity->velocity.y += entity->abilitySpeed >> (2 - (entity->underwater != 0));
        if (entity->velocity.y < entity->abilitySpeed) {
            entity->velocity.y   = entity->abilitySpeed;
            entity->abilitySpeed = 0;
        }
    }
    else {
        entity->velocity.y += entity->gravityStrength * RSDK.Cos512(entity->abilityValue) >> 9;
    }

    if (entity->velocity.y < -0x60000)
        entity->velocity.y = -0x60000;
    if (entity->rotation == 1) {
        if (entity->velocity.y > 0x10000)
            entity->velocity.y = entity->velocity.y - (entity->velocity.y >> 2);
    }

    if (entity->velocity.y <= 0) {
        entity->abilityValues[0] -= 22 * RSDK.Sin256(80 - entity->abilityValue);
        if (entity->abilityValues[0] < 0x40000)
            entity->abilityValues[0] = 0x40000;
    }
    else if (entity->velocity.y > entity->abilityValues[0]) {
        entity->abilityValues[0] = entity->velocity.y - (entity->velocity.y >> 6);
    }

    if (entity->velocity.x) {
        int32 angle = 80 - entity->abilityValue;
        if (entity->direction) {
            entity->velocity.x -= 22 * RSDK.Sin256(angle) >> (uint8)(entity->underwater != 0);
            if (entity->velocity.x > -0x10000)
                entity->velocity.x = -0x10000;

            if (entity->velocity.x < -entity->abilityValues[0])
                entity->velocity.x = -entity->abilityValues[0];
        }
        else {
            entity->velocity.x += 22 * RSDK.Sin256(angle) >> (uint8)(entity->underwater != 0);
            if (entity->velocity.x < 0x10000)
                entity->velocity.x = 0x10000;

            if (entity->velocity.x > entity->abilityValues[0]) {
                entity->velocity.x = entity->abilityValues[0];
            }
        }
    }

    if (entity->controlLock) {
        entity->controlLock--;
    }
    else if ((!entity->right || entity->abilityValue != 16) && entity->direction == FLIP_X) {
        if (entity->left && entity->abilityValue == 112 && entity->rotation == 1) {
            entity->abilitySpeed = 0;
            entity->rotation     = 0;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
        }
    }
    else if ((!entity->left || entity->abilityValue != 16) && entity->direction == FLIP_NONE) {
        if (entity->right && entity->abilityValue == 112 && entity->rotation == 1) {
            entity->abilitySpeed = 0;
            entity->rotation     = 0;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 0);
        }
    }
    else if (!entity->rotation) {
        entity->rotation = 1;
        if (entity->velocity.y > 0x28000 || entity->abilityTimer == 256 || (entity->underwater && entity->velocity.y > 0x18000)) {
            int32 xVel = abs(entity->velocity.x);

            entity->abilitySpeed = -(entity->abilityTimer * ((xVel >> 1) + (xVel >> 2) + (xVel >> 4)) >> 8);
            if (entity->underwater)
                entity->abilitySpeed = (entity->abilitySpeed >> 1) + (entity->abilitySpeed >> 3);
            if (entity->abilityTimer > 16)
                entity->abilityTimer = entity->abilityTimer - 32;
            if (entity->abilitySpeed < -0x60000)
                entity->abilitySpeed = -0x60000;
        }
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 0);
    }

    if (!entity->isGhost && !entity->underwater) {
        if (entity->playerAnimator.animationID == ANI_FLYTIRED) {
            Player->rayDiveTimer = 256;
        }
        else if (entity->playerAnimator.animationID == ANI_FLY) {
            Player->raySwoopTimer = 256;
        }
    }

    if (entity->onGround) {
        if (abs(entity->groundVel) < 0x20000)
            entity->groundVel <<= 1;
        entity->state = Player_State_Ground;
        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;
        entity->skidding         = 0;
        entity->abilityValue     = 0;
    }
    else {
        if (!entity->jumpHold || entity->position.y < Zone->screenBoundsT2[entity->playerID] + 0x100000) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
            entity->state = Player_State_Air;
        }
        else if (abs(entity->velocity.x) < 0x10000) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
            entity->state = Player_State_Air;
        }
    }

    if (RSDK.ObjectTileCollision(entity, entity->collisionLayers, 2, entity->collisionPlane, 0, -0x100000, 0))
        entity->abilitySpeed = 0;
}
#endif
void Player_State_FlyIn(void)
{
    RSDK_THIS(Player);
    EntityPlayer *player1 = NULL;
    if (entity->playerID)
        player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
    else
        player1 = (EntityPlayer *)Zone->entityData[1];
#endif

    Player->jumpInDelay    = 0;
    entity->tileCollisions = false;
    entity->interaction    = false;
#if RETRO_GAMEVER != VER_100
    if (SizeLaser) {
        if (player1->isChibi != entity->isChibi)
            SizeLaser_SetP2State(entity, player1->isChibi);
    }
#endif

    if (entity->characterID == ID_TAILS) {
        if (entity->underwater) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIM, &entity->playerAnimator, false, 0);
        }
        else {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 0);
        }
        entity->scale.x = 0x200;
        entity->scale.y = 0x200;
        entity->drawFX &= ~FX_SCALE;
    }
    else if (entity->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 0);
        entity->scale.x = 0x200;
        entity->scale.y = 0x200;
        entity->drawFX &= ~FX_SCALE;
    }
    else {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
    }

    if (player1->underwater && player1->position.y < Water->waterLevel)
        entity->drawOrder = player1->drawOrder;
    else
        entity->drawOrder = Zone->playerDrawHigh;

    Entity *parent = entity->abilityPtrs[0];
    int32 screenX  = (RSDK_screens->width + RSDK_screens->centerX) << 16;
    int32 screenY  = (RSDK_screens->height + RSDK_screens->centerY) << 16;
    if (parent->position.x >= player1->position.x - screenX && parent->position.x <= screenX + player1->position.x
        && parent->position.y >= player1->position.y - screenY && parent->position.y <= player1->position.y + screenY) {
        // le troll
    }
    else {
        parent->position.x = player1->position.x;
        parent->position.y = player1->position.y;
        parent->position.y = (RSDK_screens->position.y - 128) << 16;
    }

    if (entity->camera && entity->camera->targetPtr != parent) {
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_ENCORE) {
#endif
            entity->camera->position.x = parent->position.x;
            entity->camera->position.y = parent->position.y;
            Camera_SetTargetEntity(entity->camera->screenID, parent);
#if RETRO_USE_PLUS
        }
#endif
    }

    int32 xDif = 0;
    if (entity->abilityValues[0] || entity->characterID == ID_TAILS || entity->characterID == ID_KNUCKLES) {
        int32 xDif3 = Player->curFlyCarryPos.x - parent->position.x;
        int32 xDif2 = (Player->curFlyCarryPos.x - parent->position.x) >> 4;
        xDif        = Player->curFlyCarryPos.x - parent->position.x;

        if (entity->characterID != ID_TAILS && entity->characterID != ID_KNUCKLES) {
            xDif2 = ((Player->curFlyCarryPos.x - parent->position.x) >> 5) + ((Player->curFlyCarryPos.x - parent->position.x) >> 4);
        }

        if (parent->position.x > Player->curFlyCarryPos.x - 0x40000) {
            if (parent->position.x >= Player->curFlyCarryPos.x + 0x40000) {
                entity->direction = FLIP_X;
                if (xDif2 < -0xC0000)
                    xDif2 = -0xC0000;
                if (player1->velocity.x < 0)
                    xDif2 += player1->velocity.x;
                xDif2 -= 0x10000;
                if (xDif2 < xDif3) {
                    xDif2 = xDif3;
                    xDif  = 0;
                }
            }
        }
        else {
            entity->direction = FLIP_NONE;
            if (xDif2 > 0xC0000)
                xDif2 = 0xC0000;
            if (player1->velocity.x < 0)
                xDif2 += player1->velocity.x;
            xDif2 += 0x10000;
            if (xDif2 > xDif3) {
                xDif2 = xDif3;
                xDif  = 0;
            }
        }
        parent->position.x += xDif2;

        int32 spd = 0;
        if (entity->characterID == ID_TAILS || entity->characterID == ID_KNUCKLES) {
            spd = 0x10000;
#if RETRO_USE_PLUS
            if (globals->gameMode == MODE_ENCORE)
                spd = 0x20000;
#endif
        }
        else {
            spd = 0x30000;
        }

        if (parent->position.y >= Player->curFlyCarryPos.y) {
            if (parent->position.y > Player->curFlyCarryPos.y)
                parent->position.y -= spd;
        }
        else {
            parent->position.y += spd;
        }
        parent->position.x &= 0xFFFF0000;
        parent->position.y &= 0xFFFF0000;
    }
    else {
        entity->drawOrder  = Zone->playerDrawHigh;
        parent->position.x = Player->curFlyCarryPos.x;
        parent->position.y = (RSDK_screens->centerY + 32 + RSDK_screens->position.y) << 16;
        parent->position.y += (RSDK_screens->centerY - 32) * RSDK.Sin512(entity->angle) << 8;
        entity->drawFX |= FX_SCALE;
        entity->scale.x = 0x2000 - 16 * entity->angle - 8 * entity->angle;
        entity->scale.y = 0x2000 - 16 * entity->angle - 8 * entity->angle;
        if (entity->scale.x < 0x200) {
            entity->scale.x = 0x200;
            entity->scale.y = 0x200;
        }

        entity->angle += 3;
        if ((entity->angle >= 512 || (entity->angle - 3) >= 368) && parent->position.y >= player1->position.y) {
            entity->abilityValues[0] = 1;
            entity->drawFX &= ~FX_SCALE;
            entity->scale.x    = 0x200;
            entity->scale.y    = 0x200;
            entity->position.x = parent->position.x;
        }
    }

    if (player1->objectID == Player->objectID) {
        if (player1->state != Player_State_Die && player1->state != Player_State_Drown && player1->state != Player_State_ForceRoll_Ground) {
            if (abs(xDif) <= 0x40000) {
                if (abs(Player->curFlyCarryPos.y - parent->position.y) < 0x20000)
                    Player_EndFlyJumpIn(entity, player1);
            }
        }
    }
}
void Player_State_JumpIn(void)
{
    RSDK_THIS(Player);
    EntityPlayer *player1 = NULL;
    if (entity->playerID)
        player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
    else
        player1 = (EntityPlayer *)Zone->entityData[1];
#endif

    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
    Entity *parent = entity->abilityPtrs[0];
    if (Player_CheckValidState(player1)) {
        parent->position.x = player1->position.x;
        parent->position.y = player1->position.y;
        parent->position.y -= 0x180000;
    }

    entity->velocity.y += 0x4800;
    entity->onGround   = false;
    entity->velocity.x = 0;
    entity->abilityValues[0] += entity->velocity.y;
    entity->position.x = parent->position.x;
    entity->position.y = parent->position.y + entity->abilityValues[0];

    if (entity->scale.x > 512) {
        entity->scale.x -= 10;
        if (entity->scale.x <= 0x200) {
            entity->drawFX &= ~FX_SCALE;
            entity->scale.x = 0x200;
        }
        entity->scale.y = entity->scale.x;
    }

    if (entity->velocity.y >= 0) {
        if (entity->position.y >= parent->position.y) {
            entity->drawFX &= ~FX_SCALE;
            entity->scale.x = 0x200;
            entity->scale.y = 0x200;
            Player_EndFlyJumpIn(entity, player1);
        }
    }
}
void Player_State_StartJumpIn(void)
{
    RSDK_THIS(Player);
    if (++entity->abilityValues[0] == 2) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (player1->objectID == Player->objectID) {
            EntityDust *dust  = CREATE_ENTITY(Dust, intToVoid(1), player1->position.x, player1->position.y);
            dust->visible     = false;
            dust->active      = ACTIVE_NEVER;
            dust->isPermanent = true;
            dust->position.y  = (RSDK_screens->position.y - 128) << 16;
#if RETRO_USE_PLUS
            entity->tileCollisions = false;
            entity->interaction    = false;
            entity->forceJumpIn    = false;
            entity->drawOrder      = Zone->playerDrawHigh + 1;
            entity->angle          = 128;

            if ((entity->characterID != ID_TAILS && entity->characterID != ID_KNUCKLES) || globals->gameMode == MODE_ENCORE) {
                entity->state = Player_State_JumpIn;
                entity->drawFX |= FX_SCALE;
                entity->scale.x = 0x400;
                entity->scale.y = 0x400;

                entity->abilityValues[0] = 0x100000 - player1->position.y + ((RSDK_screens->height + RSDK_screens->position.y) << 16);
                if (entity->abilityValues[0] < 0xA80000) {
                    entity->velocity.y = entity->abilityValues[0] / -12;
                }
                else
                    entity->velocity.y = -0xE0000;
            }
            else {
                entity->state            = Player_State_FlyIn;
                entity->abilityValues[0] = 0;
            }
#else
            entity->forceJumpIn = false;
            entity->state = Player_State_FlyIn;
#endif
            entity->abilityPtrs[0] = dust;
        }
    }
}
void Player_EndFlyJumpIn(EntityPlayer *thisEntity, EntityPlayer *player)
{
    RSDK_THIS(Player);
    Entity *parent             = thisEntity->abilityPtrs[0];
    thisEntity->abilityPtrs[0] = NULL;
    if (!SizeLaser || thisEntity->state == Player_State_FlyIn) {
        thisEntity->state = Player_State_Air;
    }
#if RETRO_USE_PLUS
    else if (globals->gameMode != MODE_ENCORE) {
        thisEntity->state = Player_State_Air;
    }
#endif
    else if (!thisEntity->isChibi && (player->isChibi || player->state == SizeLaser_P2JumpInShrink)) {
        thisEntity->drawFX |= FX_SCALE;
        thisEntity->scale.x          = 0x200;
        thisEntity->scale.y          = 0x200;
        thisEntity->velocity.x       = 0;
        thisEntity->velocity.y       = 0;
        thisEntity->groundVel        = 0;
        thisEntity->nextAirState     = 0;
        thisEntity->nextGroundState  = 0;
        thisEntity->interaction      = 0;
        thisEntity->abilityPtrs[0]   = SizeLaser_P2JumpInResize;
        thisEntity->abilityValues[0] = parent->position.x - thisEntity->position.x;
        thisEntity->abilityValues[1] = parent->position.y - thisEntity->position.y;
        thisEntity->state            = SizeLaser_P2JumpInShrink;
    }
    else if (!thisEntity->isChibi || (player->isChibi || player->state == SizeLaser_P2JumpInGrow)) {
        thisEntity->state = Player_State_Air;
    }
    else {
        thisEntity->drawFX |= FX_SCALE;
        thisEntity->scale.x         = 0x140;
        thisEntity->scale.y         = 0x140;
        thisEntity->velocity.x      = 0;
        thisEntity->velocity.y      = 0;
        thisEntity->groundVel       = 0;
        thisEntity->nextAirState    = StateMachine_None;
        thisEntity->nextGroundState = StateMachine_None;

        thisEntity->tailSpriteIndex = -1;
        switch (thisEntity->characterID) {
            case ID_TAILS:
                thisEntity->spriteIndex     = Player->tailsSpriteIndex;
                thisEntity->tailSpriteIndex = Player->tailsTailsSpriteIndex;
                break;
            case ID_KNUCKLES: thisEntity->spriteIndex = Player->knuxSpriteIndex; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: thisEntity->spriteIndex = Player->mightySpriteIndex; break;
            case ID_RAY: thisEntity->spriteIndex = Player->raySpriteIndex; break;
#endif
            default:
                if (entity->superState == SUPERSTATE_SUPER)
                    thisEntity->spriteIndex = Player->superSpriteIndex;
                else
                    thisEntity->spriteIndex = Player->sonicSpriteIndex;
                break;
        }
        thisEntity->interaction      = false;
        thisEntity->abilityPtrs[0]   = SizeLaser_P2JumpInResize;
        thisEntity->abilityValues[0] = parent->position.x - thisEntity->position.x;
        thisEntity->abilityValues[1] = parent->position.y - thisEntity->position.y;
        thisEntity->state            = SizeLaser_P2JumpInGrow;
    }
    RSDK.SetSpriteAnimation(thisEntity->spriteIndex, ANI_JUMP, &thisEntity->playerAnimator, false, 0);
    thisEntity->onGround       = false;
    thisEntity->tileCollisions = true;
    thisEntity->interaction    = true;
    thisEntity->controlLock    = 0;
    thisEntity->angle          = 0;
    if (!thisEntity->sidekick) {
        thisEntity->stateInput = Player_ProcessP1Input;
    }
    else {
        thisEntity->stateInput = Player_ProcessP2Input_AI;
        EntityPlayer *plr1     = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        thisEntity->velocity.x = plr1->velocity.x;
        thisEntity->groundVel  = plr1->groundVel;
        Player->upState        = 0;
        Player->downState      = 0;
        Player->leftState      = 0;
        Player->rightState     = 0;
        Player->jumpHoldState  = 0;
        Player->jumpPressState = 0;
    }

    thisEntity->collisionPlane = player->collisionPlane;
    thisEntity->drawOrder      = player->drawOrder;
    thisEntity->active         = ACTIVE_NORMAL;
    thisEntity->position.x     = parent->position.x;
    thisEntity->position.y     = parent->position.y;
    if (thisEntity->playerID == 0)
        thisEntity->blinkTimer = 120;

    if (thisEntity->camera) {
        Camera_SetTargetEntity(thisEntity->camera->screenID, (Entity *)thisEntity);
        thisEntity->camera->state = Camera_State_Follow;
    }
    destroyEntity(parent);
    Player->upState        = false;
    Player->downState      = false;
    Player->leftState      = false;
    Player->rightState     = false;
    Player->jumpPressState = false;
    Player->jumpHoldState  = false;
    if (BoundsMarker)
        BoundsMarker_CheckAllBounds(thisEntity, false);
}
void Player_State_EncoreRespawn(void)
{
    RSDK_THIS(Player);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->state != Player_State_Die && player1->state != Player_State_Drown) {
#if RETRO_USE_PLUS
        if (globals->stock) {
            Player_ChangeCharacter(entity, globals->stock & 0xFF);
            globals->stock >>= 8;
            EntityDust *dust         = CREATE_ENTITY(Dust, intToVoid(1), player1->position.x, player1->position.y);
            dust->visible            = false;
            dust->active             = ACTIVE_NEVER;
            dust->isPermanent        = true;
            dust->position.y         = (RSDK_screens->position.y - 128) << 16;
            player1->angle           = 128;
            entity->state            = Player_State_FlyIn;
            entity->abilityPtrs[0]   = dust;
            entity->abilityValues[0] = 0;
            entity->nextAirState     = StateMachine_None;
            entity->nextGroundState  = StateMachine_None;
            entity->stateInput       = Player_ProcessP2InputLag;
            entity->position.x       = -0x400000;
            entity->position.y       = -0x400000;
            entity->velocity.x       = 0;
            entity->velocity.y       = 0;
            entity->groundVel        = 0;
            entity->tileCollisions   = false;
            entity->interaction      = false;
            entity->drawOrder        = Zone->playerDrawHigh;
            entity->airTimer         = 0;
            entity->active           = ACTIVE_NORMAL;
        }
        else {
#endif
            player1->state    = StateMachine_None;
            player1->objectID = TYPE_BLANK;
#if RETRO_USE_PLUS
        }
#endif
    }
}
void Player_State_Victory(void)
{
    RSDK_THIS(Player);
    entity->jumpAbility = 0;
    entity->airTimer    = 0;

    if (!entity->onGround) {
        Player_HandleAirMovement();
    }
    else {
        switch (entity->characterID) {
            case ID_SONIC:
                if (entity->playerAnimator.frameID != 3) {
                    entity->groundVel = 0;
                }
                else {
                    entity->velocity.y = -0x32000;
                    if (entity->direction == FLIP_X)
                        entity->velocity.x = -0xE000;
                    else
                        entity->velocity.x = 0xE000;
                    entity->onGround = false;
                }
                break;
            case ID_KNUCKLES:
                if (entity->playerAnimator.frameID != 2 || entity->playerAnimator.animationTimer != 3) {
                    entity->groundVel = 0;
                }
                else {
                    entity->velocity.y = -0x30000;
                    if (entity->direction == FLIP_X)
                        entity->velocity.x = -0xE000;
                    else
                        entity->velocity.x = 0xE000;
                    entity->onGround = false;
                }
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
            case ID_RAY:
                entity->nextAirState    = StateMachine_None;
                entity->nextGroundState = StateMachine_None;
                entity->groundVel       = 0;
                entity->velocity.x      = 0;
                entity->velocity.y      = 0;
                if (entity->playerAnimator.animationID == ANI_VICTORY) {
                    if (entity->playerAnimator.animationTimer != 1) {
                        entity->velocity.x = 0;
                        entity->groundVel  = 0;
                    }
                    else {
                        int32 id = RSDK.GetFrameID(&entity->playerAnimator) - 48;
                        if (!entity->direction)
                            entity->groundVel = -(id << 16);
                        else
                            entity->groundVel = (id << 16);
                    }
                }
                break;
#endif
            default: entity->groundVel = 0; break;
        }

        if (entity->camera)
            entity->camera->offsetYFlag = false;
        entity->jumpAbilityTimer = 0;
    }
}
void Player_State_Bubble(void)
{
    // yep, thats all it is
}
void Player_State_WaterSlide(void)
{
    RSDK_THIS(Player);
    if (!entity->onGround) {
        entity->state = Player_State_Air;
        Player_HandleAirMovement();
        if (entity->jumpPress) {
            Player_StartJump(entity);
            if (entity->angle <= 128) {
                if (entity->velocity.x < 0)
                    entity->velocity.x += (entity->jumpStrength + (entity->jumpStrength >> 1)) * RSDK.Sin256(entity->angle) >> 8;
            }
            else if (entity->velocity.x <= 0) {
            }
            else {
                entity->velocity.x += (entity->jumpStrength + (entity->jumpStrength >> 1)) * RSDK.Sin256(entity->angle) >> 8;
            }
        }
    }
    else {
        if (entity->camera)
            entity->camera->offsetYFlag = false;
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

        int32 vel = RSDK.Sin256(entity->angle) << 13 >> 8;
        entity->groundVel += vel;
        entity->controlLock = 30;
        entity->direction   = vel + entity->groundVel < 0;
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLUME, &entity->playerAnimator, false, 0);
#endif

        if (entity->jumpPress) {
            Player_StartJump(entity);
            if (entity->angle <= 128) {
                if (entity->velocity.x < 0)
                    entity->velocity.x += (entity->jumpStrength + (entity->jumpStrength >> 1)) * RSDK.Sin256(entity->angle) >> 8;
            }
            else if (entity->velocity.x <= 0) {
            }
            else {
                entity->velocity.x += (entity->jumpStrength + (entity->jumpStrength >> 1)) * RSDK.Sin256(entity->angle) >> 8;
            }
        }
    }
}

void Player_SonicJumpAbility(void)
{
    RSDK_THIS(Player);
    bool32 flag = false;
    if (entity->jumpAbilityTimer == 1) {
#if RETRO_USE_PLUS
        if (entity->stateInput != Player_ProcessP2Input_AI || (entity->up && globals->gameMode != MODE_ENCORE)) {
#else
        if (entity->stateInput != Player_ProcessP2Input_AI) {
#endif
            if (entity->jumpPress
#if RETRO_GAMEVER == VER_100
                && !Player_CheckGoSuper(entity, SaveGame->saveRAM->chaosEmeralds)
#endif
            ) {
                EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(entity), Shield);
                if (entity->invincibleTimer) {
                    if (shield->objectID != Shield->objectID || shield->animator.animationID != ANI_JUMP) {
                        if (!(globals->medalMods & getMod(MEDAL_NODROPDASH)))
                            ++entity->jumpAbilityTimer;
                    }
                }
                else {
                    switch (entity->shield) {
                        case SHIELD_NONE:
                            if (globals->medalMods & getMod(MEDAL_INSTASHIELD)) {
                                entity->invincibleTimer  = -8;
                                entity->jumpAbilityTimer = 0;
                                RSDK.PlaySfx(Shield->sfx_InstaShield, false, 255);
                                RSDK.ResetEntityPtr(shield, Shield->objectID, entity);
                                shield->inkEffect = INK_ADD;
                                shield->alpha     = 0x100;
                                RSDK.SetSpriteAnimation(Shield->spriteIndex, 10, &shield->animator, true, 0);
                                shield->state = Shield_State_Insta;
                            }
                        case SHIELD_BLUE:
                            // returns 0 if dropdash (bit 4) is disabled
                            // returns 1 if dropdash is enabled and instashield (bit 3) is disabled
                            // returns 2 if dropdash AND instashield are enabled
                            if (!(globals->medalMods & getMod(MEDAL_NODROPDASH)))
                                entity->jumpAbilityTimer = ((~(globals->medalMods & 0xFF) >> 3) & 2);
                            break;
                        case SHIELD_BUBBLE:
                            entity->velocity.x >>= 1;
                            entity->velocity.y      = 0x80000;
                            entity->state           = Player_State_BubbleBounce;
                            entity->nextGroundState = StateMachine_None;
                            entity->nextAirState    = StateMachine_None;
                            RSDK.SetSpriteAnimation(Shield->spriteIndex, 7, &shield->animator2, true, 0);
                            RSDK.SetSpriteAnimation(Shield->spriteIndex, 8, &shield->animator, true, 0);
                            shield->state = Shield_State_Bubble;
                            RSDK.PlaySfx(Shield->sfx_BubbleBounce, false, 255);
                            break;
                        case SHIELD_FIRE:
                            entity->jumpAbilityTimer = 0;
                            if (entity->direction == FLIP_X)
                                entity->velocity.x = -0x80000;
                            else
                                entity->velocity.x = 0x80000;
                            entity->velocity.y = 0;
                            RSDK.SetSpriteAnimation(Shield->spriteIndex, 2, &shield->animator, true, 0);
                            shield->state     = Shield_State_Fire;
                            shield->direction = entity->direction;
                            if (entity->camera && !Zone->autoScrollSpeed) {
                                entity->scrollDelay   = 15;
                                entity->camera->state = Camera_State_VLock;
                            }
                            RSDK.PlaySfx(Shield->sfx_FireDash, false, 255);
                            break;
                        case SHIELD_LIGHTNING:
                            entity->jumpAbilityTimer = 0;
                            if (entity->invertGravity)
                                entity->velocity.y = 0x58000;
                            else
                                entity->velocity.y = -0x58000;
                            shield->state = Shield_State_Lightning;
                            RSDK.PlaySfx(Shield->sfx_LightningJump, false, 255);
                            break;
                        default: break;
                    }
                }
            }
#if RETRO_GAMEVER != VER_100
            else {
                if (RSDK_controller[entity->controllerID].keyY.press)
                    Player_CheckGoSuper(entity, SaveGame->saveRAM->chaosEmeralds);
            }
#endif
            return;
        }
        flag = true;
    }

    if ((entity->jumpAbilityTimer >= 2 || flag) && entity->jumpHold) {
        entity->jumpAbilityTimer++;
        if (entity->jumpAbilityTimer > 22) {
            entity->state           = Player_State_DropDash;
            entity->nextGroundState = StateMachine_None;
            entity->nextAirState    = StateMachine_None;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DROPDASH, &entity->playerAnimator, false, 0);
            RSDK.PlaySfx(Player->sfx_Dropdash, false, 255);
        }
    }
}
void Player_TailsJumpAbility(void)
{
    RSDK_THIS(Player);
    if (entity->jumpPress && entity->jumpAbilityTimer == 1
        && (entity->stateInput != Player_ProcessP2Input_AI
            || (entity->up
#if RETRO_USE_PLUS
                && globals->gameMode != MODE_ENCORE
#endif
                ))
#if RETRO_GAMEVER == VER_100
        && !Player_CheckGoSuper(entity, SaveGame->saveRAM->chaosEmeralds)
#endif
    ) {
        if (!entity->invertGravity) {
            entity->jumpAbilityTimer = 0;
            entity->timer            = 0;
            entity->abilitySpeed     = 0x800;
            if (!entity->underwater)
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 0);
            else
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIM, &entity->playerAnimator, false, 0);
            entity->state           = Player_State_TailsFlight;
            entity->nextGroundState = StateMachine_None;
            entity->nextAirState    = StateMachine_None;
        }
    }
#if RETRO_GAMEVER != VER_100
    else {
        if (RSDK_controller[entity->controllerID].keyY.press)
            Player_CheckGoSuper(entity, SaveGame->saveRAM->chaosEmeralds);
    }
#endif
}
void Player_KnuxJumpAbility(void)
{
    RSDK_THIS(Player);

    if (entity->jumpPress && entity->jumpAbilityTimer == 1
        && (entity->stateInput != Player_ProcessP2Input_AI
            || (entity->up
#if RETRO_USE_PLUS
                && globals->gameMode != MODE_ENCORE
#endif
                ))
#if RETRO_GAMEVER == VER_100
        && !Player_CheckGoSuper(entity, SaveGame->saveRAM->chaosEmeralds)
#endif
    ) {
        if (!entity->invertGravity) {
            entity->jumpAbilityTimer = 0;
            entity->abilitySpeed     = 0x40000;
            if (entity->velocity.y < 0)
                entity->velocity.y = 0;
            if (entity->direction) {
                entity->state      = Player_State_KnuxGlideLeft;
                entity->velocity.x = -0x40000;
                entity->timer      = 0x100;
            }
            else {
                entity->state      = Player_State_KnuxGlideRight;
                entity->velocity.x = 0x40000;
                entity->timer      = 0;
            }
            entity->nextGroundState = StateMachine_None;
            entity->nextAirState    = StateMachine_None;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 6);
        }
    }
#if RETRO_GAMEVER != VER_100
    else {
        if (RSDK_controller[entity->controllerID].keyY.press)
            Player_CheckGoSuper(entity, SaveGame->saveRAM->chaosEmeralds);
    }
#endif
}
#if RETRO_USE_PLUS
void Player_MightyJumpAbility(void)
{
    RSDK_THIS(Player);
    if (entity->jumpAbilityTimer <= 1) {
        if (entity->jumpPress && entity->jumpAbilityTimer == 1
            && (entity->stateInput != Player_ProcessP2Input_AI || (entity->up && globals->gameMode != MODE_ENCORE))) {
            if (!entity->invertGravity) {
                entity->velocity.x >>= 1;
                if (entity->underwater)
                    entity->velocity.y = 0x80000;
                else
                    entity->velocity.y = 0xC0000;
                entity->jumpAbilityTimer = 0;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DROPDASH, &entity->playerAnimator, false, 2);
                entity->nextAirState    = StateMachine_None;
                entity->nextGroundState = StateMachine_None;
                RSDK.PlaySfx(Player->sfx_Release, false, 0xFF);

                EntityImageTrail *trail = RSDK_GET_ENTITY(2 * Player->playerCount + RSDK.GetEntityID(entity), ImageTrail);
                RSDK.ResetEntityPtr(trail, ImageTrail->objectID, entity);
                if (entity->camera && !Zone->autoScrollSpeed) {
                    entity->scrollDelay   = 8;
                    entity->camera->state = Camera_State_HLock;
                }
                RSDK.PlaySfx(Player->sfx_MightyDrill, false, 0xFE);
                entity->state = Player_State_MightyHammerDrop;
            }
        }
        else {
            if (RSDK_controller[entity->controllerID].keyY.press)
                Player_CheckGoSuper(entity, SaveGame->saveRAM->chaosEmeralds);
        }
    }
    else {
        entity->jumpAbilityTimer--;
        if (entity->jumpAbilityTimer == 1)
            entity->jumpAbilityTimer = 0;
    }
}
void Player_RayJumpAbility(void)
{
    RSDK_THIS(Player);

    if (entity->jumpPress && entity->jumpAbilityTimer == 1
        && (entity->stateInput != Player_ProcessP2Input_AI || (entity->up && globals->gameMode != MODE_ENCORE))) {
        if (!entity->invertGravity) {
            int32 xDif               = entity->velocity.x - (entity->velocity.x >> 3);
            entity->jumpAbilityTimer = 0;
            if (entity->direction) {
                int32 spd = -0x30000;
                if (entity->underwater)
                    spd = -0x18000;
                if (xDif > spd)
                    xDif = spd;
                entity->velocity.x = xDif;
            }
            else {
                entity->velocity.x = xDif;
                int32 spd          = 0x30000;
                if (entity->underwater)
                    spd = 0x18000;
                if (xDif < spd)
                    entity->velocity.x = spd;
            }

            if ((entity->direction || !entity->right) && (entity->direction != FLIP_X || !entity->left)) {
                if (!entity->underwater)
                    Player->raySwoopTimer = 256;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 3);
                entity->rotation = 1;

                entity->velocity.x >>= 1;
                int32 spd = -((abs(entity->velocity.x) >> 1) + (abs(entity->velocity.x) >> 2) + (abs(entity->velocity.x) >> 4))
                            >> (uint8)(entity->underwater != 0);
                if (spd > 0x40000)
                    spd = 0x40000;
                entity->abilitySpeed = spd;
            }
            else {
                if (!entity->underwater)
                    Player->rayDiveTimer = 256;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYTIRED, &entity->playerAnimator, false, 3);
                entity->rotation     = 0;
                entity->abilitySpeed = 0;
            }
            entity->velocity.y >>= 1;
            entity->abilityValue     = 64;
            entity->controlLock      = 0;
            entity->abilityValues[0] = abs(entity->velocity.x);
            entity->state            = Player_State_RayGlide;
            entity->abilityTimer     = 256;
            entity->nextAirState     = StateMachine_None;
            entity->timer            = 0;
        }
    }
    else {
        if (RSDK_controller[entity->controllerID].keyY.press)
            Player_CheckGoSuper(entity, SaveGame->saveRAM->chaosEmeralds);
    }
}

bool32 Player_CheckRayDiving(void) { return Player->rayDiveTimer > 0; }
bool32 Player_CheckRaySwooping(void) { return Player->raySwoopTimer > 0; }

void Player_UpdateRayDiveSFX(int32 sfxID)
{
    RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfxID], (float)Player->rayDiveTimer * (1.0f / 256.0f), 0.0, 1.0);
}
void Player_UpdateRaySwoopSFX(int32 sfxID)
{
    RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfxID], (Player->raySwoopTimer * 0.8f) * (1.0f / 256.0f), 0.0, 1.0);
}
#endif

void Player_ProcessP1Input(void)
{
    RSDK_THIS(Player);
    if (entity->controllerID < PLAYER_MAX) {
        if (globals->gameMode != MODE_COMPETITION || Announcer->finishedCountdown) {
#if RETRO_USE_TOUCH_CONTROLS
            for (int32 t = 0; t < RSDK_touchMouse->count; ++t) {
                int32 tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
                int32 ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

                if (RSDK_touchMouse->down[t]) {
                    if (tx >= 0 && ty >= 96 && tx <= RSDK_screens->centerX && ty <= RSDK_screens->height) {
                        int32 tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
                        int32 ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);
                        tx -= 64;
                        ty -= 192;

                        switch (((RSDK.ATan2(tx, ty) + 32) & 0xFF) >> 6) {
                            case 0:
                                RSDK_controller->keyRight.down |= true;
                                RSDK_controller[entity->controllerID].keyRight.down = true;
                                break;
                            case 1:
                                RSDK_controller->keyDown.down |= true;
                                RSDK_controller[entity->controllerID].keyDown.down = true;
                                break;
                            case 2:
                                RSDK_controller->keyLeft.down |= true;
                                RSDK_controller[entity->controllerID].keyLeft.down = true;
                                break;
                            case 3:
                                RSDK_controller->keyUp.down |= true;
                                RSDK_controller[entity->controllerID].keyUp.down = true;
                                break;
                        }
                        break;
                    }
                }
            }

            for (int32 t = 0; t < RSDK_touchMouse->count; ++t) {
                int32 tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
                int32 ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

                if (RSDK_touchMouse->down[t]) {
                    if (tx >= RSDK_screens->centerX && ty >= 96 && tx <= RSDK_screens->width && ty <= RSDK_screens->height) {
                        RSDK_controller->keyA.down |= true;
                        RSDK_controller[entity->controllerID].keyA.down = true;
                        break;
                    }
                }
            }

            if (!entity->touchJump) {
                RSDK_controller->keyA.press |= RSDK_controller->keyA.down;
                RSDK_controller[entity->controllerID].keyA.press |= RSDK_controller[entity->controllerID].keyA.down;
            }
            entity->touchJump = RSDK_controller[entity->controllerID].keyA.down;

            for (int32 t = 0; t < RSDK_touchMouse->count; ++t) {
                int32 tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
                int32 ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

                if (RSDK_touchMouse->down[t]) {
                    if (tx >= RSDK_screens->width - 0x80 && ty >= 0 && tx <= RSDK_screens->width && ty <= 0x40) {
                        if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
                            EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                            bool32 flag                = true;
#if RETRO_USE_PLUS
                            if (ActClear && ActClear->dword34)
                                flag = false;
#endif
                            if (!RSDK.GetEntityCount(TitleCard->objectID, 0) && !pauseMenu->objectID && flag) {
                                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                                pauseMenu->triggerPlayer = entity->playerID;
                                if (globals->gameMode == MODE_COMPETITION)
                                    pauseMenu->disableRestart = true;
                            }
                        }
                        break;
                    }
                }
            }

#if RETRO_USE_PLUS
            for (int32 t = 0; t < RSDK_touchMouse->count; ++t) {
                int32 tx = (RSDK_touchMouse->x[t] * RSDK_screens->width);
                int32 ty = (RSDK_touchMouse->y[t] * RSDK_screens->height);

                if (RSDK_touchMouse->down[t]) {
                    if (tx >= RSDK_screens->width - 0x100 && ty >= 0 && tx <= RSDK_screens->width - 0x80 && ty <= 0x40) {
                        if (globals->gameMode == MODE_ENCORE) {
                            if (!HUD->swapCooldown && Player_CheckValidState(entity)) {
                                if (Player_SwapMainPlayer(false)) {
                                    return;
                                }
                                else {
                                    RSDK.PlaySfx(Player->sfx_SwapFail, false, 0xFF);
                                }
                            }
                            else {
                                RSDK.PlaySfx(Player->sfx_SwapFail, false, 0xFF);
                            }
                        }
                        break;
                    }
                }
            }
#endif
#endif

            ControllerState *controller = &RSDK_controller[entity->controllerID];
            entity->up                  = controller->keyUp.down;
            entity->down                = controller->keyDown.down;
            entity->left                = controller->keyLeft.down;
            entity->right               = controller->keyRight.down;

            entity->up |= RSDK_stickL[entity->controllerID].keyUp.down;
            entity->down |= RSDK_stickL[entity->controllerID].keyDown.down;
            entity->left |= RSDK_stickL[entity->controllerID].keyLeft.down;
            entity->right |= RSDK_stickL[entity->controllerID].keyRight.down;
            entity->up |= RSDK_stickL[entity->controllerID].vDelta > 0.3;
            entity->down |= RSDK_stickL[entity->controllerID].vDelta < -0.3;
            entity->left |= RSDK_stickL[entity->controllerID].hDelta < -0.3;
            entity->right |= RSDK_stickL[entity->controllerID].hDelta > 0.3;
            if (entity->left && entity->right) {
                entity->left  = false;
                entity->right = false;
            }
            entity->jumpPress = controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press;
            entity->jumpHold  = controller->keyA.down || controller->keyB.down || controller->keyC.down || controller->keyX.down;

            if (!LottoMachine || !((1 << entity->playerID) & LottoMachine->activePlayers)) {
#if RETRO_USE_PLUS
                if (sku_platform == PLATFORM_DEV && controller->keyZ.press) {
                    Zone->swapGameMode = true;
                    RSDK.PlaySfx(Player->sfx_Transform2, false, 0xFE);
                    Zone_StartFadeOut(64, 0xF0F0F0);
                }
                // TEMP!! I SOULD REMOVE THIS!!!
                else if (sku_platform == PLATFORM_DEV && controller->keySelect.press) {
                    entity->characterID <<= 1;
                    if (entity->characterID > ID_RAY)
                        entity->characterID = 1;
                    Player_ChangeCharacter(entity, entity->characterID);
                }

                if (globals->gameMode == MODE_ENCORE && controller->keyY.press) {
                    if (!HUD->swapCooldown && Player_CheckValidState(entity)) {
                        if (Player_SwapMainPlayer(false)) {
                            return;
                        }
                        else {
                            RSDK.PlaySfx(Player->sfx_SwapFail, false, 0xFF);
                        }
                    }
                    else {
                        RSDK.PlaySfx(Player->sfx_SwapFail, false, 0xFF);
                    }
                }
#else
                // TEMP!! I SOULD REMOVE THIS!!!
                if (RSDK_info->platform == PLATFORM_DEV && controller->keySelect.press) {
                    entity->characterID <<= 1;
                    if (entity->characterID > ID_KNUCKLES)
                        entity->characterID = 1;
                    Player_ChangeCharacter(entity, entity->characterID);
                }
#endif

                if (entity->controllerID == CONT_P1 && RSDK_sceneInfo->debugMode && entity->state != Player_State_Transform
                    && RSDK_controller[CONT_P1].keyX.press && globals->gameMode != MODE_TIMEATTACK) {
                    entity->objectID   = DebugMode->objectID;
                    entity->velocity.x = 0;
                    entity->velocity.y = 0;
                    entity->groundVel  = 0;
                    entity->drawOrder  = Zone->playerDrawHigh;
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimator, true, 0);
                    RSDK.SetGameMode(ENGINESTATE_REGULAR);
                    entity->jumpHold       = false;
                    entity->jumpPress      = false;
                    entity->visible        = true;
                    entity->active         = ACTIVE_NORMAL;
                    DebugMode->debugActive = true;
                }
            }

#if RETRO_USE_PLUS
            if (RSDK_controller[entity->controllerID].keyStart.press || RSDK_unknown->field_10 == 1) {
#else
            if (RSDK_controller[entity->controllerID].keyStart.press) {
#endif

                if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
                    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                    bool32 flag                = true;
#if RETRO_USE_PLUS
                    if (ActClear && ActClear->dword34)
                        flag = false;
#endif
                    if (!RSDK.GetEntityCount(TitleCard->objectID, 0) && !pauseMenu->objectID && flag) {
                        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                        pauseMenu->triggerPlayer = entity->playerID;
                        if (globals->gameMode == MODE_COMPETITION)
                            pauseMenu->disableRestart = true;
                    }
                }
            }
        }
    }
}
void Player_ProcessP2InputLag(void)
{
    RSDK_THIS(Player);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Player->upState <<= 1;
    Player->upState |= player1->up;
    Player->upState &= 0xFFFF;

    Player->downState <<= 1;
    Player->downState |= player1->down;
    Player->downState &= 0xFFFF;

    Player->leftState <<= 1;
    Player->leftState |= player1->left;
    Player->leftState &= 0xFFFF;

    Player->rightState <<= 1;
    Player->rightState |= player1->right;
    Player->rightState &= 0xFFFF;

    Player->jumpPressState <<= 1;
    Player->jumpPressState |= player1->jumpPress;
    Player->jumpPressState &= 0xFFFF;

    Player->jumpHoldState <<= 1;
    Player->jumpHoldState |= player1->jumpHold;
    Player->jumpHoldState &= 0xFFFF;

    if (player1->state == Player_State_FlyCarried) {
        Player->downState <<= 15;
        Player->leftState <<= 15;
        Player->rightState <<= 15;
        Player->jumpPressState <<= 15;
        Player->jumpHoldState <<= 15;
    }
    entity->up        = Player->upState >> 15;
    entity->down      = Player->downState >> 15;
    entity->left      = Player->leftState >> 15;
    entity->right     = Player->rightState >> 15;
    entity->jumpPress = Player->jumpPressState >> 15;
    entity->jumpHold  = Player->jumpHoldState >> 15;

    if (Player_CheckValidState(player1)) {
        Player->flyCarryPositions[Player->flyCarryPosB].x = player1->position.x;
        Player->flyCarryPositions[Player->flyCarryPosB].y = player1->position.y;
        ++Player->flyCarryPosB;
        Player->flyCarryPosB &= 0xF;
        ++Player->flyCarryPosA;
        Player->flyCarryPosA &= 0xF;
        if (!player1->onGround && player1->groundedStore) {
            Player->curFlyCarryPos.x = player1->position.x;
            Player->curFlyCarryPos.y = player1->position.y;
        }
        else {
            Player->curFlyCarryPos.x = Player->flyCarryPositions[Player->flyCarryPosA].x;
            Player->curFlyCarryPos.y = Player->flyCarryPositions[Player->flyCarryPosA].y;
        }
    }
    else {
        int32 pos = Player->flyCarryPosB - 1;
        if (pos < 0)
            pos += 16;
        Player->curFlyCarryPos.x = Player->flyCarryPositions[pos].x;
        Player->curFlyCarryPos.y = Player->flyCarryPositions[pos].y;
    }
}
void Player_ProcessP2Input_AI(void)
{
    RSDK_THIS(Player);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Player_ProcessP2InputLag();
    if (entity->state == Player_State_TailsFlight && player1->state == Player_State_FlyCarried) {
        entity->up        = player1->up;
        entity->down      = player1->down;
        entity->left      = player1->left;
        entity->right     = player1->right;
        entity->jumpHold  = player1->jumpHold;
        entity->jumpPress = player1->jumpPress;
    }
    else {
        if (player1->objectID == Player->objectID && player1->state != Player_State_FlyCarried) {
            int32 val = Player->curFlyCarryPos.x;
            if (player1->onGround || player1->groundedStore) {
                if (abs(player1->groundVel) < 0x20000) {
                    if (!player1->direction)
                        val -= 0x200000;
                    else
                        val += 0x200000;
                }
            }

            int32 dif = val - entity->position.x;
            if (dif) {
                if (dif >= 0) {
                    if (dif >= 0x300000) {
                        entity->left  = false;
                        entity->right = true;
                    }
                    if (entity->groundVel && entity->direction == FLIP_NONE) {
                        entity->position.x += 0xC0 * RSDK.Cos256(entity->angle);
                    }
                }
                else {
                    if (dif <= -0x300000) {
                        entity->right = false;
                        entity->left  = true;
                    }

                    if (entity->groundVel && entity->direction == FLIP_X) {
                        entity->position.x -= 0xC0 * RSDK.Cos256(entity->angle);
                    }
                }
            }

            bool32 jumpFlag = false;
            if (entity->playerAnimator.animationID == ANI_PUSH) {
                ++Player->P2JumpActionDelay;
                if (player1->direction == entity->direction && player1->playerAnimator.animationID == ANI_PUSH)
                    Player->P2JumpActionDelay = 0;
                jumpFlag = Player->P2JumpActionDelay < 30;
            }
            else {
                if (entity->position.y - Player->curFlyCarryPos.y <= 0x200000) {
                    Player->P2JumpActionDelay = 0;
                    if (entity->controlLock > 0) {
                        if (abs(entity->groundVel) < 0x8000)
                            entity->stateInput = Player_ProcessP2Input_None;
                    }
                    jumpFlag = 2;
                }
                else {
                    ++Player->P2JumpActionDelay;
                    jumpFlag = Player->P2JumpActionDelay < 64;
                }
            }

            if (jumpFlag == true) {
                entity->jumpHold = true;
            }
            else if (jumpFlag == false) {
                if (entity->onGround) {
                    if (!entity->jumpHold)
                        entity->jumpPress = true;
                    entity->jumpHold = true;
                }
                entity->jumpAbility       = 0;
                Player->P2JumpActionDelay = 0;
            }

            if (entity->controlLock > 0) {
                if (abs(entity->groundVel) < 0x8000)
                    entity->stateInput = Player_ProcessP2Input_None;
            }
        }
    }
    if (Player_CheckKeyPress())
        entity->stateInput = Player_ProcessP2Input_Player;
    Player_P2JumpBackIn();
}
void Player_ProcessP2Input_None(void)
{
    RSDK_THIS(Player);
    entity->up        = false;
    entity->down      = false;
    entity->left      = false;
    entity->right     = false;
    entity->jumpPress = false;
    entity->jumpHold  = false;
    if (!entity->controlLock && entity->onGround && entity->groundVel < 0x4000) {
        entity->groundVel         = 0;
        entity->stateInput        = Player_ProcessP2Input_Unknown;
        Player->P2JumpActionDelay = 1;
        if (entity->playerAnimator.animationID != ANI_SPINDASH) {
            entity->down      = true;
            entity->direction = entity->position.x >= Player->curFlyCarryPos.x;
        }
    }
    if (Player_CheckKeyPress())
        entity->stateInput = Player_ProcessP2Input_Player;
    Player_P2JumpBackIn();
}
void Player_ProcessP2Input_Unknown(void)
{
    RSDK_THIS(Player);
    if (Player->P2JumpActionDelay >= 64) {
        Player->P2JumpActionDelay = 0;
        entity->down              = false;
        entity->jumpPress         = false;
        entity->stateInput        = Player_ProcessP2Input_AI;
    }
    else {
        entity->down      = true;
        entity->jumpPress = !(Player->P2JumpActionDelay & 0xF);
        ++Player->P2JumpActionDelay;
    }
    if (Player_CheckKeyPress())
        entity->stateInput = Player_ProcessP2Input_Player;
    Player_P2JumpBackIn();
}
void Player_ProcessP2Input_Player(void)
{
    RSDK_THIS(Player);
    if (entity->controllerID <= CONT_P4) {
#if RETRO_USE_PLUS
        if (!RSDK.InputIDIsDisconnected(entity->controllerID)) {
#else
        if (false) {
#endif
            entity->up    = RSDK_controller[entity->controllerID].keyUp.down;
            entity->down  = RSDK_controller[entity->controllerID].keyDown.down;
            entity->left  = RSDK_controller[entity->controllerID].keyLeft.down;
            entity->right = RSDK_controller[entity->controllerID].keyRight.down;
            entity->up |= RSDK_stickL[entity->controllerID].vDelta > 0.3;
            entity->down |= RSDK_stickL[entity->controllerID].vDelta < -0.3;
            entity->left |= RSDK_stickL[entity->controllerID].hDelta < -0.3;
            entity->right |= RSDK_stickL[entity->controllerID].hDelta > 0.3;
            if (entity->left && entity->right) {
                entity->left  = false;
                entity->right = false;
            }
            entity->jumpPress = RSDK_controller[entity->controllerID].keyA.press || RSDK_controller[entity->controllerID].keyB.press
                                || RSDK_controller[entity->controllerID].keyC.press || RSDK_controller[entity->controllerID].keyX.press;
            entity->jumpHold = RSDK_controller[entity->controllerID].keyA.down || RSDK_controller[entity->controllerID].keyB.down
                               || RSDK_controller[entity->controllerID].keyC.down || RSDK_controller[entity->controllerID].keyX.down;
            if (entity->right || entity->up || entity->down || entity->left) {
                Player->p2InputDelay = 0;
            }
            else if (++Player->p2InputDelay >= 600) {
                entity->stateInput = Player_ProcessP2Input_AI;
#if RETRO_USE_PLUS
                RSDK.AssignControllerID(entity->controllerID, CONT_AUTOASSIGN);
#else
                APICallback_AssignControllerID(entity->controllerID, CONT_AUTOASSIGN);
#endif
            }

            if (RSDK_controller[entity->controllerID].keyStart.press && RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
                EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                if (!RSDK.GetEntityCount(TitleCard->objectID, 0) && !pauseMenu->objectID) {
                    RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                    pauseMenu->triggerPlayer = 0;
                }
            }
            Player_P2JumpBackIn();
        }
        else {
            entity->stateInput = Player_ProcessP2Input_AI;
#if RETRO_USE_PLUS
            RSDK.AssignControllerID(entity->controllerID, CONT_AUTOASSIGN);
#else
            APICallback_AssignControllerID(entity->controllerID, CONT_AUTOASSIGN);
#endif
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Player_EditorDraw(void)
{
    RSDK_THIS(Player);
    RSDK.SetSpriteAnimation(Player->sonicSpriteIndex, 0, &entity->playerAnimator, true, entity->characterID);
    RSDK.DrawSprite(&entity->playerAnimator, NULL, false);
}

void Player_EditorLoad(void)
{
    Player->sonicSpriteIndex = RSDK.LoadSpriteAnimation("Editor/PlayerIcons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Player, characterID);
    RSDK_ENUM_VAR("None", CHAR_NONE);
    RSDK_ENUM_VAR("Sonic", CHAR_SONIC);
    RSDK_ENUM_VAR("Tails", CHAR_TAILS);
    RSDK_ENUM_VAR("Sonic & Tails", CHAR_SONIC_TAILS);
    RSDK_ENUM_VAR("Knuckles", CHAR_KNUX);
    RSDK_ENUM_VAR("Sonic & Knuckles", CHAR_SONIC_KNUX);
    RSDK_ENUM_VAR("Tails & Knuckles", CHAR_TAILS_KNUX);
    RSDK_ENUM_VAR("Sonic, Tails & Knuckles", CHAR_SONIC_TAILS_KNUX);
}
#endif

void Player_Serialize(void) { RSDK_EDITABLE_VAR(Player, VAR_ENUM, characterID); }

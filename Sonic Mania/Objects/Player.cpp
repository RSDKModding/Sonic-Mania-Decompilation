#include "../SonicMania.hpp"

ObjectPlayer *Player;

void Player_Update()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (!User.CheckDLC(DLC_PLUS) && entity->characterID > ID_KNUCKLES)
        Player_ChangeCharacter(entity, ID_SONIC);

    if (entity->inputState2)
        entity->inputState2();

    if (entity->inputState)
        entity->inputState();
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
                    if (options->gameMode != MODE_ENCORE || !entity->sidekick) {
                        EntityPlayer *player = NULL;
                        bool32 flag          = false;
                        while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
                            if (player->invincibleTimer > 0)
                                flag = false;
                        }
                        // if (flag)
                        //    Music_Unknown6(1, 1);
                    }
                }
            }
        }

        if (entity->speedShoesTimer > 0) {
            entity->speedShoesTimer--;
            if (!entity->speedShoesTimer) {
                Player_ChangePhysicsState(entity);
                EntityPlayer *player = NULL;
                bool32 flag          = false;
                while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
                    if (player->speedShoesTimer > 0)
                        flag = false;
                }
                // if (flag)
                //    Music_Unknown6(2, 1);
            }
        }

        if (entity->state != Player_State_Hit) {
            if (entity->blinkTimer > 0) {
                entity->blinkTimer--;
                entity->visible = !(entity->blinkTimer & 4);
            }
        }

        // if (entity->characterID == ID_RAY && entity->state != Player_State_RayGlide && !entity->field_1F0) {
        //    Player->raySwoopTimer = 0;
        //    Player->rayDiveTimer  = 0;
        //}

        if (entity->uncurlTimer > 0)
            entity->uncurlTimer--;

        // Player_HandleSuperPalette();
        // if (entity->characterID == ID_TAILS && entity->state != Player_State_TailsFlight && entity->glideSpeedStore)
        //    entity->glideSpeedStore = 0;

        if (entity->killFlagA == 3 || entity->killFlagB == 3)
            entity->hurtFlag = 1;

        entity->killFlagA = 0;
        entity->killFlagB = 0;
        if (entity->collisionLayers & Zone->moveID) {
            TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
            move->position.x = -entity->field_194.x >> 16;
            move->position.y = -entity->field_194.y >> 16;
        }

        //if (entity->field_1EC)
        //    entity->state = Player_Unknown8;
        if (entity->forceTransform)
            entity->state = Player_State_Transform;

        if (entity->state)
            entity->state();

        if (entity->objectID == Player->objectID) {
            entity->flailing = 0;
            entity->flipY    = 0;
            if (entity->outerbox) {
                entity->groundedStore = entity->onGround;
                RSDK.ProcessTileCollisions(entity, entity->outerbox, entity->innerbox);
            }
            else {
                entity->outerbox      = RSDK.GetHitbox(&entity->playerAnimData, 0);
                entity->innerbox      = RSDK.GetHitbox(&entity->playerAnimData, 1);
                entity->groundedStore = entity->onGround;
                RSDK.ProcessTileCollisions(entity, entity->outerbox, entity->innerbox);
                entity->outerbox = 0;
            }
            entity->collisionLayers &= ~Zone->moveID;
            if (entity->onGround && !entity->collisionMode)
                entity->killFlagB |= 1;
        }
    }
}

void Player_LateUpdate()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    // if (entity->superState == 1 && entity->state != Player_State_Transform)
    //    Player_CheckGoSuper((EntityPlayer *)RSDK_sceneInfo->entity, 127);
    // if (entity->state == Player_State_FlyCarried) {
    //    entity->leaderPosition.x         = entity->position.x >> 0x10 << 0x10;
    //    entity->leaderPosition.y         = entity->position.y >> 0x10 << 0x10;
    //}

    if (entity->hurtFlag) {
        entity->maxGlideSpeed = 0;
        // if (!entity->sidekick)
        //    RSDK_CopyEntity(&Zone->data[1114], entity, 0);
        if (entity->sidekick == 1 || options->gameMode == MODE_COMPETITION || options->gameMode == 1) {
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
            entity->direction &= FLIP_X;
        entity->visible        = 1;
        entity->onGround       = 0;
        entity->groundVel      = 0;
        entity->velocity.x     = 0;
        entity->nextGroundState   = 0;
        entity->nextAirState      = 0;
        entity->interaction    = 0;
        entity->tileCollisions = 0;
        if (options->gameMode != MODE_COMPETITION)
            entity->active = ACTIVE_ALWAYS;
        entity->shield     = 0;
        entity->killFlagA  = 0;
        entity->killFlagB  = 0;
        entity->underwater = 0;
        Player_ChangePhysicsState(entity);
        Entity *powerup = RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(entity));
        RSDK.DestroyEntity(powerup, 0, 0);
        if (entity->hurtFlag == 1) {
            RSDK.PlaySFX(Player->sfx_Hurt, 0, 255);
        }
        else if (entity->hurtFlag != 2) {
            if (entity->hurtFlag == 3) {
                entity->hurtFlag        = 0;
                entity->gravityStrength = 0x1000;
                entity->velocity.y      = 0;
                // RSDK.PlaySFX(*(unsigned __int16 *)&Water[42].active, 0, 255);
                entity->state = Player_State_Drown;
                if (!entity->sidekick) {
                    if (options->gameMode == MODE_COMPETITION) {
                        // Music_Unknown6(8, 0);
                    }
                    else if (options->gameMode != MODE_ENCORE) {
                        RSDK_sceneInfo->timeEnabled = false;

                        if (entity->camera) {
                            entity->scrollDelay   = 2;
                            entity->camera->state = NULL;
                        }
                    }
                    else {
                        EntityPlayer *sidekick = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER2);
                        if (options->stock == 0 && !sidekick->objectID) {
                            RSDK_sceneInfo->timeEnabled = false;
                        }

                        if (entity->camera) {
                            entity->scrollDelay   = 2;
                            entity->camera->state = NULL;
                        }
                    }
                }
            }
            if (entity->camera) {
                entity->scrollDelay   = 2;
                entity->camera->state = NULL;
            }
        }
        else if (entity->hurtFlag == 2) {
            entity->hurtFlag   = 0;
            entity->velocity.y = -0x68000;
            entity->state      = Player_State_Die;
            if ((entity->drawFX & FX_SCALE) == 0 || entity->scale.x == 0x200)
                entity->drawOrder = Zone->uiDrawLow;
            if (entity->sidekick) {
                if (entity->camera) {
                    entity->scrollDelay   = 2;
                    entity->camera->state = NULL;
                }
            }
            else if (options->gameMode == MODE_ENCORE) {
                EntityPlayer *sidekick = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER2);
                if (options->stock == 0 && !sidekick->objectID) {

                    if (entity->camera) {
                        entity->scrollDelay   = 2;
                        entity->camera->state = NULL;
                    }
                }
            }
            else if (options->gameMode == MODE_COMPETITION) {
                if (entity->camera) {
                    entity->scrollDelay   = 2;
                    entity->camera->state = NULL;
                }
            }
            else {
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
                RSDK_sceneInfo->timeEnabled = false;
                if (entity->camera) {
                    entity->scrollDelay   = 2;
                    entity->camera->state = NULL;
                }
            }
        }
    }

    if (entity->onGround) {
        if (entity->nextGroundState) {
            entity->state        = entity->nextGroundState;
            entity->nextGroundState = NULL;
            if (entity->sidekick) {
                EntityPlayer *player = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
                player->scoreBonus   = 0;
            }
            else {
                EntityPlayer *player = entity;
                player->scoreBonus   = 0;
            }
        }

        if (entity->camera) {
            if (entity->playerAnimData.animationID == ANI_JUMP)
                entity->camera->adjustY = entity->cameraOffset;
            else
                entity->camera->adjustY = 0;
        }
    }
    else {
        if (entity->nextAirState) {
            entity->state     = entity->nextAirState;
            entity->nextAirState = NULL;
        }
    }

    if (entity->tailSpriteIndex != 0xFFFF) {
        switch (entity->playerAnimData.animationID) {
            case ANI_IDLE:
            case ANI_BORED1:
            case ANI_LOOKUP:
            case ANI_CROUCH:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 0, &entity->tailSpriteAnimData, 0, 0);
                entity->tailDirection = entity->direction;
                entity->tailRotation  = entity->rotation;
                break;
            case ANI_JUMP:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 1, &entity->tailSpriteAnimData, 0, 0);
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
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 3, &entity->tailSpriteAnimData, 0, 0);
                entity->tailDirection = entity->direction;
                break;
            case ANI_SPINDASH:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 2, &entity->tailSpriteAnimData, 0, 0);
                entity->tailDirection = entity->direction;
                break;
            case ANI_HANGMOVE:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 4, &entity->tailSpriteAnimData, 0, 0);
                entity->tailDirection = entity->direction;
                break;
            case ANI_PULLEYHOLD:
                RSDK.SetSpriteAnimation(entity->tailSpriteIndex, 5, &entity->tailSpriteAnimData, 0, 0);
                entity->tailDirection = entity->direction;
                if (entity->tailDirection)
                    entity->tailRotation = entity->rotation + 32;
                else
                    entity->tailRotation = entity->rotation - 32;
                break;
            default: RSDK.SetSpriteAnimation(-1, 0, &entity->tailSpriteAnimData, 0, 0); break;
        }
        RSDK.ProcessAnimation(&entity->tailSpriteAnimData);
    }
    RSDK.ProcessAnimation(&entity->playerAnimData);
}

void Player_StaticUpdate()
{
    if (Player->field_A70 > 0) {
        RSDK.GetEntityByID(SLOT_PLAYER1);
        EntityHUD *hud = NULL;
        if (RSDK.GetObjects(HUD->objectID, (Entity **)&hud)) {
            RSDK.NextForEachLoop();
            // if (hud)
            //    *(_DWORD *)&hud[3].priority = 1;
        }
        --Player->field_A70;
    }

    bool flying = false;
    bool tired  = false;
    if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
        EntityPlayer *player = NULL;
        if (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
            do {
                if (player->characterID == ID_TAILS) {
                    int anim = player->playerAnimData.animationID;
                    if (anim == ANI_FLY || anim == ANI_FLYLIFT)
                        flying = true;
                    if (anim == ANI_FLYTIRED || anim == ANI_FLYLIFTTIRED)
                        tired = true;
                }
            } while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player));

            if (flying) {
                if (!Player->playingFlySFX) {
                    RSDK.PlaySFX(Player->sfx_Flying, 1, 255);
                    Player->playingFlySFX = true;
                }
            }
        }
    }
    else if (Player->playingFlySFX) {
        RSDK.StopSFX(Player->sfx_Flying);
        Player->playingFlySFX = false;
    }

    if (tired && !Player->playingTiredSFX) {
        RSDK.PlaySFX(Player->sfx_Tired, 1, 255);
        Player->playingTiredSFX = true;
    }
    else if (Player->playingTiredSFX) {
        RSDK.StopSFX(Player->sfx_Tired);
        Player->playingTiredSFX = false;
    }

    Player->raySwoopTimer -= 8;
    if (Player->raySwoopTimer < 0)
        Player->raySwoopTimer = 0;

    Player->rayDiveTimer -= 16;
    if (Player->rayDiveTimer < 0)
        Player->rayDiveTimer = 0;
}

void Player_Draw()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->field_1F0) {
        entity->inkEffect = INK_BLEND;
        entity->alpha     = 0x7F;
    }
    Vector2 *posPtr  = (Vector2 *)entity->field_214;
    Vector2 posStore = entity->position;
    // if (entity->state == Player_State_FlyIn && posPtr) {
    //    entity->position.x = posPtr->x;
    //    entity->position.y = posPtr->y;
    //}
    if (entity->tailSpriteIndex != 0xFFFF) {
        int rotStore      = entity->rotation;
        int dirStore      = entity->direction;
        entity->rotation  = entity->tailRotation;
        entity->direction = entity->tailDirection;
        RSDK.DrawSprite(&entity->tailSpriteAnimData, NULL, 0);
        entity->rotation  = rotStore;
        entity->direction = dirStore;
    }

    if (entity->playerID != 1 || options->playerID != ((options->playerID >> 8) & 0xFF) || options->gameMode == MODE_TIMEATTACK) {
        RSDK.DrawSprite(&entity->playerAnimData, NULL, 0);
    }
    else {
        uint colourStore[6];
        switch (entity->characterID) {
            case ID_SONIC:
                for (int c = 0; c < 6; ++c) {
                    colourStore[c] = RSDK.GetPaletteEntry(0, c + 0x40);
                    RSDK.SetPaletteEntry(0, c + 0x40, Player->superPalette_Sonic[c]);
                }

                RSDK.DrawSprite(&entity->playerAnimData, NULL, 0);

                for (int c = 0; c < 6; ++c) {
                    RSDK.SetPaletteEntry(0, c + 0x40, colourStore[c]);
                }
                break;
            case ID_TAILS:
                for (int c = 0; c < 6; ++c) {
                    colourStore[c] = RSDK.GetPaletteEntry(0, c + 0x46);
                    RSDK.SetPaletteEntry(0, c + 0x46, Player->superPalette_Tails[c]);
                }

                RSDK.DrawSprite(&entity->playerAnimData, NULL, 0);

                for (int c = 0; c < 6; ++c) {
                    RSDK.SetPaletteEntry(0, c + 0x46, colourStore[c]);
                }
                break;
            case ID_KNUCKLES:
                for (int c = 0; c < 6; ++c) {
                    colourStore[c] = RSDK.GetPaletteEntry(0, c + 0x50);
                    RSDK.SetPaletteEntry(0, c + 0x50, Player->superPalette_Knux[c]);
                }

                RSDK.DrawSprite(&entity->playerAnimData, NULL, 0);

                for (int c = 0; c < 6; ++c) {
                    RSDK.SetPaletteEntry(0, c + 0x50, colourStore[c]);
                }
                break;
        }
    }
    // if (entity->state == Player_State_FlyIn) {
    //    if (posPtr) {
    //        entity->position.x = posStore.x;
    //        entity->position.y = posStore.y;
    //    }
    //}
}

void Player_Create(void *data)
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Player->sonicSpriteIndex, 0, &entity->playerAnimData, 1, 0);
        entity->characterID = ID_SONIC;
    }
    else {
        entity->playerID = RSDK.GetEntityID(RSDK_sceneInfo->entity);
        switch (entity->characterID) {
            case ID_TAILS:
                entity->spriteIndex     = Player->tailsSpriteIndex;
                entity->tailSpriteIndex = Player->tailsTailsSpriteIndex;
                entity->cameraOffset    = 0;
                entity->movesetPtr      = Player_TailsJumpAbility;
                entity->sensorY         = 0x100000;
                break;
            case ID_KNUCKLES:
                entity->spriteIndex     = Player->knuxSpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                entity->movesetPtr      = Player_KnuxJumpAbility;
                entity->sensorY         = 0x140000;
                break;
            case ID_MIGHTY:
                entity->spriteIndex     = Player->mightySpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                entity->movesetPtr      = Player_MightyJumpAbility;
                entity->sensorY         = 0x140000;
                break;
            case ID_RAY:
                entity->spriteIndex     = Player->raySpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                entity->movesetPtr      = Player_RayJumpAbility;
                entity->sensorY         = 0x140000;
                break;
            default:
                entity->spriteIndex     = Player->sonicSpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                entity->movesetPtr      = Player_SonicJumpAbility;
                entity->sensorY         = 0x140000;
                if (options->medalMods & getMod(MEDAL_PEELOUT)) {
                    entity->move_Peelout = Player_State_StartPeelout;
                    for (int f = 0; f < 4; ++f) {
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
        entity->controllerID   = entity->playerID;
        entity->state          = Player_State_Ground;

        if (RSDK_sceneInfo->entitySlot && options->gameMode != MODE_COMPETITION) {
            if (RSDK_sceneInfo->entitySlot != 1 || options->gameMode != MODE_TIMEATTACK) {
                RSDK.Unknown102(entity->controllerID, -1);
                entity->inputState = Player_GetP2Inputs;
                entity->sidekick   = true;
            }
            else {
                Player->replayState();
            }
        }
        else {
            entity->inputState = Player_GetP1Inputs;
        }
        RSDK_stickL[entity->controllerID].unknown = 0.30000001;
        entity->rings                             = Player->rings;
        entity->ringExtraLife                     = Player->ringExtraLife;
        Player->rings                             = 0;
        Player->ringExtraLife                     = 100;
        entity->hyperRing                         = (Player->powerups >> 6) & 1;
        Player->powerups &= 0xBF;
        if (Player->powerups >= 128) {
            // entity->state = Player_Unknown9;
            Player->powerups &= 0x7F;
        }
        if (Player->powerups) {
            entity->shield = Player->powerups;
            Player_ApplyShieldEffect(entity);
        }

        Player->powerups = 0;
        if (options->gameMode == MODE_COMPETITION) {
            entity->lives    = options->competitionSession[entity->playerID + 0x50];
            entity->score    = 0;
            entity->score1UP = 50000;
        }
        else if (options->gameMode == MODE_TIMEATTACK) {
            entity->lives    = 1;
            entity->score    = 0;
            entity->score1UP = 50000;
        }
        else {
            entity->lives    = Player->savedLives;
            entity->score    = Player->savedScore;
            entity->score1UP = Player->savedScore1UP;
        }

        int target = entity->score1UP;
        int score  = entity->score;
        if (score <= target) {
            do
                score += 50000;
            while (score <= target);
            entity->score1UP = score;
        }

        entity->collisionLayers = Zone->fgLayers;
        entity->drawFX          = FX_ROTATE | FX_FLIP;
        RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->playerAnimData, 1, 0);
        Player_ChangePhysicsState(entity);
        entity->maxWalkSpeed = 0x40000;
        entity->maxJogSpeed  = 0x60000;
        entity->maxRunSpeed  = 0xC0000;
    }
}

void Player_StageLoad()
{
    if (!options->playerID) {
        options->playerID = RSDK.CheckStageFolder("MSZCutscene") != 1 ? ID_KNUCKLES : ID_KNUCKLES;
    }

    RSDK_sceneInfo->debugMode = (options->medalMods & getMod(MEDAL_DEBUGMODE)) > 0;
    RSDK.SetDebugValue("Debug Mode", RSDK_sceneInfo->debugMode, 1, 0, 1);
    if (options->medalMods & getMod(MEDAL_ANDKNUCKLES)) {
        options->playerID &= 0xFF;
        options->playerID |= (ID_KNUCKLES << 8);
    }
    Player->playerCount = 0;
    Player->active      = ACTIVE_ALWAYS;
    if (options->gameMode == MODE_COMPETITION)
        Player_LoadSpritesVS();
    else
        Player_LoadSprites();
    if (options->gameMode == MODE_ENCORE) {
        Player->playerCount    = 2;
        EntityPlayer *sidekick = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER2);
        sidekick->characterID  = ID_SONIC;
    }
    else {
        Player->playerCount = RSDK.GetEntityCount(Player->objectID, 0);
    }
    Player->field_950         = 1;
    Player->field_954         = 0;
    Player->field_9EC         = 0;
    Player->upState           = 0;
    Player->downState         = 0;
    Player->leftState         = 0;
    Player->rightState        = 0;
    Player->jumpPressState    = 0;
    Player->jumpHoldState     = 0;
    Player->sfx_Jump          = RSDK.GetSFX("Global/Jump.wav");
    Player->sfx_LoseRings     = RSDK.GetSFX("Global/LoseRings.wav");
    Player->sfx_Hurt          = RSDK.GetSFX("Global/Hurt.wav");
    Player->sfx_Roll          = RSDK.GetSFX("Global/Roll.wav");
    Player->sfx_Charge        = RSDK.GetSFX("Global/Charge.wav");
    Player->sfx_Release       = RSDK.GetSFX("Global/Release.wav");
    Player->sfx_PeelCharge    = RSDK.GetSFX("Global/PeelCharge.wav");
    Player->sfx_PeelRelease   = RSDK.GetSFX("Global/PeelRelease.wav");
    Player->sfx_Dropdash      = RSDK.GetSFX("Global/DropDash.wav");
    Player->sfx_Skidding      = RSDK.GetSFX("Global/Skidding.wav");
    Player->sfx_Grab          = RSDK.GetSFX("Global/Grab.wav");
    Player->sfx_Flying        = RSDK.GetSFX("Global/Flying.wav");
    Player->sfx_Tired         = RSDK.GetSFX("Global/Tired.wav");
    Player->sfx_Land          = RSDK.GetSFX("Global/Land.wav");
    Player->sfx_Slide         = RSDK.GetSFX("Global/Slide.wav");
    Player->sfx_Outtahere     = RSDK.GetSFX("Global/OuttaHere.wav");
    Player->sfx_Transform2    = RSDK.GetSFX("Stage/Transform2.wav");
    Player->sfx_PimPom        = RSDK.GetSFX("Stage/PimPom.wav");
    Player->sfx_Swap          = RSDK.GetSFX("Global/Swap.wav");
    Player->sfx_SwapFail      = RSDK.GetSFX("Global/SwapFail.wav");
    Player->sfx_MightyDeflect = RSDK.GetSFX("Global/MightyDeflect.wav");
    Player->sfx_MightyDrill   = RSDK.GetSFX("Global/MightyDrill.wav");
    Player->sfx_MightyLand    = RSDK.GetSFX("Global/MightyLand.wav");
    Player->sfx_MightyUnspin  = RSDK.GetSFX("Global/MightyUnspin.wav");

    Soundboard_LoadSFX("Global/RaySwoop.wav", 41417, NULL, NULL); // Player_CheckRaySwooping, Player_ChangeRaySwoopSFX
    Soundboard_LoadSFX("Global/RayDive.wav", 72323, NULL, NULL);  // Player_CheckRayDiving, Player_ChangeRayDiveSFX

    for (int p = 0; p < PLAYER_MAX; ++p) Player->gotHit[p] = false;
}

void Player_EditorDraw() {}

void Player_EditorLoad() {}

void Player_Serialize() { RSDK_EDITABLE_VAR(Player, VAR_ENUM, characterID); }

// Extra Entity Functions
void Player_LoadSprites()
{
    EntityPlayer *entity = NULL;
    while (RSDK.GetObjects(Player->objectID, (Entity **)&entity)) {
        int pID = options->playerID & 0xFF;
        if (pID == ID_MIGHTY || pID == ID_RAY)
            pID = ID_SONIC;

        if (pID & entity->characterID) {
            entity->characterID = options->playerID & 0xFF;
            switch (entity->characterID) {
                case ID_TAILS:
                    Player->tailsSpriteIndex      = RSDK.LoadAnimation("Players/Tails.bin", SCOPE_STAGE);
                    Player->tailsTailsSpriteIndex = RSDK.LoadAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                    break;
                case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK.LoadAnimation("Players/Knux.bin", SCOPE_STAGE); break;
                case ID_MIGHTY: Player->mightySpriteIndex = RSDK.LoadAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
                case ID_RAY: Player->raySpriteIndex = RSDK.LoadAnimation("Players/Ray.bin", SCOPE_STAGE); break;
                default:
                    Player->sonicSpriteIndex = RSDK.LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
                    Player->superSpriteIndex = RSDK.LoadAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                    break;
            }
            EntityPlayer *player1 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
            RSDK.CopyEntity(player1, entity, true);
            player1->camera = Camera_SetTargetEntity(0, player1);
            RSDK.AddScreen(&player1->position, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, true);
        }
        else {
            RSDK.DestroyEntity(entity, 0, 0);
        }
    }

    if ((options->playerID & -0x100) > 0) {
        entity                 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
        EntityPlayer *sidekick = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER2);

        for (int i = 0; i < 0x10; ++i) {
            Player->flyCarryPositions[i] = entity->position;
        }
        sidekick->objectID   = Player->objectID;
        sidekick->position.x = entity->position.x;
        sidekick->position.y = entity->position.y;

        if (options->gameMode != MODE_TIMEATTACK) {
            RSDK.AddScreen(&sidekick->position, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, true);
            sidekick->position.x -= 0x100000;
        }

        sidekick->characterID = options->playerID >> 8;
        switch (sidekick->characterID) {
            case ID_TAILS:
                Player->tailsSpriteIndex      = RSDK.LoadAnimation("Players/Tails.bin", SCOPE_STAGE);
                Player->tailsTailsSpriteIndex = RSDK.LoadAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                break;
            case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK.LoadAnimation("Players/Knux.bin", SCOPE_STAGE); break;
            case ID_MIGHTY: Player->mightySpriteIndex = RSDK.LoadAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
            case ID_RAY: Player->raySpriteIndex = RSDK.LoadAnimation("Players/Ray.bin", SCOPE_STAGE); break;
            default:
                sidekick->characterID    = ID_SONIC;
                Player->sonicSpriteIndex = RSDK.LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
                Player->superSpriteIndex = RSDK.LoadAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                break;
        }
    }
}
void Player_LoadSpritesVS()
{
    EntityPlayer *entity = 0;
    while (RSDK.GetObjects(Player->objectID, (Entity **)&entity)) {

        if (entity->characterID & 1) {
            int slotID = 0;
            for (int i = 0; i < options->competitionSession[23]; ++i, ++slotID) {
                EntityPlayer *player = (EntityPlayer *)RSDK.GetEntityByID(slotID);
                RSDK.CopyEntity(player, entity, 0);
                player->characterID = options->playerID >> 8 * i;
                switch (player->characterID) {
                    case ID_TAILS:
                        Player->tailsSpriteIndex      = RSDK.LoadAnimation("Players/Tails.bin", SCOPE_STAGE);
                        Player->tailsTailsSpriteIndex = RSDK.LoadAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                        break;
                    case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK.LoadAnimation("Players/Knux.bin", SCOPE_STAGE); break;
                    case ID_MIGHTY: Player->mightySpriteIndex = RSDK.LoadAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
                    case ID_RAY: Player->raySpriteIndex = RSDK.LoadAnimation("Players/Ray.bin", SCOPE_STAGE); break;
                    default:
                        player->characterID      = ID_SONIC;
                        Player->sonicSpriteIndex = RSDK.LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
                        Player->superSpriteIndex = RSDK.LoadAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                        break;
                }
                player->controllerID = i + 1;
                player->camera       = Camera_SetTargetEntity(i, player);
            }
        }
        RSDK.DestroyEntity(entity, 0, 0);
    }
}
void Player_SaveValues()
{
    EntityPlayer *entity     = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    Player->savedLives       = entity->lives;
    options->restartLives[0] = Player->savedLives;
    Player->savedScore       = entity->score;
    options->restartScore    = Player->savedScore;
    Player->savedScore1UP    = entity->score1UP;
    options->restartScore1UP = Player->savedScore1UP;
}
void Player_GiveRings(int amount, EntityPlayer *player, bool32 playSFX)
{
    if (options->gameMode == MODE_COMPETITION)
        options->competitionSession[player->playerID + 72] += amount;
    player->rings += amount;

    if (player->rings < 0) {
            player->rings = 0;
    }
    else if (player->rings >= 999) {
        player->rings = 999;
    }

    if (player->rings >= player->ringExtraLife) {
        if (options->gameMode != MODE_TIMEATTACK && options->gameMode != MODE_ENCORE) {
            if (player->lives < 99)
                player->lives++;
            //Music_PlayMusicTrack(13);
        }
        player->ringExtraLife += 100;
        if (player->ringExtraLife > 300)
            player->ringExtraLife = 1000;
    }

    if (playSFX) {
        if (Ring->pan) {
            int slot = RSDK.PlaySFX(Ring->sfx_Ring, 0, 255);
            RSDK.SetSoundAttributes(0, slot, 1.0, -1.0, 1.0);
            Ring->pan = 0;
        }
        else {
            int slot = RSDK.PlaySFX(Ring->sfx_Ring, 0, 255);
            RSDK.SetSoundAttributes(0, slot, 1.0, 1.0, 1.0);
            Ring->pan = 1;
        }
    }
}
void Player_GiveLife(EntityPlayer *entity)
{
    if (options->gameMode != MODE_TIMEATTACK && options->gameMode != MODE_ENCORE) {
        if (entity->lives < 99)
            entity->lives++;
        // Music_PlayMusicTrack(13);
    }
}
void Player_ApplyShieldEffect(EntityPlayer *player)
{
    if (player->shield && player->superState != 2 && player->invincibleTimer <= 0) {
        Entity *shield = (Entity *)RSDK.GetEntityByID((ushort)(Player->playerCount + RSDK.GetEntityID(player)));
        RSDK.DestroyEntity(shield, Shield->objectID, player);
    }
}
void Player_ChangeCharacter(EntityPlayer *entity, int character)
{
    ushort playerID     = entity->playerID;
    entity->characterID = character;
    options->playerID &= ~(255 << 8 * playerID);
    options->playerID |= character << 8 * entity->playerID;
    switch (entity->characterID) {
        case ID_TAILS:
            Player->tailsSpriteIndex      = RSDK.LoadAnimation("Players/Tails.bin", SCOPE_STAGE);
            Player->tailsTailsSpriteIndex = RSDK.LoadAnimation("Players/TailSprite.bin", SCOPE_STAGE);
            if (SizeLaser) {
                // SizeLaser[28].objectID     = RSDK.LoadAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
                //*(_WORD *)&SizeLaser[28].active = RSDK.LoadAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            }

            if (entity->isChibi == 1) {
                // entity->spriteIndex = SizeLaser[28].objectID;
                // entity->tailSpriteIndex = *(_WORD *)&SizeLaser[28].active;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex     = Player->tailsSpriteIndex;
                entity->tailSpriteIndex = Player->tailsTailsSpriteIndex;
                entity->cameraOffset    = 0;
            }
            entity->movesetPtr = Player_TailsJumpAbility;
            entity->sensorY    = 0x100000;
            break;
        case ID_KNUCKLES:
            Player->knuxSpriteIndex = RSDK.LoadAnimation("Players/Knux.bin", SCOPE_STAGE);
            if (SizeLaser) {
                SizeLaser[29].objectID = RSDK.LoadAnimation("Players/ChibiKnux.bin", SCOPE_STAGE);
            }
            if (entity->isChibi == 1) {
                // entity->spriteIndex  = SizeLaser[29].objectID;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex  = Player->knuxSpriteIndex;
                entity->cameraOffset = 0x50000;
            }
            entity->tailSpriteIndex = -1;
            entity->movesetPtr      = Player_KnuxJumpAbility;
            entity->sensorY         = 0x140000;
            break;
        case ID_MIGHTY:
            Player->mightySpriteIndex = RSDK.LoadAnimation("Players/Mighty.bin", SCOPE_STAGE);
            if (SizeLaser) {
                //*(_WORD *)&SizeLaser[29].active = RSDK.LoadAnimation("Players/ChibiMighty.bin", SCOPE_STAGE);
            }
            if (entity->isChibi == 1) {
                // entity->spriteIndex  = *(_WORD *)&SizeLaser[29].active;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex  = Player->mightySpriteIndex;
                entity->cameraOffset = 0x50000;
            }
            entity->tailSpriteIndex = -1;
            entity->movesetPtr      = Player_MightyJumpAbility;
            entity->sensorY         = 0x140000;
            break;
        case ID_RAY:
            Player->raySpriteIndex = RSDK.LoadAnimation("Players/Ray.bin", SCOPE_STAGE);
            if (SizeLaser) {
                SizeLaser[30].objectID = RSDK.LoadAnimation("Players/ChibiRay.bin", SCOPE_STAGE);
            }
            if (entity->isChibi == 1) {
                // entity->spriteIndex  = SizeLaser[30].objectID;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex  = Player->raySpriteIndex;
                entity->cameraOffset = 0x50000;
            }
            entity->tailSpriteIndex = -1;
            entity->movesetPtr      = Player_RayJumpAbility;
            entity->sensorY         = 0x140000;
            break;
        default:
            Player->sonicSpriteIndex = RSDK.LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
            Player->superSpriteIndex = RSDK.LoadAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
            if (SizeLaser) {
                //*(_WORD *)&SizeLaser[27].active = RSDK.LoadAnimation("Players/ChibiSonic.bin", SCOPE_STAGE);
            }
            if (entity->isChibi == 1) {
                // entity->spriteIndex  = *(_WORD *)&v19[27].active;
                entity->cameraOffset = 0x40000;
            }
            else {
                entity->spriteIndex  = Player->sonicSpriteIndex;
                entity->cameraOffset = 327680;
            }
            entity->tailSpriteIndex = -1;
            entity->movesetPtr      = Player_SonicJumpAbility;
            entity->sensorY         = 0x140000;
            if (options->medalMods & getMod(MEDAL_PEELOUT)) {
                entity->move_Peelout = Player_State_StartPeelout;
                for (int f = 0; f < 4; ++f) {
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

    // if (entity->state == Player_State_KnuxWallClimb || entity->state == Player_State_Hammerdrop || entity->state == Player_State_Dropdash ||
    // entity->state == Player_State_TailsFlight
    //    || entity->state == Player_State_KnuxGlideDrop || entity->state == Player_State_KnuxGlideLeft || entity->state ==
    //    Player_State_KnuxGlideRight || entity->state == Player_State_GlideSlide
    //    || entity->state == Player_State_LedgePullUp || entity->state == Player_State_RayGlide || entity->state == Player_State_MightyUnspin) {
    //    entity->state = Player_State_Air;
    //    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, 0, 0);
    //}
    // else {
    RSDK.SetSpriteAnimation(entity->spriteIndex, entity->playerAnimData.animationID, &entity->playerAnimData, 0, 0);
    //}

    if (entity->superState) {
        for (int i = 0; i < 6; ++i) {
            RSDK.SetPaletteEntry(0, i + 64, Player->superPalette_Sonic[i]);
            RSDK.SetPaletteEntry(0, i + 70, Player->superPalette_Tails[i]);
            RSDK.SetPaletteEntry(0, i + 80, Player->superPalette_Knux[i]);
            RSDK.SetPaletteEntry(0, i + 96, Player->superPalette_Mighty[i]);
            RSDK.SetPaletteEntry(0, i + 113, Player->superPalette_Ray[i]);

            if (HCZSetup) {
                RSDK.SetPaletteEntry(2, i + 64, Player->superPalette_Sonic_HCZ[i]);
                RSDK.SetPaletteEntry(2, i + 70, Player->superPalette_Tails_HCZ[i]);
                RSDK.SetPaletteEntry(2, i + 80, Player->superPalette_Knux_HCZ[i]);
                RSDK.SetPaletteEntry(2, i + 96, Player->superPalette_Mighty_HCZ[i]);
                RSDK.SetPaletteEntry(2, i + 113, Player->superPalette_Ray_HCZ[i]);
            }
            else if (CPZSetup) {
                RSDK.SetPaletteEntry(2, i + 64, Player->superPalette_Sonic_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 70, Player->superPalette_Tails_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 80, Player->superPalette_Knux_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 96, Player->superPalette_Mighty_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 113, Player->superPalette_Ray_CPZ[i]);
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
        if (entity->superState == 2)
            Player_CheckGoSuper(entity, 255);
    }
    Player_ChangePhysicsState(entity);
}
bool32 Player_CheckGoSuper(EntityPlayer *player, byte emeraldflags)
{
    byte emeralds = 127;
    if (emeraldflags != 255)
        emeralds = emeraldflags;
    // if (RSDK_sceneInfo->timeEnabled == false && !ERZStart && (!PhantomEgg || *(_DWORD *)&PhantomEgg[145] == 1)) {
    //    return false;
    //}

    if (Player->field_A6C) {
        if (!Player->field_A6C())
            return 0;
    }
    if ((player->superState >= 2 || emeralds != 127 || player->rings < 50) && emeraldflags != 255)
        return 0;

    RSDK.StopSFX(Player->sfx_SwapFail);
    // if (options->secrets & 2)
    // player->movesetPtr = layer_State_ERZSuperDash;

    switch (player->characterID) {
        case ID_SONIC:
            for (int c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 0x40, Player->superPalette_Sonic[c + 6]);
                RSDK.SetPaletteEntry(7, c + 0x40, Player->superPalette_Sonic[c]);
            }
            break;
        case ID_TAILS:
            for (int c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 0x40, Player->superPalette_Tails[c + 6]);
                RSDK.SetPaletteEntry(7, c + 0x40, Player->superPalette_Tails[c]);
            }
            break;
        case ID_KNUCKLES:
            for (int c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 0x40, Player->superPalette_Knux[c + 6]);
                RSDK.SetPaletteEntry(7, c + 0x40, Player->superPalette_Knux[c]);
            }
            break;
        case ID_MIGHTY:
            for (int c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 0x40, Player->superPalette_Mighty[c + 6]);
                RSDK.SetPaletteEntry(7, c + 0x40, Player->superPalette_Mighty[c]);
            }
            break;
        case ID_RAY:
            for (int c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 0x40, Player->superPalette_Ray[c + 6]);
                RSDK.SetPaletteEntry(7, c + 0x40, Player->superPalette_Ray[c]);
            }
            break;
        default: break;
    }

    if (player->characterID == ID_SONIC && !player->isChibi)
        player->spriteIndex = Player->superSpriteIndex;
    if (emeraldflags == 255) {
        player->superState = 2;
        Player_ChangePhysicsState(player);
    }
    else {
        if (player->isChibi)
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimData, true, 0);
        else
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_TRANSFORM, &player->playerAnimData, true, 0);
        player->invincibleTimer = 60;
        player->velocity.x      = 0;
        player->velocity.y      = 0;
        player->groundVel       = 0;
        player->onGround        = 0;
        player->nextAirState       = 0;
        player->nextGroundState    = 0;
        player->interaction     = 0;
        player->state           = Player_State_Transform;
        player->forceTransform  = 1;
        // if (!ERZStart && options->superMusicEnabled)
        //   Music_Unknown10(0.079999998);
        player->jumpAbilityTimer = 0;
        player->superState       = 1;
    }
    player->superBlendAmount    = 0;
    player->superBlendDirection = 0;
    player->superSecondTimer    = 60;
    player->timer               = 0;
    return true;
}

// Collision
Hitbox *Player_GetHitbox(EntityPlayer *player)
{
    Hitbox *playerHitbox = RSDK.GetHitbox(&player->playerAnimData, 0);
    if (playerHitbox)
        return playerHitbox;
    else
        return &defaultHitbox;
}

bool32 Player_CheckCollisionTouch(EntityPlayer *player, Entity *entity, Hitbox *entityHitbox)
{
    Hitbox *playerHitbox;
    Hitbox *otherHitbox;

    if (player->field_1F0)
        return 0;
    playerHitbox = player->outerbox;
    if (!playerHitbox)
        playerHitbox = RSDK.GetHitbox(&player->playerAnimData, 0);
    otherHitbox = &defaultHitbox;
    if (playerHitbox)
        otherHitbox = playerHitbox;
    return RSDK.CheckObjectCollisionTouchBox(entity, entityHitbox, player, otherHitbox);
}
bool32 Player_CheckCollisionBox(EntityPlayer *player, Entity *entity, Hitbox *entityHitbox)
{
    if (player->field_1F0)
        return 0;
    Hitbox *playerHitbox = player->outerbox;
    if (!playerHitbox)
        playerHitbox = RSDK.GetHitbox(&player->playerAnimData, 0);
    Hitbox *otherHitbox = &defaultHitbox;
    if (playerHitbox)
        otherHitbox = playerHitbox;
    switch (RSDK.CheckObjectCollisionBox(entity, entityHitbox, player, otherHitbox, true)) {
        case 0: 
        default: return 0;
        case 1: {
            player->glideTimer    = 0;
            player->collisionMode = CMODE_FLOOR;

            int colPos[2];
            if (entity->direction & FLIP_X) {
                colPos[0] = entity->position.x - (entityHitbox->right << 16);
                colPos[1] = entity->position.x - (entityHitbox->left << 16);
            }
            else {
                colPos[0] = entity->position.x + (entityHitbox->left << 16);
                colPos[1] = entity->position.x + (entityHitbox->right << 16);
            }

            int sensorX1 = player->position.x + player->sensorX[0];
            int sensorX3 = player->position.x + player->sensorX[2];
            int sensorX2 = player->position.x + player->sensorX[1];
            int sensorX4 = player->position.x + player->sensorX[3];
            int sensorX5 = player->position.x + player->sensorX[4];

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
                player->killFlagB |= 1;
            return 1;
        }
        case 2:
            player->glideTimer = 0;
            if (player->left && player->onGround) {
                player->groundVel  = -0x8000;
                player->position.x = 0;
            }
            return 2;
        case 3:
            player->glideTimer = 0;
            if (player->right && player->onGround)
                player->groundVel = 0x8000;
            return 3;
        case 4: return 4;
    }
}
bool32 Player_CheckCollisionPlatform(EntityPlayer *player, Entity *entity, Hitbox *entityHitbox)
{
    if (player->field_1F0)
        return false;
    Hitbox *playerHitbox = player->outerbox;
    if (!playerHitbox)
        playerHitbox = RSDK.GetHitbox(&player->playerAnimData, 0);
    Hitbox *otherHitbox = &defaultHitbox;
    if (playerHitbox)
        otherHitbox = playerHitbox;
    if (!RSDK.CheckObjectCollisionPlatform(entity, entityHitbox, player, otherHitbox, true))
        return false;
    player->glideTimer    = 0;
    player->collisionMode = CMODE_FLOOR;

    int colPos[2];
    if (entity->direction & FLIP_X) {
        colPos[0] = entity->position.x - (entityHitbox->right << 16);
        colPos[1] = entity->position.x - (entityHitbox->left << 16);
    }
    else {
        colPos[0] = entity->position.x + (entityHitbox->left << 16);
        colPos[1] = entity->position.x + (entityHitbox->right << 16);
    }

    int sensorX1 = player->position.x + player->sensorX[0];
    int sensorX3 = player->position.x + player->sensorX[2];
    int sensorX2 = player->position.x + player->sensorX[1];
    int sensorX4 = player->position.x + player->sensorX[3];
    int sensorX5 = player->position.x + player->sensorX[4];

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
        player->killFlagB |= 1;
    return true;
}

bool32 Player_CheckHit(EntityPlayer *player, Entity *entity)
{
    if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown || player->invincibleTimer
        || player->blinkTimer > 0) {
        return false;
    }

    if (player->position.x > entity->position.x)
        player->velocity.x = 0x20000;
    else
        player->velocity.x = -0x20000;

    // Player_Hit(player);
    return true;
}
bool32 Player_CheckBadnikHit(EntityPlayer *player, Entity *entity, Hitbox *entityHitbox)
{
    if (player->field_1F0)
        return 0;

    Hitbox playerHitbox;
    if (player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1) {
        Hitbox *plrHitbox = RSDK.GetHitbox(&player->playerAnimData, 0);
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

    EntityShield *shield = (EntityShield *)RSDK.GetEntityByID((ushort)(player->playerID + Player->playerCount));
    Hitbox *otherHitbox  = &defaultHitbox;
    if (shield->objectID != Shield->objectID || shield->state != Shield_State_Insta) {
        Hitbox *hitbox = player->outerbox;
        if (hitbox)
            hitbox = RSDK.GetHitbox(&player->playerAnimData, 0);

        if (hitbox)
            otherHitbox = hitbox;
    }
    else {
        Hitbox *plrHitbox = RSDK.GetHitbox(&player->playerAnimData, 0);
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
bool32 Player_CheckBossHit(EntityPlayer *player, Entity *entity)
{
    int active = player->invincibleTimer != 0 || player->playerAnimData.animationID == ANI_JUMP || player->playerAnimData.animationID == ANI_SPINDASH;
    bool flag  = false;
    switch (player->characterID) {
        case ID_SONIC:
        case ID_MIGHTY: active |= player->playerAnimData.animationID == ANI_DROPDASH; break;
        case ID_TAILS:
            if (!active) {
                active = player->playerAnimData.animationID == ANI_FLY || player->playerAnimData.animationID == ANI_FLYTIRED
                         || player->playerAnimData.animationID == ANI_FLYLIFT;
                if (player->position.y <= entity->position.y)
                    flag = true;
            }
            break;
        case ID_KNUCKLES: active |= player->playerAnimData.animationID == ANI_FLY || player->playerAnimData.animationID == ANI_FLYLIFTTIRED; break;
        default: break;
    }

    if (active && !flag) {
        player->groundVel  = -player->groundVel;
        player->velocity.x = -player->velocity.x;
        player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
        if (player->characterID == ID_KNUCKLES && player->playerAnimData.animationID == ANI_FLY) {
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLYTIRED, &player->playerAnimData, 0, 0);
            // player->state = Player_State_KnuxGlideDrop;
            return true;
        }
        else {
            /*if (player->state == Player_State_Hammerdrop || player->state == Player_State_RayGlide) {
                player->state = Player_State_Air;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimData, 0, 0);
                if (player->velocity.y < -0x40000)
                    player->velocity.y = -0x40000;
            }*/
            return true;
        }
    }
    else {
        if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown && !player->invincibleTimer
            && !player->blinkTimer) {
            if (player->position.x > entity->position.x)
                player->velocity.x = 0x20000;
            else
                player->velocity.x = -0x20000;
            Player_Hit(player);
        }
        return false;
    }
}
bool32 Player_CheckProjectileHit(EntityPlayer *player, Entity *projectile)
{
    bool32 active = false;
    if (player->characterID == ID_MIGHTY) {
        int anim = player->playerAnimData.animationID;
        if (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)
            active = true;
    }

    if (player->shield > 1) {
        if (active) {
            RSDK.PlaySFX(Player->sfx_MightyDeflect, 0, 0xFE);
        }
        int angle               = RSDK.ATan2(player->position.x - projectile->position.x, player->position.y - projectile->position.y);
        projectile->velocity.x  = -0x800 * RSDK.Cos256(angle);
        projectile->velocity.y  = -0x800 * RSDK.Sin256(angle);
        projectile->interaction = false;
        return true;
    }
    else if (active) {
        RSDK.PlaySFX(Player->sfx_MightyDeflect, 0, 0xFE);
        int angle               = RSDK.ATan2(player->position.x - projectile->position.x, player->position.y - projectile->position.y);
        projectile->velocity.x  = -0x800 * RSDK.Cos256(angle);
        projectile->velocity.y  = -0x800 * RSDK.Sin256(angle);
        projectile->interaction = false;
        return true;
    }

    if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown && !player->invincibleTimer
        && !player->blinkTimer) {
        if (player->position.x > projectile->position.x)
            player->velocity.x = 0x20000;
        else
            player->velocity.x = -0x20000;
        Player_Hit(player);
    }
    return false;
}
bool32 Player_CheckHit2(EntityPlayer *player, Entity *entity, bool32 flag)
{
    int anim      = player->playerAnimData.animationID;
    int character = player->characterID;
    bool32 active = player->invincibleTimer != 0 || anim == ANI_JUMP || anim == ANI_SPINDASH;
    switch (character) {
        case ID_SONIC:
        case ID_MIGHTY: active |= anim == ANI_DROPDASH; break;
        case ID_TAILS:
            if (!active) {
                active = anim == ANI_FLY || anim == ANI_FLYTIRED || anim == ANI_FLYLIFT;
                if (player->position.y <= entity->position.y) {
                    if (flag) {
                        if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown
                            && !player->invincibleTimer && !player->blinkTimer && (character != ID_MIGHTY || anim != ANI_CROUCH)) {
                            if (player->position.x > entity->position.x)
                                player->velocity.x = 0x20000;
                            else
                                player->velocity.x = -0x20000;
                            Player_Hit(player);
                        }
                    }
                    return false;
                }
            }
            break;
        case ID_KNUCKLES:
            active |= anim == ANI_FLY || anim == ANI_FLYLIFTTIRED;
            character = player->characterID;
            break;
        default: break;
    }

    if (active) {
        if (player->velocity.y <= 0) {
            player->velocity.y += 0x10000;
            return true;
        }
        else if (player->position.y >= entity->position.y || (player->characterID & 0xFF) == ID_MIGHTY && anim == ID_RAY) {
            player->velocity.y -= 0x10000;
            return true;
        }
        else {
            player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
            return true;
        }
    }
    else {
        if (flag) {
            if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown
                && !player->invincibleTimer && !player->blinkTimer && (character != ID_MIGHTY || anim != ANI_CROUCH)) {
                if (player->position.x > entity->position.x)
                    player->velocity.x = 0x20000;
                else
                    player->velocity.x = -0x20000;
                Player_Hit(player);
            }
        }
        return false;
    }
}

// Player State Helpers
void Player_ChangePhysicsState(EntityPlayer *entity)
{
    int *tablePtr = NULL;
    switch (entity->characterID) {
        case ID_TAILS: tablePtr = Player->tailsPhysicsTable; break;
        case ID_KNUCKLES: tablePtr = Player->knuxPhysicsTable; break;
        case ID_MIGHTY: tablePtr = Player->mightyPhysicsTable; break;
        case ID_RAY: tablePtr = Player->rayPhysicsTable; break;
        default: tablePtr = Player->sonicPhysicsTable; break;
    }

    int state = 0;
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

    int decelShift = 0;
    if (entity->superState == 2) {
        state |= 2;
        decelShift = 2;
    }
    if (entity->speedShoesTimer > 0) {
        state |= 4;
        decelShift = 1;
    }
    int tableID                 = 8 * state;
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
void Player_HandleGroundMovement()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;

    if (entity->glideTimer > 0) {
        entity->glideTimer--;
        if (!entity->flipY) {
            entity->groundVel += RSDK.Sin256(entity->angle) << 13 >> 8;
            byte cmode = entity->collisionMode - 1; // makes CMODE_FLOOR 0xFF so only non-floor modes are checked
            if (cmode <= CMODE_ROOF) {
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
    else {
        bool32 left  = false;
        bool32 right = false;
        if (entity->flipY) {
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
                    if (!entity->collisionMode && entity->groundVel > 0x40000 && !Zone->field_154) {
                        entity->direction = FLIP_NONE;
                        entity->skidding  = 24;
                    }
                    int skid = entity->skidSpeed;
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
                    if (!entity->collisionMode && entity->groundVel < -0x40000 && !Zone->field_154) {
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
            if (!entity->flipY) {
                entity->groundVel += RSDK.Sin256(entity->angle) << 13 >> 8;

                if (entity->right) {
                    if (!entity->left) {
                        if (entity->angle > 0xC0 && entity->angle < 0xE4 && entity->groundVel > -0x20000) {
                            if (entity->groundVel >= -0x28000) {
                                entity->glideTimer = 30;
                                byte cmode         = entity->collisionMode - 1; // makes CMODE_FLOOR 0xFF so only non-floor modes are checked
                                if (cmode <= CMODE_ROOF) {
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
                    }
                }
                else if (entity->left) {
                    if (entity->angle > 0x1C && entity->angle < 0x40 && entity->groundVel > -0x28000) {
                        if (entity->groundVel <= 0x20000) {
                            entity->glideTimer = 30;
                            byte cmode         = entity->collisionMode - 1; // makes CMODE_FLOOR 0xFF so only non-floor modes are checked
                            if (cmode <= CMODE_ROOF) {
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
                }
                else {
                    byte cmode = entity->collisionMode - 1; // makes CMODE_FLOOR 0xFF so only non-floor modes are checked
                    if (cmode <= CMODE_ROOF) {
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

            if (!entity->flipY) {
                if (entity->groundVel > 0x2000 || entity->groundVel < -0x2000)
                    entity->groundVel += RSDK.Sin256(entity->angle) << 13 >> 8;

                if ((entity->angle <= 0xC0 || entity->angle >= 0xE4) && (entity->angle <= 0x1C || entity->angle >= 0x40)) {
                    byte cmode = entity->collisionMode - 1; // makes CMODE_FLOOR 0xFF so only non-floor modes are checked
                    if (cmode <= CMODE_ROOF) {
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
                else if (entity->groundVel <= -0x10000) {
                    byte cmode = entity->collisionMode - 1; // makes CMODE_FLOOR 0xFF so only non-floor modes are checked
                    if (cmode <= CMODE_ROOF) {
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
                else if (entity->groundVel >= -0x10000) {
                    entity->glideTimer = 30;

                    byte cmode = entity->collisionMode - 1; // makes CMODE_FLOOR 0xFF so only non-floor modes are checked
                    if (cmode <= CMODE_ROOF) {
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
        }
    }
}
void Player_HandleGroundRotation()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;

    if (entity->angle <= 4 || entity->angle >= 252) {
        if (entity->angle <= 16 || entity->angle >= 232) {
            entity->rotation = 0;
        }
        else {
            entity->rotation <<= 1;
        }
    }
    else {
        int vel    = (abs(entity->groundVel) <= 0x60000) + 1;
        int newRot = 0;
        if (entity->angle <= 16 || entity->angle >= 232)
            newRot = 0;
        else
            newRot = entity->angle << 1;
        int dif = newRot - entity->rotation;

        int rot = 0;
        if (dif >= 0)
            rot = newRot - entity->rotation;
        else
            rot = entity->rotation - newRot;

        int rot2 = 0;
        if (dif + 0x200 >= 0)
            rot2 = dif + 0x200;
        else
            rot2 = entity->rotation - newRot - 0x200;

        int check = dif - 0x200;
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
void Player_HandleAirMovement()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;

    if (entity->camera) {
        entity->camera->field_94 = 1;
        entity->camera->field_90 = 0x200000;
    }
    entity->velocity.y += entity->gravityStrength;
    if (entity->velocity.y < entity->jumpCap && entity->playerAnimData.animationID == ANI_JUMP && !entity->jumpHold && entity->jumpAbility == 1) {
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
void Player_HandleAirFriction()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;

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
    entity->glideTimer = 0;
    entity->onGround   = 0;
    if (entity->collisionMode == CMODE_FLOOR && entity->state != Player_State_Roll)
        entity->position.y += entity->cameraOffset;

    entity->velocity.x = (entity->gravityStrength + entity->jumpStrength) * RSDK.Sin256(entity->angle);
    entity->velocity.x = (entity->groundVel * RSDK.Cos256(entity->angle) + entity->velocity.x) >> 8;
    entity->velocity.y = entity->groundVel * RSDK.Sin256(entity->angle);
    entity->velocity.y = (entity->velocity.y - (entity->gravityStrength + entity->jumpStrength) * RSDK.Cos256(entity->angle)) >> 8;
    if (entity->camera) {
        // entity->camera->field_94 = 1;
        // entity->camera->field_90 = 0x200000;
    }
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, 0, 0);
    if (entity->characterID == ID_TAILS) {
        entity->playerAnimData.animationSpeed = 120;
    }
    else {
        entity->playerAnimData.animationSpeed = ((abs(entity->groundedStore) * 0xF0) / 0x60000) + 0x30;
    }
    if (entity->playerAnimData.animationSpeed > 0xF0)
        entity->playerAnimData.animationSpeed = 0xF0;
    entity->angle            = 0;
    entity->collisionMode    = CMODE_FLOOR;
    entity->skidding         = 0;
    entity->jumpAbility      = 1;
    entity->jumpAbilityTimer = 1;
    RSDK.PlaySFX(Player->sfx_Jump, 0, 255);
    entity->state = Player_State_Air;
}
void Player_StartRoll()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, false, 0);
    entity->pushing = 0;
    // entity->state        = Player_State_Roll;
    if (entity->collisionMode == CMODE_FLOOR) {
        entity->position.y += entity->cameraOffset;
    }
}
void Player_State_StartPeelout()
{
    EntityPlayer *entity; // esi

    entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, true, 0);
    entity->state          = Player_State_Peelout;
    entity->abilityTimer   = 0;
    entity->spindashCharge = 0;
    RSDK.PlaySFX(Player->sfx_PeelCharge, 0, 255);
}

// Player States
void Player_HandleRollDeceleration()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    int initialVel       = entity->groundVel;
    if (entity->right && entity->groundVel < 0)
        entity->groundVel += entity->rollingDeceleration;

    if (entity->left && entity->groundVel > 0)
        entity->groundVel -= entity->rollingDeceleration;

    if (entity->groundVel) {
        if (entity->groundVel <= 0) {
            entity->groundVel += entity->rollingFriction;

            if (RSDK.Sin256(entity->angle) >= 0)
                entity->groundVel += (0x1400 * RSDK.Sin256(entity->angle)) >> 8;
            else
                entity->groundVel += (0x5000 * RSDK.Sin256(entity->angle)) >> 8;
            if (entity->groundVel < -0x120000)
                entity->groundVel = -0x120000;
        }
        else {
            entity->groundVel -= entity->rollingFriction;
            if (RSDK.Sin256(entity->angle) >= 0)
                entity->groundVel += (0x1400 * RSDK.Sin256(entity->angle)) >> 8;
            else
                entity->groundVel += (0x5000 * RSDK.Sin256(entity->angle)) >> 8;
            if (entity->groundVel > 0x120000)
                entity->groundVel = 0x120000;
        }
    }
    else {
        entity->groundVel += 0x5000 * RSDK.Sin256(entity->angle) >> 8;
    }

    switch (entity->collisionMode) {
        case CMODE_FLOOR:
            if (entity->state != Player_State_ForceRoll) {
                if (initialVel <= 0) {
                    if (entity->groundVel >= 0) {
                        entity->groundVel = 0;
                        entity->state     = Player_State_Ground;
                    }
                }
                else if (entity->groundVel <= 0) {
                    entity->groundVel = 0;
                    entity->state     = Player_State_Ground;
                }
            }
            else {
                if (entity->groundVel < 0x10000 && entity->groundVel > -0x10000) {
                    entity->groundVel = 0x40000;
                    if (entity->direction & FLIP_Y)
                        entity->groundVel = -0x40000;
                }
            }
            break;
        case CMODE_LWALL:
        case CMODE_RWALL:
            if (entity->angle >= 0x40 && entity->angle <= 0xC0) {
                if (entity->groundVel > -0x20000 && entity->groundVel < 0x20000) {
                    entity->velocity.x    = entity->groundVel * RSDK.Cos256(entity->angle) >> 8;
                    entity->onGround      = false;
                    entity->angle         = 0;
                    entity->collisionMode = CMODE_FLOOR;
                    entity->velocity.y    = (entity->groundVel * RSDK.Sin256(entity->angle)) >> 8;
                }
            }
            break;
        case CMODE_ROOF:
            if (entity->flipY) {
                if (initialVel <= 0) {
                    if (entity->groundVel >= 0) {
                        entity->groundVel = 0;
                        entity->state     = Player_State_Ground;
                    }
                }
                else if (entity->groundVel <= 0) {
                    entity->groundVel = 0;
                    entity->state     = Player_State_Ground;
                }
            }
            else {
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
            break;
    }
}
void Player_Hit(EntityPlayer *player)
{
    byte flag = 0;
    if (player->sidekick) {
        flag = 1;
    }
    else {
        int entityID   = RSDK.GetEntityID(player);
        Entity *shield = (Entity *)RSDK.GetEntityByID((ushort)(Player->playerCount + entityID));
        if (shield->objectID == Shield->objectID) {
            player->shield = 0;
            RSDK.DestroyEntity(shield, 0, 0);
            flag = 1;
        }
        else {
            flag = (player->rings <= 0) + 2;
        }
        if (!Player->gotHit[player->playerID])
            Player->gotHit[player->playerID] = true;
    }

    player->nextAirState    = NULL;
    player->nextGroundState = NULL;
    if (options->coolBonus[player->playerID] > 0) {
        options->coolBonus[player->playerID] -= 1000;
    }

    switch (flag) {
        default: break;
        case 1: // Hurt, no rings (shield/p2/etc)
            player->state = Player_State_Hit;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimData, 0, 0);
            player->velocity.y     = -0x40000;
            player->onGround       = 0;
            player->tileCollisions = 1;
            player->blinkTimer     = 120;
            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y = -0x20000;
            }
            RSDK.PlaySFX(Player->sfx_Hurt, 0, 0xFF);
            break;
        case 2: // Hurt, lost rings
            player->state = Player_State_Hit;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimData, 0, 0);
            player->velocity.y     = -0x40000;
            player->onGround       = 0;
            player->tileCollisions = 1;
            player->blinkTimer     = 120;
            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y = -0x20000;
            }

            // if (player->hyperRing)
            //    Player_LoseHyperRings(player, player->rings, player->collisionPlane);
            // else
            //    Player_LoseRings(player, player->rings, player->collisionPlane);
            player->hyperRing     = 0;
            player->rings         = 0;
            player->ringExtraLife = 100;
            RSDK.PlaySFX(Player->sfx_LoseRings, 0, 0xFF);
            break;
        case 3: // Hurt, dies.
            player->hurtFlag = 1;
            break;
    }
}
bool32 Player_CheckValidState(EntityPlayer *player)
{
    if (player->objectID == Player->objectID && !player->hurtFlag) {
        if (                                                                         // player->state != Player_State_Unknown1 &&
            player->state != Player_State_Die && player->state != Player_State_Drown //&&
                                                                                     // player->state != Player_Unknown8 &&
                                                                                     // player->state != Player_State_FlyIn &&
                                                                                     // player->state != Player_State_JumpIn &&
            /*player->state != Player_State_Transform*/) {
            return true;
        }
    }
    return false;
}

void Player_State_None()
{ /* oh yeah buddy, NOTHING!*/
}
void Player_State_Ground()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (!entity->onGround) {
        entity->state = Player_State_Air;
        return Player_HandleAirMovement();
    }

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
        if (!entity->left && !entity->right)
            entity->pushing = 0;
        else {
            if (abs(entity->groundVel) > 0x10000)
                entity->pushing = 0;
        }
    }

    if (entity->flipY) {
        if (entity->collisionMode != CMODE_ROOF) {
            entity->pushing = 0;
        }
    }
    else if (entity->collisionMode != CMODE_FLOOR) {
        entity->pushing = 0;
    }

    Player_HandleGroundRotation();
    Player_HandleGroundMovement();

    // if (entity->camera)
    //    entity->camera->field_94 = 0;
    entity->jumpAbilityTimer = 0;

    entity->nextAirState = Player_State_Air;
    if (entity->skidding <= 0) {
        if (entity->pushing > -3 && entity->pushing < 3) {
            if (entity->groundVel || entity->angle >= 32 && entity->angle <= 224 && !entity->flipY) {
                entity->timer          = 0;
                entity->outtaHereTimer = 0;
                if (abs(entity->groundVel) >= entity->maxWalkSpeed) {
                    if (abs(entity->groundVel) >= entity->maxJogSpeed) {
                        if (abs(entity->groundVel) < entity->maxRunSpeed) {
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_RUN, &entity->playerAnimData, 0,
                                                    entity->playerAnimData.animationID - ANI_RUN > 1);

                            entity->playerAnimData.animationSpeed = (abs(entity->groundVel) >> 12);
                            if ((abs(entity->groundVel) >> 12) + 96 > 0x200)
                                entity->playerAnimData.animationSpeed = 0x200;
                            entity->maxJogSpeed = 0x58000;
                            entity->maxRunSpeed = 0xC0000;
                        }
                        else {
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DASH, &entity->playerAnimData, 0,
                                                    entity->playerAnimData.animationID - ANI_RUN > 1);
                            entity->maxRunSpeed = 0xB8000;
                        }
                    }
                    else {
                        if (entity->playerAnimData.animationID != ANI_WALK || entity->playerAnimData.frameID == 3)
                            RSDK.SetSpriteAnimation(entity->spriteIndex, 7, &entity->playerAnimData, 0, 0);
                        entity->playerAnimData.animationSpeed = (abs(entity->groundVel) >> 12) + 64;
                        entity->maxWalkSpeed                  = 0x38000;
                        entity->maxJogSpeed                   = 0x60000;
                    }
                }
                else {
                    if (entity->playerAnimData.animationID == ANI_JOG) {
                        if (entity->playerAnimData.frameID == 9)
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, 9);
                    }
                    else if (entity->playerAnimData.animationID == ANI_AIRWALK) {
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, entity->playerAnimData.frameID);
                    }
                    else {
                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, 0);
                    }
                    entity->playerAnimData.animationSpeed = (abs(entity->groundVel) >> 12) + 48;
                    entity->maxWalkSpeed                  = 0x40000;
                }
            }
            else {
                entity->maxWalkSpeed = 0x40000;
                entity->maxJogSpeed  = 0x60000;
                entity->maxRunSpeed  = 0xC0000;
                Vector2 posStore;
                posStore = entity->position;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[0], entity->sensorY, 10)
                    << 0;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[1], entity->sensorY, 10)
                    << 1;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[2], entity->sensorY, 10)
                    << 2;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[3], entity->sensorY, 10)
                    << 3;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, CMODE_FLOOR, entity->collisionPlane, entity->sensorX[4], entity->sensorY, 10)
                    << 4;

                entity->position = posStore;
                switch (entity->flailing - 1) {
                    case 0:
                    case 2:
                        if (entity->direction == FLIP_X || entity->characterID == ID_SONIC && entity->superState == 2 || entity->isChibi) {
                            entity->direction = FLIP_X;
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimData, 0, 0);
                        }
                        else {
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE2, &entity->playerAnimData, 0, 0);
                        }
                        break;
                    case 15:
                    case 23:
                        if (entity->direction && (entity->characterID != ID_SONIC || entity->superState != 2) && entity->isChibi != 1) {
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE2, &entity->playerAnimData, 0, 0);
                        }
                        else {
                            entity->direction = FLIP_NONE;
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimData, 0, 0);
                        }
                        break;
                    case 110:
                    case 115: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimData, 0, 0); break;
                    default:
                        switch (entity->characterID) {
                            case ID_SONIC:
                                if (entity->timer != 720 || entity->isChibi || entity->superState == 2) {
                                    if (entity->timer < 240) {
                                        entity->timer++;
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimData, 0, 0);
                                    }
                                    else {
                                        entity->timer++;
                                        if (entity->playerAnimData.animationID == ANI_BORED1) {
                                            if (entity->playerAnimData.frameID == 41)
                                                entity->timer = 0;
                                        }
                                        else
                                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimData, 0, 0);
                                    }
                                }
                                else {
                                    if (entity->playerAnimData.animationID == ANI_BORED2) {
                                        if (entity->playerAnimData.frameID == 67)
                                            entity->timer = 0;
                                    }
                                    else
                                        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED2, &entity->playerAnimData, 0, 0);
                                }
                                break;
                            case ID_TAILS:
                                if (entity->timer < 240) {
                                    entity->timer++;
                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimData, 0, 0);
                                }
                                else if (entity->playerAnimData.animationID == ANI_BORED1) {
                                    if (entity->playerAnimData.frameID == 45)
                                        entity->timer = 0;
                                }
                                else
                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimData, 0, 0);
                                break;
                            case ID_KNUCKLES:
                                if (entity->timer < 240) {
                                    entity->timer++;
                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimData, 0, 0);
                                }
                                else if (entity->playerAnimData.animationID == ANI_BORED1) {
                                    if (entity->playerAnimData.frameID == 69)
                                        entity->timer = 0;
                                }
                                else
                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimData, 0, 0);
                                break;
                            case ID_MIGHTY:
                                if (entity->timer < 240) {
                                    entity->timer++;
                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimData, 0, 0);
                                }
                                else if (entity->playerAnimData.animationID == ANI_BORED1) {
                                    if (entity->playerAnimData.frameID == 35)
                                        entity->timer = 0;
                                }
                                else
                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimData, 0, 0);
                                break;
                            case ID_RAY:
                                if (entity->timer < 240) {
                                    entity->timer++;
                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_IDLE, &entity->playerAnimData, 0, 0);
                                }
                                else if (entity->playerAnimData.animationID == ANI_BORED1) {
                                    if (entity->playerAnimData.frameID == 55)
                                        entity->timer = 0;
                                }
                                else {
                                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BORED1, &entity->playerAnimData, 0, 0);
                                }
                                break;
                            default: break;
                        }
                        break;
                }

                if (++entity->outtaHereTimer >= 72000000) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_OUTTAHERE, &entity->playerAnimData, 0, 0);
                    entity->state          = Player_State_OuttaHere;
                    entity->tileCollisions = 0;
                    entity->interaction    = 0;
                    entity->nextAirState      = 0;
                    entity->nextGroundState   = 0;
                    entity->velocity.x     = 0;
                    entity->velocity.y     = 0;
                    RSDK.PlaySFX(Player->sfx_Outtahere, 0, 255);
                }
            }
        }
        else {
            if (entity->pushing < -3)
                entity->pushing = -3;
            if (entity->pushing > 3)
                entity->pushing = 3;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_PUSH, &entity->playerAnimData, 0, 0);
        }
    }
    else {
        if (entity->playerAnimData.animationID != ANI_SKID) {
            if (entity->playerAnimData.animationID == ANI_SKIDTURN) {
                if (entity->playerAnimData.frameID == entity->playerAnimData.frameCount - 1) {
                    entity->direction ^= FLIP_X;
                    entity->skidding = 1;
                    RSDK.SetSpriteAnimation(entity->spriteIndex, 5, &entity->playerAnimData, 0, 0);
                }
                --entity->skidding;
            }
            else {
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SKID, &entity->playerAnimData, 0, 0);
                if (abs(entity->groundVel) >= 0x60000) {
                    int spd = 144;
                    if (abs(entity->groundVel) >= 0xA0000)
                        spd = 64;
                    entity->playerAnimData.animationSpeed = spd;
                }
                else {
                    entity->skidding -= 8;
                }
                RSDK.PlaySFX(Player->sfx_Skidding, 0, 255);
                EntityDust *dust = (EntityDust *)RSDK.SpawnEntity(Dust->objectID, entity, entity->position.x, entity->position.y);
                dust->state      = Dust_State_Skid;
                --entity->skidding;
            }
        }

        if (entity->direction) {
            if (entity->groundVel >= 0) {
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SKIDTURN, &entity->playerAnimData, 0, 0);
            }
        }
        else if (entity->groundVel <= 0) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SKIDTURN, &entity->playerAnimData, 0, 0);
        }
        --entity->skidding;
    }

    if (entity->jumpPress) {
        Player_StartJump(entity);
        entity->timer = 0;
    }
    else {
        if (entity->groundVel) {
            int cap = 0x8800;
            if (entity->state == Player_State_Crouch)
                cap = 0x11000;
            if (abs(entity->groundVel) >= cap && !entity->left && !entity->right && entity->down) {
                Player_StartRoll();
                RSDK.PlaySFX(Player->sfx_Roll, 0, 255);
            }
        }
        else {
            if (entity->angle < 32 || entity->angle > 224 && !entity->collisionMode || entity->flipY && entity->angle == 128) {
                if (entity->up) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_LOOKUP, &entity->playerAnimData, true, 1);
                    entity->timer = 0;
                    entity->state = Player_State_LookUp;
                }
                else if (entity->down) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_CROUCH, &entity->playerAnimData, true, 1);
                    entity->timer = 0;
                    entity->state = Player_State_Crouch;
                }
            }
        }
    }
}
void Player_State_Air()
{
    EntityPlayer *entity   = (EntityPlayer *)RSDK_sceneInfo->entity;
    entity->tileCollisions = true;
    Player_HandleAirFriction();
    if (entity->onGround) {
        entity->state = Player_State_Ground;
        // if (entity->camera)
        //    entity->camera->field_94 = 0;
        entity->jumpAbilityTimer = 0;
    }
    else {
        Player_HandleAirMovement();
        entity->nextGroundState = Player_State_Ground;
        if (entity->velocity.y > 0) {
            if (entity->playerAnimData.animationID >= ANI_SPRINGTWIRL) {
                if (entity->playerAnimData.animationID <= ANI_SPRINGDIAGONAL) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, entity->storedAnim, &entity->playerAnimData, 0, 0);
                }
                else if (entity->playerAnimData.animationID == ANI_SPRINGCS && !entity->playerAnimData.frameID) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0, 0);
                }
            }
        }

        switch (entity->playerAnimData.animationID) {
            case ANI_IDLE:
            case ANI_WALK:
                if (entity->playerAnimData.animationSpeed < 64)
                    entity->playerAnimData.animationSpeed = 64;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0, entity->playerAnimData.frameID);
                break;
            case ANI_LOOKUP:
            case ANI_CROUCH:
            case ANI_SKIDTURN: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0, 0); break;
            case ANI_JOG: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0, 0); break;
            case ANI_JUMP:
                if (entity->movesetPtr) {
                    if (entity->velocity.y >= entity->jumpCap)
                        entity->movesetPtr();
                }
                break;
            case ANI_SKID:
                if (entity->skidding < 1) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0, entity->playerAnimData.frameID);
                }
                else {
                    entity->skidding--;
                }
                break;
            case ANI_SPINDASH: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, 0, 0); break;
            default: break;
        }
    }
}
void Player_State_Roll()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    Player_HandleGroundRotation();
    Player_HandleRollDeceleration();
    entity->jumpAbility = 0;
    if (!entity->onGround) {
        entity->state = Player_State_Air;
        Player_HandleAirMovement();
    }
    else {
        if (entity->characterID == ID_TAILS) {
            entity->playerAnimData.animationSpeed = 120;
        }
        else {
            entity->playerAnimData.animationSpeed = ((abs(entity->groundedStore) * 0xF0) / 0x60000) + 0x30;
        }
        if (entity->playerAnimData.animationSpeed > 0xF0)
            entity->playerAnimData.animationSpeed = 0xF0;
        if (entity->camera)
            entity->camera->field_94 = 0;
        entity->jumpAbilityTimer = 0;
        if (entity->jumpPress)
            Player_StartJump(entity);
    }
}
void Player_State_ForceRoll()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    Player_HandleGroundRotation();
    bool32 left  = entity->left;
    bool32 right = entity->right;
    if (entity->glideTimer > 0) {
        entity->left  = 0;
        entity->right = 0;
        entity->glideTimer--;
    }
    Player_HandleRollDeceleration();
    entity->left        = left;
    entity->right       = right;
    entity->jumpAbility = 0;
    if (!entity->onGround) {
        entity->state = Player_State_RollLock;
        Player_HandleAirMovement();
    }
    else {
        if (entity->characterID == ID_TAILS) {
            entity->playerAnimData.animationSpeed = 120;
        }
        else {
            entity->playerAnimData.animationSpeed = ((abs(entity->groundedStore) * 0xF0) / 0x60000) + 0x30;
        }
        if (entity->playerAnimData.animationSpeed > 0xF0)
            entity->playerAnimData.animationSpeed = 0xF0;
        if (entity->camera) {
            entity->camera->field_94 = 0;
        }
        entity->jumpAbilityTimer = 0;
    }
}
void Player_State_RollLock()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    Player_HandleGroundRotation();
    bool32 left  = entity->left;
    bool32 right = entity->right;
    if (entity->glideTimer > 0) {
        entity->left  = 0;
        entity->right = 0;
        entity->glideTimer--;
    }
    Player_HandleRollDeceleration();
    entity->left        = left;
    entity->right       = right;
    entity->jumpAbility = 0;
    if (!entity->onGround) {
        Player_HandleAirMovement();
    }
    else {
        if (entity->camera) {
            entity->camera->field_94 = 0;
        }
        entity->jumpAbilityTimer = 0;
    }
}
void Player_State_LookUp()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->flipY)
        entity->rotation = 0x80;
    entity->left  = 0;
    entity->right = 0;
    Player_HandleGroundMovement();
    if (entity->camera)
        entity->camera->field_94 = 0;
    entity->jumpAbilityTimer = 0;
    entity->nextAirState        = Player_State_Air;
    if (entity->up) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_LOOKUP, &entity->playerAnimData, false, 1);
        if (entity->playerAnimData.frameID == 5)
            entity->playerAnimData.animationSpeed = 0;

        if (entity->timer < 60) {
            entity->timer++;
        }
        else if (entity->camera) {
            if (entity->flipY) {
                if (entity->camera->lookPos.y < 96)
                    entity->camera->lookPos.y += 2;
            }
            else {
                if (entity->camera->lookPos.y > -96)
                    entity->camera->lookPos.y -= 2;
            }
        }
        if (entity->jumpPress) {
            if (entity->move_Peelout)
                entity->move_Peelout();
            else
                Player_StartJump(entity);
        }
    }
    else {
        entity->playerAnimData.animationSpeed = 64;
        if (entity->playerAnimData.frameID == 0 || entity->left || entity->right)
            entity->state = Player_State_Ground;
        if (entity->jumpPress)
            Player_StartJump(entity);
    }
}
void Player_State_Crouch()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->flipY)
        entity->rotation = 0x80;
    entity->left  = 0;
    entity->right = 0;
    Player_HandleGroundMovement();
    if (entity->camera)
        entity->camera->field_94 = 0;
    entity->jumpAbilityTimer = 0;
    entity->nextAirState        = Player_State_Air;
    if (entity->down) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_CROUCH, &entity->playerAnimData, 0, 1);
        if (entity->playerAnimData.frameID == 4)
            entity->playerAnimData.animationSpeed = 0;

        if (entity->timer < 60) {
            entity->timer++;
        }
        else if (entity->camera) {
            if (entity->flipY) {
                if (entity->camera->lookPos.y > -96)
                    entity->camera->lookPos.y -= 2;
            }
            else {
                if (entity->camera->lookPos.y < 96)
                    entity->camera->lookPos.y += 2;
            }
        }

        if (entity->jumpPress) {
            EntityDust *dust = (EntityDust *)RSDK.SpawnEntity(Dust->objectID, entity, entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(Dust->spriteIndex, 1, &dust->data, true, 0);
            dust->state     = Dust_State_Spindash;
            dust->drawOrder = entity->drawOrder;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SPINDASH, &entity->playerAnimData, true, 0);
            entity->state          = Player_State_Spindash;
            entity->abilityTimer   = 0;
            entity->spindashCharge = 0;
            RSDK.PlaySFX(Player->sfx_Charge, 0, 255);
        }
    }
    else {
        entity->playerAnimData.animationSpeed = 128;
        if (entity->playerAnimData.frameID == 0 || entity->left || entity->right)
            entity->state = Player_State_Ground;
        if (entity->jumpPress)
            Player_StartJump(entity);
    }
}
void Player_State_Spindash()
{
    EntityPlayer *entity   = (EntityPlayer *)RSDK_sceneInfo->entity;
    float chargeSpeeds[12] = { 1.00000000, 1.06140053, 1.12553096, 1.19264507, 1.26303434, 1.33703494,
                               1.41503751, 1.49749970, 1.58496249, 1.67807186, 1.77760756, 1.88452280 };
    ///*(_OWORD *)chargeSpeeds     = SpindashChargeSpeeds;
    ///*(_OWORD *)&chargeSpeeds[4] = SpindashChargeSpeeds2;
    ///*(_OWORD *)&chargeSpeeds[8] = SpindashChargeSpeeds3;
    // v10                         = 2.0;
    if (entity->jumpPress) {
        entity->abilityTimer += 0x20000;
        if (entity->abilityTimer > 0x90000)
            entity->abilityTimer = 0x90000;

        if (entity->spindashCharge < 12)
            entity->spindashCharge++;
        if (entity->spindashCharge < 0)
            entity->spindashCharge = 0;

        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SPINDASH, &entity->playerAnimData, true, 0);
        int slot = RSDK.PlaySFX(Player->sfx_Charge, 0, 255);
        RSDK.SetSoundAttributes(0, slot, 1.0, 0.0, chargeSpeeds[entity->spindashCharge]);
    }
    else {
        entity->abilityTimer -= entity->abilityTimer >> 5;
    }

    if (!entity->down) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, 0, 0);

        if (entity->camera && !Zone->field_154) {
            entity->scrollDelay   = 15;
            entity->camera->state = Camera_State_VLock;
        }

        int vel = 0;
        if (entity->superState == 2)
            vel = (((uint)entity->abilityTimer >> 1) & 0x7FFF8000) + 0xB0000;
        else
            vel = (((uint)entity->abilityTimer >> 1) & 0x7FFF8000) + 0x80000;

        if (entity->direction)
            entity->groundVel = -vel;
        else
            entity->groundVel = vel;

        RSDK.StopSFX(Player->sfx_Charge);
        RSDK.PlaySFX(Player->sfx_Release, 0, 0xFF);
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, 0, 0);
        if (!entity->collisionMode) {
            entity->position.y += entity->cameraOffset;
        }
        entity->pushing = 0;
        entity->state   = Player_State_Roll;
    }
}
void Player_State_Peelout()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->gravityStrength == 0x1000) {
        if (entity->superState == 2) {
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
    else if (entity->superState == 2) {
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
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JOG, &entity->playerAnimData, 0, entity->playerAnimData.animationID - 8 > 1);
                entity->playerAnimData.animationSpeed = (entity->abilityTimer >> 12) + 96;
                if (entity->playerAnimData.animationSpeed > 0x200u)
                    entity->playerAnimData.animationSpeed = 512;
                entity->maxRunSpeed = 0xC0000;
                entity->maxJogSpeed = 0x58000;
            }
            else {
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DASH, &entity->playerAnimData, 0, entity->playerAnimData.animationID - 8 > 1);
                entity->maxRunSpeed = 0xB8000;
            }
        }
        else {
            if (entity->playerAnimData.animationID != ANI_WALK || entity->playerAnimData.frameID == 3)
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_RUN, &entity->playerAnimData, 0, 0);
            entity->playerAnimData.animationSpeed = (entity->abilityTimer >> 12) + 64;
            entity->maxWalkSpeed                  = 0x38000;
            entity->maxJogSpeed                   = 0x60000;
        }
    }
    else {
        if (entity->playerAnimData.animationID == ANI_JOG) {
            if (entity->playerAnimData.frameID == ANI_DASH)
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, 9);
        }
        else if (entity->playerAnimData.animationID == ANI_AIRWALK) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, entity->playerAnimData.frameID);
        }
        else {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, 0);
        }
        entity->playerAnimData.animationSpeed = (entity->abilityTimer >> 12) + 48;
        entity->maxWalkSpeed                  = 0x40000;
    }

    if (!entity->up) {
        RSDK.StopSFX(Player->sfx_PeelCharge);
        if (entity->abilityTimer >= 0x60000) {
            if (entity->camera && !Zone->field_154) {
                entity->scrollDelay   = 15;
                entity->camera->state = Camera_State_VLock;
            }

            if (entity->direction)
                entity->groundVel = -entity->abilityTimer;
            else
                entity->groundVel = entity->abilityTimer;
            RSDK.PlaySFX(Player->sfx_PeelRelease, 0, 0xFF);
        }
        entity->state = Player_State_Ground;
    }
}

void Player_State_OuttaHere()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (RSDK_sceneInfo->entity->onGround == 1) {
        if (entity->playerAnimData.frameID == 14) {
            entity->jumpAbility = 0;
            if (entity->direction == FLIP_X)
                entity->velocity.x = -0x10000;
            else
                entity->velocity.x = 0x10000;
            entity->velocity.y = -0x58000;
            entity->onGround   = false;
            entity->inputState = NULL;
            if (entity->camera)
                entity->camera->state = NULL;
        }
    }
    else {
        Player_HandleAirMovement();
        if (entity->velocity.y > 0x100000) {
            RSDK.ResetEntity(SLOT_PAUSEMENU, GameOver->objectID, 0);
            RSDK.DestroyEntity(entity, 0, 0);
        }
    }
}
void Player_State_Transform()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    entity->position.x += Zone->field_154;
    entity->invincibleTimer = 60;
    RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, 0, 0x140000, 1);
    if (++entity->timer != 36) {
        if (!entity->isChibi) {
            if (entity->playerAnimData.frameID == entity->playerAnimData.frameCount - 1) {
                entity->forceTransform = 0;
                entity->interaction    = 1;
                entity->state          = Player_State_Air;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, 3);
                if (!ERZStart) {
                    // if (options->superMusicEnabled)
                    //    Music_PlayMusicTrack(10);
                }
            }
        }
    }
    else {
        Entity *powerup = RSDK.GetEntityByID((ushort)(entity->playerID + 2 * Player->playerCount));
        RSDK.DestroyEntity(powerup, ImageTrail->objectID, entity);
        if (ERZStart)
            entity->shield = SHIELD_LIGHTNING;
        RSDK.PlaySFX(Player->sfx_Transform2, 0, 255);
        Entity *shield = RSDK.GetEntityByID((ushort)(entity->playerID + Player->playerCount));
        RSDK.DestroyEntity(shield, SuperSparkle->objectID, entity);
        entity->superState = 2;
        Player_ChangePhysicsState(entity);

        if (!entity->isChibi) {
            if (entity->playerAnimData.frameID == entity->playerAnimData.frameCount - 1) {
                entity->forceTransform = 0;
                entity->interaction    = 1;
                entity->state          = Player_State_Air;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, 3);
                if (!ERZStart) {
                    // if (options->superMusicEnabled)
                    //    Music_PlayMusicTrack(10);
                }
            }
        }
        else {
            entity->forceTransform = 0;
            entity->interaction    = true;
            entity->state          = Player_State_Air;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_WALK, &entity->playerAnimData, 0, 3);
            // Music_PlayMusicTrack(10);
        }
    }
}
void Player_State_Hit()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
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
        entity->pushing    = 0;
        entity->glideTimer = 0;
        if (entity->camera)
            entity->camera->field_94 = 0;
        entity->jumpAbilityTimer = 0;
        entity->skidding         = 0;
    }
    else {
        if (entity->camera) {
            entity->camera->field_94 = 1;
            entity->camera->field_90 = 0x200000;
        }
        entity->skidding = 0;
        if (!entity->underwater)
            entity->velocity.y += 0x3000;
        else
            entity->velocity.y += 0xF00;
    }
}
void Player_State_Die()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->superState == 2)
        entity->superState = 3;

    if (entity->blinkTimer) {
        entity->blinkTimer = 0;
        entity->visible    = 1;
    }
    entity->velocity.y += entity->gravityStrength;
    entity->velocity.x = 0;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DIE, &entity->playerAnimData, 0, 0);

    if (!entity->sidekick) {
        if (entity->camera) {
            int y = entity->camera->position.y - entity->camera->field_6C.y + 0x1800000;
            if (entity->position.y > y)
                entity->position.y = y;
            entity->scrollDelay = 2;
        }
    }

    // if (entity->velocity.y > 0x100000)
    //    Player_HandleDeath(entity);
}
void Player_State_Drown()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->superState == 2)
        entity->superState = 3;

    if (entity->blinkTimer) {
        entity->blinkTimer = 0;
        entity->visible    = 1;
    }
    entity->velocity.y += entity->gravityStrength;
    entity->velocity.x = 0;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DROWN, &entity->playerAnimData, 0, 0);
    if (!entity->sidekick) {
        if (entity->camera) {
            int y = entity->camera->position.y + 0x1000000;
            if (entity->position.y > y)
                entity->position.y = y;
        }
    }
    // if (entity->velocity.y > 0x100000)
    //    Player_HandleDeath(entity);
}
void Player_State_DropDash()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->onGround) {
        if (entity->camera)
            entity->camera->field_94 = 0;
        entity->jumpAbilityTimer = 0;
        if (entity->right)
            entity->direction = FLIP_NONE;
        if (entity->left)
            entity->direction = FLIP_X;

        int vel = 0;
        int cap = 0;
        if (entity->superState == 2) {
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

        if (entity->camera && !Zone->field_154) {
            entity->scrollDelay   = 8;
            entity->camera->state = Camera_State_VLock;
        }
        EntityDust *dust = (EntityDust *)RSDK.SpawnEntity(Dust->objectID, entity, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Dust->spriteIndex, 2, &dust->data, true, 0);
        dust->state = Dust_State_DropDash;
        dust->position.y += RSDK.GetHitbox(&entity->playerAnimData, 0)->bottom << 16;
        dust->direction = entity->direction;
        dust->drawOrder = entity->drawOrder;
        RSDK.StopSFX(Player->sfx_Dropdash);
        if (entity->superState == 2)
            RSDK.PlaySFX(Player->sfx_PeelRelease, 0, 255);
        else
            RSDK.PlaySFX(Player->sfx_Release, 0, 255);

        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, 0, 0);
        if (!entity->collisionMode) {
            entity->position.y += entity->cameraOffset;
        }
        entity->pushing = 0;
        entity->state   = Player_State_Roll;
    }
    else if (entity->jumpHold) {
        Player_HandleAirFriction();
        Player_HandleAirMovement();
        entity->playerAnimData.animationSpeed += 8;
        if (entity->playerAnimData.animationSpeed > 0x100)
            entity->playerAnimData.animationSpeed = 0x100;
    }
    else {
        entity->jumpAbilityTimer = 0;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, 0, 0);
        entity->state = Player_State_Air;
    }
}
void Player_State_BubbleBounce()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->shield == SHIELD_BUBBLE && !entity->invincibleTimer && entity->superState != 2) {
        if (!entity->onGround) {
            Player_HandleAirFriction();
            Player_HandleAirMovement();
        }
        else {
            entity->glideTimer = 0;
            int vel            = 0;
            if (!entity->underwater)
                vel = entity->gravityStrength + 0x78000;
            else
                vel = entity->gravityStrength + 0x40000;
            entity->onGround = false;

            entity->velocity.x = (entity->groundVel * RSDK.Cos256(entity->angle) + (vel * RSDK.Sin256(entity->angle))) >> 8;
            entity->velocity.y = (entity->velocity.y - (entity->groundVel * RSDK.Sin256(entity->angle)) * RSDK.Cos256(entity->angle)) >> 8;

            if (entity->camera) {
                entity->camera->field_94 = 1;
                entity->camera->field_90 = 0x200000;
            }

            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, 0, 0);
            entity->playerAnimData.animationSpeed = (abs(entity->groundVel) >> 12) + 48;
            int id                                = RSDK.GetEntityID(entity);
            EntityShield *shield                  = (EntityShield *)RSDK.GetEntityByID((ushort)(Player->playerCount + id));
            RSDK.SetSpriteAnimation(Shield->spriteIndex, 9, &shield->altData, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &shield->data, true, 0);
            shield->state = Shield_State_BubbleAlt;
            if (entity->playerAnimData.animationSpeed > 0xF0)
                entity->playerAnimData.animationSpeed = 0xF0;
            entity->angle            = 0;
            entity->collisionMode    = CMODE_FLOOR;
            entity->jumpAbility      = 0;
            entity->jumpAbilityTimer = 1;
            RSDK.PlaySFX(Shield->sfx_BubbleBounce, 0, 255);
            entity->state = Player_State_Air;
        }
    }
    else {
        entity->state = Player_State_Air;
    }
}
void Player_State_MightyUnspin()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    Player_HandleAirFriction();
    if (entity->onGround) {
        entity->state = Player_State_Ground;
        if (entity->camera)
            entity->camera->field_94 = 0;
        entity->jumpAbilityTimer = 0;
    }
    else {
        ++entity->blinkTimer;
        if (entity->camera) {
            entity->camera->field_94 = 1;
            entity->camera->field_90 = 0x200000;
        }
        Player_HandleAirMovement();
        entity->nextGroundState = Player_State_Ground;
    }
}

void Player_SonicJumpAbility()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    entity               = (EntityPlayer *)RSDK_sceneInfo->entity;
    bool flag            = false;
    if (entity->jumpAbilityTimer == 1) {
        if (entity->inputState != Player_GetP2Inputs || entity->up && options->gameMode != MODE_ENCORE) {
            if (entity->jumpPress) {
                int id               = RSDK.GetEntityID(entity);
                EntityShield *shield = (EntityShield *)RSDK.GetEntityByID((ushort)(Player->playerCount + id));
                if (entity->invincibleTimer) {
                    if (shield->objectID != Shield->objectID || shield->data.animationID != ANI_JUMP) {
                        if (!(options->medalMods & getMod(MEDAL_DROPDASH)))
                            ++entity->jumpAbilityTimer;
                    }
                }
                else {
                    switch (entity->shield) {
                        case SHIELD_NONE:
                            if (options->medalMods & getMod(MEDAL_INSTASHIELD)) {
                                entity->invincibleTimer  = -8;
                                entity->jumpAbilityTimer = 0;
                                RSDK.PlaySFX(Shield->sfx_InstaShield, 0, 255);
                                RSDK.DestroyEntity(shield, Shield->objectID, entity);
                                shield->inkEffect = INK_ADD;
                                shield->alpha     = 256;
                                RSDK.SetSpriteAnimation(Shield->spriteIndex, 10, &shield->data, true, 0);
                                shield->state = Shield_State_Insta;
                            }
                            if (!(options->medalMods & getMod(MEDAL_DROPDASH)))
                                ++entity->jumpAbilityTimer;
                            break;
                        case SHIELD_BLUE:
                            if (!(options->medalMods & getMod(MEDAL_DROPDASH)))
                                ++entity->jumpAbilityTimer;
                            break;
                        case SHIELD_BUBBLE:
                            entity->velocity.x >>= 1;
                            entity->velocity.y   = 0x80000;
                            entity->state        = Player_State_BubbleBounce;
                            entity->nextGroundState = NULL;
                            entity->nextAirState    = NULL;
                            RSDK.SetSpriteAnimation(Shield->spriteIndex, 7, &shield->altData, true, 0);
                            RSDK.SetSpriteAnimation(Shield->spriteIndex, 8, &shield->data, true, 0);
                            shield->state = Shield_State_Bubble;
                            RSDK.PlaySFX(Shield->sfx_BubbleBounce, 0, 255);
                            break;
                        case SHIELD_FIRE:
                            entity->jumpAbilityTimer = 0;
                            if (entity->direction == FLIP_X)
                                entity->velocity.x = -0x80000;
                            else
                                entity->velocity.x = 0x80000;
                            entity->velocity.y = 0;
                            RSDK.SetSpriteAnimation(Shield->spriteIndex, 2, &shield->data, true, 0);
                            shield->state     = Shield_State_Fire;
                            shield->direction = entity->direction;
                            if (entity->camera && !Zone->field_154) {
                                entity->scrollDelay   = 15;
                                entity->camera->state = Camera_State_VLock;
                            }
                            RSDK.PlaySFX(Shield->sfx_FireDash, 0, 255);
                            break;
                        case SHIELD_LIGHTNING:
                            entity->jumpAbilityTimer = 0;
                            if (entity->flipY)
                                entity->velocity.y = 0x58000;
                            else
                                entity->velocity.y = -0x58000;
                            shield->state = Shield_State_Lightning;
                            RSDK.PlaySFX(Shield->sfx_LightningJump, 0, 255);
                            break;
                        default: break;
                    }
                }
            }
            else {
                if (RSDK_controller[entity->controllerID].keyY.press)
                   Player_CheckGoSuper(entity, SaveGame->saveRAM[28]);
            }
            return;
        }
        flag = true;
    }

    if ((entity->jumpAbilityTimer >= 2 || flag) && entity->jumpHold) {
        entity->jumpAbilityTimer++;
        if (entity->jumpAbilityTimer > 22) {
            entity->state        = Player_State_DropDash;
            entity->nextGroundState = 0;
            entity->nextAirState    = 0;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DROPDASH, &entity->playerAnimData, 0, 0);
            RSDK.PlaySFX(Player->sfx_Dropdash, 0, 255);
        }
    }
}
void Player_TailsJumpAbility()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (!entity->jumpPress || !entity->jumpAbilityTimer
        || entity->inputState == Player_GetP2Inputs && (!entity->up || options->gameMode == MODE_ENCORE)) {
        if (RSDK_controller[entity->controllerID].keyY.press)
           Player_CheckGoSuper(entity, SaveGame->saveRAM[28]);
    }
    else if (!entity->flipY) {
        entity->jumpAbilityTimer = 0;
        entity->timer            = 0;
        entity->glideSpeedStore  = 0x800;
        if (!entity->underwater)
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimData, 0, 0);
        else
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SWIM, &entity->playerAnimData, 0, 0);
        // entity->state        = Player_State_TailsFlight;
        entity->nextGroundState = NULL;
        entity->nextAirState    = NULL;
    }
}
void Player_KnuxJumpAbility()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (!entity->jumpPress || !entity->jumpAbilityTimer
        || entity->inputState == Player_GetP2Inputs && (!entity->up || options->gameMode == MODE_ENCORE)) {
        if (RSDK_controller[entity->controllerID].keyY.press)
           Player_CheckGoSuper(entity, SaveGame->saveRAM[28]);
    }
    else if (!entity->flipY) {
        entity->jumpAbilityTimer = 0;
        entity->glideSpeedStore  = 0x40000;
        if (entity->velocity.y < 0)
            entity->velocity.y = 0;
        if (entity->direction) {
            // entity->state           = Player_State_KnuxGlideLeft;
            entity->velocity.x = -0x40000;
            entity->timer      = 0x100;
        }
        else {
            // entity->state           = Player_State_KnuxGlideRight;
            entity->velocity.x = 0x40000;
            entity->timer      = 0;
        }
        entity->nextGroundState = 0;
        entity->nextAirState    = 0;
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimData, false, 6);
    }
}
void Player_MightyJumpAbility()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->jumpAbilityTimer <= 1) {
        if (!entity->jumpPress || !entity->jumpAbilityTimer
            || entity->inputState == Player_GetP2Inputs && (!entity->up || options->gameMode == MODE_ENCORE)) {
            if (RSDK_controller[entity->controllerID].keyY.press)
               Player_CheckGoSuper(entity, SaveGame->saveRAM[28]);
        }
        else if (!entity->flipY) {
            entity->velocity.x >>= 1;
            if (entity->underwater)
                entity->velocity.y = 0x80000;
            else
                entity->velocity.y = 0xC0000;
            entity->jumpAbilityTimer = 0;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_DROPDASH, &entity->playerAnimData, 0, 2);
            entity->nextAirState    = 0;
            entity->nextGroundState = 0;
            RSDK.PlaySFX(Player->sfx_Release, 0, 255);
            int id          = RSDK.GetEntityID(entity);
            Entity *powerup = RSDK.GetEntityByID((ushort)((2 * Player->playerCount) + id));
            RSDK.DestroyEntity(powerup, ImageTrail->objectID, entity);
            if (entity->camera && !Zone->field_154) {
                entity->scrollDelay   = 8;
                entity->camera->state = Camera_State_HLock;
            }
            RSDK.PlaySFX(Player->sfx_MightyDrill, 0, 254);
            // entity->state = Player_State_Hammerdrop;
        }
    }
    else {
        entity->jumpAbilityTimer--;
        if (entity->jumpAbilityTimer == 1)
            entity->jumpAbilityTimer = 0;
    }
}
void Player_RayJumpAbility()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (!entity->jumpPress || !entity->jumpAbilityTimer
        || entity->inputState == Player_GetP2Inputs && (!entity->up || options->gameMode == MODE_ENCORE)) {
        if (RSDK_controller[entity->controllerID].keyY.press)
           Player_CheckGoSuper(entity, SaveGame->saveRAM[28]);
    }
    else if (!entity->flipY) {
        int xDif                 = entity->velocity.x - (entity->velocity.x >> 3);
        entity->jumpAbilityTimer = 0;
        if (entity->direction) {
            int spd = -0x30000;
            if (entity->underwater)
                spd = -0x18000;
            if (xDif > spd)
                xDif = spd;
            entity->velocity.x = xDif;
        }
        else {
            entity->velocity.x = xDif;
            int spd            = 0x30000;
            if (entity->underwater)
                spd = 0x18000;
            if (xDif < spd)
                entity->velocity.x = spd;
        }
        if ((entity->direction || !entity->right) && (entity->direction != FLIP_X || !entity->left)) {
            if (!entity->underwater)
                Player->raySwoopTimer = 256;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimData, 0, 3);
            entity->rotation = 1;

            entity->velocity.x >>= 1;
            int xspd = entity->velocity.x;
            if (entity->velocity.x >> 1 < 0)
                xspd = -xspd;

            int spd = -((xspd >> 1) + (abs(entity->velocity.x) >> 2) + (abs(entity->velocity.x) >> 4)) >> entity->underwater;
            if (spd > 0x40000)
                spd = 0x40000;
            entity->glideSpeedStore = spd;
        }
        else {
            if (!entity->underwater)
                Player->rayDiveTimer = 256;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLYLIFTTIRED, &entity->playerAnimData, 0, 3);
            entity->rotation        = 0;
            entity->glideSpeedStore = 0;
        }
        entity->velocity.y >>= 1;
        entity->glideTurnCos = 64;
        entity->glideTimer   = 0;
        if (entity->velocity.x < 0)
            entity->maxGlideSpeed = -entity->velocity.x;
        else
            entity->maxGlideSpeed = entity->velocity.x;
        // entity->state        = Player_State_RayGlide;
        entity->abilityTimer = 256;
        entity->nextAirState    = 0;
        entity->timer        = 0;
    }
}

void Player_GetP1Inputs()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (entity->controllerID < PLAYER_MAX) {
        if (options->gameMode != MODE_COMPETITION /*|| Announcer[14]*/) {
            ControllerState *controller = &RSDK_controller[entity->controllerID];
            entity->up                  = controller->keyUp.down;
            entity->down                = controller->keyDown.down;
            entity->left                = controller->keyLeft.down;
            entity->right               = controller->keyRight.down;

            entity->up |= RSDK_stickL[entity->controllerID].keyUp.down;
            entity->down |= RSDK_stickL[entity->controllerID].keyDown.down;
            entity->left |= RSDK_stickL[entity->controllerID].keyLeft.down;
            entity->right |= RSDK_stickL[entity->controllerID].keyRight.down;
            if (entity->left && entity->right) {
                entity->left  = false;
                entity->right = false;
            }
            entity->jumpPress = controller->keyA.press | controller->keyB.press | controller->keyC.press | controller->keyX.press;
            entity->jumpHold  = controller->keyA.down | controller->keyB.down | controller->keyC.down | controller->keyX.down;
            // if (!LottoMachine || !((1 << entity->playerID) & LottoMachine[5].objectID)) {
            if (RSDK_sku->platform == PLATFORM_DEV && controller->keyZ.press) {
                Zone->swapGameMode = true;
                RSDK.PlaySFX(Player->sfx_Transform2, 0, 254);
                Zone_Unknown1(64, 0xF0F0F0);
            }
            if (options->gameMode == MODE_ENCORE && RSDK_controller[entity->controllerID].keyY.press) {
                /*if (!HUD->field_24 && Player_CheckValidState(entity)) {
                    if (Player_UnknownP2State(0) == 1)
                        return result;
                }*/
                RSDK.PlaySFX(Player->sfx_SwapFail, 0, 255);
            }
            if (entity->controllerID == 0 /*&& RSDK_sceneInfo->debugMode*/ && entity->state != Player_State_Transform
                && RSDK_controller[0].keyX.press && options->gameMode != MODE_TIMEATTACK) {
                entity->objectID   = DebugMode->objectID;
                entity->velocity.x = 0;
                entity->velocity.y = 0;
                entity->groundVel  = 0;
                entity->drawOrder  = Zone->uiDrawLow;
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, true, 0);
                RSDK.SetGameMode(ENGINESTATE_REGULAR);
                entity->jumpHold  = 0;
                entity->jumpPress = 0;
                entity->visible   = 1;
                entity->active    = ACTIVE_ALWAYS;
                DebugMode->active = true;
            }
            //}

            if (RSDK_controller[entity->controllerID].keyStart.press /*|| RSDK_unknown->field_10 == 1*/) {
                if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
                    Entity *pauseMenu = RSDK.GetEntityByID(SLOT_PAUSEMENU);
                    bool flag         = true;
                    // if (ActClear && ActClear->dword34)
                    //    v6 = 0;
                    if (!RSDK.GetEntityCount(TitleCard->objectID, 0) && !pauseMenu->objectID && flag) {
                        RSDK.ResetEntity(SLOT_PAUSEMENU, PauseMenu->objectID, 0);
                        // pauseMenu->field_7C = entity->playerID;
                        // if (options->gameMode == MODE_COMPETITION)
                        //    result->field_80 = 1;
                    }
                }
            }
        }
    }
}
void Player_GetP2Inputs()
{
    EntityPlayer *entity  = (EntityPlayer *)RSDK_sceneInfo->entity;
    EntityPlayer *player1 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    // Player_FlyCarryInputState();
    if (false /*entity->state == Player_State_TailsFlight && player1->state == Player_State_FlyCarried*/) {
        entity->up        = player1->up;
        entity->down      = player1->down;
        entity->left      = player1->left;
        entity->right     = player1->right;
        entity->jumpHold  = player1->jumpHold;
        entity->jumpPress = player1->jumpPress;
        // if (Player_Unknown1() == 1)
        //    entity->inputState = Player_UnknownInputState;
        // Player_FlyCarryUnknown();
    }
    else {
        if (player1->objectID == Player->objectID /*&& player1->state != Player_State_FlyCarried*/) {
            int val = Player->field_9D8;
            if (player1->onGround || player1->groundedStore) {
                if (player1->groundVel < 0x20000 && player1->groundVel > -0x20000) {
                    val += 0x200000;
                    if (!player1->direction)
                        val = val - 0x200000;
                }
            }

            int dif = val - entity->position.x;
            if (dif) {
                if (dif >= 0) {
                    if (dif >= 0x300000) {
                        entity->left  = false;
                        entity->right = true;
                    }
                    if (entity->groundVel && !entity->direction) {
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

            if (entity->playerAnimData.animationID == ANI_PUSH) {
                ++Player->field_9E0;
                if (player1->direction == entity->direction && player1->playerAnimData.animationID == ANI_PUSH)
                    Player->field_9E0 = 0;
                if (Player->field_9E0 > 30) {
                    entity->jumpHold = true;
                }
                else {
                    if (entity->onGround) {
                        if (!entity->jumpHold)
                            entity->jumpPress = true;
                        entity->jumpHold = true;
                    }
                    entity->jumpAbility = 0;
                    Player->field_9E0   = 0;
                }
                if (entity->glideTimer > 0) {
                    // if (entity->groundVel < 0x8000 && entity->groundVel > -0x8000)
                    //    entity->inputState = Player_NoInput;
                }
            }
            else {
                if (entity->position.y - Player->field_9DC <= 0x200000) {
                    Player->field_9E0 = 0;
                    if (entity->glideTimer > 0) {
                        // if (entity->groundVel < 0x8000 && entity->groundVel > -0x8000)
                        //    entity->inputState = Player_NoInput;
                    }
                }
                else {
                    ++Player->field_9E0;
                    if (Player->field_9E0 > 64) {
                        entity->jumpHold = true;
                    }
                    else {
                        if (entity->onGround) {
                            if (!entity->jumpHold)
                                entity->jumpPress = true;
                            entity->jumpHold = true;
                        }
                        entity->jumpAbility = 0;
                        Player->field_9E0   = 0;
                    }
                    if (entity->glideTimer > 0) {
                        // if (entity->groundVel < 0x8000 && entity->groundVel > -0x8000)
                        //    entity->inputState = Player_NoInput;
                    }
                }
            }
        }

        // if (Player_Unknown1())
        //    entity->inputState = Player_UnknownInputState;
        // Player_FlyCarryUnknown();
    }
}
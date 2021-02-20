#include "../SonicMania.hpp"

ObjectPlayer *Player;

void Player_Update()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    //if (!User.CheckDLC(DLC_PLUS) && entity->characterID > ID_KNUCKLES)
    //    Player_ChangeCharacter(entity);

    if (entity->inputState2)
        entity->inputState2();

    if (entity->inputState)
        entity->inputState();
    if (entity->objectID == Player->objectID) {
        EntityCamera *camera = entity->camera;
        if (camera) {
            /*if (entity->scrollDelay > 0) {
                entity->scrollDelay--;
                if (!entity->scrollDelay)
                    camera->state = Camera_Unknown8;
            }
            else if (entity->state != PlayerState_LookUp && entity->state != PlayerState_Crouch) {
                if (camera->lookPos.y > 0) {
                    camera->lookPos.y -= 2;
                }
                else if (v6 < 0) {
                    camera->lookPos.y += 2;
                }
            }*/
        }

        if (entity->invincibleTimer) {
            if (entity->invincibleTimer <= 0) {
                entity->invincibleTimer++;
            }
            else {
                entity->invincibleTimer--;
                if (!entity->invincibleTimer) {
                    //Player_ApplyShieldEffect(entity);
                    if (options->gameMode != MODE_ENCORE || !entity->sidekick) {
                        EntityPlayer *player = NULL;
                        bool32 flag      = false;
                        while (RSDK.GetActiveObjects(Player->objectID, (Entity**)&player)) {
                            if (player->invincibleTimer > 0)
                                flag = false;
                        }
                        //if (flag)
                        //    Music_Unknown6(1, 1);
                    }
                }
            }
        }

        if (entity->speedShoesTimer > 0) {
            entity->speedShoesTimer--;
            if (!entity->speedShoesTimer) {
                //Player_ChangePhysicsState(entity);
                EntityPlayer *player = NULL;
                bool32 flag          = false;
                while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
                    if (player->speedShoesTimer > 0)
                        flag = false;
                }
                //if (flag)
                //    Music_Unknown6(2, 1);
            }
        }

        //if (entity->state != PlayerState_Hit) {
        //    if (entity->blinkTimer > 0) {
        //        entity->blinkTimer--;
        //        entity->visible = (entity->blinkTimer & 4) == 0;
        //    }
        //}

        //if (entity->characterID == ID_RAY && entity->state != PlayerState_RayGlide && !entity->field_1F0) {
        //    Player->raySwoopTimer = 0;
        //    Player->rayDiveTimer  = 0;
        //}

        if (entity->uncurlTimer > 0)
            entity->uncurlTimer--;

        //Player_HandleSuperPalette();
        //if (entity->characterID == ID_TAILS && entity->state != PlayerState_TailsFlight && entity->glideSpeedStore)
        //    entity->glideSpeedStore = 0;
       
        if (entity->killFlagA == 3 || entity->killFlagB == 3)
            entity->hurtFlag = 1;

        entity->killFlagA = 0;
        entity->killFlagB = 0;
        if (entity->collisionLayers & Zone->moveID) {
            TileLayer* move             = RSDK.GetSceneLayer(Zone->moveLayer);
            move->position.x = -entity->field_194.x >> 16;
            move->position.y            = -entity->field_194.y >> 16;
        }

        //if (entity->field_1EC)
        //    entity->state = Player_Unknown8;
        //if (entity->forceTransform)
        //    entity->state = PlayerState_Transform;

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
    //if (entity->superState == 1 && entity->state != PlayerState_Transform)
    //    Player_CheckGoSuper((EntityPlayer *)RSDK_sceneInfo->entity, 127);
    //if (entity->state == PlayerState_FlyCarried) {
    //    entity->leaderPosition.x         = entity->position.x >> 0x10 << 0x10;
    //    entity->leaderPosition.y         = entity->position.y >> 0x10 << 0x10;
    //}

    if (entity->hurtFlag) {
        entity->maxGlideSpeed = 0;
        //if (!entity->sidekick)
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
        entity->stateUnknown            = 0;
        entity->stateDraw           = 0;
        entity->interaction    = 0;
        entity->tileCollisions = 0;
        if (options->gameMode != MODE_COMPETITION)
            entity->active = ACTIVE_ALWAYS;
        entity->shield     = 0;
        entity->killFlagA  = 0;
        entity->killFlagB  = 0;
        entity->underwater = 0;
        //Player_ChangePhysicsState(entity);
        Entity* powerup = RSDK.GetObjectByID(Player->playerCount + RSDK.GetEntityID(entity));
        RSDK.DestroyEntity(powerup, 0, 0);
        if (entity->hurtFlag == 1) {
            RSDK.PlaySFX(Player->sfx_Hurt, 0, 255);
        }
        else if (entity->hurtFlag != 2) {
            if (entity->hurtFlag == 3) {
                entity->hurtFlag        = 0;
                entity->gravityStrength = 0x1000;
                entity->velocity.y = 0;
                //RSDK.PlaySFX(*(unsigned __int16 *)&Water[42].active, 0, 255);
                //entity->state = PlayerState_Drown;
                if (!entity->sidekick) {
                    if (options->gameMode == MODE_COMPETITION) {
                        //Music_Unknown6(8, 0);
                    }
                    else if (options->gameMode != MODE_ENCORE) {
                        RSDK_sceneInfo->timeEnabled = false;

                        if (entity->camera) {
                            entity->scrollDelay   = 2;
                            //entity->camera->state = NULL;
                        }
                    }
                    else {
                        EntityPlayer *sidekick = (EntityPlayer *)RSDK.GetObjectByID(SLOT_PLAYER2);
                        if (options->stock == 0 && !sidekick->objectID) {
                            RSDK_sceneInfo->timeEnabled = false;
                        }

                        if (entity->camera) {
                            entity->scrollDelay   = 2;
                            //entity->camera->state = NULL;
                        }
                    }
                }
            }
            if (entity->camera) {
                entity->scrollDelay = 2;
                //entity->camera->state = NULL;
            }
        }
        else if (entity->hurtFlag == 2) {
            entity->hurtFlag   = 0;
            entity->velocity.y = -0x68000;
            //entity->state      = PlayerState_Die;
            if ((entity->drawFX & FX_SCALE) == 0 || entity->scale.x == 0x200)
                entity->drawOrder = Zone->uiDrawLow;
            if (entity->sidekick) {
                if (entity->camera) {
                    entity->scrollDelay   = 2;
                    //entity->camera->state = NULL;
                }
            }
            else if (options->gameMode == MODE_ENCORE) {
                EntityPlayer *sidekick = (EntityPlayer *)RSDK.GetObjectByID(SLOT_PLAYER2);
                if (options->stock == 0 && !sidekick->objectID) {

                    if (entity->camera) {
                        entity->scrollDelay   = 2;
                        //entity->camera->state = NULL;
                    }
                }
            }
            else if (options->gameMode == MODE_COMPETITION) {
                if (entity->camera) {
                    entity->scrollDelay   = 2;
                    //entity->camera->state = NULL;
                }
            }
            else {
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
                RSDK_sceneInfo->timeEnabled = false;
                if (entity->camera) {
                    entity->scrollDelay   = 2;
                    //entity->camera->state = NULL;
                }
            }
        }
    }

    if (entity->onGround) {
        if (entity->stateUnknown) {
            entity->state    = entity->stateUnknown;
            entity->stateUnknown = NULL;
            if (entity->sidekick) {
                EntityPlayer *player = (EntityPlayer*)RSDK.GetObjectByID(SLOT_PLAYER1);
                player->scoreBonus = 0;
            }
            else {
                EntityPlayer *player = entity;
                player->scoreBonus = 0;
            }
        }
        if (entity->camera) {
            //if (entity->playerAnimData.animationID == ANI_JUMP)
            //    entity->camera->adjustY = entity->cameraOffset;
            //else
            //    entity->camera->adjustY = 0;
        }
    }
    else {
        if (entity->stateDraw) {
            entity->state     = entity->stateDraw;
            entity->stateDraw = NULL;
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
            default:
                RSDK.SetSpriteAnimation(-1, 0, &entity->tailSpriteAnimData, 0, 0);
                break;
        }
        RSDK.ProcessAnimation(&entity->tailSpriteAnimData);
    }
    RSDK.ProcessAnimation(&entity->playerAnimData);
}

void Player_StaticUpdate()
{
    if (Player->field_A70 > 0) {
        RSDK.GetObjectByID(SLOT_PLAYER1);
        EntityHUD *hud = NULL;
        if (RSDK.GetObjects(HUD->objectID, (Entity**)&hud)) {
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
    //if (entity->state == PlayerState_FlyIn && posPtr) {
    //    entity->position.x = posPtr->x;
    //    entity->position.y = posPtr->y;
    //}
    if (entity->tailSpriteIndex != 0xFFFF) {
        int rotStore                     = entity->rotation;
        int dirStore                     = entity->direction;
        entity->rotation  = entity->tailRotation;
        entity->direction = entity->tailDirection;
        RSDK.DrawSprite(&entity->tailSpriteAnimData, NULL, 0);
        entity->rotation  = rotStore;
        entity->direction = dirStore;
    }

    if (entity->playerID != ID_SONIC || options->playerID != ((options->playerID >> 8) & 0xFF) || options->gameMode == MODE_TIMEATTACK) {
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
    //if (entity->state == PlayerState_FlyIn) {
    //    if (posPtr) {
    //        entity->position.x = posStore.x;
    //        entity->position.y = posStore.y;
    //    }
    //}
}

void Player_Create(void* data)
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
                //entity->movesetPtr      = PlayerState_TailsJumpAbility;
                entity->sensorY         = 0x100000;
                break;
            case ID_KNUCKLES:
                entity->spriteIndex     = Player->knuxSpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                //entity->movesetPtr      = PlayerState_KnucklesJumpAbility;
                entity->sensorY         = 0x140000;
                break;
            case ID_MIGHTY:
                entity->spriteIndex     = Player->mightySpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                //entity->movesetPtr      = PlayerState_MightyJumpAbility;
                entity->sensorY         = 0x140000;
                break;
            case ID_RAY:
                entity->spriteIndex     = Player->raySpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                //entity->movesetPtr      = PlayerState_RayJumpAbility;
                entity->sensorY         = 0x140000;
                break;
            default:
                entity->spriteIndex     = Player->sonicSpriteIndex;
                entity->tailSpriteIndex = -1;
                entity->cameraOffset    = 0x50000;
                //entity->movesetPtr      = PlayerState_SonicJumpAbility;
                entity->sensorY         = 0x140000;
                if (options->medalMods & (1 << MEDAL_PEELOUT)) {
                    for (int f = 0; f < 4; ++f) {
                        SpriteFrame *dst = RSDK.GetFrame(entity->spriteIndex, ANI_DASH, f);
                        SpriteFrame *src = RSDK.GetFrame(entity->spriteIndex, ANI_FLY, f);

                        *dst = *src;
                    }
                }
                break;
        }
        entity->sensorX[0]          = 0xA0000;
        entity->sensorX[1]          = 0x50000;
        entity->sensorX[2]          = 0;
        entity->sensorX[3]          = -0x50000;
        entity->sensorX[4]          = -0xA0000;
        entity->active       = ACTIVE_NORMAL;
        entity->tileCollisions = true;
        entity->visible        = true;
        entity->drawOrder      = Zone->playerDrawLow;
        entity->scale.x             = 0x200;
        entity->scale.y             = 0x200;
        entity->controllerID        = entity->playerID + 1;
        entity->state               = Player_State_Ground;

        if (RSDK_sceneInfo->entitySlot && options->gameMode != MODE_COMPETITION) {
            if (RSDK_sceneInfo->entitySlot != 1 || options->gameMode != MODE_TIMEATTACK) {
                RSDK.Unknown102(entity->controllerID, -1);
                //entity->inputState = (void (*)(void))Player_GetP2Inputs;
                entity->sidekick    = 1;
            }
            else {
                Player->replayState();
            }
        }
        else {
            //entity->inputState = (void (*)(void))Player_GetP1Inputs;
        }
        RSDK_stickL[entity->controllerID].unknown = 0.30000001;
        entity->rings                             = Player->rings;
        entity->ringExtraLife                     = Player->ringExtraLife;
        Player->rings                         = 0;
        Player->ringExtraLife                 = 100;
        entity->hyperRing                         = (Player->powerups >> 6) & 1;
        Player->powerups &= 0xBFu;
        if (Player->powerups >= 128) {
            //entity->state = Player_Unknown9;
            Player->powerups &= 0x7Fu;
        }
        if (Player->powerups) {
            entity->shield = Player->powerups;
            //Player_ApplyShieldEffect(entity);
        }

        Player->powerups = 0;
        if (options->gameMode == MODE_COMPETITION) {
            entity->lives       = options->competitionSession[entity->playerID + 0x50];
            entity->score       = 0;
            entity->score1UP = 50000;
        }
        else if (options->gameMode == MODE_TIMEATTACK) {
            entity->lives       = 1;
            entity->score       = 0;
            entity->score1UP = 50000;
        }
        else {
            entity->lives       = Player->savedLives;
            entity->score       = Player->savedScore;
            entity->score1UP = Player->savedScore1UP;
        }

        int target = entity->score1UP;
        int score = entity->score;
        if (score <= target) {
            do
                score += 50000;
            while (score <= target);
            entity->score1UP = score;
        }

        //entity->collisionLayers = Zone[89].ObjectID;
        entity->drawFX          = FX_ROTATE | FX_FLIP;
        RSDK.SetSpriteAnimation(entity->spriteIndex, 0, &entity->playerAnimData, 1, 0);
        //Player_ChangePhysicsState(entity);
        entity->maxWalkSpeed = 0x40000;
        entity->maxJogSpeed  = 0x60000;
        entity->maxRunSpeed  = 0xC0000;
    }
}

void Player_StageLoad()
{
    if (!options->playerID) {
        options->playerID = RSDK.CheckStageFolder("MSZCutscene") != 1 ? ID_DEFAULT_PLAYER : ID_KNUCKLES;
    }

    RSDK_sceneInfo->debugMode = (options->medalMods & (1 << MEDAL_DEBUGMODE)) > 0;
    RSDK.SetDebugValue("Debug Mode", RSDK_sceneInfo->debugMode, 1, 0, 1);
    if (options->medalMods & 2) {
        options->playerID &= 0xFFu;
        options->playerID += 1024;
    }
    Player->playerCount = 0;
    Player->active    = ACTIVE_ALWAYS;
    if (options->gameMode == MODE_COMPETITION)
        Player_LoadSpritesVS();
    else
        Player_LoadSprites();
    if (options->gameMode == MODE_ENCORE) {
        Player->playerCount                     = 2;
        EntityPlayer *sidekick = (EntityPlayer *)RSDK.GetObjectByID(1);
        sidekick->characterID     = ID_SONIC;
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

    if (Soundboard) {
        //if (Soundboard->sfxCount < 32) {
        //    Soundboard->sfxList[Soundboard->sfxCount]        = RSDK_GetSFX("Global/RaySwoop.wav");
        //    Soundboard->field_48[Soundboard->sfxCount]       = 0xA1C9;
        //    Soundboard->sfxPtrs1[Soundboard->sfxCount + 19]  = Player_CheckRaySwooping;
        //    Soundboard->sfxPtrs2[Soundboard->sfxCount + 19]  = Player_ChangeRaySwoopSFX;
        //    Soundboard->field_37C[Soundboard->sfxCount + 19] = 0;
        //    ++Soundboard->sfxCount;
        //    RSDK_StopSFX(Soundboard->sfxList[v26]);
        //    v25 = Soundboard;
        //}
        //
        //if (Soundboard->sfxCount < 32) {
        //    Soundboard->sfxList[Soundboard->sfxCount]        = RSDK_GetSFX("Global/RayDive.wav");
        //    Soundboard->field_48[Soundboard->sfxCount]       = 0x11A83;
        //    Soundboard->sfxPtrs1[Soundboard->sfxCount + 19]  = (Player_CheckRayDiving;
        //    Soundboard->sfxPtrs2[Soundboard->sfxCount + 19]  = (Player_ChangeRayDiveSFX;
        //    Soundboard->field_37C[Soundboard->sfxCount + 19] = 0;
        //    ++Soundboard->sfxCount;
        //    RSDK_StopSFX(Soundboard->sfxList[v28]);
        //}
    }
    Player->gotHit    = 0;
    Player->field_A5C = 0;
    Player->field_A60 = 0;
    Player->field_A64 = 0;
    Player->field_A6C = 0;
}

void Player_EditorDraw()
{

}

void Player_EditorLoad()
{

}

void Player_Serialize() { RSDK_EDITABLE_VAR(Player, VAR_ENUM, characterID); }

//Extra Entity Functions
void Player_LoadSprites()
{
    EntityPlayer *entity = NULL;
    while (RSDK.GetObjects(Player->objectID, (Entity**)&entity)) {
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
            EntityPlayer *player1 = (EntityPlayer *)RSDK.GetObjectByID(0);
            RSDK.CopyEntity(player1, entity, 1);
            //EntityCamera *cam = Camera_SetTargetEntity(0, player1);
            //player1->camera   = cam;
            //RSDK_Unknown58(player1, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, 1);
        }
        else {
            RSDK.DestroyEntity(entity, 0, 0);
        }
    }

    if ((options->playerID & -0x100) > 0) {
        entity   = (EntityPlayer *)RSDK.GetObjectByID(0);
        EntityPlayer* sidekick = (EntityPlayer *)RSDK.GetObjectByID(1);

        for (int i = 0; i < 0x10; ++i) {
            Player->flyCarryPositions[i] = entity->position;
        }
        sidekick->objectID       = Player->objectID;
        sidekick->position.x = entity->position.x;
        sidekick->position.y = entity->position.y;

        if (options->gameMode != MODE_TIMEATTACK) {
            //RSDK.Unknown58(sidekick, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, 1);
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
    EntityPlayer* entity = 0;
    while (RSDK.GetObjects(Player->objectID, (Entity **)&entity)) {

        if (entity->characterID & 1) {
            int slotID = 0;
            for (int i = 0; i < options->competitionSession[23]; ++i, ++slotID) {
                EntityPlayer* player = (EntityPlayer *)RSDK.GetObjectByID(slotID);
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
                //player->camera       = Camera_SetTargetEntity(i, player);
            }
        }
        RSDK.DestroyEntity(entity, 0, 0);
    }
}
void Player_SaveValues() {
    EntityPlayer *entity     = (EntityPlayer *)RSDK.GetObjectByID(SLOT_PLAYER1);
    Player->savedLives       = entity->lives;
    options->restartLives[0] = Player->savedLives;
    Player->savedScore       = entity->score;
    options->restartScore    = Player->savedScore;
    Player->savedScore1UP    = entity->score1UP;
    options->restartScore1UP = Player->savedScore1UP;
}
void Player_GiveLife(EntityPlayer *entity) {
    if (options->gameMode != MODE_TIMEATTACK && options->gameMode != MODE_ENCORE) {
        if (entity->lives < 99)
            entity->lives++;
        //Music_PlayMusicTrack(13);
    }
}

//Player State Helpers
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
                        entity->groundVel = abs(skid);
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
        int vel      = (abs(entity->groundVel) <= 0x60000) + 1;
        int newRot = 0;
        if (entity->angle <= 16 || entity->angle >= 232)
            newRot = 0;
        else
            newRot = entity->angle << 1;
        int dif  = newRot - entity->rotation;

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
        //entity->camera->field_94 = 1;
        //entity->camera->field_90 = 0x200000;
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
    entity->angle         = 0;
    entity->collisionMode = CMODE_FLOOR;
    entity->skidding           = 0;
    entity->jumpAbility        = 1;
    entity->jumpAbilityTimer   = 1;
    RSDK.PlaySFX(Player->sfx_Jump, 0, 255);
    entity->state              = Player_State_Air;
}
void Player_StartRoll()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimData, false, 0);
    entity->pushing      = 0;
    //entity->state        = Player_State_Roll;
    if (entity->collisionMode == CMODE_FLOOR) {
        entity->position.y += entity->cameraOffset;
    }
}

//Player States
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
                        entity->state          = Player_State_Ground;
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

void Player_State_Ground() {
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

    //if (entity->camera)
    //    entity->camera->field_94 = 0;
    entity->jumpAbilityTimer = 0;

    entity->stateDraw         = Player_State_Air;
    if (entity->skidding <= 0) {
        if (entity->pushing > -3 || entity->pushing < 3) {
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
                        entity->playerAnimData.animationSpeed = (entity->groundVel >> 12) + 48 + 64;
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
                    entity->playerAnimData.animationSpeed = (entity->groundVel >> 12) + 48;
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
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, 0, entity->collisionPlane, entity->sensorX[0], entity->sensorY, 10) << 0;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, 0, entity->collisionPlane, entity->sensorX[1], entity->sensorY, 10) << 1;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, 0, entity->collisionPlane, entity->sensorX[2], entity->sensorY, 10) << 2;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, 0, entity->collisionPlane, entity->sensorX[3], entity->sensorY, 10) << 3;
                entity->flailing |=
                    RSDK.ObjectTileGrip(entity, entity->collisionLayers, 0, entity->collisionPlane, entity->sensorX[4], entity->sensorY, 10) << 4;

                entity->position = posStore;
                switch (entity->flailing - 1) {
                    case 0:
                    case 2:
                        if (entity->direction == FLIP_X || entity->characterID == ID_SONIC && entity->superState == 2 || entity->transforming == 1) {
                            entity->direction = 1;
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimData, 0, 0);
                        }
                        else {
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE2, &entity->playerAnimData, 0, 0);
                        }
                        break;
                    case 15:
                    case 23:
                        if (entity->direction && (entity->characterID != ID_SONIC || entity->superState != 2) && entity->transforming != 1) {
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE2, &entity->playerAnimData, 0, 0);
                        }
                        else {
                            entity->direction = 0;
                            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimData, 0, 0);
                        }
                        break;
                    case 110:
                    case 115: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_BALANCE1, &entity->playerAnimData, 0, 0); break;
                    default:
                        switch (entity->characterID) {
                            case ID_SONIC:
                                if (entity->timer != 720 || entity->transforming || entity->superState == 2) {
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
                    entity->stateDraw      = 0;
                    entity->stateUnknown   = 0;
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
                //dust->state      = Dust_State_Skid;
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
    EntityPlayer *entity                        = (EntityPlayer *)RSDK_sceneInfo->entity;
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
        entity->stateUnknown = Player_State_Ground;
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
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0,
                                                                 entity->playerAnimData.frameID);
                break;
            case ANI_LOOKUP:
            case ANI_CROUCH:
            case ANI_SKIDTURN: RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0, 0); break;
            case ANI_JOG:
                RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0, 0);
                break;
            case ANI_JUMP:
                if (entity->movesetPtr) {
                    if (entity->velocity.y >= entity->jumpCap)
                        entity->movesetPtr();
                }
                break;
            case ANI_SKID:
                if (entity->skidding < 1) {
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_AIRWALK, &entity->playerAnimData, 0,
                                                                     entity->playerAnimData.frameID);
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
        // if (entity->camera)
        //    entity->camera->field_94 = 0;
        entity->jumpAbilityTimer = 0;
        if (entity->jumpPress)
            Player_StartJump(entity);
    }
}
void Player_State_ForceRoll()
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    Player_HandleGroundRotation();
    bool32 left = entity->left;
    bool32 right = entity->right;
    if (entity->glideTimer > 0) {
        entity->left       = 0;
        entity->right      = 0;
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
            // entity->camera->field_94 = 0;
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
            // entity->camera->field_94 = 0;
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
    // if (entity->camera)
    //    entity->camera->field_94 = 0;
    entity->jumpAbilityTimer = 0;
    entity->stateDraw        = Player_State_Air;
    if (entity->down) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_LOOKUP, &entity->playerAnimData, false, 1);
        if (entity->playerAnimData.frameID == 5)
            entity->playerAnimData.animationSpeed = 0;

        if (entity->timer < 60) {
            entity->timer++;
        }
        else if (entity->camera) {
            if (entity->flipY) {
                // if (entity->camera->lookPos.y > -96)
                //    entity->camera->lookPos.y = -= 2;
            }
            else {
                // if (entity->camera->lookPos.y = < 96)
                //    entity->camera->lookPos.y = += 2;
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
    // if (entity->camera)
    //    entity->camera->field_94 = 0;
    entity->jumpAbilityTimer = 0;
    entity->stateDraw    = Player_State_Air;
    if (entity->down) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_CROUCH, &entity->playerAnimData, 0, 1);
        if (entity->playerAnimData.frameID == 4)
            entity->playerAnimData.animationSpeed = 0;

        if (entity->timer < 60) {
            entity->timer++;
        }
        else if (entity->camera) {
            if (entity->flipY) {
                //if (entity->camera->lookPos.y > -96)
                //    entity->camera->lookPos.y = -= 2;
            }
            else {
                //if (entity->camera->lookPos.y = < 96)
                //    entity->camera->lookPos.y = += 2;
            }
        }
        if (entity->jumpPress) {
            //EntityDust *dust = (EntityDust *)RSDK.SpawnEntity(Dust->objectID, entity, entity->position.x, entity->position.y);
            //RSDK.SetSpriteAnimation(Dust->spriteIndex, 1, &dust->data, true, 0);
            //dust->state          = Dust_State_Spindash;
            //dust->drawOrder = entity->drawOrder;
            //RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_SPINDASH, &entity->playerAnimData, true, 0);
            //entity->state          = Player_State_Spindash;
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

void Player_State_OuttaHere() {
}
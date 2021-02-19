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
        //entity->state               = PlayerState_Ground;

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
            entity->targetScore = 50000;
        }
        else if (options->gameMode == MODE_TIMEATTACK) {
            entity->lives       = 1;
            entity->score       = 0;
            entity->targetScore = 50000;
        }
        else {
            entity->lives       = Player->savedLives;
            entity->score       = Player->savedScore;
            entity->targetScore = Player->savedTargetScore;
        }

        int target = entity->targetScore;
        int score = entity->score;
        if (score <= target) {
            do
                score += 50000;
            while (score <= target);
            entity->targetScore = score;
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

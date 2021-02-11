#include "../SonicMania.hpp"

ObjectPlayer *Player;

void Player_Update()
{

}

void Player_LateUpdate()
{

}

void Player_StaticUpdate()
{

}

void Player_Draw()
{

}

void Player_Create(void* data)
{
    EntityPlayer *entity = (EntityPlayer *)RSDK_sceneInfo->entity;
    if (RSDK_sceneInfo->inEditor) {
        RSDK_SetSpriteAnimation(Player->sonicSpriteIndex, 0, &entity->playerAnimData, 1, 0);
        entity->characterID = ID_SONIC;
    }
    else {
        entity->playerID = RSDK_GetEntityID(RSDK_sceneInfo->entity);
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
                        SpriteFrame *dst = RSDK_GetFrame(entity->spriteIndex, ANI_DASH, f);
                        SpriteFrame *src = RSDK_GetFrame(entity->spriteIndex, ANI_FLY, f);

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
        entity->priority       = ACTIVE_NORMAL;
        entity->tileCollisions = true;
        entity->visible        = true;
        //entity->drawOrder      = Zone[94].ObjectID;
        entity->scale.x             = 0x200;
        entity->scale.y             = 0x200;
        entity->controllerID        = entity->playerID + 1;
        //entity->state               = PlayerState_Ground;

        if (RSDK_sceneInfo->entitySlot && options->gameMode != MODE_COMPETITION) {
            if (RSDK_sceneInfo->entitySlot != 1 || options->gameMode != MODE_TIMEATTACK) {
                RSDK_Unknown102(entity->controllerID, -1);
                //entity->inputStatus = (void (*)(void))Player_GetP2Inputs;
                entity->sidekick    = 1;
            }
            else {
                Player->replayState();
            }
        }
        else {
            //entity->inputStatus = (void (*)(void))Player_GetP1Inputs;
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
        RSDK_SetSpriteAnimation(entity->spriteIndex, 0, &entity->playerAnimData, 1, 0);
        //Player_ChangePhysicsState(entity);
        entity->maxWalkSpeed = 0x40000;
        entity->maxJogSpeed  = 0x60000;
        entity->maxRunSpeed  = 0xC0000;
    }
}

void Player_StageLoad()
{
    if (!options->playerID) {
        options->playerID = RSDK_CheckStageFolder("MSZCutscene") != 1 ? ID_DEFAULT_PLAYER : ID_KNUCKLES;


    }

    RSDK_sceneInfo->debugMode = (options->medalMods & (1 << MEDAL_DEBUGMODE)) > 0;
    RSDK_SetDebugValue("Debug Mode", RSDK_sceneInfo->debugMode, 1, 0, 1);
    if (options->medalMods & 2) {
        options->playerID &= 0xFFu;
        options->playerID += 1024;
    }
    Player->playerCount = 0;
    Player->priority    = ACTIVE_ALWAYS;
    if (options->gameMode == MODE_COMPETITION)
        Player_LoadSpritesVS();
    else
        Player_LoadSprites();
    if (options->gameMode == MODE_ENCORE) {
        Player->playerCount                     = 2;
        EntityPlayer *sidekick = (EntityPlayer *)RSDK_GetObjectByID(1);
        sidekick->playerID     = ID_SONIC;
    }
    else {
        Player->playerCount = RSDK_GetEntityCount(Player->objectID, 0);
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
    Player->sfx_Jump          = RSDK_GetSFX("Global/Jump.wav");
    Player->sfx_LoseRings     = RSDK_GetSFX("Global/LoseRings.wav");
    Player->sfx_Hurt          = RSDK_GetSFX("Global/Hurt.wav");
    Player->sfx_Roll          = RSDK_GetSFX("Global/Roll.wav");
    Player->sfx_Charge        = RSDK_GetSFX("Global/Charge.wav");
    Player->sfx_Release       = RSDK_GetSFX("Global/Release.wav");
    Player->sfx_PeelCharge    = RSDK_GetSFX("Global/PeelCharge.wav");
    Player->sfx_PeelRelease   = RSDK_GetSFX("Global/PeelRelease.wav");
    Player->sfx_Dropdash      = RSDK_GetSFX("Global/DropDash.wav");
    Player->sfx_Skidding      = RSDK_GetSFX("Global/Skidding.wav");
    Player->sfx_Grab          = RSDK_GetSFX("Global/Grab.wav");
    Player->sfx_Flying        = RSDK_GetSFX("Global/Flying.wav");
    Player->sfx_Tired         = RSDK_GetSFX("Global/Tired.wav");
    Player->sfx_Land          = RSDK_GetSFX("Global/Land.wav");
    Player->sfx_Slide         = RSDK_GetSFX("Global/Slide.wav");
    Player->sfx_Outtahere     = RSDK_GetSFX("Global/OuttaHere.wav");
    Player->sfx_Transform2    = RSDK_GetSFX("Stage/Transform2.wav");
    Player->sfx_PimPom        = RSDK_GetSFX("Stage/PimPom.wav");
    Player->sfx_Swap          = RSDK_GetSFX("Global/Swap.wav");
    Player->sfx_SwapFail      = RSDK_GetSFX("Global/SwapFail.wav");
    Player->sfx_MightyDeflect = RSDK_GetSFX("Global/MightyDeflect.wav");
    Player->sfx_MightyDrill   = RSDK_GetSFX("Global/MightyDrill.wav");
    Player->sfx_MightyLand    = RSDK_GetSFX("Global/MightyLand.wav");
    Player->sfx_MightyUnspin  = RSDK_GetSFX("Global/MightyUnspin.wav");

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

void Player_Serialize() { RSDK_SetEditableVar(ATTRIBUTE_VAR, "characterID", Player->objectID, offsetof(EntityPlayer, playerID)); }


void Player_LoadSprites()
{
    EntityPlayer *entity = NULL;
    while (RSDK_GetObjects(Player->objectID, (Entity**)&entity)) {
        int pID = options->playerID & 0xFF;
        if (pID == ID_MIGHTY || pID == ID_RAY)
            pID = ID_SONIC;

        if (pID & entity->characterID) {
            entity->characterID = options->playerID & 0xFF;
            switch (entity->characterID) {
                case ID_TAILS:
                    Player->tailsSpriteIndex      = RSDK_LoadAnimation("Players/Tails.bin", SCOPE_STAGE);
                    Player->tailsTailsSpriteIndex = RSDK_LoadAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                    break;
                case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK_LoadAnimation("Players/Knux.bin", SCOPE_STAGE); break;
                case ID_MIGHTY: Player->mightySpriteIndex = RSDK_LoadAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
                case ID_RAY: Player->raySpriteIndex = RSDK_LoadAnimation("Players/Ray.bin", SCOPE_STAGE); break;
                default:
                    Player->sonicSpriteIndex = RSDK_LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
                    Player->superSpriteIndex = RSDK_LoadAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                    break;
            }
            EntityPlayer *player1 = (EntityPlayer *)RSDK_GetObjectByID(0);
            RSDK_CopyEntity(player1, entity, 1);
            //EntityCamera *cam = Camera_SetTargetEntity(0, player1);
            //player1->camera   = cam;
            //RSDK_Unknown58(player1, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, 1);
        }
        else {
            RSDK_DestroyEntity(entity, 0, 0);
        }
    }

    if ((options->playerID & -0x100) > 0) {
        entity   = (EntityPlayer *)RSDK_GetObjectByID(0);
        EntityPlayer* sidekick = (EntityPlayer *)RSDK_GetObjectByID(1);

        for (int i = 0; i < 0x10; ++i) {
            Player->flyCarryPositions[i] = entity->position;
        }
        sidekick->type       = Player->objectID;
        sidekick->position.x = entity->position.x;
        sidekick->position.y = entity->position.y;

        if (options->gameMode != MODE_TIMEATTACK) {
            //RSDK_Unknown58(sidekick, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, 1);
            sidekick->position.x -= 0x100000;
        }

        sidekick->characterID = options->playerID >> 8;
        switch (sidekick->characterID) {
            case ID_TAILS:
                Player->tailsSpriteIndex      = RSDK_LoadAnimation("Players/Tails.bin", SCOPE_STAGE);
                Player->tailsTailsSpriteIndex = RSDK_LoadAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                break;
            case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK_LoadAnimation("Players/Knux.bin", SCOPE_STAGE); break;
            case ID_MIGHTY: Player->mightySpriteIndex = RSDK_LoadAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
            case ID_RAY: Player->raySpriteIndex = RSDK_LoadAnimation("Players/Ray.bin", SCOPE_STAGE); break;
            default:
                sidekick->characterID    = ID_SONIC;
                Player->sonicSpriteIndex = RSDK_LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
                Player->superSpriteIndex = RSDK_LoadAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                break;
        }
    }
}
void Player_LoadSpritesVS()
{
    EntityPlayer* entity = 0;
    while (RSDK_GetObjects(Player->objectID, (Entity **)&entity)) {

        if (entity->characterID & 1) {
            int slotID = 0;
            for (int i = 0; i < options->competitionSession[23]; ++i, ++slotID) {
                EntityPlayer* player = (EntityPlayer *)RSDK_GetObjectByID(slotID);
                RSDK_CopyEntity(player, entity, 0);
                player->characterID = options->playerID >> 8 * i;
                switch (player->characterID) {
                    case ID_TAILS:
                        Player->tailsSpriteIndex      = RSDK_LoadAnimation("Players/Tails.bin", SCOPE_STAGE);
                        Player->tailsTailsSpriteIndex = RSDK_LoadAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                        break;
                    case ID_KNUCKLES: Player->knuxSpriteIndex = RSDK_LoadAnimation("Players/Knux.bin", SCOPE_STAGE); break;
                    case ID_MIGHTY: Player->mightySpriteIndex = RSDK_LoadAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
                    case ID_RAY: Player->raySpriteIndex = RSDK_LoadAnimation("Players/Ray.bin", SCOPE_STAGE); break;
                    default:
                        player->characterID      = ID_SONIC;
                        Player->sonicSpriteIndex = RSDK_LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
                        Player->superSpriteIndex = RSDK_LoadAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                        break;
                }
                player->controllerID = i + 1;
                //player->camera       = Camera_SetTargetEntity(i, player);
            }
        }
        RSDK_DestroyEntity(entity, 0, 0);
    }
}
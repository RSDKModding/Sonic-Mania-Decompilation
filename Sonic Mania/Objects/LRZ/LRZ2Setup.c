// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ2Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectLRZ2Setup *LRZ2Setup;

void LRZ2Setup_Update(void) {}

void LRZ2Setup_LateUpdate(void) {}

void LRZ2Setup_StaticUpdate(void)
{
    // Lava
    LRZ2Setup->lavaPalTimer += 24;
    if (LRZ2Setup->lavaPalTimer > 255) {
        LRZ2Setup->lavaPalTimer -= 256;
        RSDK.RotatePalette(1, 224, 227, true);
        RSDK.RotatePalette(2, 224, 227, true);
        RSDK.RotatePalette(3, 224, 227, true);
        RSDK.RotatePalette(4, 224, 227, true);
    }

    // Conveyor Belt Animations
    ++LRZ2Setup->conveyorPalTimer;
    if (LRZ2Setup->conveyorPalTimer == 128) {
        LRZ2Setup->conveyorPalTimer = 0;
        ++LRZ2Setup->conveyorDstPal;
        ++LRZ2Setup->conveyorSrcPal;

        if (LRZ2Setup->conveyorDstPal > 3)
            LRZ2Setup->conveyorDstPal = 1;
        if (LRZ2Setup->conveyorSrcPal > 3)
            LRZ2Setup->conveyorSrcPal = 1;
    }

    RSDK.SetLimitedFade(0, LRZ2Setup->conveyorSrcPal, LRZ2Setup->conveyorDstPal, (RSDK.Cos256(LRZ2Setup->conveyorPalTimer) >> 1) + 128, 160, 168);
    if (!LRZ2Setup->conveyorOff && !(Zone->timer & 1))
        RSDK.RotatePalette(0, 228, 231, (LRZ2Setup->conveyorDir & 0xFF));

    // Rock Hues
    int32 blend   = RSDK.Cos1024(2 * (Zone->timer & 0x1FF));

    RSDK.SetLimitedFade(5, 1, 4, abs(blend >> 3), 224, 227);
    RSDK.RotatePalette(3, 224, 227, true);
    RSDK.RotatePalette(4, 224, 227, true);
    RSDK.SetLimitedFade(6, 2, 4, abs(blend >> 3), 224, 227);
    RSDK.RotatePalette(3, 224, 227, false);
    RSDK.RotatePalette(4, 224, 227, false);
    // Apply Lava Fading
    RSDK.SetLimitedFade(0, 5, 6, LRZ2Setup->lavaPalTimer, 224, 227);

    // Tile Behaviours
    foreach_active(Player, player)
    {
        if (player->onGround) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            uint8 behaviour      = LRZ2_TFLAGS_NORMAL;
            int32 tileInfo       = 0;

            LRZ2Setup_GetTileInfo(&tileInfo, player->collisionPlane, player->position.x, (playerHitbox->bottom << 16) + player->position.y,
                                  player->moveLayerPosition.x, player->moveLayerPosition.y, &behaviour);
            if (behaviour == LRZ2_TFLAGS_NORMAL) {
                LRZ2Setup_GetTileInfo(&tileInfo, player->collisionPlane, (playerHitbox->right << 16) + player->position.x,
                                      (playerHitbox->bottom << 16) + player->position.y, player->moveLayerPosition.x, player->moveLayerPosition.y, &behaviour);
                if (behaviour == LRZ2_TFLAGS_NORMAL) {
                    LRZ2Setup_GetTileInfo(&tileInfo, player->collisionPlane, (playerHitbox->left << 16) + player->position.x,
                                          (playerHitbox->bottom << 16) + player->position.y, player->moveLayerPosition.x, player->moveLayerPosition.y, &behaviour);
                }
            }

            bool32 conveyorCollided = false;
            uint8 conveyorDir       = 0;
            switch (behaviour) {
                default: break;
                case LRZ2_TFLAGS_LAVA: {
                    int32 solid = 1 << 14;
                    if (player->collisionPlane)
                        solid = 1 << 12;
                    if ((solid & tileInfo) && player->shield != SHIELD_FIRE)
                        Player_CheckHitFlip(player);
                    break;
                }
                case LRZ2_TFLAGS_CONVEYOR_L:
                    conveyorCollided = true;
                    conveyorDir      = 0;
                    break;
                case LRZ2_TFLAGS_CONVEYOR_R:
                    conveyorCollided = true;
                    conveyorDir      = 1;
                    break;
            }

            if (!LRZ2Setup->conveyorOff && conveyorCollided) {
                if (player->onGround) {
                    player->position.x += (2 * ((((tileInfo & 0x400) != 0) ^ (LRZ2Setup->conveyorDir & 0xFF)) != conveyorDir) - 1) << 17;
                    player->position.y += 0x10000;
                }
            }
        }
    }
}

void LRZ2Setup_Draw(void) {}

void LRZ2Setup_Create(void *data) {}

void LRZ2Setup_StageLoad(void)
{
    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_CUCKY;
    if (!isMainGameMode() || !globals->atlEnabled || PlayerHelpers_CheckStageReload()) {
        for (int32 p = 0; p < Player->playerCount; ++p) {
            Zone->cameraBoundsL[p] += 0x100;
        }
    }
    else {
        LRZ2Setup_HandleStageReload();
    }

#if RETRO_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreLRZ2.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 1, 128, 128);
    }
#endif

    LRZ2Setup->conveyorDstPal = 1;
    LRZ2Setup->conveyorSrcPal = 2;
    if (globals->gameMode == MODE_TIMEATTACK || globals->gameMode == MODE_COMPETITION)
        GenericTrigger->callbacks[GENERICTRIGGER_LRZ2_OUTRO] = NULL;
    else
        GenericTrigger->callbacks[GENERICTRIGGER_LRZ2_OUTRO] = LRZ2Setup_GenericTrigger_CB;
}

void LRZ2Setup_HandleStageReload(void)
{
    Vector2 pos;
    pos.x = 0;
    pos.y = 0;
    foreach_all(DashLift, lift)
    {
        pos.x = lift->position.x;
        pos.y = lift->position.y;
        foreach_break;
    }

    Zone_ReloadStoredEntities(pos.x, pos.y, false);

    foreach_all(Player, player)
    {
        player->position.x = pos.x;
        player->position.y = pos.y - 0x100000;
    }

    CREATE_ENTITY(LRZ1Outro, NULL, 0, 0);
}

void LRZ2Setup_GenericTrigger_CB(void)
{
    if (isMainGameMode()) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE) {
            globals->tempFlags = player1->position.y > 0x4000000;
        }
#endif

        if (player1->stateInput) {
            player1->stateInput = StateMachine_None;
            player1->left       = false;
            player1->right      = true;

            for (int32 i = 0; i < Player->playerCount; ++i) {
                StarPost->postIDs[i] = 0;
            }

            SaveGame_SavePlayerState();
            globals->enableIntro       = true;
            globals->suppressAutoMusic = true;
            globals->suppressTitlecard = true;
            ++SceneInfo->listPos;
            if (!RSDK.CheckValidScene())
                RSDK.SetScene("Presentation", "Title Screen");
            Zone_StartFadeOut(10, 0x000000);
        }

        if (player1->superState == SUPERSTATE_SUPER || player1->state == Player_State_Transform) {
            globals->restartPowerups |= 0x80;
        }
        globals->restartMusicID = Music->activeTrack;
    }
}

EntityButton *LRZ2Setup_HandleTagSetup(int32 tag, Entity *entityPtr)
{
    if (SceneInfo->inEditor)
        return NULL;

    EntityButton *taggedEntity = NULL;
    if (tag > 0) {
        if (Button) {
            foreach_all(Button, button)
            {
                if (button->tag == tag) {
                    taggedEntity = button;
                    foreach_break;
                }
            }
        }

        if (TurretSwitch && !taggedEntity) {
            foreach_all(TurretSwitch, turretSwitch)
            {
                if (turretSwitch->tag == tag) {
                    taggedEntity = (EntityButton *)turretSwitch;
                    foreach_break;
                }
            }
        }
    }

    if (taggedEntity) {
        if ((Button && taggedEntity->objectID == Button->objectID) || (TurretSwitch && taggedEntity->objectID == TurretSwitch->objectID)) {
            if (entityPtr) {
                int32 distX = abs(entityPtr->position.x - taggedEntity->position.x);
                int32 distY = abs(entityPtr->position.y - taggedEntity->position.y);

                if (entityPtr->updateRange.x < 0x800000 + distX)
                    entityPtr->updateRange.x = 0x800000 + distX;

                if (entityPtr->updateRange.y < 0x800000 + distY)
                    entityPtr->updateRange.y = 0x800000 + distY;
            }
        }
        else {
            taggedEntity = NULL;
        }
    }
    return taggedEntity;
}

void LRZ2Setup_GetTileInfo(int32 *tileInfo, int32 cPlane, int32 x, int32 y, int32 offsetX, int32 offsetY, uint8 *behaviour)
{
    int32 tileInfoLow   = RSDK.GetTileInfo(Zone->fgLow, x >> 20, y >> 20);
    int32 tileInfoHigh  = RSDK.GetTileInfo(Zone->fgHigh, x >> 20, y >> 20);
    int32 behaviourLow  = RSDK.GetTileFlags(tileInfoLow, cPlane);
    int32 behaviourHigh = RSDK.GetTileFlags(tileInfoHigh, cPlane);

    int32 tileInfoMove  = 0;
    int32 behaviourMove = 0;
    if (Zone->moveLayer) {
        tileInfoMove  = RSDK.GetTileInfo(Zone->moveLayer, (offsetX + x) >> 20, (offsetY + y) >> 20);
        behaviourMove = RSDK.GetTileFlags(tileInfoMove, cPlane);
    }

    int32 tileSolidLow  = 0;
    int32 tileSolidHigh = 0;
    int32 tileSolidMove = 0;
    if (cPlane) {
        tileSolidHigh = tileInfoHigh >> 14;
        tileSolidLow  = tileInfoLow >> 14;
    }
    else {
        tileSolidHigh = tileInfoHigh >> 12;
        tileSolidLow  = tileInfoLow >> 12;
    }
    if (Zone->moveLayer)
        tileSolidMove = tileInfoMove >> 12;

    *tileInfo  = 0;
    *behaviour = LRZ2_TFLAGS_NORMAL;
    if (behaviourMove && (tileSolidMove & 3)) {
        *tileInfo  = tileInfoMove;
        *behaviour = behaviourMove;
    }
    else if (behaviourHigh && (tileSolidHigh & 3)) {
        *tileInfo  = tileInfoHigh;
        *behaviour = behaviourHigh;
    }
    else if (behaviourLow && (tileSolidLow & 3)) {
        *tileInfo  = tileInfoLow;
        *behaviour = behaviourLow;
    }
}

#if RETRO_INCLUDE_EDITOR
void LRZ2Setup_EditorDraw(void) {}

void LRZ2Setup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("LRZ2 Outro", GENERICTRIGGER_LRZ2_OUTRO);
}
#endif

void LRZ2Setup_Serialize(void) {}

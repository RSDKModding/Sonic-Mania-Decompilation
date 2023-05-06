// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ2Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLRZ2Setup *LRZ2Setup;

void LRZ2Setup_Update(void) {}

void LRZ2Setup_LateUpdate(void) {}

void LRZ2Setup_StaticUpdate(void)
{
    // Lava
    LRZ2Setup->lavaPalTimer += 24;
    if (LRZ2Setup->lavaPalTimer >= 256) {
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

    RSDK.SetLimitedFade(0, LRZ2Setup->conveyorSrcPal, LRZ2Setup->conveyorDstPal, (RSDK.Cos256(LRZ2Setup->conveyorPalTimer) >> 1) + 0x80, 160, 168);
    if (!LRZ2Setup->conveyorOff && !(Zone->timer & 1))
        RSDK.RotatePalette(0, 228, 231, (LRZ2Setup->conveyorDir & 0xFF));

    // Rock Hues
    RSDK.SetLimitedFade(5, 1, 4, abs(RSDK.Cos1024(2 * (Zone->timer & 0x1FF)) >> 3), 224, 227);
    RSDK.RotatePalette(3, 224, 227, true);
    RSDK.RotatePalette(4, 224, 227, true);

    RSDK.SetLimitedFade(6, 2, 4, abs(RSDK.Cos1024(2 * (Zone->timer & 0x1FF)) >> 3), 224, 227);
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

            LRZ2Setup_GetTileInfo(player->position.x, player->position.y + (playerHitbox->bottom << 16), player->moveLayerPosition.x,
                                  player->moveLayerPosition.y, player->collisionPlane, &tileInfo, &behaviour);

            if (behaviour == LRZ2_TFLAGS_NORMAL) {
                LRZ2Setup_GetTileInfo(player->position.x + (playerHitbox->right << 16), player->position.y + (playerHitbox->bottom << 16),
                                      player->moveLayerPosition.x, player->moveLayerPosition.y, player->collisionPlane, &tileInfo, &behaviour);
            }

            if (behaviour == LRZ2_TFLAGS_NORMAL) {
                LRZ2Setup_GetTileInfo(player->position.x + (playerHitbox->left << 16), player->position.y + (playerHitbox->bottom << 16),
                                      player->moveLayerPosition.x, player->moveLayerPosition.y, player->collisionPlane, &tileInfo, &behaviour);
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
                        Player_HurtFlip(player);
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

    if (!isMainGameMode() || !globals->atlEnabled || CutsceneRules_CheckStageReload()) {
        for (int32 p = 0; p < Player->playerCount; ++p) {
            Zone->cameraBoundsL[p] += 0x100;
        }
    }
    else {
        LRZ2Setup_HandleStageReload();
    }

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreLRZ2.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 1, 128, 128);
    }
#endif

    LRZ2Setup->conveyorDstPal = 1;
    LRZ2Setup->conveyorSrcPal = 2;

    if (globals->gameMode == MODE_TIMEATTACK || globals->gameMode == MODE_COMPETITION)
        GenericTrigger->callbacks[GENERICTRIGGER_LRZ2_OUTRO] = StateMachine_None;
    else
        GenericTrigger->callbacks[GENERICTRIGGER_LRZ2_OUTRO] = LRZ2Setup_Trigger_StartOutro;
}

void LRZ2Setup_HandleStageReload(void)
{
    Vector2 pos = { 0, 0 };

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

void LRZ2Setup_Trigger_StartOutro(void)
{
    if (isMainGameMode()) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            globals->tempFlags = player1->position.y > (1024 << 16);
#endif

        if (player1->stateInput) {
            player1->stateInput = StateMachine_None;
            player1->left       = false;
            player1->right      = true;

            for (int32 p = 0; p < Player->playerCount; ++p) StarPost->postIDs[p] = 0;

            SaveGame_SavePlayerState();

            globals->enableIntro       = true;
            globals->suppressAutoMusic = true;
            globals->suppressTitlecard = true;

            ++SceneInfo->listPos;
            if (!RSDK.CheckValidScene())
                RSDK.SetScene("Presentation", "Title Screen");

            Zone_StartFadeOut(10, 0x000000);
        }

        if (player1->superState == SUPERSTATE_SUPER || player1->state == Player_State_Transform)
            globals->restartPowerups |= 0x80;

#if GAME_VERSION != VER_100
        globals->restartMusicID = Music->activeTrack;
#endif
    }
}

EntityButton *LRZ2Setup_SetupTagLink(int32 tag, Entity *entity)
{
    // Yes..... :smirk:
    // if (SceneInfo->inEditor)
    //     return NULL;

    EntityButton *taggedButton = NULL;
    if (tag > 0) {
        if (Button) {
            foreach_all(Button, button)
            {
                if (button->tag == tag) {
                    taggedButton = button;
                    foreach_break;
                }
            }
        }

        if (TurretSwitch && !taggedButton) {
            foreach_all(TurretSwitch, turretSwitch)
            {
                if (turretSwitch->tag == tag) {
                    taggedButton = (EntityButton *)turretSwitch;
                    foreach_break;
                }
            }
        }
    }

    if (taggedButton) {
        if ((Button && taggedButton->classID == Button->classID) || (TurretSwitch && taggedButton->classID == TurretSwitch->classID)) {
            if (entity) {
                int32 distX = abs(entity->position.x - taggedButton->position.x);
                int32 distY = abs(entity->position.y - taggedButton->position.y);

                if (entity->updateRange.x < 0x800000 + distX)
                    entity->updateRange.x = 0x800000 + distX;

                if (entity->updateRange.y < 0x800000 + distY)
                    entity->updateRange.y = 0x800000 + distY;
            }
        }
        else {
            taggedButton = NULL;
        }
    }

    return taggedButton;
}

void LRZ2Setup_GetTileInfo(int32 x, int32 y, int32 moveOffsetX, int32 moveOffsetY, int32 cPlane, int32 *tile, uint8 *flags)
{
    int32 tileLow  = RSDK.GetTile(Zone->fgLayer[0], x >> 20, y >> 20);
    int32 tileHigh = RSDK.GetTile(Zone->fgLayer[1], x >> 20, y >> 20);

    int32 flagsLow  = RSDK.GetTileFlags(tileLow, cPlane);
    int32 flagsHigh = RSDK.GetTileFlags(tileHigh, cPlane);

    int32 tileMove  = 0;
    int32 flagsMove = 0;
    if (Zone->moveLayer) {
        tileMove  = RSDK.GetTile(Zone->moveLayer, (moveOffsetX + x) >> 20, (moveOffsetY + y) >> 20);
        flagsMove = RSDK.GetTileFlags(tileMove, cPlane);
    }

    int32 tileSolidLow  = 0;
    int32 tileSolidHigh = 0;
    int32 tileSolidMove = 0;
    if (cPlane) {
        tileSolidHigh = (tileHigh >> 14) & 3;
        tileSolidLow  = (tileLow >> 14) & 3;
    }
    else {
        tileSolidHigh = (tileHigh >> 12) & 3;
        tileSolidLow  = (tileLow >> 12) & 3;
    }

    if (Zone->moveLayer)
        tileSolidMove = (tileMove >> 12) & 3;

    *tile  = 0;
    *flags = LRZ2_TFLAGS_NORMAL;
    if (flagsMove && tileSolidMove) {
        *tile  = tileMove;
        *flags = flagsMove;
    }
    else if (flagsHigh && tileSolidHigh) {
        *tile  = tileHigh;
        *flags = flagsHigh;
    }
    else if (flagsLow && tileSolidLow) {
        *tile  = tileLow;
        *flags = flagsLow;
    }
}

#if GAME_INCLUDE_EDITOR
void LRZ2Setup_EditorDraw(void) {}

void LRZ2Setup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("LRZ2 Outro", GENERICTRIGGER_LRZ2_OUTRO);
}
#endif

void LRZ2Setup_Serialize(void) {}

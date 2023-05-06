// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZ1Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPSZ1Setup *PSZ1Setup;

void PSZ1Setup_Update(void) {}

void PSZ1Setup_LateUpdate(void) {}

void PSZ1Setup_StaticUpdate(void)
{
    PSZ1Setup->newspaperHAniFrame -= 8;
    PSZ1Setup->newspaperHAniFrame &= 0x3F;

    PSZ1Setup->newspaperVAniFrame += 4;
    if (PSZ1Setup->newspaperVAniFrame == 80)
        PSZ1Setup->newspaperVAniFrame = 0;

    RSDK.DrawAniTiles(PSZ1Setup->aniTiles2, 985, 0, PSZ1Setup->newspaperVAniFrame, 48, 80);
    RSDK.DrawAniTiles(PSZ1Setup->aniTiles3, 716, PSZ1Setup->newspaperHAniFrame, 0, 64, 48);

    if (!(Zone->timer & 1))
        RSDK.RotatePalette(0, 236, 239, false);

    if (++PSZ1Setup->inkRollerAniDuration == 3) {
        PSZ1Setup->inkRollerAniDuration = 0;

        ++PSZ1Setup->inkRollerAniFrame;
        PSZ1Setup->inkRollerAniFrame &= 0xF;

        RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 552, 16 * PSZ1Setup->inkRollerAniFrame, 0, 16, 128);
    }

    if (--PSZ1Setup->buttonFrontAniDuration < 1) {
        ++PSZ1Setup->buttonFrontAniFrame;

        PSZ1Setup->buttonFrontAniFrame %= 7;
        PSZ1Setup->buttonFrontAniDuration = PSZ1Setup->buttonSideAniDurationTable[PSZ1Setup->buttonFrontAniFrame];

        RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 216, 16 * PSZ1Setup->buttonFrontAniFrame, 128, 16, 16);
    }

    if (--PSZ1Setup->buttonBackAniDuration < 1) {
        ++PSZ1Setup->buttonBackAniFrame;

        PSZ1Setup->buttonBackAniFrame %= 7;
        PSZ1Setup->buttonBackAniDuration = PSZ1Setup->buttonSideAniDurationTable[PSZ1Setup->buttonBackAniFrame];

        RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 217, 16 * (PSZ1Setup->buttonBackAniFrame + 7), 128, 16, 16);
    }

    if (--PSZ1Setup->buttonSideFrontAniDuration < 1) {
        ++PSZ1Setup->buttonSideFrontAniFrame;

        PSZ1Setup->buttonSideFrontAniFrame %= 14;
        PSZ1Setup->buttonSideFrontAniDuration = PSZ1Setup->buttonSideBackAniDurationTable[PSZ1Setup->buttonSideFrontAniFrame];

        RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 218, 16 * PSZ1Setup->buttonSideFrontAniFrame, 144, 16, 32);
    }

    if (--PSZ1Setup->buttonSideBackAniDuration < 1) {
        ++PSZ1Setup->buttonSideBackAniFrame;

        PSZ1Setup->buttonSideBackAniFrame %= 14;
        PSZ1Setup->buttonSideBackAniDuration = PSZ1Setup->buttonSideBackAniDurationTable[PSZ1Setup->buttonSideBackAniFrame];

        RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 220, 16 * PSZ1Setup->buttonSideBackAniFrame, 176, 16, 48);
    }

    if (--PSZ1Setup->buttonSideAniDuration < 1) {
        ++PSZ1Setup->buttonSideAniFrame;

        PSZ1Setup->buttonSideAniFrame %= 7;
        PSZ1Setup->buttonSideAniDuration = PSZ1Setup->buttonSideAniDurationTable[PSZ1Setup->buttonSideAniFrame];

        RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 223, 16 * PSZ1Setup->buttonSideAniFrame, 224, 16, 32);
    }

    if (!(Zone->timer & 1)) {
        ++PSZ1Setup->buttonBladeAniFrame;

        PSZ1Setup->buttonBladeAniFrame %= 3;
        switch (PSZ1Setup->buttonBladeAniFrame) {
            case 0: RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 227, 112, 224, 48, 32); break;

            case 1:
            case 3: RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 227, 160, 224, 48, 32); break;

            case 2: RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 227, 208, 224, 48, 32); break;

            default: break;
        }
    }

    if (Zone->cameraBoundsB[0] == 2944) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (player->position.y < 0xAA00000) {
            if (player->position.y <= 0x900000) {
                EntityCamera *camera = player->camera;

                camera->position.y += 0xA000000;
                camera->center.y += 0xA00;
                ScreenInfo[camera->screenID].position.y += 0xA00;
                player->position.y += 0xA000000;

                TileLayer *background1 = RSDK.GetTileLayer(0);
                TileLayer *background2 = RSDK.GetTileLayer(1);
                background1->scrollPos -= 0xA00000;
                background2->scrollPos -= 0x2800000;

                if (background1->scrollPos < 0)
                    background1->scrollPos += 0x4000000;

                if (background2->scrollPos < 0)
                    background2->scrollPos += 0x6000000;

                if (ScreenInfo->position.y >= 0x180)
                    ScreenInfo->position.y += 0xA00;
            }
        }
        else {
            EntityCamera *camera = player->camera;

            camera->position.y -= 0xA000000;
            camera->center.y -= 0xA00;
            ScreenInfo[camera->screenID].position.y -= 0xA00;
            player->position.y -= 0xA000000;

            TileLayer *background1 = RSDK.GetTileLayer(0);
            TileLayer *background2 = RSDK.GetTileLayer(1);
            background1->scrollPos += 0xA00000;
            background2->scrollPos += 0x2800000;

            if (background1->scrollPos >= 0x6E000000)
                background1->scrollPos -= 0x4000000;

            if (background2->scrollPos >= 0x6E000000)
                background2->scrollPos -= 0x6000000;

            if (ScreenInfo->position.y >= 0xA00)
                ScreenInfo->position.y -= 0xA00;
        }

        if (player->camera->position.y < 0x6100000) {
            if (player->camera->position.y <= 0x2800000) {
                if (PSZ1Setup->levelWrapType == PSZ1_WRAP_BOTTOM) {
                    PSZ1Setup->levelWrapType = PSZ1_WRAP_TOP;
                    PSZ1Setup_LevelWrap_Top();
                }
            }
        }
        else {
            if (PSZ1Setup->levelWrapType == PSZ1_WRAP_TOP) {
                PSZ1Setup->levelWrapType = PSZ1_WRAP_BOTTOM;
                PSZ1Setup_LevelWrap_Bottom();
            }
        }
    }

    if (PSZ1Setup->petalBehaviourActive) {
        if (PSZ1Setup->petalTimer <= 0) {
            foreach_active(Player, player)
            {
                Hitbox *playerHitbox = Player_GetHitbox(player);

                uint16 tile = RSDK.GetTile(Zone->fgLayer[0], player->position.x >> 20, (player->position.y + (playerHitbox->bottom << 16)) >> 20);
                bool32 isLowLayer = true;
                if (tile == (uint16)-1) {
                    tile       = RSDK.GetTile(Zone->fgLayer[1], player->position.x >> 20, (player->position.y + (playerHitbox->bottom << 16)) >> 20);
                    isLowLayer = false;
                }

                if (RSDK.GetTileFlags(tile, player->collisionPlane)) {
                    if (abs(player->groundVel) >= 0x60000 || player->state == Player_State_DropDash) {
                        RSDK_THIS(PSZ1Setup); // not sure what this is meant to be since this is a StaticUpdate event...

                        EntityPetalPile *pile = CREATE_ENTITY(PetalPile, self, player->position.x, player->position.y + (playerHitbox->bottom << 16));
                        pile->leafPattern     = PETALPILE_PATTERN_4;
                        pile->tileLayer       = isLowLayer;
                        pile->pileSize.x      = 0x40000;
                        pile->pileSize.y      = 0x40000;
                        pile->noRemoveTiles   = true;
                        pile->petalRadius     = 0xB5555;
                        pile->petalDir        = 2 * (player->direction != FLIP_NONE) - 1;
                        pile->petalVel        = player->groundVel >> 1;
                        PSZ1Setup->petalTimer = 3;
                    }
                }
            }
        }
        else {
            PSZ1Setup->petalTimer--;
        }
    }
}

void PSZ1Setup_Draw(void) {}

void PSZ1Setup_Create(void *data) {}

void PSZ1Setup_StageLoad(void)
{
    PSZ1Setup->aniTiles1 = RSDK.LoadSpriteSheet("PSZ1/AniTiles.gif", SCOPE_STAGE);
    PSZ1Setup->aniTiles2 = RSDK.LoadSpriteSheet("PSZ1/AniTiles2.gif", SCOPE_STAGE);
    PSZ1Setup->aniTiles3 = RSDK.LoadSpriteSheet("PSZ1/AniTiles3.gif", SCOPE_STAGE);

    PSZ1Setup->petalBehaviourActive = false;
    PSZ1Setup->levelWrapType        = PSZ1_WRAP_TOP;

    GenericTrigger->callbacks[GENERICTRIGGER_PSZ1_PETALSINACTIVE] = PSZ1Setup_Trigger_DeactivatePetalBehaviour;
    GenericTrigger->callbacks[GENERICTRIGGER_PSZ1_PETALSACTIVE]   = PSZ1Setup_Trigger_ActivatePetalBehaviour;
    GenericTrigger->callbacks[GENERICTRIGGER_PSZ1_ACHIEVEMENT]    = PSZ1Setup_Trigger_AwardAchievement;

    PSZ1Setup->buttonFrontAniDuration = 12;
    PSZ1Setup->buttonFrontAniFrame    = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 216, 0, 128, 16, 16);

    PSZ1Setup->buttonBackAniDuration = 24;
    PSZ1Setup->buttonBackAniFrame    = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 217, 112, 128, 16, 16);

    PSZ1Setup->buttonSideFrontAniDuration = 36;
    PSZ1Setup->buttonSideFrontAniFrame    = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 218, 0, 144, 16, 32);

    PSZ1Setup->buttonSideBackAniDuration = 48;
    PSZ1Setup->buttonSideBackAniFrame    = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 220, 0, 176, 16, 48);

    PSZ1Setup->buttonSideAniDuration = 64;
    PSZ1Setup->buttonSideAniFrame    = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTiles1, 223, 0, 224, 16, 32);

    if (isMainGameMode() && CutsceneRules_IsAct1())
        Zone->stageFinishCallback = PSZ1Setup_StageFinish_EndAct1;

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE)
        RSDK.LoadPalette(0, "EncorePSZ1.act", 0b0000000011111111);

    // Fun Fact: Pre-Plus didn't have animal types set for PGZ! It'd always be flickies due to that being the default value!
    Animals->animalTypes[0] = ANIMAL_POCKY;
    Animals->animalTypes[1] = ANIMAL_BECKY;

    BGSwitch->switchCallback[PSZ1_BG_INSIDE]  = PSZ1Setup_BGSwitch_Inside;
    BGSwitch->switchCallback[PSZ1_BG_OUTSIDE] = PSZ1Setup_BGSwitch_Outside;

    BGSwitch->layerIDs[0] = PSZ1_BG_INSIDE;
    BGSwitch->layerIDs[1] = PSZ1_BG_INSIDE;
    BGSwitch->layerIDs[2] = PSZ1_BG_INSIDE;
    BGSwitch->layerIDs[3] = PSZ1_BG_INSIDE;
#endif
}

#if MANIA_USE_PLUS
void PSZ1Setup_BGSwitch_Inside(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = 0; // Background 1
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = 0; // Background 2
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = 0; // Background 3
}

void PSZ1Setup_BGSwitch_Outside(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = 0;               // Background 1
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT; // Background 2
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT; // Background 3
}
#endif

void PSZ1Setup_Trigger_DeactivatePetalBehaviour(void) { PSZ1Setup->petalBehaviourActive = true; }

void PSZ1Setup_Trigger_ActivatePetalBehaviour(void) { PSZ1Setup->petalBehaviourActive = false; }

void PSZ1Setup_Trigger_AwardAchievement(void)
{
    RSDK_THIS(GenericTrigger);

    if (!PSZ1Setup->hasAchievement) {
        int32 count = 0;
        foreach_all(Crate, crate)
        {
            if (MathHelpers_PointInHitbox(self->position.x, self->position.y, crate->position.x, crate->position.y, self->direction, &self->hitbox)) {
                if (crate->frameID == 1)
                    ++count;
            }
        }

        if (!count) {
            API_UnlockAchievement(&achievementList[ACH_PGZ]);
            PSZ1Setup->hasAchievement = true;
        }
    }
}

void PSZ1Setup_StageFinish_EndAct1(void)
{
    ++SceneInfo->listPos;
    globals->enableIntro       = true;
    globals->suppressTitlecard = true;
    globals->suppressAutoMusic = true;

    for (int32 p = 0; p < Player->playerCount; ++p) StarPost->postIDs[p] = 0;

    SaveGame_SavePlayerState();
    Zone_StoreEntities(15876 << 16, 1316 << 16);

    RSDK.LoadScene();
}

void PSZ1Setup_LevelWrap_Top(void)
{
    for (int32 i = 1; i < ENTITY_COUNT; ++i) {
        EntityPlatform *entity = RSDK_GET_ENTITY(i, Platform);
        if (entity->classID != BoundsMarker->classID) {
            if (entity->position.y >= 0x6800000) {
                entity->position.y -= 0xA000000;

                if (entity->classID == PrintBlock->classID || entity->classID == Platform->classID || entity->classID == Crate->classID) {
                    EntityPlatform *platform = entity;
                    platform->drawPos.y -= 0xA000000;
                    platform->centerPos.y -= 0xA000000;
                }
            }
        }
    }
}

void PSZ1Setup_LevelWrap_Bottom(void)
{
    for (int32 i = 1; i < ENTITY_COUNT; ++i) {
        EntityPlatform *entity = RSDK_GET_ENTITY(i, Platform);
        if (entity->classID != BoundsMarker->classID) {
            if (entity->position.y <= 0x1800000) {
                entity->position.y += 0xA000000;

                if (entity->classID == PrintBlock->classID || entity->classID == Platform->classID || entity->classID == Crate->classID) {
                    EntityPlatform *platform = entity;
                    platform->drawPos.y += 0xA000000;
                    platform->centerPos.y += 0xA000000;
                }
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void PSZ1Setup_EditorDraw(void) {}

void PSZ1Setup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(BGSwitch, bgID);
    RSDK_ENUM_VAR("Inside BG", PSZ1_BG_INSIDE);
    RSDK_ENUM_VAR("Outside BG", PSZ1_BG_OUTSIDE);

    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Stop Petal Behaviour", GENERICTRIGGER_PSZ1_PETALSINACTIVE);
    RSDK_ENUM_VAR("Start Petal Behaviour", GENERICTRIGGER_PSZ1_PETALSACTIVE);
    RSDK_ENUM_VAR("PSZ1 Achievement Area", GENERICTRIGGER_PSZ1_ACHIEVEMENT);
}
#endif

void PSZ1Setup_Serialize(void) {}

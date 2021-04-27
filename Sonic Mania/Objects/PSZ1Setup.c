#include "../SonicMania.h"

ObjectPSZ1Setup *PSZ1Setup;

void PSZ1Setup_Update(void) {}

void PSZ1Setup_LateUpdate(void) {}

void PSZ1Setup_StaticUpdate(void)
{
    PSZ1Setup->aniTilesFrameB -= 8;
    PSZ1Setup->aniTilesFrameB &= 0x3F;

    PSZ1Setup->aniTilesFrameA += 4;
    if (PSZ1Setup->aniTilesFrameA == 80) {
        PSZ1Setup->aniTilesFrameA = 0;
    }
    RSDK.DrawAniTiles(PSZ1Setup->aniTilesB, 985, 0, PSZ1Setup->aniTilesFrameA, 48, 80);
    RSDK.DrawAniTiles(PSZ1Setup->aniTilesC, 716, PSZ1Setup->aniTilesFrameB, 0, 64, 48);

    if (!(Zone->timer & 1))
        RSDK.RotatePalette(0, 236, 239, 0);

    if (++PSZ1Setup->aniTilesDelayC == 3) {
        PSZ1Setup->aniTilesDelayC = 0;
        ++PSZ1Setup->aniTilesFrameC;
        PSZ1Setup->aniTilesFrameC &= 0xF;
        RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 552, 16 * PSZ1Setup->aniTilesFrameC, 0, 16, 128);
    }

    if (--PSZ1Setup->aniTilesDelayD < 1) {
        ++PSZ1Setup->aniTilesFrameD;
        PSZ1Setup->aniTilesFrameD %= 7;
        PSZ1Setup->aniTilesDelayD = PSZ1Setup->aniTileDelays1[PSZ1Setup->aniTilesFrameD];
        RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 216, 16 * PSZ1Setup->aniTilesFrameD, 128, 16, 16);
    }

    if (--PSZ1Setup->aniTilesDelayE < 1) {
        ++PSZ1Setup->aniTilesFrameE;
        PSZ1Setup->aniTilesFrameE %= 7;
        PSZ1Setup->aniTilesDelayE = PSZ1Setup->aniTileDelays1[PSZ1Setup->aniTilesFrameE];
        RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 217, 16 * (PSZ1Setup->aniTilesFrameE + 7), 128, 16, 16);
    }

    if (--PSZ1Setup->aniTilesDelayF < 1) {
        ++PSZ1Setup->aniTilesFrameF;
        PSZ1Setup->aniTilesFrameF %= 14;
        PSZ1Setup->aniTilesDelayF = PSZ1Setup->aniTileDelays2[PSZ1Setup->aniTilesFrameF];
        RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 218, 16 * PSZ1Setup->aniTilesFrameF, 144, 16, 32);
    }

    if (--PSZ1Setup->aniTilesDelayG < 1) {
        ++PSZ1Setup->aniTilesFrameG;
        PSZ1Setup->aniTilesFrameG %= 14;
        PSZ1Setup->aniTilesDelayG = PSZ1Setup->aniTileDelays2[PSZ1Setup->aniTilesFrameG];
        RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 220, 16 * PSZ1Setup->aniTilesFrameG, 176, 16, 48);
    }

    if (--PSZ1Setup->aniTilesDelayH < 1) {
        ++PSZ1Setup->aniTilesFrameH;
        PSZ1Setup->aniTilesFrameH %= 7;
        PSZ1Setup->aniTilesDelayH = PSZ1Setup->aniTileDelays1[PSZ1Setup->aniTilesFrameH];
        RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 223, 16 * PSZ1Setup->aniTilesFrameH, 224, 16, 32);
    }

    if (!(Zone->timer & 1)) {
        ++PSZ1Setup->aniTilesFrameI;
        PSZ1Setup->aniTilesFrameI %= 3;
        switch (PSZ1Setup->aniTilesFrameI) {
            case 0: RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 227, 112, 224, 48, 32); break;
            case 1:
            case 3: RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 227, 160, 224, 48, 32); break;
            case 2: RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 227, 208, 224, 48, 32); break;
            default: break;
        }
    }

    //TODO: the rest
}

void PSZ1Setup_Draw(void) {}

void PSZ1Setup_Create(void *data) {}

void PSZ1Setup_StageLoad(void)
{
    PSZ1Setup->aniTilesA         = RSDK.LoadSpriteSheet("PSZ1/AniTiles.gif", SCOPE_STAGE);
    PSZ1Setup->aniTilesB         = RSDK.LoadSpriteSheet("PSZ1/AniTiles2.gif", SCOPE_STAGE);
    PSZ1Setup->aniTilesC         = RSDK.LoadSpriteSheet("PSZ1/AniTiles3.gif", SCOPE_STAGE);
    PSZ1Setup->flag              = 0;
    PSZ1Setup->field_A0          = 0;
    GenericTrigger->callbacks[0] = PSZ1Setup_TriggerCB1;
    GenericTrigger->callbacks[1] = PSZ1Setup_TriggerCB2;
    GenericTrigger->callbacks[2] = PSZ1Setup_TriggerCB3;

    PSZ1Setup->aniTilesDelayD = 12;
    PSZ1Setup->aniTilesFrameD = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 216, 0, 128, 16, 16);

    PSZ1Setup->aniTilesDelayE = 24;
    PSZ1Setup->aniTilesFrameE = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 217, 112, 128, 16, 16);

    PSZ1Setup->aniTilesDelayF = 36;
    PSZ1Setup->aniTilesFrameF = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 218, 0, 144, 16, 32);

    PSZ1Setup->aniTilesDelayG = 48;
    PSZ1Setup->aniTilesFrameG = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 220, 0, 176, 16, 48);

    PSZ1Setup->aniTilesDelayH = 64;
    PSZ1Setup->aniTilesFrameH = 0;
    RSDK.DrawAniTiles(PSZ1Setup->aniTilesA, 223, 0, 224, 16, 32);

    if ((globals->gameMode == MODE_MANIA || globals->gameMode == MODE_ENCORE) && PlayerHelpers_CheckAct1())
        Zone->stageFinishCallback = PSZ1Setup_ActTransitionCB;
#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter & FILTER_ENCORE)
        RSDK.LoadPalette(0, "EncorePSZ1.act", 0xFF);
    Animals->animalTypes[0] = ANIMAL_POCKY;
    Animals->animalTypes[1] = ANIMAL_BECKY;
#endif
    BGSwitch->switchCallback[0] = PSZ1Setup_BGSwitchCB1;
    BGSwitch->switchCallback[1] = PSZ1Setup_BGSwitchCB2;
    BGSwitch->layerIDs[0]       = 0;
    BGSwitch->layerIDs[1]       = 0;
    BGSwitch->layerIDs[2]       = 0;
    BGSwitch->layerIDs[3]       = 0;
}

void PSZ1Setup_BGSwitchCB1(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->layerID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->layerID] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->layerID] = 0;
}

void PSZ1Setup_BGSwitchCB2(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->layerID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
}

void PSZ1Setup_TriggerCB1(void) { PSZ1Setup->flag = true; }

void PSZ1Setup_TriggerCB2(void) { PSZ1Setup->flag = false; }

void PSZ1Setup_TriggerCB3(void)
{
    if (!PSZ1Setup->hasAchievement) {
        RSDK_THIS(GenericTrigger);

        int count = 1; // 0;
        foreach_all(Crate, crate)
        {
            //if (MathHelpers_Unknown8(entity->position.x, entity->position.y, &entity->hitbox, crate->position.x, crate->position.y)) {
            //    if (LOBYTE(v3[1].updateRange.y) == 1)
            //       ++v1;
            //}
        }

        if (!count) {
            User.UnlockAchievement("ACH_PGZ");
            PSZ1Setup->hasAchievement = true;
        }
    }
}

void PSZ1Setup_ActTransitionCB(void)
{
    ++RSDK_sceneInfo->listPos;
    globals->enableIntro       = true;
    globals->suppressTitlecard = true;
    globals->suppressAutoMusic = true;

    for (int p = 0; p < Player->playerCount; ++p) {
        StarPost->postIDs[p] = 0;
    }

    SaveGame_SavePlayerState();
    Zone_StoreEntities(0x3E040000, 0x5240000);
    RSDK.InitSceneLoad();
}

void PSZ1Setup_LevelWrap_Top(void)
{
    for (int i = 1; i < ENTITY_COUNT; ++i) {
        Entity *entity = RSDK.GetEntityByID(i);
        if (entity->objectID != BoundsMarker->objectID) {
            if (entity->position.y >= 0x6800000) {
                entity->position.y -= 0xA000000;
                if (entity->objectID == PrintBlock->objectID || entity->objectID == Platform->objectID || entity->objectID == Crate->objectID) {
                    EntityPlatform *platform = (EntityPlatform *)entity;
                    platform->drawPos.y -= 0xA000000;
                    platform->centerPos.y -= 0xA000000;
                }
            }
        }
    }
}

void PSZ1Setup_LevelWrap_Bottom(void)
{
    for (int i = 1; i < ENTITY_COUNT; ++i) {
        Entity* entity = RSDK.GetEntityByID(i);
        if (entity->objectID != BoundsMarker->objectID) {
            if (entity->position.y <= 0x1800000) {
                entity->position.y += 0xA000000;
                if (entity->objectID == PrintBlock->objectID || entity->objectID == Platform->objectID || entity->objectID == Crate->objectID) {
                    EntityPlatform *platform = (EntityPlatform *)entity;
                    platform->drawPos.y += 0xA000000;
                    platform->centerPos.y += 0xA000000;
                }
            }
        }
    }
}

void PSZ1Setup_EditorDraw(void) {}

void PSZ1Setup_EditorLoad(void) {}

void PSZ1Setup_Serialize(void) {}

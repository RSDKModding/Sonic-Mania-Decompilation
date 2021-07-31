#include "SonicMania.h"

ObjectTMZ1Setup *TMZ1Setup;

void TMZ1Setup_Update(void)
{
    RSDK_THIS(TMZ1Setup);
    StateMachine_Run(entity->state);
}

void TMZ1Setup_LateUpdate(void) {}

void TMZ1Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 1)) {
        ++TMZ1Setup->bgPtr->deformationOffset;
    }

    if (!(Zone->timer & 3)) {
        TMZ1Setup->aniTileFrame += 32;
        TMZ1Setup->aniTileFrame = TMZ1Setup->aniTileFrame;
        RSDK.DrawAniTiles(TMZ1Setup->aniTiles, 34, TMZ1Setup->aniTileFrame, 0, 32, 112);
    }

    if (!(Zone->timer & 1))
        RSDK.RotatePalette(0, 204, 207, false);

    if (!RSDK.GetEntityCount(TMZ1Setup->objectID, true)) {
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(2 * Zone->timer) >> 1) + 128, 192, 197);
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(2 * Zone->timer + 128) >> 1) + 128, 198, 203);
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(4 * Zone->timer + 128) >> 1) + 64, 220, 223);
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(Zone->timer) >> 1) + 128, 211, 213);
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(Zone->timer + 128) >> 1) + 128, 237, 239);
    }

    if (TMZ1Setup->aniTileDelay <= 0) {
        TMZ1Setup->angle += 32;
        TMZ1Setup->angle = TMZ1Setup->angle;
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(TMZ1Setup->angle) >> 1) + 128, 184, 190);
        if (TMZ1Setup->angle == 160) {
            TMZ1Setup->aniTileDelay = RSDK.Rand(2, 60);
        }
    }
    else {
        TMZ1Setup->aniTileDelay--;
    }

    if (TMZ1Setup->stageState < 2) {
        int posY = RSDK_screens->centerY + RSDK_screens->position.y;
        if (posY >= 4192) {
            if (posY > 4528 && TMZ1Setup->stageState != 1 && !RSDK.GetEntityCount(TMZ1Setup->objectID, true))
                RSDK.CreateEntity(TMZ1Setup->objectID, intToVoid(1), 0, 0);
        }
        else if (TMZ1Setup->stageState && !RSDK.GetEntityCount(TMZ1Setup->objectID, true)) {
            RSDK.CreateEntity(TMZ1Setup->objectID, intToVoid(0), 0, 0);
        }
    }
    EntityActClear *actClear = RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear);
    if (!TMZ1Setup->reloadFlag && actClear->objectID == ActClear->objectID && actClear->state == ActClear_Unknown6 && !Player->gotHit[0]) {
#if RETRO_USE_PLUS 
        API.UnlockAchievement("ACH_TMZ");
#else
        APICallback_UnlockAchievement("ACH_TMZ");
#endif
        TMZ1Setup->reloadFlag = true;
    }
}

void TMZ1Setup_Draw(void) {}

void TMZ1Setup_Create(void *data)
{
    RSDK_THIS(TMZ1Setup);
    RSDK_sceneInfo->entity->active = ACTIVE_NORMAL;

    switch (TMZ1Setup->stageState) {
        default: entity->state = TMZ1Setup_StateUnknown4; break;
        case -1: entity->state = TMZ1Setup_StateUnknown3; break;
        case 2: entity->state = TMZ1Setup_StateUnknown2; break;
    }
}

void TMZ1Setup_StageLoad(void)
{
    TMZ1Setup->aniTiles     = RSDK.LoadSpriteSheet("TMZ1/AniTiles.gif", SCOPE_STAGE);
    TMZ1Setup->aniTileDelay = RSDK.Rand(2, 60);
    TMZ1Setup->angle        = 192;
    TMZ1Setup->stageState   = -1;
    TMZ1Setup->bgPtr        = RSDK.GetSceneLayer(0);

    int ang = 0;
    for (int i = 0; i < 0x200; ++i) {
        TMZ1Setup->bgPtr->deformationData[i] = (4 * RSDK.Sin1024(ang)) >> 10;
        ang += 16;
    }
    memcpy(TMZ1Setup->bgPtr->deformationData + 0x200, TMZ1Setup->bgPtr->deformationData, 0x200 * sizeof(int));

    RSDK.GetSceneLayer(2)->scrollPos = -0x1000000;
    RSDK.GetSceneLayer(3)->scrollPos = -0x1000000;
    if (!TMZ1Setup->paletteInit) {
#if RETRO_USE_PLUS
        if ((RSDK_sceneInfo->filter & FILTER_ENCORE)) {
            RSDK.LoadPalette(0, "EncoreTMZ1.act", 0xFF);
            RSDK.LoadPalette(1, "EncoreTMZ1d.act", 0xFF);
            RSDK.LoadPalette(2, "EncoreTMZ1l.act", 0xFF);
        }
#endif
        for (int i = 0; i < 256; ++i) RSDK.SetPaletteEntry(5, i, 0);
        RSDK.CopyPalette(0, 0, 4, 0, 255);
        TMZ1Setup->paletteInit = true;
    }
    RSDK.SetLimitedFade(3, 3, 4, 128, 0, 256);
    RSDK.SetLimitedFade(6, 0, 5, 192, 0, 256);
    Animals->animalTypes[0]     = ANIMAL_TOCKY;
    Animals->animalTypes[1]     = ANIMAL_PICKY;
    BGSwitch->switchCallback[0] = TMZ1Setup_BGCallback_A;
    BGSwitch->switchCallback[1] = TMZ1Setup_BGCallback_B;
    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder - 1, false, 0);
    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder, false, 0);
    if (isMainGameMode() && PlayerHelpers_CheckAct1())
        Zone->stageFinishCallback = TMZ1Setup_StageFinishCB;
    if (PlayerHelpers_CheckStageReload())
        TMZ1Setup->reloadFlag = true;
}

void TMZ1Setup_BGCallback_A(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(4)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    foreach_active(MonarchBG, monarch) { monarch->visible = true; }
    TMZ1Setup->stageState = 0;
}

void TMZ1Setup_BGCallback_B(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(4)->drawLayer[BGSwitch->screenID] = 0;
    foreach_active(MonarchBG, monarch) { monarch->visible = false; }
    TMZ1Setup->stageState = 2;
}

void TMZ1Setup_StageFinishCB(void)
{
    foreach_active(CrimsonEye, crimsonEye)
    {
        // if (crimsonEye->state == CrimsonEye_Unknown10) {
        //     crimsonEye->state = CrimsonEye_Unknown11;
        //     RSDK.CreateEntity(TMZ1Outro->objectID, NULL, crimsonEye->position.x, crimsonEye->position.y);
        // }
    }
}

void TMZ1Setup_StateUnknown2(void)
{
    RSDK_THIS(TMZ1Setup);
    if (entity->timer < 256) {
        entity->timer += 8;
        RSDK.SetLimitedFade(0, 4, 3, entity->timer, 128, 207);
    }
    else {
        destroyEntity(entity);
    }
}

void TMZ1Setup_StateUnknown3(void)
{
    RSDK_THIS(TMZ1Setup);
    if (++entity->timer >= 2) {
        if (RSDK_screens->position.y + RSDK_screens->centerY > 4192)
            entity->state = TMZ1Setup_StateUnknown5;
        else
            entity->state = TMZ1Setup_StateUnknown6;
    }
}

void TMZ1Setup_StateUnknown4(void)
{
    RSDK_THIS(TMZ1Setup);
    if (entity->timer >= 256) {
        if (TMZ1Setup->stageState) {
            TMZ1Setup->stageState               = 0;
            RSDK.GetSceneLayer(0)->drawLayer[0] = 0;
            RSDK.GetSceneLayer(1)->drawLayer[0] = 0;
            RSDK.GetSceneLayer(2)->drawLayer[0] = DRAWLAYER_COUNT;
            RSDK.GetSceneLayer(3)->drawLayer[0] = DRAWLAYER_COUNT;
        }
        else {
            TMZ1Setup->stageState               = 1;
            RSDK.GetSceneLayer(0)->drawLayer[0] = DRAWLAYER_COUNT;
            RSDK.GetSceneLayer(1)->drawLayer[0] = DRAWLAYER_COUNT;
            RSDK.GetSceneLayer(2)->drawLayer[0] = 0;
            RSDK.GetSceneLayer(3)->drawLayer[0] = 0;
        }
        entity->state = TMZ1Setup_StateUnknown7;
    }
    else {
        entity->timer += 8;
        if (TMZ1Setup->stageState)
            RSDK.SetLimitedFade(0, 3, 4, entity->timer >> 1, 128, 207);
        else
            RSDK.SetLimitedFade(0, 4, 3, entity->timer >> 1, 128, 207);
        RSDK.SetLimitedFade(0, 4, 5, entity->timer, 208, 256);
    }
}

void TMZ1Setup_StateUnknown5(void)
{
    RSDK_THIS(TMZ1Setup);
    TMZ1Setup->stageState               = 1;
    RSDK.GetSceneLayer(0)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[0] = 0;
    RSDK.GetSceneLayer(3)->drawLayer[0] = 0;
    RSDK.CopyPalette(3, 128, 0, 128, 80);
    destroyEntity(entity);
}

void TMZ1Setup_StateUnknown6(void)
{
    RSDK_THIS(TMZ1Setup);
    TMZ1Setup->stageState               = 0;
    RSDK.GetSceneLayer(0)->drawLayer[0] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[0] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.CopyPalette(4, 128, 0, 128, 80);
    destroyEntity(entity);
}

void TMZ1Setup_StateUnknown7(void)
{
    RSDK_THIS(TMZ1Setup);
    if (entity->timer <= 0) {
        if (TMZ1Setup->stageState)
            RSDK.CopyPalette(4, 208, 0, 208, 48);
        else
            RSDK.CopyPalette(4, 128, 0, 128, 128);
        destroyEntity(entity);
    }
    else {
        entity->timer -= 8;
        if (TMZ1Setup->stageState)
            RSDK.SetLimitedFade(0, 3, 4, entity->timer >> 1, 128, 207);
        else
            RSDK.SetLimitedFade(0, 4, 3, entity->timer >> 1, 128, 207);
        RSDK.SetLimitedFade(0, 4, 5, entity->timer, 208, 256);
    }
}

void TMZ1Setup_EditorDraw(void) {}

void TMZ1Setup_EditorLoad(void) {}

void TMZ1Setup_Serialize(void) {}

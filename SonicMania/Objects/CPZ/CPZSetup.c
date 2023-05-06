// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CPZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCPZSetup *CPZSetup;

void CPZSetup_Update(void) {}

void CPZSetup_LateUpdate(void) {}

void CPZSetup_StaticUpdate(void)
{
    // Animate the converyor belt thingys in the background
    if (!(Zone->timer & 3)) {
        --CPZSetup->aniTileFrame;
        CPZSetup->aniTileFrame &= 0xF;
        RSDK.DrawAniTiles(CPZSetup->aniTiles, 762, 109, CPZSetup->aniTileFrame + 48, 16, 16);
    }

    if (!(Zone->timer & 7)) {
        ++CPZSetup->background->deformationOffset;

        ++CPZSetup->bgTowerLightPalIndex;
        CPZSetup->bgTowerLightPalIndex &= 15;
        RSDK.SetPaletteEntry(0, 159, RSDK.GetPaletteEntry(3, CPZSetup->bgTowerLightPalIndex));

        ++CPZSetup->rainbowLightPalIndex;
        CPZSetup->rainbowLightPalIndex %= 21;
        RSDK.SetPaletteEntry(0, 158, RSDK.GetPaletteEntry(3, CPZSetup->rainbowLightPalIndex + 16));

        CPZSetup->chemLiquidPalIndex1 += 3;
        CPZSetup->chemLiquidPalIndex1 %= 27;
        RSDK.SetPaletteEntry(0, 141, RSDK.GetPaletteEntry(3, CPZSetup->chemLiquidPalIndex1 + 48));

        CPZSetup->chemLiquidPalIndex2 += 3;
        CPZSetup->chemLiquidPalIndex2 %= 27;
        RSDK.SetPaletteEntry(0, 142, RSDK.GetPaletteEntry(3, CPZSetup->chemLiquidPalIndex2 + 48));

        CPZSetup->chemLiquidPalIndex3 += 3;
        CPZSetup->chemLiquidPalIndex3 %= 27;
        RSDK.SetPaletteEntry(0, 143, RSDK.GetPaletteEntry(3, CPZSetup->chemLiquidPalIndex3 + 48));
    }
}

void CPZSetup_Draw(void) {}

void CPZSetup_Create(void *data) {}

void CPZSetup_StageLoad(void)
{
    CPZSetup->aniTiles = RSDK.LoadSpriteSheet("CPZ/Objects.gif", SCOPE_STAGE);

    RSDK.SetDrawGroupProperties(0, 0, Water_DrawHook_ApplyWaterPalette);
    RSDK.SetDrawGroupProperties(Zone->hudDrawGroup, 0, Water_DrawHook_RemoveWaterPalette);
    Water->waterPalette = 2;

    CPZSetup->chemLiquidPalIndex1 = 0;
    CPZSetup->chemLiquidPalIndex2 = 1;
    CPZSetup->chemLiquidPalIndex3 = 2;

    CPZSetup->background = RSDK.GetTileLayer(0);
    for (int32 i = 0; i < 0x400; ++i) {
        CPZSetup->background->deformationData[i] = CPZSetup->deformation[i & 0x3F];
    }

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreCPZ.act", 0b0000000011111111);
        RSDK.LoadPalette(2, "EncoreCPZw.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 1, 0, 10);
    }
#endif

    Animals->animalTypes[0] = ANIMAL_LOCKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;

    if (Zone->actID) {
        RSDK.SetPaletteMask(0x00F000);
        RSDK.SetPaletteEntry(0, 0xFF, 0x00F000);

        BGSwitch->switchCallback[CPZ_BG_CPZ2] = CPZSetup_BGSwitch_Act2BG;
        BGSwitch->switchCallback[CPZ_BG_CPZ1] = CPZSetup_BGSwitch_Act1BG;

        BGSwitch->layerIDs[0] = CPZ_BG_CPZ1;
        BGSwitch->layerIDs[1] = CPZ_BG_CPZ1;
        BGSwitch->layerIDs[2] = CPZ_BG_CPZ1;
        BGSwitch->layerIDs[3] = CPZ_BG_CPZ1;

        bool32 setCPZ1BG = false;
        if (!CutsceneRules_CheckStageReload() && CutsceneRules_CheckPlayerPos(TO_FIXED(172), TO_FIXED(6288), TO_FIXED(598), TO_FIXED(6528))) {
            setCPZ1BG = true;

            Zone->cameraBoundsB[0] = 6528;
            Zone->cameraBoundsB[1] = 6528;
            Zone->cameraBoundsB[2] = 6528;
            Zone->cameraBoundsB[3] = 6528;
        }

        if (isMainGameMode() && globals->atlEnabled) {
            if (!CutsceneRules_CheckStageReload())
                Zone_ReloadStoredEntities(TO_FIXED(384), TO_FIXED(6528), true);
        }

        if (isMainGameMode() && CutsceneRules_IsAct2()) {
            foreach_all(CPZ2Outro, outro)
            {
                CPZSetup->outro           = outro;
                Zone->stageFinishCallback = CPZSetup_StageFinish_EndAct2;
                foreach_break;
            }
        }

        BGSwitch->screenID = 0;
        if (setCPZ1BG) {
            for (; BGSwitch->screenID < RSDK.GetVideoSetting(VIDEOSETTING_SCREENCOUNT); BGSwitch->screenID++) CPZSetup_BGSwitch_Act1BG();

            BGSwitch->layerIDs[0] = CPZ_BG_CPZ1;
            BGSwitch->layerIDs[1] = CPZ_BG_CPZ1;
            BGSwitch->layerIDs[2] = CPZ_BG_CPZ1;
            BGSwitch->layerIDs[3] = CPZ_BG_CPZ1;

            TileLayer *backgroundAct1 = RSDK.GetTileLayer(3);

            backgroundAct1->scrollPos += -0x118000 * backgroundAct1->parallaxFactor;
            for (int32 i = 0; i < backgroundAct1->scrollInfoCount; ++i)
                backgroundAct1->scrollInfo[i].scrollPos += TO_FIXED(71) * backgroundAct1->scrollInfo[i].parallaxFactor;
        }
        else {
            for (; BGSwitch->screenID < RSDK.GetVideoSetting(VIDEOSETTING_SCREENCOUNT); BGSwitch->screenID++) CPZSetup_BGSwitch_Act2BG();

            BGSwitch->layerIDs[0] = CPZ_BG_CPZ2;
            BGSwitch->layerIDs[1] = CPZ_BG_CPZ2;
            BGSwitch->layerIDs[2] = CPZ_BG_CPZ2;
            BGSwitch->layerIDs[3] = CPZ_BG_CPZ2;
        }
    }
    else {
        if (isMainGameMode()) {
            if (globals->enableIntro) {
                if (!CutsceneRules_CheckStageReload()) {
                    globals->suppressAutoMusic = true;
                    globals->suppressTitlecard = true;
                }
            }
        }

        if (isMainGameMode()) {
            if (CutsceneRules_IsAct1())
                Zone->stageFinishCallback = CPZSetup_StageFinish_EndAct1;
        }

        RSDK.SetPaletteEntry(0, 0xFF, 0xF0F0F0);
        RSDK.SetPaletteEntry(2, 0xFF, RSDK.GetPaletteEntry(2, 137));
    }
}

void CPZSetup_BGSwitch_Act2BG(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(3)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
}

void CPZSetup_BGSwitch_Act1BG(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(3)->drawGroup[BGSwitch->screenID] = 0;
}

void CPZSetup_StageFinish_EndAct1(void)
{
    RSDK.GetTileLayer(0);
    Zone_StoreEntities(TO_FIXED(ScreenInfo->position.x + ScreenInfo->center.x), TO_FIXED(ScreenInfo->size.y + ScreenInfo->position.y));
    RSDK.LoadScene();
}

void CPZSetup_StageFinish_EndAct2(void) { CPZSetup->outro->active = ACTIVE_NORMAL; }

#if GAME_INCLUDE_EDITOR
void CPZSetup_EditorDraw(void) {}

void CPZSetup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(BGSwitch, bgID);
    RSDK_ENUM_VAR("Act 2 BG", CPZ_BG_CPZ2);
    RSDK_ENUM_VAR("Act 1 BG", CPZ_BG_CPZ1);

    RSDK_ACTIVE_VAR(Decoration, type);
    RSDK_ENUM_VAR("Warning Sign", CPZ_DECORATION_WARNSIGN);

    RSDK_ACTIVE_VAR(ParallaxSprite, aniID);
    RSDK_ENUM_VAR("Girder", CPZ_PARALLAXSPRITE_GIRDER);
    RSDK_ENUM_VAR("Chemical 1", CPZ_PARALLAXSPRITE_CHEMICAL1);
    RSDK_ENUM_VAR("Chemical 2", CPZ_PARALLAXSPRITE_CHEMICAL2);
}
#endif

void CPZSetup_Serialize(void) {}

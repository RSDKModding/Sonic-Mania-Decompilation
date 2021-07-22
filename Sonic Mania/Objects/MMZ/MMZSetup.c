#include "SonicMania.h"

ObjectMMZSetup *MMZSetup;

void MMZSetup_Update(void) {}

void MMZSetup_LateUpdate(void) {}

void MMZSetup_StaticUpdate(void)
{
    if (MMZSetup->paletteTimer == 4) {
        RSDK.RotatePalette(0, 140, 142, true);
        MMZSetup->paletteTimer = 0;
    }
    else {
        MMZSetup->paletteTimer++;
    }

    if (Zone->actID == 1) {
        if (!MMZSetup->aniTileDelay) {
            ++MMZSetup->aniTileFrameA;
            if (MMZSetup->aniTileFrameA > 11) {
                MMZSetup->aniTileFrameA = 0;
            }
            RSDK.DrawAniTiles(MMZSetup->aniTiles, 282, 0, 16 * MMZSetup->aniTileFrameA, 16, 16);
        }

        ++MMZSetup->aniTileDelay;
        if (MMZSetup->aniTileDelay == MMZSetup->aniTileDelay2 >> 1) {
            MMZSetup->aniTileDelay = 0;
            --MMZSetup->aniTileDelay2;
            if (MMZSetup->aniTileDelay2 < 4) {
                MMZSetup->aniTileDelay2 = 4;
            }
        }
    }

    --MMZSetup->aniTileDelay3;
    if (MMZSetup->aniTileDelay3 < 1) {
        MMZSetup->aniTileFrameB += 32;
        MMZSetup->aniTileFrameB %= 384;
        MMZSetup->aniTileDelay3 = MMZSetup->aniTileDelays[MMZSetup->aniTileFrameB >> 5];
        RSDK.DrawAniTiles(MMZSetup->aniTiles, 336, ((MMZSetup->aniTileFrameB >> 4) & 0xFFF0) + 96, MMZSetup->aniTileFrameB & 0xFF, 16, 32);
    }

    ++MMZSetup->aniTileDelay4;
    if (MMZSetup->aniTileDelay4 == 2) {
        MMZSetup->aniTileDelay4 = 0;
        MMZSetup->aniTileFrameC += 16;
        MMZSetup->aniTileFrameC %= 384;
        RSDK.DrawAniTiles(MMZSetup->aniTiles, 338, ((MMZSetup->aniTileFrameC >> 4) & 0xFFF0) + 16, MMZSetup->aniTileFrameC & 0xFF, 16, 16);
        MMZSetup->aniTileFrameD += 32;
        MMZSetup->aniTileFrameD &= 0xFF;
        RSDK.DrawAniTiles(MMZSetup->aniTiles, 330, 48, MMZSetup->aniTileFrameD, 16, 32);
        RSDK.DrawAniTiles(MMZSetup->aniTiles, 332, 64, MMZSetup->aniTileFrameD, 32, 32);
    }
}

void MMZSetup_Draw(void) {}

void MMZSetup_Create(void *data) {}

void MMZSetup_StageLoad(void)
{
    MMZSetup->aniTiles      = RSDK.LoadSpriteSheet("MMZ/AniTiles.gif", SCOPE_STAGE);
    Animals->animalTypes[0] = ANIMAL_CUCKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;

    if (Zone->actID) {
        RSDK.GetSceneLayer(0)->scrollPos = 0x1800000;
        RSDK.GetSceneLayer(1)->scrollPos = 0x1800000;

        for (int i = 3; i < 5; ++i) {
            RSDK.GetSceneLayer(i)->scrollPos = (256 - RSDK_screens->centerX) << 16;
        }

        MMZSetup->aniTileDelay2 = 16;
        if (!PlayerHelpers_CheckStageReload() && PlayerHelpers_CheckPlayerPos(0x73C0000, 0x2C0000, 0x1040000, 0x82C0000)) {
            Zone->screenBoundsB1[0] = 2092;
            Zone->screenBoundsB1[1] = 2092;
#if RETRO_USE_PLUS
            Zone->screenBoundsB1[2] = 2092;
            Zone->screenBoundsB1[3] = 2092;
#endif
        }

        if (isMainGameMode() && globals->atlEnabled) {
            if (!PlayerHelpers_CheckStageReload())
                Zone_ReloadStoredEntities(0x82C0000, 0x1000000, true);
        }

#if RETRO_USE_PLUS
        if (isMainGameMode() && PlayerHelpers_CheckAct2())
            Zone->stageFinishCallback = MMZSetup_StageFinishCB_Act2;
#endif
    }
    else {
        if (isMainGameMode() && PlayerHelpers_CheckAct1()) {
            Zone->forcePlayerOnScreenFlag = true;
            Zone->stageFinishCallback     = MMZSetup_StageFinishCB_Act1;
        }
        Zone->screenBoundsB1[0] = 5120;
        Zone->screenBoundsB1[1] = 5120;
#if RETRO_USE_PLUS
        Zone->screenBoundsB1[2] = 5120;
        Zone->screenBoundsB1[3] = 5120;
#endif
    }

#if RETRO_USE_PLUS
    if ((RSDK_sceneInfo->filter & FILTER_ENCORE)) {
        RSDK.LoadPalette(0, "EncoreMMZ.act", 0xFF);
        RSDK.LoadPalette(3, "EncoreMMZfp.act", 0xFF);
        RSDK.LoadPalette(4, "EncoreMMZf.act", 0xFF);
    }
#endif
}

void MMZSetup_StageFinishCB_Act1(void)
{
    Zone_StoreEntities((Zone->screenBoundsL1[0] + RSDK_screens->centerX) << 16, Zone->screenBoundsB1[0] << 16);
    RSDK.InitSceneLoad();
}
#if RETRO_USE_PLUS
void MMZSetup_StageFinishCB_Act2(void) { RSDK.CreateEntity(MMZ2Outro->objectID, NULL, 0, 0); }
#endif

void MMZSetup_EditorDraw(void) {}

void MMZSetup_EditorLoad(void) {}

void MMZSetup_Serialize(void) {}

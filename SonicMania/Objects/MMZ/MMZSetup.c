// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MMZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        if (!MMZSetup->panelAniTimer) {
            if (++MMZSetup->panelAniFrame > 11)
                MMZSetup->panelAniFrame = 0;

            RSDK.DrawAniTiles(MMZSetup->aniTiles, 282, 0, 16 * MMZSetup->panelAniFrame, 16, 16);
        }

        if (++MMZSetup->panelAniTimer == (MMZSetup->panelAniDuration >> 1)) {
            MMZSetup->panelAniTimer = 0;

            if (--MMZSetup->panelAniDuration < 4)
                MMZSetup->panelAniDuration = 4;
        }
    }

    if (--MMZSetup->pistonAniDuration < 1) {
        MMZSetup->pistonAniFrame += 32;
        MMZSetup->pistonAniFrame %= 384;

        MMZSetup->pistonAniDuration = MMZSetup->pistonAniDurationTable[MMZSetup->pistonAniFrame >> 5];
        RSDK.DrawAniTiles(MMZSetup->aniTiles, 336, 96 + ((MMZSetup->pistonAniFrame >> 4) & 0xFFF0), MMZSetup->pistonAniFrame & 0xFF, 16, 32);
    }

    if (++MMZSetup->lightsAniDuration == 2) {
        MMZSetup->lightsAniDuration = 0;

        MMZSetup->spinLightsAniFrame += 16;
        MMZSetup->spinLightsAniFrame %= 384;
        RSDK.DrawAniTiles(MMZSetup->aniTiles, 338, 16 + ((MMZSetup->spinLightsAniFrame >> 4) & 0xFFF0), MMZSetup->spinLightsAniFrame & 0xFF, 16, 16);

        MMZSetup->generatorAniFrame += 32;
        MMZSetup->generatorAniFrame &= 0xFF;
        RSDK.DrawAniTiles(MMZSetup->aniTiles, 330, 48, MMZSetup->generatorAniFrame, 16, 32);
        RSDK.DrawAniTiles(MMZSetup->aniTiles, 332, 64, MMZSetup->generatorAniFrame, 32, 32);
    }
}

void MMZSetup_Draw(void) {}

void MMZSetup_Create(void *data) {}

void MMZSetup_StageLoad(void)
{
    MMZSetup->aniTiles = RSDK.LoadSpriteSheet("MMZ/AniTiles.gif", SCOPE_STAGE);

    Animals->animalTypes[0] = ANIMAL_CUCKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;

    if (Zone->actID) {
        RSDK.GetTileLayer(0)->scrollPos = 384 << 16;
        RSDK.GetTileLayer(1)->scrollPos = 384 << 16;

#if MANIA_USE_PLUS
        for (int32 l = 3; l < 5; ++l) RSDK.GetTileLayer(l)->scrollInfo[0].scrollPos = (0x100 - ScreenInfo->center.x) << 16;
#endif

        MMZSetup->panelAniDuration = 16;
        if (!CutsceneRules_CheckStageReload() && CutsceneRules_CheckPlayerPos(TO_FIXED(44), TO_FIXED(1852), TO_FIXED(260), TO_FIXED(2092))) {
            Zone->cameraBoundsB[0] = 2092;
            Zone->cameraBoundsB[1] = 2092;
            Zone->cameraBoundsB[2] = 2092;
            Zone->cameraBoundsB[3] = 2092;
        }

        if (isMainGameMode() && globals->atlEnabled) {
            if (!CutsceneRules_CheckStageReload())
                Zone_ReloadStoredEntities(TO_FIXED(256), TO_FIXED(2092), true);
        }

#if MANIA_USE_PLUS
        if (isMainGameMode() && CutsceneRules_IsAct2())
            Zone->stageFinishCallback = MMZSetup_StageFinish_EndAct2;
#endif
    }
    else {
        if (isMainGameMode() && CutsceneRules_IsAct1()) {
            Zone->shouldRecoverPlayers = true;
            Zone->stageFinishCallback  = MMZSetup_StageFinish_EndAct1;
        }

        Zone->cameraBoundsB[0] = 5120;
        Zone->cameraBoundsB[1] = 5120;
        Zone->cameraBoundsB[2] = 5120;
        Zone->cameraBoundsB[3] = 5120;
    }

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreMMZ.act", 0b0000000011111111);
        RSDK.LoadPalette(3, "EncoreMMZfp.act", 0b0000000011111111);
        RSDK.LoadPalette(4, "EncoreMMZf.act", 0b0000000011111111);
    }
#endif
}

void MMZSetup_StageFinish_EndAct1(void)
{
    Zone_StoreEntities((Zone->cameraBoundsL[0] + ScreenInfo->center.x) << 16, Zone->cameraBoundsB[0] << 16);
    RSDK.LoadScene();
}

#if MANIA_USE_PLUS
void MMZSetup_StageFinish_EndAct2(void) { CREATE_ENTITY(MMZ2Outro, NULL, 0, 0); }
#endif

#if GAME_INCLUDE_EDITOR
void MMZSetup_EditorDraw(void) {}

void MMZSetup_EditorLoad(void) {}
#endif

void MMZSetup_Serialize(void) {}

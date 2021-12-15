// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ3Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectLRZ3Setup *LRZ3Setup;

void LRZ3Setup_Update(void) {}

void LRZ3Setup_LateUpdate(void) {}

void LRZ3Setup_StaticUpdate(void) {}

void LRZ3Setup_Draw(void) {}

void LRZ3Setup_Create(void *data) {}

void LRZ3Setup_StageLoad(void)
{
#if RETRO_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreLRZ3.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 240, 1, 240, 16);
    }
#endif
    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_CUCKY;
    if (globals->suppressTitlecard) {
        SaveGame_LoadPlayerState();
        Zone_StartFadeIn(10, 0x000000);
    }

    if (isMainGameMode() && globals->enableIntro && (!PlayerHelpers_CheckStageReload()
        || !PlayerHelpers_CheckStageReload())) {
        CREATE_ENTITY(LRZ3Cutscene, NULL, 0, 0);
    }

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE && globals->tempFlags) {
        if (!PlayerHelpers_CheckStageReload()) {
            foreach_all(Player, player) { player->position.y += 0x8000000; }
        }
    }
#endif

    if (isMainGameMode() && PlayerHelpers_CheckAct2()) {
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE) {
            if (!globals->tempFlags) {
                Zone->stageFinishCallback = LRZ3Outro_StageFinishCB;
            }
        }
        else {
            if (!checkPlayerID(ID_KNUCKLES, 1)) {
                Zone->stageFinishCallback = LRZ3Outro_StageFinishCB;
            }
            else {
                LRZ3Setup->cutscenePtr = CutsceneSeq_GetEntity(LRZ3OutroK->objectID);
                if (LRZ3Setup->cutscenePtr)
                    Zone->stageFinishCallback = LRZ3Setup_StartCutscene;
            }
        }
#else
        if (checkPlayerID(ID_KNUCKLES, 1)) 
            LRZ3Setup->cutscenePtr = CutsceneSeq_GetEntity(LRZ3OutroK->objectID);
        if (LRZ3Setup->cutscenePtr)
            Zone->stageFinishCallback = LRZ3Setup_StartCutscene;
#endif
    }
}

void LRZ3Setup_StartCutscene(void) { LRZ3Setup->cutscenePtr->active = ACTIVE_NORMAL; }

void LRZ3Setup_EditorDraw(void) {}

void LRZ3Setup_EditorLoad(void) {}

void LRZ3Setup_Serialize(void) {}

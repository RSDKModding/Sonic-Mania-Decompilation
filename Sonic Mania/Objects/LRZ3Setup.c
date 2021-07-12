#include "../SonicMania.h"

ObjectLRZ3Setup *LRZ3Setup;

void LRZ3Setup_Update(void) {}

void LRZ3Setup_LateUpdate(void) {}

void LRZ3Setup_StaticUpdate(void) {}

void LRZ3Setup_Draw(void) {}

void LRZ3Setup_Create(void *data) {}

void LRZ3Setup_StageLoad(void)
{
#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreLRZ3.act", 255);
        RSDK.CopyPalette(0, 240, 1, 240, 16);
    }
#endif
    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_CUCKY;
    if (globals->suppressTitlecard) {
        SaveGame_LoadPlayerState();
        EntityZone *entZone = (EntityZone *)RSDK.CreateEntity(Zone->objectID, NULL, 0, 0);
        entZone->screenID   = SCREEN_MAX;
        entZone->timer      = 640;
        entZone->fadeSpeed  = 10;
        entZone->fadeColour = 0x000000;
        entZone->state      = Zone_State_Fadeout_Restart;
        entZone->stateDraw  = Zone_StateDraw_Fadeout;
        entZone->visible    = true;
        entZone->drawOrder  = DRAWLAYER_COUNT - 1;
    }
    if ((globals->gameMode == MODE_MANIA || globals->gameMode == MODE_ENCORE) && globals->enableIntro && !PlayerHelpers_CheckStageReload()
        || !PlayerHelpers_CheckStageReload()) {
        RSDK.CreateEntity(LRZ3Cutscene->objectID, NULL, 0, 0);
    }

    if (globals->gameMode == MODE_ENCORE && globals->tempFlags) {
        if (!PlayerHelpers_CheckStageReload()) {
            foreach_all(Player, player) { player->position.y += 0x8000000; }
        }
    }

    if ((globals->gameMode == MODE_MANIA || globals->gameMode == MODE_ENCORE) && PlayerHelpers_CheckAct2()) {
        if (globals->gameMode == MODE_ENCORE) {
            if (!globals->tempFlags) {
                // Zone->stageFinishCallback = LRZ3Outro_Unknown5;
            }
        }
        else {
            if (!(globals->playerID & ID_KNUCKLES)) {
                // Zone->stageFinishCallback = LRZ3Outro_Unknown5;
            }
            else {
                LRZ3Setup->cutscenePtr = CutsceneSeq_GetEntity(LRZ3OutroK->objectID);
                if (LRZ3Setup->cutscenePtr)
                    Zone->stageFinishCallback = LRZ3Setup_StartCutscene;
            }
        }
    }
}

void LRZ3Setup_StartCutscene(void) { LRZ3Setup->cutscenePtr->active = ACTIVE_NORMAL; }

void LRZ3Setup_EditorDraw(void) {}

void LRZ3Setup_EditorLoad(void) {}

void LRZ3Setup_Serialize(void) {}

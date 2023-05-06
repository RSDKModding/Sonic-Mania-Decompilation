// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZ1Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSSZ1Setup *SSZ1Setup;

void SSZ1Setup_Update(void) {}

void SSZ1Setup_LateUpdate(void) {}

void SSZ1Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 1))
        ++SSZ1Setup->background2->deformationOffset;
}

void SSZ1Setup_Draw(void) {}

void SSZ1Setup_Create(void *data) {}

void SSZ1Setup_StageLoad(void)
{
    // The low lake style bg
    SSZ1Setup->background2 = RSDK.GetTileLayer(1);

    int32 id = 0;
    for (int32 i = 0; i < 0x200; ++i) {
        SSZ1Setup->background2->deformationData[i] = (8 * RSDK.Sin1024(id)) >> 10;
        id += 8;
    }
    memcpy(&SSZ1Setup->background2->deformationData[0x200], &SSZ1Setup->background2->deformationData[0], 0x200 * sizeof(int32));

    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_RICKY;

    GenericTrigger->callbacks[GENERICTRIGGER_SSZ1_DESTROYHOTARUMKII] = SSZ1Outro_Trigger_DestroyHotarus;

    if (isMainGameMode()) {
        if (globals->enableIntro) {
            if (!CutsceneRules_CheckStageReload()) {
                globals->suppressAutoMusic = true;
                globals->suppressTitlecard = true;
            }
        }
    }

    if (isMainGameMode() && CutsceneRules_IsAct1()) {
        foreach_all(SSZ1Outro, outro)
        {
            SSZ1Setup->outro = outro;
            foreach_break;
        }
        Zone->stageFinishCallback = SSZ1Setup_StageFinish_EndAct1;
    }

#if MANIA_USE_PLUS
    if ((SceneInfo->filter & FILTER_ENCORE)) {
        RSDK.LoadPalette(0, "EncoreSSZ1.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 1, 128, 128);
    }
#endif
}

void SSZ1Setup_StageFinish_EndAct1(void) { SSZ1Setup->outro->active = ACTIVE_NORMAL; }

#if GAME_INCLUDE_EDITOR
void SSZ1Setup_EditorDraw(void) {}

void SSZ1Setup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Destroy Active HotaruMKIIs", GENERICTRIGGER_SSZ1_DESTROYHOTARUMKII);

    RSDK_ACTIVE_VAR(Decoration, type);
    RSDK_ENUM_VAR("GoalPost", SSZ1_DECOR_GOALPOST);
}
#endif

void SSZ1Setup_Serialize(void) {}

// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZ1Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSSZ1Setup *SSZ1Setup;

void SSZ1Setup_Update(void) {}

void SSZ1Setup_LateUpdate(void) {}

void SSZ1Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 1))
        ++SSZ1Setup->layer1Ptr->deformationOffset;
}

void SSZ1Setup_Draw(void) {}

void SSZ1Setup_Create(void *data) {}

void SSZ1Setup_StageLoad(void)
{
    SSZ1Setup->layer1Ptr = RSDK.GetSceneLayer(1);
    int32 id               = 0;
    for (int32 i = 0; i < 0x200; ++i) {
        SSZ1Setup->layer1Ptr->deformationData[i] = (8 * RSDK.Sin1024(id)) >> 10;
        id += 8;
    }
    memcpy(SSZ1Setup->layer1Ptr->deformationData + 0x200, SSZ1Setup->layer1Ptr->deformationData, 0x200 * sizeof(int32));

    Animals->animalTypes[0]      = ANIMAL_FLICKY;
    Animals->animalTypes[1]      = ANIMAL_RICKY;
    GenericTrigger->callbacks[0] = SSZ1Outro_Unknown4;

    if (isMainGameMode()) {
        if (globals->enableIntro) {
            if (!PlayerHelpers_CheckStageReload()) {
                globals->suppressAutoMusic = true;
                globals->suppressTitlecard = true;
            }
        }
    }

    if (isMainGameMode() && PlayerHelpers_CheckAct1()) {
        foreach_all(SSZ1Outro, outro)
        {
            SSZ1Setup->outroPtr = outro;
            foreach_break;
        }
        Zone->stageFinishCallback = SSZ1Setup_StageFinishCB;
    }

#if RETRO_USE_PLUS
    if ((SceneInfo->filter & FILTER_ENCORE)) {
        RSDK.LoadPalette(0, "EncoreSSZ1.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 1, 128, 128);
    }
#endif
}

void SSZ1Setup_StageFinishCB(void) { SSZ1Setup->outroPtr->active = ACTIVE_NORMAL; }

void SSZ1Setup_EditorDraw(void) {}

void SSZ1Setup_EditorLoad(void) {}

void SSZ1Setup_Serialize(void) {}

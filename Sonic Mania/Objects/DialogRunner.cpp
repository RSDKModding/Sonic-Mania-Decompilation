#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
ObjectDialogRunner *DialogRunner;

void DialogRunner_Update() {}

void DialogRunner_LateUpdate() {}

void DialogRunner_StaticUpdate() {}

void DialogRunner_Draw() {}

void DialogRunner_Create(void *data)
{
    EntityDialogRunner *entity = (EntityDialogRunner *)RSDK_sceneInfo->entity;
    entity->active             = ACTIVE_ALWAYS;
    entity->visible            = false;
    entity->state              = (void (*)())data;
    entity->timer              = 0;
    entity->field_88           = 0;
}

void DialogRunner_StageLoad()
{
    DialogRunner->field_4  = 0;
    DialogRunner->field_8  = 0;
    DialogRunner->field_10 = 0;
    DialogRunner->field_C  = 0;
    DialogRunner->field_14 = 0;
    if (!options->hasPlusInitial) {
        options->lastHasPlus    = User.CheckDLC(DLC_PLUS);
        options->hasPlusInitial = true;
    }
    DialogRunner->entityPtr = NULL;
    SaveGame_LoadSaveData();
    // TimeAttackData->base                  = 0;
    TimeAttackData->field_C  = 0;
    TimeAttackData->field_10 = -1;
    TimeAttackData->field_14 = 0;
    TimeAttackData->field_18 = 0;
    TimeAttackData->dword1C  = 0;
    Options->state           = 0;
    if (RSDK_sku->platform && RSDK_sku->platform != PLATFORM_DEV) {
        options->optionsRAM[29] = 0;
        options->optionsRAM[32] = 0;
        options->optionsRAM[31] = 0;
        options->optionsRAM[33] = 0;
    }
    else {
        Options_Reload();
    }
}

void DialogRunner_HandleCallback()
{
    EntityDialogRunner *entity = (EntityDialogRunner *)RSDK_sceneInfo->entity;
    if (entity->timer <= 0) {
        Game_Print("Callback: %x", entity->callback);
        if (entity->callback)
            entity->callback();
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer--;
    }
}

void DialogRunner_EditorDraw() {}

void DialogRunner_EditorLoad() {}

void DialogRunner_Serialize() {}
#endif
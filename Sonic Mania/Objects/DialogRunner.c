#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectDialogRunner *DialogRunner;

void DialogRunner_Update(void)
{
    RSDK_THIS(DialogRunner);
    if (entity->state)
        entity->state();
}

void DialogRunner_LateUpdate(void) {}

void DialogRunner_StaticUpdate(void) {}

void DialogRunner_Draw(void) {}

void DialogRunner_Create(void *data)
{
    RSDK_THIS(DialogRunner);
    entity->active   = ACTIVE_ALWAYS;
    entity->visible  = false;
    entity->state    = (void (*)(void))data;
    entity->timer    = 0;
    entity->field_88 = 0;
}

void DialogRunner_StageLoad(void)
{
    DialogRunner->field_4  = 0;
    DialogRunner->field_8  = 0;
    DialogRunner->field_10 = 0;
    DialogRunner->field_C  = 0;
    DialogRunner->field_14 = 0;
    if (!globals->hasPlusInitial) {
        globals->lastHasPlus    = User.CheckDLC(DLC_PLUS);
        globals->hasPlusInitial = true;
    }
    DialogRunner->entityPtr = NULL;
    SaveGame_LoadSaveData();
    TimeAttackData->status   = 0;
    TimeAttackData->uuid     = 0;
    TimeAttackData->unknown  = -1;
    TimeAttackData->field_14 = 0;
    TimeAttackData->rank     = 0;
    TimeAttackData->dword1C  = 0;
    Options->state           = 0;
    if (RSDK_sku->platform && RSDK_sku->platform != PLATFORM_DEV) {
        globals->optionsRAM[29] = 0;
        globals->optionsRAM[32] = 0;
        globals->optionsRAM[31] = 0;
        globals->optionsRAM[33] = 0;
    }
    else {
        Options_Reload();
    }
}

void DialogRunner_HandleCallback(void)
{
    RSDK_THIS(DialogRunner);
    if (entity->timer <= 0) {
        LogHelpers_Print("Callback: %x", entity->callback);
        if (entity->callback)
            entity->callback();
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer--;
    }
}

void DialogRunner_NotifyAutoSave_CB(void)
{
    DialogRunner->field_14    = 0;
    globals->notifiedAutosave = true;
    UIWaitSpinner_WaitReplay();
}

void DialogRunner_NotifyAutoSave(void)
{
    TextInfo info;

    RSDK_THIS(DialogRunner);
    if (DialogRunner->field_14) {
        /*if (!UIDialog->activeDialog) {
            Localization_GetString(&info, STR_AUTOSAVENOTIF);
            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
            dialog->field_B8       = 1;
            UIDialog_Unknown2(2, dialog, DialogRunner_NotifyAutoSave_CB, 1);
            UIDialog_Unknown3(dialog);
        }*/
    }
    else {
        DialogRunner->entityPtr = 0;
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
}

void DialogRunner_Unknown4(void)
{
    User.UserStorageStatusUnknown4();
    User.SetUserStorageNoSave(0);
}

void DialogRunner_Unknown5(void)
{
    User.UserStorageStatusUnknown5();
    User.SetUserStorageNoSave(1);
}

void DialogRunner_Unknown6(void)
{
    TextInfo info;

    RSDK_THIS(DialogRunner);
    if (User.UserStorageStatusUnknown2() == STATUS_CONTINUE) {
        /*if (!UIDialog->activeDialog) {
            int stringID = 30;
            switch (entity->unknownID) {
                case 500:
                    stringID = STR_NOXBOXPROFILE;
                    if (RSDK_sku->platform != PLATFORM_XB1)
                        stringID = STR_SAVELOADFAIL;
                    break;
                case 505: stringID = STR_CORRUPTSAVE; break;
                case 506: stringID = (RSDK_sku->platform == PLATFORM_XB1) + STR_NOSAVESPACE; break;
            }
            Localization_GetString(&info, stringID);
            //EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
            //dialog->field_B8       = 1;
            //UIDialog_Unknown2(1u, dialog, DialogRunner_Unknown4, 1);
            //UIDialog_Unknown2(0, dialog, DialogRunner_Unknown5, 1);
            //UIDialog_Unknown3(dialog);
        }*/
    }
    else {
        DialogRunner->entityPtr = NULL;
        RSDK.ResetEntityPtr(&entity, 0, 0);
    }
}
void DialogRunner_Unknown7(int a1, int a2)
{
    RSDK_THIS(DialogRunner);
    if (entity->timer) {
        if (DialogRunner->field_8) {
            /*if (!UIDialog->activeDialog) {
                if (Zone && Zone_GetZoneID() != -1) {
                    RSDK.LoadScene("Presentation", "Title Screen");
                    Zone_StartFadeOut(10, 0);
                }
                else if (MenuSetup) {
                    //MenuSetup_Unknown11(a1, a2);
                }
                else if (UFO_Setup) {
                    //UFO_Setup->dword4C = 1;
                    foreach_all(UFO_Setup, setup)
                    {
                        //setup->dword60 = 0;
                        //setup->state   = UFO_Setup_Unknown12;
                        setup->active  = ACTIVE_ALWAYS;
                        setup->visible = true;
                    }
                }
                else if (FXFade) {
                    RSDK.LoadScene("Presentation", "Title Screen");
                    EntityFXFade *fxFade    = (EntityFXFade *)RSDK.CreateEntity(FXFade->objectID, 0, v2->position.x, v2->position.y);
                    fxFade->active     = ACTIVE_ALWAYS;
                    fxFade->timer           = 0;
                    fxFade->speedIn         = 16;
                    fxFade->speedOut        = 16;
                    fxFade->state           = FXFade_State_FadeIn;
                    fxFade->drawOrder  = 15;
                    fxFade->field_3C   = 1;
                    fxFade->oneWay          = true;
                    fxFade->transitionScene = true;
                }
                DialogRunner->entityPtr = NULL;
                RSDK.ResetEntityPtr(entity, 0, 0);
            }*/
        }
        /*else if (!UIDialog->activeDialog) {
            TextInfo info;
            int id = STR_SIGNOUTDETECTED;
            if (entity->field_88)
                id = STR_RETRURNINGTOTITLE;
            Localization_GetString(&info, id);
            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
            dialog->field_B8       = 1;
            UIDialog_Unknown2(2, dialog, DialogRunner_Unknown14, 1);
            UIDialog_Unknown3(dialog);
        }*/
    }
    else {
        /*EntityUIDialog *dialog = UIDialog->activeDialog;
        if (dialog) {
            if (dialog->state != UIDialog_Unknown13) {
                dialog->uiControl->gapC8 = 1;
                dialog->field_5C         = 0;
                dialog->state            = UIDialog_Unknown13;
                dialog->field_B0         = 0;
            }
        }
        else {
            if (UIControl) {
                if (UIControl_Unknown8())
                    UIControl_Unknown6(UIControl_Unknown8());
            }
            RSDK.SetGameMode(ENGINESTATE_FROZEN);
            RSDK.StopChannel(Music->slotID);
            dialog->timer = 1;
        }*/
    }
}
void DialogRunner_ManageNotifs(int a1)
{
    RSDK_THIS(DialogRunner);
    if (SaveGame_CountUnreadNotifs()) {
        /*TextInfo info;
        if (!UIDialog->activeDialog) {
            int str = SaveGame_GetNotifStringID(SaveGame_GetNextNotif());
            Localization_GetString(&info, str);
            EntityUIDialog *dialog           = UIDialog_CreateActiveDialog(&info);
            dialog->field_B4       = 1;
            dialog->field_B8                 = 1;
            UIDialog_Unknown2(2, dialog, DialogRunner_GetNextNotif, 1);
            UIDialog_Unknown3(v4);
        }*/
    }
    else {
        DialogRunner->entityPtr = NULL;
        UIWaitSpinner_Wait();
        SaveGame_TrackGameProgress(DialogRunner_Wait);
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}
void DialogRunner_Wait(int success)
{
    UIWaitSpinner_WaitReplay();
}
void DialogRunner_GetNextNotif(void)
{
    int *saveRAM = NULL;
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageNoSave() || globals->saveLoaded != STATUS_OK)
        saveRAM = NULL;
    else
        saveRAM = &globals->saveRAM[0x900];
    int id             = SaveGame_GetNextNotif();
    saveRAM[id + 0x35] = true;
}
bool32 DialogRunner_CheckUnreadNotifs(void)
{
    if (!SaveGame_CountUnreadNotifs())
        return false;
    if (!DialogRunner->entityPtr)
        DialogRunner->entityPtr = RSDK.CreateEntity(DialogRunner->objectID, DialogRunner_ManageNotifs, 0, 0);
    return true;
}
bool32 DialogRunner_NotifyAutosave(void)
{
    if (!DialogRunner->field_14 && !DialogRunner->entityPtr) {
        return false;
    }

    if (!DialogRunner->entityPtr || (!globals->notifiedAutosave && !DialogRunner->field_14)) {
        UIWaitSpinner_Wait();
        DialogRunner->field_14    = true;
        globals->notifiedAutosave = false;
        LogHelpers_Print("DUMMY NotifyAutosave()");
        EntityDialogRunner *dialogRunner = (EntityDialogRunner *)RSDK.CreateEntity(DialogRunner->objectID, DialogRunner_NotifyAutoSave, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        DialogRunner->entityPtr          = dialogRunner;
    }
    return true;
}
void DialogRunner_GetUserAuthStatus(void)
{
    if (User.GetUserAuthStatus() == STATUS_FORBIDDEN) {
        if (DialogRunner->field_4)
            return;
        EntityDialogRunner *dialogRunner = (EntityDialogRunner *)RSDK.CreateEntity(DialogRunner->objectID, DialogRunner_Unknown7, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        DialogRunner->entityPtr          = dialogRunner;
        DialogRunner->field_4            = 1;
    }

    if (User.CheckDLC(DLC_PLUS) != globals->lastHasPlus && !DialogRunner->field_4) {
        EntityDialogRunner *dialogRunner = (EntityDialogRunner *)RSDK.CreateEntity(DialogRunner->objectID, DialogRunner_Unknown7, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        dialogRunner->field_88           = 1;
        DialogRunner->entityPtr          = dialogRunner;
        DialogRunner->field_4            = 1;
        globals->lastHasPlus             = User.CheckDLC(DLC_PLUS);
    }
}
void DialogRunner_PromptSavePreference(int id)
{
    if (User.GetUserStorageNoSave()) {
        LogHelpers_Print("PromptSavePreference() returning due to noSave");
        return;
    }
    LogHelpers_Print("PromptSavePreference()");
    if (User.UserStorageStatusUnknown2() == STATUS_CONTINUE) {
        LogHelpers_Print("WARNING PromptSavePreference() when prompt already in progress.");
    }
    User.SetUserStorageStatus();
    EntityDialogRunner *dialogRunner = (EntityDialogRunner *)RSDK.CreateEntity(DialogRunner->objectID, DialogRunner_Unknown6, 0, 0);
    dialogRunner->unknownID          = id;
    DialogRunner->entityPtr          = dialogRunner;
}
void DialogRunner_Unknown14(void) { DialogRunner->field_8 = 1; }

void DialogRunner_EditorDraw(void) {}

void DialogRunner_EditorLoad(void) {}

void DialogRunner_Serialize(void) {}
#endif

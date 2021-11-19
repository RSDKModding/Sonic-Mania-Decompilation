#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectDialogRunner *DialogRunner;

void DialogRunner_Update(void)
{
    RSDK_THIS(DialogRunner);
    StateMachine_Run(self->state);
}

void DialogRunner_LateUpdate(void) {}

void DialogRunner_StaticUpdate(void) {}

void DialogRunner_Draw(void) {}

void DialogRunner_Create(void *data)
{
    RSDK_THIS(DialogRunner);
    self->active         = ACTIVE_ALWAYS;
    self->visible        = false;
    self->state          = (Type_StateMachine)data;
    self->timer          = 0;
    self->useGenericText = false;
}

void DialogRunner_StageLoad(void)
{
    DialogRunner->authForbiddenFlag = false;
    DialogRunner->signoutFlag       = false;
    DialogRunner->unused2           = 0;
    DialogRunner->unused1           = 0;
    DialogRunner->isAutoSaving      = 0;
    if (!globals->hasPlusInitial) {
        globals->lastHasPlus    = API.CheckDLC(DLC_PLUS);
        globals->hasPlusInitial = true;
    }
    DialogRunner->entityPtr = NULL;
    SaveGame_LoadSaveData();
    TimeAttackData->status  = 0;
    TimeAttackData->uuid    = 0;
    TimeAttackData->rowID   = -1;
    TimeAttackData->dbRank  = 0;
    TimeAttackData->rank    = 0;
    TimeAttackData->dword1C = 0;
    Options->state          = 0;
    if (sku_platform && sku_platform != PLATFORM_DEV) {
        EntityOptions *options   = (EntityOptions *)globals->optionsRAM;
        options->vSync           = false;
        options->windowed        = false;
        options->windowBorder    = false;
        options->tripleBuffering = false;
    }
    else {
        Options_Reload();
    }
}

void DialogRunner_HandleCallback(void)
{
    RSDK_THIS(DialogRunner);
    if (self->timer <= 0) {
        LogHelpers_Print("Callback: %x", self->callback);
        StateMachine_Run(self->callback);
        destroyEntity(self);
    }
    else {
        self->timer--;
    }
}

void DialogRunner_NotifyAutoSave_CB(void)
{
    DialogRunner->isAutoSaving = false;
    globals->notifiedAutosave  = true;
    UIWaitSpinner_Wait2();
}

void DialogRunner_NotifyAutoSave(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    RSDK_THIS(DialogRunner);
    if (DialogRunner->isAutoSaving) {
        if (!UIDialog->activeDialog) {
            Localization_GetString(&info, STR_AUTOSAVENOTIF);
            EntityUIDialog *dialog = UIDialog_CreateDialogOk(&info, DialogRunner_NotifyAutoSave_CB, true);
            dialog->field_B8       = 1;
        }
    }
    else {
        DialogRunner->entityPtr = NULL;
        destroyEntity(self);
    }
}

void DialogRunner_SetNoSaveDisabled(void)
{
    API.SetSaveStatusForbidden();
    API.SetUserStorageNoSave(false);
}

void DialogRunner_SetNoSaveEnabled(void)
{
    API.SetSaveStatusError();
    API.SetUserStorageNoSave(true);
}

void DialogRunner_PromptSavePreference_CB(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    RSDK_THIS(DialogRunner);
    if (API.GetSaveStatus() == STATUS_CONTINUE) {
        if (!UIDialog->activeDialog) {
            int32 stringID = STR_SAVELOADFAIL;
            switch (self->status) {
                case STATUS_ERROR:
                    stringID = STR_NOXBOXPROFILE;
                    if (sku_platform != PLATFORM_XB1)
                        stringID = STR_SAVELOADFAIL;
                    break;
                case STATUS_CORRUPT: stringID = STR_CORRUPTSAVE; break;
                case STATUS_NOSPACE: stringID = (sku_platform == PLATFORM_XB1) + STR_NOSAVESPACE; break;
            }
            Localization_GetString(&info, stringID);
            EntityUIDialog *dialog = UIDialog_CreateDialogYesNo(&info, DialogRunner_SetNoSaveEnabled, DialogRunner_SetNoSaveDisabled, true, true);
            dialog->field_B8       = 1;
        }
    }
    else {
        DialogRunner->entityPtr = NULL;
        destroyEntity(self);
    }
}
void DialogRunner_CheckUserAuth_CB(int32 a1, int32 a2)
{
    RSDK_THIS(DialogRunner);
    if (self->timer) {
        if (DialogRunner->signoutFlag) {
            if (!UIDialog->activeDialog) {
                if (Zone && Zone_GetZoneID() != -1) {
                    RSDK.SetScene("Presentation", "Title Screen");
                    Zone_StartFadeOut(10, 0x000000);
                }
                else if (MenuSetup) {
                    ManiaModeMenu_StartReturnToTitle();
                }
                else if (UFO_Setup) {
                    UFO_Setup->resetToTitle = true;
                    foreach_all(UFO_Setup, setup)
                    {
                        setup->fadeColour = 0;
                        setup->state      = UFO_Setup_State_FinishFadeout;
                        setup->active     = ACTIVE_ALWAYS;
                        setup->visible    = true;
                    }
                }
                else if (FXFade) {
                    RSDK.SetScene("Presentation", "Title Screen");
                    EntityFXFade *fxFade    = CREATE_ENTITY(FXFade, NULL, self->position.x, self->position.y);
                    fxFade->active          = ACTIVE_ALWAYS;
                    fxFade->timer           = 0;
                    fxFade->speedIn         = 16;
                    fxFade->speedOut        = 16;
                    fxFade->state           = FXFade_State_FadeIn;
                    fxFade->drawOrder       = DRAWLAYER_COUNT - 1;
                    fxFade->isPermanent     = true;
                    fxFade->oneWay          = true;
                    fxFade->transitionScene = true;
                }
                DialogRunner->entityPtr = NULL;
                destroyEntity(self);
            }
        }
        else if (!UIDialog->activeDialog) {
            TextInfo info;
            int32 id = STR_SIGNOUTDETECTED;
            if (self->useGenericText)
                id = STR_RETRURNINGTOTITLE;
            Localization_GetString(&info, id);
            EntityUIDialog *dialog = UIDialog_CreateDialogOk(&info, DialogRunner_SignedOutCB, true);
            dialog->field_B8       = 1;
        }
    }
    else {
        EntityUIDialog *dialog = UIDialog->activeDialog;
        if (dialog) {
            if (dialog->state != UIDialog_Unknown13) {
                dialog->parent->selectionDisabled = true;
                dialog->timer                     = 0;
                dialog->state                     = UIDialog_Unknown13;
                dialog->curCallback               = StateMachine_None;
            }
        }
        else {
            if (UIControl) {
                if (UIControl_GetUIControl())
                    UIControl_Unknown6(UIControl_GetUIControl());
            }
            RSDK.SetGameMode(ENGINESTATE_FROZEN);
            RSDK.StopChannel(Music->channelID);
            self->timer = 1;
        }
    }
}
void DialogRunner_ManageNotifs(int32 a1)
{
    RSDK_THIS(DialogRunner);
    if (GameProgress_CountUnreadNotifs()) {
        TextInfo info;
        INIT_TEXTINFO(info);
        if (!UIDialog->activeDialog) {
            int32 str = GameProgress_GetNotifStringID(GameProgress_GetNextNotif());
            Localization_GetString(&info, str);
            EntityUIDialog *dialog = UIDialog_CreateDialogOk(&info, DialogRunner_GetNextNotif, true);
            dialog->playEventSfx   = true;
            dialog->field_B8       = true;
        }
    }
    else {
        DialogRunner->entityPtr = NULL;
        UIWaitSpinner_Wait();
        GameProgress_TrackGameProgress(DialogRunner_Wait);
        destroyEntity(self);
    }
}
void DialogRunner_Wait(int32 success) { UIWaitSpinner_Wait2(); }
void DialogRunner_GetNextNotif(void)
{
    if (SceneInfo->inEditor || API.GetUserStorageNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to save before loading SaveGame file");
        return;
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        int32 id                     = GameProgress_GetNextNotif();
        if (id >= 0)
            progress->unreadNotifs[id] = true;
    }
}
bool32 DialogRunner_CheckUnreadNotifs(void)
{
    if (!GameProgress_CountUnreadNotifs())
        return false;
    if (!DialogRunner->entityPtr)
        DialogRunner->entityPtr = CREATE_ENTITY(DialogRunner, DialogRunner_ManageNotifs, 0, 0);
    return true;
}
bool32 DialogRunner_NotifyAutosave(void)
{
    if (!DialogRunner->isAutoSaving && !DialogRunner->entityPtr) {
        return false;
    }

    if (!DialogRunner->entityPtr || (!globals->notifiedAutosave && !DialogRunner->isAutoSaving)) {
        UIWaitSpinner_Wait();
        DialogRunner->isAutoSaving = true;
        globals->notifiedAutosave  = false;
        LogHelpers_Print("DUMMY NotifyAutosave()");
        EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_NotifyAutoSave, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        DialogRunner->entityPtr          = dialogRunner;
    }
    return true;
}
void DialogRunner_GetUserAuthStatus(void)
{
    if (API.GetUserAuthStatus() == STATUS_FORBIDDEN) {
        if (DialogRunner->authForbiddenFlag)
            return;
        EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_CheckUserAuth_CB, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        DialogRunner->entityPtr          = dialogRunner;
        DialogRunner->authForbiddenFlag  = true;
    }

    if (API.CheckDLC(DLC_PLUS) != globals->lastHasPlus && !DialogRunner->authForbiddenFlag) {
        EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_CheckUserAuth_CB, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        dialogRunner->useGenericText     = true;
        DialogRunner->entityPtr          = dialogRunner;
        DialogRunner->authForbiddenFlag  = true;
        globals->lastHasPlus             = API.CheckDLC(DLC_PLUS);
    }
}
void DialogRunner_PromptSavePreference(int32 id)
{
    if (API.GetUserStorageNoSave()) {
        LogHelpers_Print("PromptSavePreference() returning due to noSave");
        return;
    }
    LogHelpers_Print("PromptSavePreference()");
    if (API.GetSaveStatus() == STATUS_CONTINUE) {
        LogHelpers_Print("WARNING PromptSavePreference() when prompt already in progress.");
    }
    API.SetUserStorageStatus();
    EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_PromptSavePreference_CB, 0, 0);
    dialogRunner->status             = id;
    DialogRunner->entityPtr          = dialogRunner;
}
void DialogRunner_SignedOutCB(void) { DialogRunner->signoutFlag = true; }

#if RETRO_INCLUDE_EDITOR
void DialogRunner_EditorDraw(void) {}

void DialogRunner_EditorLoad(void) {}
#endif

void DialogRunner_Serialize(void) {}
#endif

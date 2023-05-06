// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DialogRunner Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
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
    DialogRunner->authForbidden = false;
    DialogRunner->signedOut     = false;
    DialogRunner->unused2       = 0;
    DialogRunner->unused1       = 0;
    DialogRunner->isAutoSaving  = false;

    if (!globals->hasPlusInitial) {
        globals->lastHasPlus    = API.CheckDLC(DLC_PLUS);
        globals->hasPlusInitial = true;
    }
    DialogRunner->activeCallback = NULL;

    SaveGame_LoadSaveData();
    TimeAttackData->loaded          = false;
    TimeAttackData->uuid            = 0;
    TimeAttackData->rowID           = -1;
    TimeAttackData->personalRank    = 0;
    TimeAttackData->leaderboardRank = 0;
    TimeAttackData->isMigratingData = false;

    Options->changed = false;
    if (sku_platform && sku_platform != PLATFORM_DEV) {
        OptionsRAM *options      = Options_GetOptionsRAM();
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
    UIWaitSpinner_FinishWait();
}

void DialogRunner_NotifyAutoSave(void)
{
    RSDK_THIS(DialogRunner);

    String string;
    INIT_STRING(string);
    if (DialogRunner->isAutoSaving) {
        if (!UIDialog->activeDialog) {
            Localization_GetString(&string, STR_AUTOSAVENOTIF);
            EntityUIDialog *dialog = UIDialog_CreateDialogOk(&string, DialogRunner_NotifyAutoSave_CB, true);
            dialog->useAltColor    = true;
        }
    }
    else {
        DialogRunner->activeCallback = NULL;
        destroyEntity(self);
    }
}

void DialogRunner_SetNoSaveDisabled(void)
{
    API.SetSaveStatusForbidden();
    API.SetNoSave(false);
}

void DialogRunner_SetNoSaveEnabled(void)
{
    API.SetSaveStatusError();
    API.SetNoSave(true);
}

void DialogRunner_PromptSavePreference_CB(void)
{
    RSDK_THIS(DialogRunner);

    String string;
    INIT_STRING(string);
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
            Localization_GetString(&string, stringID);
            EntityUIDialog *dialog = UIDialog_CreateDialogYesNo(&string, DialogRunner_SetNoSaveEnabled, DialogRunner_SetNoSaveDisabled, true, true);
            dialog->useAltColor    = true;
        }
    }
    else {
        DialogRunner->activeCallback = NULL;
        destroyEntity(self);
    }
}
void DialogRunner_CheckUserAuth_CB()
{
    RSDK_THIS(DialogRunner);

    if (self->timer) {
        if (DialogRunner->signedOut) {
            if (!UIDialog->activeDialog) {
                if (Zone && Zone_GetZoneID() != ZONE_INVALID) {
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
                        setup->fadeColor = 0;
                        setup->state     = UFO_Setup_State_FinishFadeout;
                        setup->active    = ACTIVE_ALWAYS;
                        setup->visible   = true;
                    }
                }
                else if (FXFade) {
                    RSDK.SetScene("Presentation", "Title Screen");
                    EntityFXFade *fxFade    = CREATE_ENTITY(FXFade, NULL, self->position.x, self->position.y);
                    fxFade->active          = ACTIVE_ALWAYS;
                    fxFade->timer           = 0;
                    fxFade->speedIn         = 16;
                    fxFade->speedOut        = 16;
                    fxFade->state           = FXFade_State_FadeOut;
                    fxFade->drawGroup       = DRAWGROUP_COUNT - 1;
                    fxFade->isPermanent     = true;
                    fxFade->oneWay          = true;
                    fxFade->transitionScene = true;
                }

                DialogRunner->activeCallback = NULL;
                destroyEntity(self);
            }
        }
        else if (!UIDialog->activeDialog) {
            String string;
            int32 id = STR_SIGNOUTDETECTED;
            if (self->useGenericText)
                id = STR_RETRURNINGTOTITLE;
            Localization_GetString(&string, id);
            EntityUIDialog *dialog = UIDialog_CreateDialogOk(&string, DialogRunner_CheckUserAuth_OK, true);
            dialog->useAltColor    = true;
        }
    }
    else {
        EntityUIDialog *dialog = UIDialog->activeDialog;
        if (dialog) {
            UIDialog_CloseOnSel_HandleSelection(dialog, StateMachine_None);
        }
        else {
            if (UIControl) {
                if (UIControl_GetUIControl())
                    UIControl_SetInactiveMenu(UIControl_GetUIControl());
            }

            RSDK.SetEngineState(ENGINESTATE_FROZEN);
            Music_Stop();
            self->timer = 1;
        }
    }
}
void DialogRunner_ManageNotifs(void)
{
    RSDK_THIS(DialogRunner);

    if (GameProgress_CountUnreadNotifs()) {
        String string;
        INIT_STRING(string);
        if (!UIDialog->activeDialog) {
            int32 str = GameProgress_GetNotifStringID(GameProgress_GetNextNotif());
            Localization_GetString(&string, str);
            EntityUIDialog *dialog = UIDialog_CreateDialogOk(&string, DialogRunner_GetNextNotif, true);
            dialog->playEventSfx   = true;
            dialog->useAltColor    = true;
        }
    }
    else {
        DialogRunner->activeCallback = NULL;
        UIWaitSpinner_StartWait();
        GameProgress_TrackGameProgress(DialogRunner_TrackGameProgressCB);
        destroyEntity(self);
    }
}
void DialogRunner_TrackGameProgressCB(bool32 success) { UIWaitSpinner_FinishWait(); }
void DialogRunner_GetNextNotif(void)
{
    if (SceneInfo->inEditor || API.GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to save before loading SaveGame file");
        return;
    }
    else {
        ProgressRAM *progress = GameProgress_GetProgressRAM();
        int32 id              = GameProgress_GetNextNotif();
        if (id >= 0)
            progress->unreadNotifs[id] = true;
    }
}
bool32 DialogRunner_CheckUnreadNotifs(void)
{
    if (!GameProgress_CountUnreadNotifs())
        return false;

    if (!DialogRunner->activeCallback)
        DialogRunner->activeCallback = CREATE_ENTITY(DialogRunner, DialogRunner_ManageNotifs, 0, 0);

    return true;
}
bool32 DialogRunner_NotifyAutosave(void)
{
    if (globals->notifiedAutosave) {
        if (!DialogRunner->isAutoSaving && !DialogRunner->activeCallback)
            return false;
    }
    else if (!DialogRunner->isAutoSaving || !DialogRunner->activeCallback) {
        UIWaitSpinner_StartWait();
        DialogRunner->isAutoSaving = true;
        globals->notifiedAutosave  = false;
        LogHelpers_Print("DUMMY NotifyAutosave()");
        EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_NotifyAutoSave, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        DialogRunner->activeCallback     = dialogRunner;
    }

    return true;
}
void DialogRunner_GetUserAuthStatus(void)
{
    if (API.GetUserAuthStatus() == STATUS_FORBIDDEN) {
        if (DialogRunner->authForbidden)
            return;

        EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_CheckUserAuth_CB, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        DialogRunner->activeCallback     = dialogRunner;
        DialogRunner->authForbidden      = true;
    }

    if (API.CheckDLC(DLC_PLUS) != (bool32)globals->lastHasPlus && !DialogRunner->authForbidden) {
        EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_CheckUserAuth_CB, 0, 0);
        dialogRunner->active             = ACTIVE_ALWAYS;
        dialogRunner->useGenericText     = true;
        DialogRunner->activeCallback     = dialogRunner;
        DialogRunner->authForbidden      = true;
        globals->lastHasPlus             = API.CheckDLC(DLC_PLUS);
    }
}
void DialogRunner_PromptSavePreference(int32 id)
{
    if (API.GetNoSave()) {
        LogHelpers_Print("PromptSavePreference() returning due to noSave");
        return;
    }

    LogHelpers_Print("PromptSavePreference()");

    if (API.GetSaveStatus() == STATUS_CONTINUE)
        LogHelpers_Print("WARNING PromptSavePreference() when prompt already in progress.");

    API.ClearSaveStatus();

    EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_PromptSavePreference_CB, 0, 0);
    dialogRunner->status             = id;
    DialogRunner->activeCallback     = dialogRunner;
}
void DialogRunner_CheckUserAuth_OK(void) { DialogRunner->signedOut = true; }

#if GAME_INCLUDE_EDITOR
void DialogRunner_EditorDraw(void) {}

void DialogRunner_EditorLoad(void) {}
#endif

void DialogRunner_Serialize(void) {}
#endif

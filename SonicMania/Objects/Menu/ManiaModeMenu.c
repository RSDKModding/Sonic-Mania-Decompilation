// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ManiaModeMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectManiaModeMenu *ManiaModeMenu;

void ManiaModeMenu_Update(void) {}

void ManiaModeMenu_LateUpdate(void) {}

void ManiaModeMenu_StaticUpdate(void) {}

void ManiaModeMenu_Draw(void) {}

void ManiaModeMenu_Create(void *data) {}

void ManiaModeMenu_StageLoad(void) {}

void ManiaModeMenu_Initialize(void)
{
    LogHelpers_Print("ManiaModeMenu_Initialize()");

    MainMenu_Initialize();
    UISubHeading_Initialize();
    TimeAttackMenu_Initialize();
    CompetitionMenu_Initialize();
    OptionsMenu_Initialize();
    ExtrasMenu_Initialize();

    ManiaModeMenu_HandleUnlocks();
    ManiaModeMenu_SetupActions();
}

bool32 ManiaModeMenu_InitAPI(void)
{
    if (!MenuSetup->initializedAPI)
        MenuSetup->fxFade->timer = 512;

    int32 authStatus = API.GetUserAuthStatus();
    if (!authStatus) {
        API.TryAuth();
    }
    else if (authStatus != STATUS_CONTINUE) {
        int32 storageStatus = API.GetStorageStatus();
        if (!storageStatus) {
            API.TryInitStorage();
        }
        else if (storageStatus != STATUS_CONTINUE) {
            int32 saveStatus = API.GetSaveStatus();

            if (!API_GetNoSave() && (authStatus != STATUS_OK || storageStatus != STATUS_OK)) {
                if (saveStatus != STATUS_CONTINUE) {
                    if (saveStatus != STATUS_FORBIDDEN) {
                        DialogRunner_PromptSavePreference(storageStatus);
                    }
                    else {
                        RSDK.SetScene("Presentation", "Title Screen");
                        RSDK.LoadScene();
                    }
                }

                return false;
            }

            if (!MenuSetup->initializedSaves) {
                UIWaitSpinner_StartWait();
                Options_LoadFile(Options_LoadCallback);
                SaveGame_LoadFile(SaveGame_SaveLoadedCB);
                ReplayDB_LoadDB(ReplayDB_LoadCallback);

                MenuSetup->initializedSaves = true;
            }

            if (MenuSetup->initializedAPI)
                return true;

            if (globals->optionsLoaded == STATUS_OK && globals->saveLoaded == STATUS_OK && globals->replayTableLoaded == STATUS_OK
                && globals->taTableLoaded == STATUS_OK) {

                if (!API_GetNoSave() && DialogRunner_NotifyAutosave())
                    return false;

                UIWaitSpinner_FinishWait();
                if (DialogRunner_CheckUnreadNotifs())
                    return false;

                MenuSetup->initializedAPI = true;
                return true;
            }

            if (API_GetNoSave()) {
                UIWaitSpinner_FinishWait();
                return true;
            }
            else {
                if (globals->optionsLoaded == STATUS_ERROR || globals->saveLoaded == STATUS_ERROR || globals->replayTableLoaded == STATUS_ERROR
                    || globals->taTableLoaded == STATUS_ERROR) {
                    int32 status = API.GetSaveStatus();

                    if (status != STATUS_CONTINUE) {
                        if (status == STATUS_FORBIDDEN) {
                            RSDK.SetScene("Presentation", "Title Screen");
                            RSDK.LoadScene();
                        }
                        else {
                            DialogRunner_PromptSavePreference(STATUS_CORRUPT);
                        }
                    }
                }
            }
        }
    }

    return false;
}

void ManiaModeMenu_InitLocalization(bool32 success)
{
    if (success) {
        Localization->loaded = false;

        Localization_LoadStrings();
        UIWidgets_ApplyLanguage();
        UIHeading_LoadSprites();
    }
}

int32 ManiaModeMenu_GetActiveMenu(void)
{
    EntityUIControl *control = UIControl_GetUIControl();

    if (control == MainMenu->menuControl || control == ExtrasMenu->extrasControl || control == OptionsMenu->optionsControl
        || control == OptionsMenu->videoControl || control == OptionsMenu->soundControl || control == OptionsMenu->dataOptionsControl
        || control == OptionsMenu->controlsControl_Windows || control == OptionsMenu->controlsControl_KB
        || control == OptionsMenu->controlsControl_PS4 || control == OptionsMenu->controlsControl_XB1 || control == OptionsMenu->controlsControl_NX
        || control == OptionsMenu->controlsControl_NXGrip || control == OptionsMenu->controlsControl_NXJoycon
        || control == OptionsMenu->controlsControl_NXPro) {
        return MAINMENU_MAIN;
    }

    if (control == TimeAttackMenu->timeAttackControl || control == TimeAttackMenu->timeAttackControl_Legacy
        || control == TimeAttackMenu->taZoneSelControl || control == TimeAttackMenu->taDetailsControl
        || control == TimeAttackMenu->leaderboardsControl || control == TimeAttackMenu->replaysControl
        || control == CompetitionMenu->competitionControl || control == CompetitionMenu->competitionControl_Legacy
        || control == CompetitionMenu->compRulesControl || control == CompetitionMenu->compZoneControl) {
        return MAINMENU_TIMEATTACK;
    }

    if (control == CompetitionMenu->compRoundControl || control == CompetitionMenu->compTotalControl)
        return MAINMENU_COMPETITION;

    if (control == ManiaModeMenu->saveSelectMenu || control == ManiaModeMenu->noSaveMenu || control == ManiaModeMenu->secretsMenu)
        return MAINMENU_SAVESELECT;

    if (control == ManiaModeMenu->encoreSaveSelect || control == ManiaModeMenu->noSaveMenuEncore)
        return MAINMENU_SAVESELECT_ENCORE;

    return MAINMENU_MAIN;
}

void ManiaModeMenu_ChangeMenuTrack(void)
{
    int32 trackID = 0;

    switch (ManiaModeMenu_GetActiveMenu()) {
        default:
        case MAINMENU_MAIN: trackID = 0; break;
        case MAINMENU_TIMEATTACK: trackID = 1; break;
        case MAINMENU_COMPETITION: trackID = 2; break;
        case MAINMENU_SAVESELECT: trackID = 3; break;
        case MAINMENU_SAVESELECT_ENCORE: trackID = 4; break;
    }

    if (!Music_IsPlaying())
        Music_PlayTrack(trackID);
    else if (Music->activeTrack != trackID)
        Music_TransitionTrack(trackID, 0.12);
}

void ManiaModeMenu_StartReturnToTitle(void)
{
    EntityUIControl *control = UIControl_GetUIControl();
    if (control)
        control->state = StateMachine_None;

    Music_FadeOut(0.05);
    MenuSetup_StartTransition(ManiaModeMenu_ReturnToTitle, 32);
}

void ManiaModeMenu_SetBGColors(void)
{
    switch (ManiaModeMenu_GetActiveMenu()) {
        case MAINMENU_MAIN: UIBackground->activeColors = UIBackground->bgColors; break;

        case MAINMENU_TIMEATTACK:
        case MAINMENU_COMPETITION: UIBackground->activeColors = &UIBackground->bgColors[3]; break;

        case MAINMENU_SAVESELECT: UIBackground->activeColors = &UIBackground->bgColors[6]; break;

        case MAINMENU_SAVESELECT_ENCORE: UIBackground->activeColors = &UIBackground->bgColors[15]; break;

        default: break;
    }
}

void ManiaModeMenu_ReturnToTitle(void)
{
    TimeAttackData_Clear();

    RSDK.SetScene("Presentation", "Title Screen");
    RSDK.LoadScene();
}

void ManiaModeMenu_State_HandleTransition(void)
{
    RSDK_THIS(MenuSetup);

    self->fadeTimer = CLAMP(self->timer << ((self->fadeShift & 0xFF) - 1), 0, 0x200);
}

void ManiaModeMenu_HandleUnlocks(void)
{
    MainMenu_HandleUnlocks();
    UISubHeading_HandleUnlocks();
    TimeAttackMenu_HandleUnlocks();

    int32 maxRounds            = CompetitionMenu_HandleUnlocks();
    EntityUIControl *compRules = CompetitionMenu->compRulesControl;

    EntityUIVsRoundPicker *vsRoundPicker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(compRules->buttons[1], compRules->buttons[1]->selection);
    if (vsRoundPicker) {
        vsRoundPicker->maxVal = maxRounds;
        vsRoundPicker->val    = MIN(vsRoundPicker->val, maxRounds);
    }

    OptionsMenu_HandleUnlocks();
    ExtrasMenu_HandleUnlocks();
}

void ManiaModeMenu_SetupActions(void)
{
    MainMenu_SetupActions();
    UISubHeading_SetupActions();
    TimeAttackMenu_SetupActions();
    CompetitionMenu_SetupActions();
    OptionsMenu_SetupActions();
    ExtrasMenu_SetupActions();
}

void ManiaModeMenu_HandleMenuReturn(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    char buffer[0x100];
    memset(buffer, 0, 0x100);
    if (strcmp(param->menuTag, "") == 0)
        UIUsernamePopup_ShowPopup();

    foreach_all(UIControl, control)
    {
        if (strcmp(param->menuTag, "") != 0) {
            RSDK.GetCString(buffer, &control->tag);

            if (strcmp((const char *)buffer, param->menuTag) != 0) {
                UIControl_SetInactiveMenu(control);
            }
            else {
                control->storedButtonID  = param->menuSelection;
                control->hasStoredButton = true;
                UIControl_SetActiveMenu(control);
                control->buttonID = param->menuSelection;
            }
        }
    }

    UISubHeading_HandleMenuReturn(0);
    TimeAttackMenu_HandleMenuReturn();
    CompetitionMenu_HandleMenuReturn();
    OptionsMenu_HandleMenuReturn();

    if (param->puyoSelection == PUYO_SELECTION_VS_2P) {
        EntityUIControl *extras = ExtrasMenu->extrasControl;
        UIButton_SetChoiceSelection(extras->buttons[1], 1);
    }

    int32 zoneID = 0, actID = 0, characterID = 0, isEncoreMode = false;
    bool32 inTimeAttack = param->inTimeAttack;
    if (inTimeAttack) {
        characterID  = param->characterID;
        zoneID       = param->zoneID;
        actID        = param->actID;
        isEncoreMode = param->isEncoreMode;
    }

    TimeAttackData_Clear();

    if (inTimeAttack) {
        param->characterID  = characterID;
        param->zoneID       = zoneID;
        param->actID        = actID;
        param->isEncoreMode = isEncoreMode;
    }
}

#if RETRO_INCLUDE_EDITOR
void ManiaModeMenu_EditorDraw(void) {}

void ManiaModeMenu_EditorLoad(void) {}
#endif

void ManiaModeMenu_Serialize(void) {}
#endif

// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MenuSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMenuSetup *MenuSetup;

void MenuSetup_Update(void)
{
    RSDK_THIS(MenuSetup);

    StateMachine_Run(self->state);

#if !MANIA_USE_PLUS
    if (self->state != MenuSetup_State_SetupLeaderboards) {
#endif

        if (self->timer >= self->delay) {
            StateMachine_Run(self->callback);
            destroyEntity(self);
        }
        else {
            self->timer++;
        }

#if !MANIA_USE_PLUS
    }
#endif
}

void MenuSetup_LateUpdate(void) {}

void MenuSetup_StaticUpdate(void)
{
#if MANIA_USE_PLUS
    if (!MenuSetup->initializedAPI) {
        MenuSetup->fxFade->speedOut = 0;

        String tag;
        INIT_STRING(tag);
        RSDK.SetString(&tag, "Main Menu");

        EntityUIControl *mainMenu = NULL; // this will crash if no entities have "Main Menu" as the tag, make sure one does!!!
        foreach_all(UIControl, control)
        {
            if (RSDK.CompareStrings(&tag, &control->tag, false)) {
                mainMenu = control;
                foreach_break;
            }
        }

        if (!ManiaModeMenu_InitAPI()) {
            mainMenu->selectionDisabled = true;
            return;
        }
        else {
            mainMenu->selectionDisabled = false;
            MenuSetup->initializedAPI  = true;

            String message;
            Localization_GetString(&message, STR_RPC_MENU);
            API_SetRichPresence(PRESENCE_MENU, &message);
        }
    }

    if (!MenuSetup->initializedMenu) {
        ManiaModeMenu_Initialize();
        MenuSetup->initializedMenu = true;
    }

    if (!MenuSetup->initializedMenuReturn) {
        ManiaModeMenu_HandleMenuReturn();
        MenuSetup->initializedMenuReturn = true;
        ManiaModeMenu_SetBGColors();

        if (!globals->suppressAutoMusic)
            ManiaModeMenu_ChangeMenuTrack();

        globals->suppressAutoMusic = false;
    }

    MenuSetup->fxFade->speedOut = 12;
    DialogRunner_GetUserAuthStatus();
#else
    if (!MenuSetup->initializedAPI) {
        String tag;
        INIT_STRING(tag);
        RSDK.SetString(&tag, "Main Menu");

        EntityUIControl *mainMenu = NULL; // this will crash if no entities have "Main Menu" as the tag, make sure one does!!!
        foreach_all(UIControl, control)
        {
            if (RSDK.CompareStrings(&tag, &control->tag, false)) {
                mainMenu = control;
                foreach_break;
            }
        }

        if (!MenuSetup_InitAPI()) {
            mainMenu->selectionDisabled = true;
            return;
        }
        else {
            mainMenu->selectionDisabled = false;
            MenuSetup->initializedAPI  = true;

            String message;
            Localization_GetString(&message, STR_RPC_MENU);
            API_SetRichPresence(PRESENCE_MENU, &message);
        }
    }

    if (!MenuSetup->initializedMenu) {
        MenuSetup_Initialize();
        MenuSetup->initializedMenu = true;
    }

    if (!MenuSetup->initializedMenuReturn) {
        MenuSetup_HandleMenuReturn();
        MenuSetup->initializedMenuReturn = true;
        MenuSetup_SetBGColors();

        if (!globals->suppressAutoMusic)
            MenuSetup_ChangeMenuTrack();

        globals->suppressAutoMusic = false;
    }

    APICallback_GetUserAuthStatus();
#endif
}

void MenuSetup_Draw(void)
{
    RSDK_THIS(MenuSetup);

    RSDK.FillScreen(self->fadeColor, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);
}

void MenuSetup_Create(void *data)
{
    RSDK_THIS(MenuSetup);

    self->active    = ACTIVE_ALWAYS;
    self->visible   = true;
    self->drawGroup = 14;
}

void MenuSetup_StageLoad(void)
{
#if MANIA_USE_PLUS
    EntityMenuParam *param = MenuParam_GetParam();
    LogHelpers_Print("Menu recall ctrl: %s", param->menuTag);

    MenuSetup->initializedMenuReturn = false;
    MenuSetup->initializedMenu       = false;
    MenuSetup->initializedAPI        = false;
    MenuSetup->initializedSaves      = false;
    MenuSetup->fxFade                = NULL;

    if (!globals->suppressAutoMusic) {
        Music_Stop();
        Music->activeTrack = TRACK_NONE;
    }
#else
    Music_Stop();
    Music->activeTrack = TRACK_NONE;
#endif

    if (!SceneInfo->inEditor) {
        switch (sku_platform) {
            case PLATFORM_PC: LogHelpers_Print("PC SKU"); break;
            case PLATFORM_PS4: LogHelpers_Print("PS4 SKU"); break;
            case PLATFORM_XB1: LogHelpers_Print("XB1 SKU"); break;
            case PLATFORM_SWITCH: LogHelpers_Print("NX SKU"); break;
            case PLATFORM_DEV: LogHelpers_Print("DEV SKU"); break;
            default: LogHelpers_Print("INVALID PLATFORM: %d", sku_platform); break;
        }

        switch (sku_region) {
            case REGION_US: LogHelpers_Print("US REGION"); break;
            case REGION_JP: LogHelpers_Print("JP REGION"); break;
            case REGION_EU: LogHelpers_Print("EU REGION"); break;
            default: LogHelpers_Print("INVALID REGION: %d", sku_region); break;
        }
    }

    // Bug Details(?):
    // sizeof(globals->noSaveSlot) and sizeof(saveData) is 4096 (sizeof(int32) * 0x400)
    // but the memset size is only 1024 (sizeof(uint8) * 0x400)
    // so only about 1/4th of the save slot is cleared, though nothin uses the extra space so it's not a big deal
    memset(globals->noSaveSlot, 0, 0x400);

    globals->continues = 0;
#if MANIA_USE_PLUS
    globals->recallEntities = false;
#else
    MenuSetup->vsTotalTimer = 120;
#endif

    RSDK.SetVideoSetting(VIDEOSETTING_SCREENCOUNT, 1);

    foreach_all(FXFade, fade) { MenuSetup->fxFade = fade; }
}

void MenuSetup_StartTransition(void (*callback)(void), int32 delay)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);

    menuSetup->active    = ACTIVE_ALWAYS;
    menuSetup->fadeColor = 0x000000;
    menuSetup->fadeShift = 5;
    menuSetup->delay     = delay;
#if MANIA_USE_PLUS
    menuSetup->state = ManiaModeMenu_State_HandleTransition;
#else
    menuSetup->state = MenuSetup_State_HandleTransition;
#endif
    menuSetup->callback = callback;
}

#if !MANIA_USE_PLUS
void MenuSetup_StartTransitionLB(void (*callback)(void), int32 delay)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);

    menuSetup->active    = ACTIVE_ALWAYS;
    menuSetup->fadeColor = 0x000000;
    menuSetup->fadeShift = 5;
    menuSetup->delay     = delay;
    menuSetup->state     = MenuSetup_State_SetupLeaderboards;
    menuSetup->callback  = callback;
}
#endif

// START PRE-PLUS AREA
#if !MANIA_USE_PLUS
void MenuSetup_Initialize(void)
{
    String tag;
    INIT_STRING(tag);

    foreach_all(UIControl, control)
    {
        RSDK.SetString(&tag, "Main Menu");
        if (RSDK.CompareStrings(&tag, &control->tag, false)) {
            MenuSetup->mainMenu  = control;
            control->backPressCB = MenuSetup_BackPressCB_ReturnToTitle;
        }

        RSDK.SetString(&tag, "Time Attack");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->timeAttack = control;

        RSDK.SetString(&tag, "Time Attack Zones");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->timeAttackZones = control;

        RSDK.SetString(&tag, "Leaderboards");
        if (RSDK.CompareStrings(&tag, &control->tag, false)) {
            MenuSetup->leaderboards = control;
            control->backPressCB    = MenuSetup_TA_Leaderboards_BackPressCB;
        }

        RSDK.SetString(&tag, "Competition");
        if (RSDK.CompareStrings(&tag, &control->tag, false)) {
            MenuSetup->competition = control;
            control->backPressCB   = MenuSetup_VS_BackoutFromVsCharSelect;
        }

        RSDK.SetString(&tag, "Competition Rules");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->competitionRules = control;

        RSDK.SetString(&tag, "Competition Zones");
        if (RSDK.CompareStrings(&tag, &control->tag, false)) {
            MenuSetup->competitionZones = control;
            control->backPressCB        = MenuSetup_VS_CompZones_BackPressCB;
        }

        RSDK.SetString(&tag, "Competition Round");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->competitionRound = control;

        RSDK.SetString(&tag, "Competition Total");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->competitionTotal = control;

        RSDK.SetString(&tag, "Save Select");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->saveSelect = control;

        RSDK.SetString(&tag, "No Save Mode");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->noSaveMode = control;

        RSDK.SetString(&tag, "Secrets");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->secrets = control;

        RSDK.SetString(&tag, "Extras");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->extras = control;

        RSDK.SetString(&tag, "Options");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->options = control;

        RSDK.SetString(&tag, "Language");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->language = control;

        RSDK.SetString(&tag, "Video");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->video = control;

        RSDK.SetString(&tag, "Video WIN");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->video_win = control;

        RSDK.SetString(&tag, "Sound");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->sound = control;

        RSDK.SetString(&tag, "Controls WIN");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->controls_win = control;

        RSDK.SetString(&tag, "Controls KB");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->controls_KB = control;

        RSDK.SetString(&tag, "Controls PS4");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->controls_PS4 = control;

        RSDK.SetString(&tag, "Controls XB1");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->controls_XB1 = control;

        RSDK.SetString(&tag, "Controls NX");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->controls_NX = control;

        RSDK.SetString(&tag, "Controls NX Grip");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->controls_NX_Grip = control;

        RSDK.SetString(&tag, "Controls NX Joycon");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->controls_NX_JoyCon = control;

        RSDK.SetString(&tag, "Controls NX Pro");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            MenuSetup->controls_NX_Pro = control;
    }

    Hitbox hitbox;
    foreach_all(UIButtonPrompt, prompt)
    {
        EntityUIControl *saveControl = MenuSetup->saveSelect;
        int32 x                      = saveControl->startPos.x - saveControl->cameraOffset.x;
        int32 y                      = saveControl->startPos.y - saveControl->cameraOffset.y;

        hitbox.right  = saveControl->size.x >> 17;
        hitbox.left   = -(saveControl->size.x >> 17);
        hitbox.bottom = saveControl->size.y >> 17;
        hitbox.top    = -(saveControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(x, y, prompt->position.x, prompt->position.y, FLIP_NONE, &hitbox) && prompt->buttonID == 2)
            MenuSetup->delSavePrompt = prompt;

        EntityUIControl *leaderboardsControl = MenuSetup->leaderboards;
        x                                    = leaderboardsControl->startPos.x - leaderboardsControl->cameraOffset.x;
        y                                    = leaderboardsControl->startPos.y - leaderboardsControl->cameraOffset.y;

        hitbox.right  = leaderboardsControl->size.x >> 17;
        hitbox.left   = -(leaderboardsControl->size.x >> 17);
        hitbox.bottom = leaderboardsControl->size.y >> 17;
        hitbox.top    = -(leaderboardsControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(x, y, prompt->position.x, prompt->position.y, FLIP_NONE, &hitbox) && prompt->buttonID == 3)
            MenuSetup->leaderboardPrompt = prompt;

        EntityUIControl *optionsControl = MenuSetup->options;
        x                               = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        y                               = optionsControl->startPos.y - optionsControl->cameraOffset.y;

        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.x >> 17);
        hitbox.bottom = optionsControl->size.y >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(x, y, prompt->position.x, prompt->position.y, FLIP_NONE, &hitbox) && prompt->buttonID == 3)
            MenuSetup->optionsPrompt = prompt;
    }

    foreach_all(UILeaderboard, leaderboard) { MenuSetup->leaderboardWidget = leaderboard; }

    foreach_all(UIInfoLabel, label)
    {
        EntityUIControl *roundControl = MenuSetup->competitionRound;
        int32 x                       = roundControl->startPos.x - roundControl->cameraOffset.x;
        int32 y                       = roundControl->startPos.y - roundControl->cameraOffset.y;

        hitbox.right  = roundControl->size.x >> 17;
        hitbox.left   = -(roundControl->size.x >> 17);
        hitbox.bottom = roundControl->size.y >> 17;
        hitbox.top    = -(roundControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(x, y, label->position.x, label->position.y, FLIP_NONE, &hitbox))
            MenuSetup->roundLabel = label;

        EntityUIControl *totalControl = MenuSetup->competitionTotal;
        x                             = totalControl->startPos.x - totalControl->cameraOffset.x;
        y                             = totalControl->startPos.y - totalControl->cameraOffset.y;

        hitbox.right  = totalControl->size.x >> 17;
        hitbox.left   = -(totalControl->size.x >> 17);
        hitbox.bottom = totalControl->size.y >> 17;
        hitbox.top    = -(totalControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(x, y, label->position.x, label->position.y, FLIP_NONE, &hitbox))
            MenuSetup->totalLabel = label;
    }

    foreach_all(UIVsScoreboard, scoreboard)
    {
        EntityUIControl *roundControl = MenuSetup->competitionRound;
        int32 x                       = roundControl->startPos.x - roundControl->cameraOffset.x;
        int32 y                       = roundControl->startPos.y - roundControl->cameraOffset.y;

        hitbox.right  = roundControl->size.x >> 17;
        hitbox.left   = -(roundControl->size.x >> 17);
        hitbox.bottom = roundControl->size.y >> 17;
        hitbox.top    = -(roundControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(x, y, scoreboard->position.x, scoreboard->position.y, FLIP_NONE, &hitbox)) {
            MenuSetup->roundScoreboard = scoreboard;
            scoreboard->parentPos      = &roundControl->position;
        }

        EntityUIControl *totalControl = MenuSetup->competitionTotal;
        x                             = totalControl->startPos.x - totalControl->cameraOffset.x;
        y                             = totalControl->startPos.y - totalControl->cameraOffset.y;

        hitbox.right  = totalControl->size.x >> 17;
        hitbox.left   = -(totalControl->size.x >> 17);
        hitbox.bottom = totalControl->size.y >> 17;
        hitbox.top    = -(totalControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(x, y, scoreboard->position.x, scoreboard->position.y, FLIP_NONE, &hitbox)) {
            MenuSetup->totalScoreboard = scoreboard;
            scoreboard->parentPos      = &totalControl->position;
        }
    }

    MenuSetup_HandleUnlocks();
    MenuSetup_SetupActions();
}

bool32 MenuSetup_InitAPI(void)
{
    if (!MenuSetup->initializedAPI)
        MenuSetup->fxFade->timer = 512;

    APICallback_GetUserAuthStatus();

    if (!APICallback->authStatus) {
        APICallback_TryAuth();
    }
    else if (APICallback->authStatus != STATUS_CONTINUE) {
        int32 storageStatus = APICallback_GetStorageStatus();

        if (!storageStatus) {
            APICallback_TryInitStorage();
        }
        else if (storageStatus != STATUS_CONTINUE) {
            if (!API_GetNoSave() && (APICallback->authStatus != STATUS_OK || storageStatus != STATUS_OK)) {
                if (APICallback->saveStatus != STATUS_CONTINUE) {
                    if (APICallback->saveStatus != STATUS_FORBIDDEN) {
                        APICallback_PromptSavePreference(storageStatus);
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

                MenuSetup->initializedSaves = true;
            }

            if (MenuSetup->initializedAPI)
                return true;

            if (globals->optionsLoaded == STATUS_OK && globals->saveLoaded == STATUS_OK) {

                if (!API_GetNoSave() && APICallback_NotifyAutosave())
                    return false;

                UIWaitSpinner_FinishWait();
                if (APICallback_CheckUnreadNotifs())
                    return false;

                MenuSetup->initializedAPI = true;
                return true;
            }

            if (API_GetNoSave()) {
                UIWaitSpinner_FinishWait();

                return true;
            }
            else {
                if (globals->optionsLoaded == STATUS_ERROR || globals->saveLoaded == STATUS_ERROR) {
                    if (APICallback->saveStatus != STATUS_CONTINUE) {
                        if (APICallback->saveStatus == STATUS_FORBIDDEN) {
                            RSDK.SetScene("Presentation", "Title Screen");
                            RSDK.LoadScene();
                        }
                        else {
                            APICallback_PromptSavePreference(STATUS_CORRUPT);
                        }
                    }
                }
            }
        }
    }

    return false;
}

void MenuSetup_SetupActions(void)
{
    EntityUIControl *comp         = MenuSetup->competition;
    EntityUIControl *options      = MenuSetup->options;
    EntityUIControl *video        = MenuSetup->video;
    EntityUIControl *video_win    = MenuSetup->video_win;
    EntityUIControl *controls_win = MenuSetup->controls_win;
    EntityUIControl *sound        = MenuSetup->sound;
    EntityUIControl *saveSel      = MenuSetup->saveSelect;
    EntityUIControl *secrets      = MenuSetup->secrets;
    EntityUIControl *extras       = MenuSetup->extras;
    EntityUIControl *compTotal    = MenuSetup->competitionTotal;
    EntityUIControl *compRound    = MenuSetup->competitionRound;
    EntityUIControl *compRules    = MenuSetup->competitionRules;
    EntityUIControl *language     = MenuSetup->language;

    foreach_all(UIModeButton, modeButton) { modeButton->actionCB = MenuSetup_MenuButton_ActionCB; }
    foreach_all(UISaveSlot, saveSlot) { saveSlot->actionCB = MenuSetup_SaveSlot_ActionCB; }

    Hitbox hitbox;
    foreach_all(UIButton, button)
    {
        switch (button->listID) {
            case 17:
                if (!button->frameID) {
                    if (GameInfo->platform == PLATFORM_PC || GameInfo->platform == PLATFORM_DEV) {
                        button->actionCB = MenuSetup_ExitGame_ActionCB;
                    }
                    else {
                        EntityUIControl *control = MenuSetup->mainMenu;
                        --control->buttonCount;
                        control->buttons[5] = NULL;
                        destroyEntity(button);
                    }
                }
                break;

            case 1:
                if (button->frameID == 4)
                    button->actionCB = MenuSetup_OpenExtrasMenu_ActionCB;
                break;

            case 7:
                if (button->frameID == 4) {
                    button->actionCB = MenuSetup_Extras_DAGarden_ActionCB;
                }
                else if (button->frameID == 8) {
                    button->actionCB         = MenuSetup_Extras_Credits_ActionCB;
                    button->clearParentState = true;
                }
                break;
        }

        int32 x       = controls_win->startPos.x - controls_win->cameraOffset.x;
        int32 y       = controls_win->startPos.y - controls_win->cameraOffset.y;
        hitbox.right  = controls_win->size.x >> 17;
        hitbox.left   = -(controls_win->size.x >> 17);
        hitbox.bottom = controls_win->size.y >> 17;
        hitbox.top    = -(controls_win->size.y >> 17);
        if (MathHelpers_PointInHitbox(x, y, button->position.x, button->position.y, FLIP_NONE, &hitbox))
            button->actionCB = MenuSetup_Options_OpenKBControlsMenu;

        x             = compRules->startPos.x - compRules->cameraOffset.x;
        y             = compRules->startPos.y - compRules->cameraOffset.y;
        hitbox.right  = compRules->size.x >> 17;
        hitbox.left   = -(compRules->size.x >> 17);
        hitbox.bottom = compRules->size.y >> 17;
        hitbox.top    = -(compRules->size.y >> 17);
        if (MathHelpers_PointInHitbox(x, y, button->position.x, button->position.y, FLIP_NONE, &hitbox) && button->listID == 9
            && button->frameID == 2)
            button->actionCB = MenuSetup_VS_RulesButton_ActionCB;

        x             = secrets->startPos.x - secrets->cameraOffset.x;
        y             = secrets->startPos.y - secrets->cameraOffset.y;
        hitbox.right  = secrets->size.x >> 17;
        hitbox.left   = -(secrets->size.x >> 17);
        hitbox.bottom = secrets->size.y >> 17;
        hitbox.top    = -(secrets->size.y >> 17);
        if (MathHelpers_PointInHitbox(x, y, button->position.x, button->position.y, FLIP_NONE, &hitbox) && button->listID == 9
            && button->frameID == 2)
            button->actionCB = MenuSetup_OpenSaveSelectMenu;

        x             = options->startPos.x - options->cameraOffset.x;
        y             = options->startPos.y - options->cameraOffset.y;
        hitbox.right  = options->size.x >> 17;
        hitbox.left   = -(options->size.x >> 17);
        hitbox.bottom = options->size.y >> 17;
        hitbox.top    = -(options->size.y >> 17);
        if (MathHelpers_PointInHitbox(x, y, button->position.x, button->position.y, FLIP_NONE, &hitbox) && button->listID == 3) {
            switch (button->frameID) {
                case 0: button->actionCB = MenuSetup_Options_VideoMenuButton_ActionCB; break;
                case 1: button->actionCB = MenuSetup_Options_SoundMenuButton_ActionCB; break;
                case 2: button->actionCB = MenuSetup_Options_ControlsMenuButton_ActionCB; break;
                case 3: button->actionCB = MenuSetup_Options_LanguageMenuButton_ActionCB; break;
            }
        }

        x             = language->startPos.x - language->cameraOffset.x;
        y             = language->startPos.y - language->cameraOffset.y;
        hitbox.right  = language->size.x >> 17;
        hitbox.left   = -(language->size.x >> 17);
        hitbox.bottom = language->size.y >> 17;
        hitbox.top    = -(language->size.y >> 17);
        if (MathHelpers_PointInHitbox(x, y, button->position.x, button->position.y, FLIP_NONE, &hitbox))
            button->actionCB = MenuSetup_OptionsLanguage_LanguageButton_ActionCB;

        x             = video->startPos.x - video->cameraOffset.x;
        y             = video->startPos.y - video->cameraOffset.y;
        hitbox.right  = video->size.x >> 17;
        hitbox.left   = -(video->size.x >> 17);
        hitbox.bottom = video->size.y >> 17;
        hitbox.top    = -(video->size.y >> 17);
        if (MathHelpers_PointInHitbox(x, y, button->position.x, button->position.y, FLIP_NONE, &hitbox) && button->listID == 3
            && button->frameID == 0)
            button->choiceChangeCB = MenuSetup_OptionsVideo_ShaderButton_ActionCB;

        x             = video_win->startPos.x - video_win->cameraOffset.x;
        y             = video_win->startPos.y - video_win->cameraOffset.y;
        hitbox.right  = video_win->size.x >> 17;
        hitbox.left   = -(video_win->size.x >> 17);
        hitbox.bottom = video_win->size.y >> 17;
        hitbox.top    = -(video->size.y >> 17);
        if (MathHelpers_PointInHitbox(x, y, button->position.x, button->position.y, FLIP_NONE, &hitbox) && button->listID == 17) {
            switch (button->frameID) {
                case 2: button->choiceChangeCB = MenuSetup_OptionsVideo_ShaderButton_ActionCB; break;
                case 7: button->choiceChangeCB = MenuSetup_OptionsVideo_WindowScaleButton_ActionCB; break;
                case 13: button->choiceChangeCB = MenuSetup_OptionsVideo_BorderlessButton_ActionCB; break;
                case 14: button->choiceChangeCB = MenuSetup_OptionsVideo_FullscreenButton_ActionCB; break;
                case 15: button->choiceChangeCB = MenuSetup_OptionsVideo_VSyncButton_ActionCB; break;
                case 16: button->choiceChangeCB = MenuSetup_OptionsVideo_TripleBufferButton_ActionCB; break;
                default: break;
            }
        }
    }

    foreach_all(UIChoice, choice)
    {
        if (choice->listID == 7) {
            switch (choice->frameID) {
                case 2:
                    choice->actionCB    = MenuSetup_Extras_Puyo_vsAI_ActionCB;
                    choice->textVisible = true;
                    break;

                case 3:
                    choice->actionCB    = MenuSetup_Extras_Puyo_vs2P_ActionCB;
                    choice->textVisible = true;
                    break;

                case 6:
                    choice->actionCB    = MenuSetup_Extras_BSS_3K_ActionCB;
                    choice->textVisible = true;
                    break;

                case 7:
                    choice->actionCB    = MenuSetup_Extras_BSS_Mania_ActionCB;
                    choice->textVisible = true;
                    break;

                default: break;
            }
        }
    }

    foreach_all(UICharButton, charButton)
    {
        if (charButton->parent == (Entity *)MenuSetup->timeAttack) {
            switch (charButton->characterID) {
                case 0: charButton->actionCB = MenuSetup_TA_OpenZoneList_Sonic; break;
                case 1: charButton->actionCB = MenuSetup_TA_OpenZoneList_Tails; break;
                case 2: charButton->actionCB = MenuSetup_TA_OpenZoneList_Knux; break;
            }
        }
    }

    foreach_all(UITAZoneModule, module) { module->actionCB = MenuSetup_TA_TAZoneModule_ActionCB; }

    MenuSetup->leaderboardWidget->yPressCB = MenuSetup_TA_Leaderboards_YPressCB;

    foreach_all(UISlider, slider)
    {
        hitbox.right  = sound->size.x >> 17;
        hitbox.left   = -(sound->size.x >> 17);
        hitbox.bottom = sound->size.y >> 17;
        hitbox.top    = -(sound->size.y >> 17);

        if (MathHelpers_PointInHitbox(sound->startPos.x - sound->cameraOffset.x, sound->startPos.y - sound->cameraOffset.y, slider->position.x,
                                      slider->position.y, FLIP_NONE, &hitbox)
            && slider->listID == 5)
            slider->sliderChangedCB = MenuSetup_OptionsVideo_UISlider_ChangedCB;
    }

    comp->processButtonInputCB = MenuSetup_VS_ProcessButtonCB;
    comp->menuSetupCB          = MenuSetup_VS_MenuSetupCB;

    if (comp->active == ACTIVE_ALWAYS) {
        RSDK_THIS(UIControl);
        self->childHasFocus = false;

        foreach_all(UIVsCharSelector, selector)
        {
            selector->isSelected      = true;
            selector->ready           = false;
            selector->processButtonCB = UIVsCharSelector_ProcessButtonCB;
        }
    }

    compRound->processButtonInputCB = MenuSetup_VS_Round_ProcessButtonCB;
    compRound->menuSetupCB          = MenuSetup_VS_Round_MenuSetupCB;
    if (compRound->active == ACTIVE_ALWAYS)
        MenuSetup_VS_Round_MenuSetupCB();

    compTotal->processButtonInputCB = MenuSetup_VS_Total_ProcessButtonCB;
    compTotal->menuSetupCB          = MenuSetup_VS_Total_MenuSetupCB;
    compTotal->menuUpdateCB         = MenuSetup_VS_Total_MenuUpdateCB;
    compTotal->targetPos.y          = compTotal->startPos.y;
    compTotal->position.y           = compTotal->startPos.y;
    if (compTotal->active == ACTIVE_ALWAYS)
        MenuSetup_VS_Total_MenuSetupCB();

    saveSel->menuUpdateCB = MenuSetup_SaveSel_MenuUpdateCB;
    saveSel->yPressCB     = MenuSetup_SaveSel_YPressCB;

    extras->processButtonInputCB = MenuSetup_Extras_ProcessButtonCB;

    options->menuSetupCB = MenuSetup_Options_MenuSetupCB;
    if (sku_platform == PLATFORM_DEV || sku_platform == PLATFORM_SWITCH)
        options->yPressCB = MenuSetup_Options_LaunchManual;
    else
        MenuSetup->optionsPrompt->visible = false;

    video_win->menuUpdateCB = MenuSetup_OptionsVideo_Win_MenuUpdateCB;

    foreach_all(UIVsZoneButton, zoneButton) { zoneButton->actionCB = MenuSetup_VS_StartMatch_ActionCB; }
}

void MenuSetup_HandleUnlocks(void)
{
    EntityUIControl *mainMenu = MenuSetup->mainMenu;

    EntityUIButton *taButton = mainMenu->buttons[1];
    taButton->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_TIMEATTACK);

    EntityUIButton *vsButton = mainMenu->buttons[2];
    vsButton->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_COMPETITION);

    foreach_all(UITAZoneModule, module) { module->disabled = !GameProgress_GetZoneUnlocked(module->zoneID); }

    int32 maxRounds = 0;
    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !GameProgress_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;
        if (!zoneButton->xOut)
            ++maxRounds;
    }

    EntityUIButton *matchLength = MenuSetup->competitionRules->buttons[1];
    if (matchLength->choiceCount >= 1) {
        EntityUIVsRoundPicker *vsRoundPicker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(matchLength, matchLength->selection);

        if (vsRoundPicker->classID == UIVsRoundPicker->classID) {
            vsRoundPicker->maxVal = maxRounds;
            vsRoundPicker->val    = MIN(vsRoundPicker->val, maxRounds);
        }
    }

    EntityUIControl *secrets  = MenuSetup->secrets;
    EntityUIButton *debugMode = secrets->buttons[0];
    debugMode->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_DEBUGMODE);
    if (debugMode->disabled)
        UIButton_ManageChoices(debugMode);

    EntityUIButton *sonicAbility      = secrets->buttons[1];
    EntityUIButton *peeloutChoice     = UIButton_GetChoicePtr(sonicAbility, 1);
    EntityUIButton *instaShieldChoice = UIButton_GetChoicePtr(sonicAbility, 2);

    sonicAbility->disabled = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_PEELOUT);
    if (sonicAbility->disabled)
        UIButton_ManageChoices(sonicAbility);

    peeloutChoice->disabled     = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_PEELOUT);
    instaShieldChoice->disabled = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_INSTASHIELD);

    EntityUIButton *andKnux = secrets->buttons[2];
    andKnux->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_ANDKNUX);
    if (andKnux->disabled)
        UIButton_ManageChoices(andKnux);

    EntityUIControl *extras = MenuSetup->extras;

    EntityUIButton *bssButton = extras->buttons[0];
    bssButton->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_BLUESPHERES);
    if (bssButton->disabled)
        UIButton_ManageChoices(bssButton);

    EntityUIButton *puyoButton = extras->buttons[1];
    puyoButton->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_MEANBEAN);
    if (puyoButton->disabled)
        UIButton_ManageChoices(puyoButton);

    EntityUIButton *daGardenButton = extras->buttons[2];
    daGardenButton->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_DAGARDEN) && !globals->medallionDebug;
}

void MenuSetup_HandleMenuReturn(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    char buffer[0x100];
    memset(buffer, 0, 0x100);
    if (strcmp(param->menuTag, "") == 0)
        UIUsernamePopup_ShowPopup();

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        MenuSetup_OptionsVideo_Win_MenuUpdateCB();

    foreach_all(UIControl, control)
    {
        if (strcmp(param->menuTag, "") != 0) {
            RSDK.GetCString(buffer, &control->tag);

            if (strcmp((const char *)buffer, param->menuTag) != 0) {
                UIControl_SetInactiveMenu(control);
            }
            else {
                UIControl_SetActiveMenu(control);
                control->buttonID = param->menuSelection;

                if (control == MenuSetup->timeAttackZones && param->startedTAAttempt)
                    UITAZoneModule_SetStartupModule(control, param->characterID, param->zoneID, param->actID, param->timeScore);
            }
        }

        if (control == MenuSetup->timeAttack && param->startedTAAttempt) {
            int32 charID                         = param->characterID - 1;
            control->buttonID                    = charID;
            control->buttons[charID]->isSelected = true;
        }

        if (control == MenuSetup->extras && param->puyoSelection == PUYO_SELECTION_VS_2P) {
            UIButton_SetChoiceSelection(control->buttons[1], 1);
        }

        if (control == MenuSetup->secrets) {
            EntityUIControl *control = MenuSetup->secrets;

            UIButton_SetChoiceSelection(control->buttons[0], (globals->medalMods & MEDAL_ANDKNUCKLES) != 0);

            int32 medals = globals->medalMods;
            if (medals & MEDAL_NODROPDASH) {
                if (medals & MEDAL_PEELOUT)
                    UIButton_SetChoiceSelection(control->buttons[1], 1);
                else if (medals & MEDAL_INSTASHIELD)
                    UIButton_SetChoiceSelection(control->buttons[1], 2);
            }
            else
                UIButton_SetChoiceSelection(control->buttons[1], 0);

            UIButton_SetChoiceSelection(control->buttons[2], (globals->medalMods & MEDAL_ANDKNUCKLES) != 0);
        }

        if (control == MenuSetup->video) {
            EntityUIControl *control = MenuSetup->video;
            EntityUIButton *button   = control->buttons[0];
            UIButton_SetChoiceSelection(button, RSDK.GetVideoSetting(VIDEOSETTING_SHADERID));
        }

        if (control == MenuSetup->sound) {
            EntityUIControl *control = MenuSetup->sound;

            EntityUISlider *slider = (EntityUISlider *)control->buttons[0];
            slider->sliderPos      = RSDK.GetVideoSetting(VIDEOSETTING_STREAM_VOL);

            slider            = (EntityUISlider *)control->buttons[1];
            slider->sliderPos = RSDK.GetVideoSetting(VIDEOSETTING_SFX_VOL);
        }

        if (control == MenuSetup->language) {
            EntityUIControl *control = MenuSetup->language;

            control->startingID = Localization->language;
            control->buttonID   = Localization->language;
        }

        EntityCompetitionSession *session = CompetitionSession_GetSession();
        if (session->inMatch) {
            if (control == MenuSetup->competition) {
                foreach_all(UIVsCharSelector, selector)
                {
                    selector->prevFrameID = -1;
                    switch (session->playerID[selector->playerID]) {
                        case ID_SONIC: selector->frameID = 0; break;
                        case ID_TAILS: selector->frameID = 1; break;
                        case ID_KNUCKLES: selector->frameID = 2; break;
                        default: break;
                    }
                }
            }

            if (control == MenuSetup->competitionRules) {
                switch (session->itemMode) {
                    case ITEMS_FIXED: UIButton_SetChoiceSelection(control->buttons[0], 0); break;
                    case ITEMS_RANDOM: UIButton_SetChoiceSelection(control->buttons[0], 2); break;
                    case ITEMS_TELEPORT: UIButton_SetChoiceSelection(control->buttons[0], 1); break;
                }

                EntityUIVsRoundPicker *vsRoundPicker =
                    (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(control->buttons[1], control->buttons[1]->selection);
                if (vsRoundPicker)
                    vsRoundPicker->val = session->matchCount;
            }

            if (control == MenuSetup->competitionZones) {
                for (int32 i = 0; i < COMPETITION_STAGE_COUNT; ++i) {
                    EntityUIVsZoneButton *button = (EntityUIVsZoneButton *)control->buttons[i];
                    if (button && session->completedStages[i])
                        button->xOut = true;
                }
            }
        }
    }

    TimeAttackData_Clear();
}

// Main Menu
int32 MenuSetup_GetActiveMenu(void)
{
    EntityUIControl *control = UIControl_GetUIControl();

    if (control == MenuSetup->mainMenu || control == MenuSetup->extras || control == MenuSetup->options || control == MenuSetup->video
        || control == MenuSetup->sound || control == MenuSetup->controls_win || control == MenuSetup->controls_KB
        || control == MenuSetup->controls_PS4 || control == MenuSetup->controls_XB1 || control == MenuSetup->controls_NX
        || control == MenuSetup->controls_NX_Grip || control == MenuSetup->controls_NX_JoyCon || control == MenuSetup->controls_NX_Pro) {
        return MAINMENU_MAIN;
    }

    if (control == MenuSetup->timeAttack || control == MenuSetup->timeAttackZones || control == MenuSetup->leaderboards
        || control == MenuSetup->competition || control == MenuSetup->competitionRules || control == MenuSetup->competitionZones) {
        return MAINMENU_TIMEATTACK;
    }

    if (control == MenuSetup->competitionRound || control == MenuSetup->competitionTotal)
        return MAINMENU_COMPETITION;

    if (control == MenuSetup->saveSelect || control == MenuSetup->noSaveMode || control == MenuSetup->secrets)
        return MAINMENU_SAVESELECT;

    return MAINMENU_MAIN;
}

void MenuSetup_ChangeMenuTrack(void)
{
    int32 trackID = 0;
    switch (MenuSetup_GetActiveMenu()) {
        default:
        case MAINMENU_MAIN: trackID = 0; break;
        case MAINMENU_TIMEATTACK: trackID = 1; break;
        case MAINMENU_COMPETITION: trackID = 2; break;
        case MAINMENU_SAVESELECT: trackID = 3; break;
    }

    if (!Music_IsPlaying())
        Music_PlayTrack(trackID);
    else if (Music->activeTrack != trackID)
        Music_TransitionTrack(trackID, 0.12);
}

void MenuSetup_SetBGColors(void)
{
    switch (MenuSetup_GetActiveMenu()) {
        case MAINMENU_MAIN: UIBackground->activeColors = UIBackground->bgColors; break;

        case MAINMENU_TIMEATTACK:
        case MAINMENU_COMPETITION: UIBackground->activeColors = &UIBackground->bgColors[3]; break;

        case MAINMENU_SAVESELECT: UIBackground->activeColors = &UIBackground->bgColors[6]; break;

        default: break;
    }
}

void MenuSetup_MenuButton_ActionCB(void)
{
    RSDK_THIS(UIModeButton);

    switch (self->buttonID) {
        case 0: // Mania Mode
            if (API_GetNoSave()) {
                UIControl_MatchMenuTag("No Save Mode");
            }
            else {
                MenuSetup->saveSelect->buttonID = 7;
                UIControl_MatchMenuTag("Save Select");
            }
            break;

        case 1: // Time Attack
            UIControl_MatchMenuTag("Time Attack");
            break;

        case 2: // Competition
            UIControl_MatchMenuTag("Competition");
            break;

        case 3: // Options
            UIControl_MatchMenuTag("Options");
            break;

        default: return;
    }
}

bool32 MenuSetup_BackPressCB_ReturnToTitle(void)
{
    MenuSetup_StartReturnToTitle();

    return true;
}

void MenuSetup_StartReturnToTitle(void)
{
    EntityUIControl *control = UIControl_GetUIControl();
    if (control)
        control->state = StateMachine_None;

    Music_FadeOut(0.05);
    MenuSetup_StartTransition(MenuSetup_ReturnToTitle, 32);
}

void MenuSetup_ReturnToTitle(void)
{
    TimeAttackData_Clear();

    RSDK.SetScene("Presentation", "Title Screen");
    RSDK.LoadScene();
}

void MenuSetup_ExitGame(void) { APICallback_ExitGame(); }

void MenuSetup_ExitGame_ActionCB(void)
{
    String string;
    INIT_STRING(string);

#if GAME_VERSION != VER_100
    Localization_GetString(&string, STR_QUITWARNING);
#else
    Localization_GetString(&string, STR_QUITWARNINGLOSEPROGRESS);
#endif
    UIDialog_CreateDialogYesNo(&string, MenuSetup_ExitGame_CB, NULL, true, true);
}

void MenuSetup_ExitGame_CB(void)
{
    if (UIControl_GetUIControl())
        UIControl_GetUIControl()->state = StateMachine_None;

    Music_FadeOut(0.02);
    MenuSetup_StartTransition(MenuSetup_ExitGame, 64);
}

void MenuSetup_State_HandleTransition(void)
{
    RSDK_THIS(MenuSetup);

    self->fadeTimer = CLAMP(self->timer << ((self->fadeShift & 0xFF) - 1), 0, 512);
}

// Save Select
int32 MenuSetup_GetMedalMods(void)
{
    EntityUIControl *control = MenuSetup->secrets;

    int32 mods = 0;
    if (control->buttons[0]->selection == 1)
        mods |= MEDAL_DEBUGMODE;

    if (control->buttons[1]->selection == 1) {
        mods |= MEDAL_NODROPDASH;
        mods |= MEDAL_PEELOUT;
    }
    else if (control->buttons[1]->selection == 2) {
        mods |= MEDAL_NODROPDASH;
        mods |= MEDAL_INSTASHIELD;
    }

    if (control->buttons[2]->selection == 1)
        mods |= MEDAL_ANDKNUCKLES;

    return mods;
}

void MenuSetup_OpenSaveSelectMenu(void)
{
    EntityUIControl *control = MenuSetup->saveSelect;

    UIControl_MatchMenuTag("Save Select");
    control->childHasFocus = false;
}

#if MANIA_USE_PLUS
void MenuSetup_SaveFileCB(bool32 success)
{
    UIWaitSpinner_FinishWait();
    RSDK.LoadScene();
}
#else
void MenuSetup_SaveFileCB(void)
{
    UIWaitSpinner_FinishWait();
    RSDK.LoadScene();
}
#endif

void MenuSetup_SaveSlot_ActionCB(void)
{
    RSDK_THIS(UISaveSlot);

    SaveRAM *saveRAM = (SaveRAM *)SaveGame_GetDataPtr(self->slotID);

    bool32 loadingSave = false;
    if (self->type) {
        // Bug Details(?):
        // sizeof(globals->noSaveSlot) and sizeof(saveData) is 4096 (sizeof(int32) * 0x400)
        // but the memset size is only 1024 (sizeof(uint8) * 0x400)
        // so only about 1/4th of the save slot is cleared, though nothin uses the extra space so it's not a big deal
        memset(globals->noSaveSlot, 0, 0x400);

        globals->tempFlags  = 0;
        globals->saveSlotID = NO_SAVE_SLOT;
        globals->gameMode   = MODE_NOSAVE;
        globals->medalMods  = MenuSetup_GetMedalMods();
    }
    else {
        globals->saveSlotID = self->slotID;
        globals->medalMods  = 0;
        globals->gameMode   = MODE_MANIA;

        if (self->isNewSave) {
            int32 *saveData = SaveGame_GetDataPtr(self->slotID % 8);

            // Bug Details(?):
            // see above
            memset(saveData, 0, 0x400);

            saveRAM->saveState     = 1;
            saveRAM->characterID   = self->frameID;
            saveRAM->zoneID        = 0;
            saveRAM->lives         = 3;
            saveRAM->collectedEmeralds = self->saveEmeralds;
            saveRAM->continues     = 0;

            UIWaitSpinner_StartWait();
            loadingSave = true;
            SaveGame_SaveFile(MenuSetup_SaveFileCB);
        }
        else {
            if (saveRAM->saveState == SAVEGAME_COMPLETE) {
                SaveGame_ClearCollectedSpecialRings();
                saveRAM->score                 = 0;
                saveRAM->score1UP              = 500000;
            }

            loadingSave = true;
            SaveGame_SaveFile(MenuSetup_SaveFileCB);
        }
    }

    switch (self->frameID) {
        case 0: // Sonic & Tails
        case 1: globals->playerID = ID_SONIC; break;

        case 2: globals->playerID = ID_TAILS; break;

        case 3: globals->playerID = ID_KNUCKLES; break;

        default: break;
    }

    if ((globals->medalMods & MEDAL_ANDKNUCKLES))
        globals->playerID |= ID_KNUCKLES_ASSIST;
    else if (!self->frameID)
        globals->playerID |= ID_TAILS_ASSIST;

    if (self->type == UISAVESLOT_NOSAVE || self->isNewSave) {
        if (((globals->medalMods & MEDAL_DEBUGMODE) && (ControllerInfo->keyC.down || ControllerInfo->keyX.down))
            && self->type == UISAVESLOT_NOSAVE)
            RSDK.SetScene("Presentation", "Level Select");
        else
            RSDK.SetScene("Cutscenes", "Angel Island Zone");
    }
    else {
        RSDK.SetScene("Mania Mode", "");
        SceneInfo->listPos += TimeAttackData_GetManiaListPos(self->saveZoneID, ACT_1, self->frameID);
    }

    if (!loadingSave) {
        globals->initCoolBonus = false;
        RSDK.LoadScene();
    }
}

void MenuSetup_SaveSel_MenuUpdateCB(void)
{
    EntityUIControl *control = MenuSetup->saveSelect;

    if (control->active == ACTIVE_ALWAYS) {
        EntityUIButtonPrompt *prompt = MenuSetup->delSavePrompt;

        // buttonID == 8: No Save Slot Button
        if (control->lastButtonID == 8) {
            prompt->promptID = 6;
            prompt->buttonID = 3;
        }
        else {
            prompt->promptID = 3;
            prompt->buttonID = 2;
        }
    }
}

void MenuSetup_OpenSecretsMenu(void)
{
    EntityUIControl *control = MenuSetup->secrets;
    control->childHasFocus   = false;

    UIControl_MatchMenuTag("Secrets");
}

void MenuSetup_SaveSel_YPressCB(void)
{
    EntityUIControl *control = MenuSetup->saveSelect;

    // buttonID == 8: No Save Slot Button
    if (control->active == ACTIVE_ALWAYS && control->buttonID == 8) {
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
        UIControl->inputLocked = true;

        UITransition_StartTransition(MenuSetup_OpenSecretsMenu, 0);
    }
}

void MenuSetup_TA_OpenZoneList_Sonic(void)
{
    EntityMenuParam *param = MenuParam_GetParam();
    TimeAttackData_Clear();

    param->characterID = 1;

    EntityUIControl *control = MenuSetup->timeAttackZones;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        button->characterID          = param->characterID;
    }

    UIControl_MatchMenuTag("Time Attack Zones");
}

void MenuSetup_TA_OpenZoneList_Tails(void)
{
    EntityMenuParam *param = MenuParam_GetParam();
    TimeAttackData_Clear();

    param->characterID = 2;

    EntityUIControl *control = MenuSetup->timeAttackZones;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        button->characterID          = param->characterID;
    }

    UIControl_MatchMenuTag("Time Attack Zones");
}

void MenuSetup_TA_OpenZoneList_Knux(void)
{
    EntityMenuParam *param = MenuParam_GetParam();
    TimeAttackData_Clear();

    param->characterID = 3;

    EntityUIControl *control = MenuSetup->timeAttackZones;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        button->characterID          = param->characterID;
    }

    UIControl_MatchMenuTag("Time Attack Zones");
}

void MenuSetup_TA_TAZoneModule_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_TA_StartAttempt, 32); }

void MenuSetup_TA_StartAttempt(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    sprintf(param->menuTag, "Time Attack Zones");
    param->menuSelection    = param->zoneID;
    param->startedTAAttempt = true;

    SaveGame_ResetPlayerState();

    // Bug Details(?):
    // sizeof(globals->noSaveSlot) and sizeof(saveData) is 4096 (sizeof(int32) * 0x400)
    // but the memset size is only 1024 (sizeof(uint8) * 0x400)
    // so only about 1/4th of the save slot is cleared, though nothin uses the extra space so it's not a big deal
    memset(globals->noSaveSlot, 0, 0x400);

    globals->continues  = 0;
    globals->saveSlotID = NO_SAVE_SLOT;
    globals->gameMode   = MODE_TIMEATTACK;
    globals->medalMods  = 0;

    RSDK.SetScene("Mania Mode", "");
    SceneInfo->listPos += TimeAttackData_GetManiaListPos(param->zoneID, param->actID, param->characterID);

    switch (param->characterID) {
        case 1: globals->playerID = ID_SONIC; break;
        case 2: globals->playerID = ID_TAILS; break;
        case 3: globals->playerID = ID_KNUCKLES; break;
        default: break;
    }

    RSDK.LoadScene();
}

void MenuSetup_TA_Leaderboards_TransitionCB(void)
{
    EntityUIControl *leaderboardControl = MenuSetup->leaderboards;
    EntityUILeaderboard *leaderboards   = MenuSetup->leaderboardWidget;

    UIControl->forceBackPress   = false;
    leaderboardControl->active  = ACTIVE_NEVER;
    leaderboardControl->visible = false;
    leaderboardControl->state   = StateMachine_None;

    UIControl_SetActiveMenu(MenuSetup->timeAttackZones);
    UITAZoneModule_SetStartupModule(MenuSetup->timeAttackZones, leaderboards->characterID, leaderboards->zoneID, leaderboards->actID, 0);
}

bool32 MenuSetup_TA_Leaderboards_BackPressCB(void)
{
    UITransition_StartTransition(MenuSetup_TA_Leaderboards_TransitionCB, 0);

    return true;
}

void MenuSetup_TA_Leaderboards_YPressCB(void)
{
    EntityUILeaderboard *leaderboards = MenuSetup->leaderboardWidget;

    UITAZoneModule_ShowLeaderboards(leaderboards->characterID, leaderboards->zoneID, leaderboards->actID, leaderboards->viewingUserRank, NULL);
}

void MenuSetup_State_SetupLeaderboards(void)
{
    RSDK_THIS(MenuSetup);

    EntityUIDialog *dialog           = MenuSetup->connectingDlg;
    EntityUILeaderboard *leaderboard = MenuSetup->leaderboardWidget;
    EntityUIButtonPrompt *prompt     = MenuSetup->leaderboardPrompt;

    int32 status = APICallback_LeaderboardStatus();

    if (status == STATUS_CONTINUE) {
        String message;
        INIT_STRING(message);
        RSDK.InitString(&message, "", false);

        Localization_GetString(&message, STR_CONNECTING);
        UIDialog_SetupText(dialog, &message);
    }
    else if (status >= STATUS_ERROR) {
        status = APICallback_LeaderboardStatus();

        int32 strID = status == STATUS_TIMEOUT ? STR_COMMERROR : STR_NOWIFI;

        String message;
        INIT_STRING(message);
        RSDK.InitString(&message, "", false);
        Localization_GetString(&message, strID);

        UIDialog_SetupText(dialog, &message);
        UIDialog_AddButton(DIALOG_OK, dialog, NULL, true);

        EntityUIControl *parent = dialog->parent;
        parent->rowCount        = 1;
        parent->columnCount     = 1;
        parent->buttonID        = 0;

        if (leaderboard->taRecord)
            leaderboard->viewingUserRank = !leaderboard->viewingUserRank;

        prompt->promptID   = leaderboard->viewingUserRank ? 14 : 15;
        prompt->prevPrompt = -1;
        prompt->visible    = leaderboard->taRecord != 0;

        self->callback           = StateMachine_None;
        MenuSetup->connectingDlg = NULL;
        destroyEntity(self);
    }
    else if (status == STATUS_OK) {
        prompt->promptID   = leaderboard->viewingUserRank ? 14 : 15;
        prompt->prevPrompt = -1;
        prompt->visible    = leaderboard->taRecord != 0;

        UILeaderboard_InitLeaderboard(leaderboard);
        UIDialog_CloseOnSel_HandleSelection(dialog, self->callback);

        self->callback           = StateMachine_None;
        MenuSetup->connectingDlg = NULL;
        destroyEntity(self);
    }
}

// Competition
void MenuSetup_VS_OpenCompRules(void) { UIControl_MatchMenuTag("Competition Rules"); }
void MenuSetup_VS_OpenCompZones(void) { UIControl_MatchMenuTag("Competition Zones"); }

void MenuSetup_VS_ProcessButtonCB(void)
{
    EntityUIControl *control = MenuSetup->competition;

    if (control) {
        bool32 allPlayersReady = true;

        foreach_all(UIVsCharSelector, charSel)
        {
            Entity *entStore  = SceneInfo->entity;
            SceneInfo->entity = (Entity *)charSel;
            StateMachine_Run(charSel->processButtonCB);
            SceneInfo->entity = entStore;

            if (allPlayersReady)
                allPlayersReady = charSel->ready ? !charSel->isSelected : false;
        }

        if (allPlayersReady) {
            control->selectionDisabled = true;
            UITransition_StartTransition(MenuSetup_VS_OpenCompRules, 0);
        }
    }
}

void MenuSetup_VS_MenuSetupCB(void)
{
    RSDK_THIS(UIControl);

    self->childHasFocus = false;

    foreach_all(UIVsCharSelector, charSel)
    {
        charSel->isSelected      = true;
        charSel->ready           = false;
        charSel->processButtonCB = UIVsCharSelector_ProcessButtonCB;
    }
}

void MenuSetup_VS_StartMatch(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    EntityMenuParam *param            = MenuParam_GetParam();

    sprintf(param->menuTag, "Competition Round");
    session->stageIndex = MenuSetup->competitionZones->buttonID;
    session->zoneID     = param->vsZoneID;
    session->actID      = param->vsActID;

    RSDK.SetScene("Mania Mode", "");
    SceneInfo->listPos += TimeAttackData_GetManiaListPos(param->vsZoneID, param->vsActID, CHAR_SONIC);

    SaveGame_ResetPlayerState();

    // Bug Details(?):
    // sizeof(globals->noSaveSlot) and sizeof(saveData) is 4096 (sizeof(int32) * 0x400)
    // but the memset size is only 1024 (sizeof(uint8) * 0x400)
    // so only about 1/4th of the save slot is cleared, though nothin uses the extra space so it's not a big deal
    memset(globals->noSaveSlot, 0, 0x400);

    globals->continues  = 0;
    globals->saveSlotID = NO_SAVE_SLOT;
    globals->gameMode   = MODE_COMPETITION;
    globals->medalMods  = 0;

    globals->playerID = ID_NONE;
    for (int32 i = 0; i < COMPETITION_PLAYER_COUNT; ++i) globals->playerID |= session->playerID[i] << (8 * i);

    globals->itemMode = session->itemMode;

    RSDK.LoadScene();
}

void MenuSetup_VS_StartMatch_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_VS_StartMatch, 32); }

void MenuSetup_VS_RulesButton_ActionCB(void)
{
    EntityUIControl *compControl      = MenuSetup->competition;
    EntityUIControl *rulesControl     = MenuSetup->competitionRules;
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    int32 matchCount = 0;
    foreach_all(UIVsRoundPicker, vsRoundPicker)
    {
        matchCount = vsRoundPicker->val;
        foreach_break;
    }

    int32 itemMode = ITEMS_FIXED;
    switch (rulesControl->buttons[0]->selection) {
        default: break;
        case 0: itemMode = ITEMS_FIXED; break;
        case 1: itemMode = ITEMS_TELEPORT; break;
        case 2: itemMode = ITEMS_RANDOM; break;
    }

    CompetitionSession_ResetOptions();

    session->matchCount = matchCount;
    session->inMatch    = true;
    session->itemMode   = itemMode;

    session->playerCount = 2;

    EntityUIVsCharSelector *charSel = (EntityUIVsCharSelector *)compControl->buttons[0];
    switch (charSel->frameID) {
        case 0: session->playerID[0] = ID_SONIC; break;
        case 1: session->playerID[0] = ID_TAILS; break;
        case 2: session->playerID[0] = ID_KNUCKLES; break;
        default: break;
    }

    charSel = (EntityUIVsCharSelector *)compControl->buttons[1];
    switch (charSel->frameID) {
        case 0: session->playerID[1] = ID_SONIC; break;
        case 1: session->playerID[1] = ID_TAILS; break;
        case 2: session->playerID[1] = ID_KNUCKLES; break;
        default: break;
    }

    EntityUIControl *zoneControl = MenuSetup->competitionZones;
    zoneControl->position        = zoneControl->startPos;
    zoneControl->targetPos.x     = zoneControl->startPos.x;
    zoneControl->targetPos.y     = zoneControl->startPos.y;
    zoneControl->buttonID        = 0;

    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !GameProgress_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;
    }

    UIControl_MatchMenuTag("Competition Zones");
}

void MenuSetup_VS_OpenCompTotal(void) { UIControl_MatchMenuTag("Competition Total"); }

void MenuSetup_VS_Round_ProcessButtonCB(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    if (UIControl->confirmPress[0] || UIControl->confirmPress[1] || UIControl->confirmPress[2] || UIControl->confirmPress[3]) {
        bool32 toCompTotal = false;

        int32 activePlayers = 0;
        for (int32 p = 0; p < COMPETITION_PLAYER_COUNT; ++p) {
            if (session->lives[p] > 0)
                activePlayers++;

            if (session->wins[p] > (session->matchCount >> 1))
                toCompTotal = true;
        }

        if (toCompTotal || activePlayers < 2 || session->matchID >= session->matchCount)
            UITransition_StartTransition(MenuSetup_VS_OpenCompTotal, 0);
        else
            UITransition_StartTransition(MenuSetup_VS_OpenCompZones, 0);

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;
    }
}

void MenuSetup_VS_Round_MenuSetupCB(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    EntityUIControl *roundControl     = MenuSetup->competitionRound;

    int32 matchWinner = session->matchWinner[session->matchID - 1];

    int32 winner = 0;
    if (matchWinner > 1)
        winner = matchWinner - 1;

    String roundLabelStr;
    INIT_STRING(roundLabelStr);

    char roundLabel[0x100];
    memset(roundLabel, 0, sizeof(roundLabel));

    Localization_GetZoneName(&roundLabelStr, session->zoneID);
    if (session->zoneID < 12) {
        char buf[16];
        sprintf(buf, " ZONE %d", session->actID + 1);
        RSDK.AppendText(&roundLabelStr, buf);
    }

    RSDK.GetCString(roundLabel, &roundLabelStr);
    UIInfoLabel_SetText(MenuSetup->roundLabel, roundLabel);

    EntityUIVsScoreboard *scoreboard = MenuSetup->roundScoreboard;
    UIVsScoreboard_SetScores(scoreboard, session->wins[0], session->wins[1]);
    scoreboard->showWinner = matchWinner != 0;
    scoreboard->winnerID   = winner;

    if (matchWinner) {
        LogHelpers_Print("Announce_CharWins(%d)", session->playerID[winner]);

        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceWinPlayer;
        announcer->playerID        = session->playerID[winner];
    }

    uint32 times[PLAYER_COUNT];
    for (int32 p = 0; p < COMPETITION_PLAYER_COUNT; ++p) {
        times[p] = session->time[p].milliseconds + 100 * (session->time[p].seconds + 4 * (16 * session->time[p].minutes - session->time[p].minutes));
    }

    char buffer[0x40];
    foreach_all(UIVsResults, results)
    {
        int32 x = roundControl->startPos.x - roundControl->cameraOffset.x;
        int32 y = roundControl->startPos.y - roundControl->cameraOffset.y;

        Hitbox hitbox;
        hitbox.left   = -(roundControl->size.x >> 17);
        hitbox.top    = -(roundControl->size.y >> 17);
        hitbox.right  = roundControl->size.x >> 17;
        hitbox.bottom = roundControl->size.y >> 17;

        if (MathHelpers_PointInHitbox(x, y, results->position.x, results->position.y, FLIP_NONE, &hitbox)) {
            int32 p = results->playerID;
            int32 r = results->playerID ^ 1;

            results->isWinner = p == winner && scoreboard->showWinner;

            memset(buffer, 0, sizeof(buffer));

            sprintf(buffer, "%d", session->rings[p]);
            if (!SceneInfo->inEditor) {
                RSDK.InitString(&results->rowText[0], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[0]);
            }

            sprintf(buffer, "%d", session->totalRings[p]);
            if (!SceneInfo->inEditor) {
                RSDK.InitString(&results->rowText[1], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[1]);
            }

            sprintf(buffer, "%d", session->score[p]);
            if (!SceneInfo->inEditor) {
                RSDK.InitString(&results->rowText[2], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[2]);
            }

            sprintf(buffer, "%d", session->items[p]);
            if (!SceneInfo->inEditor) {
                RSDK.InitString(&results->rowText[3], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[3]);
            }

            sprintf(buffer, "%d'%02d\"%02d", session->time[p].minutes, session->time[p].seconds, session->time[p].milliseconds);
            if (!SceneInfo->inEditor) {
                RSDK.InitString(&results->rowText[4], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[4]);
            }

            if (session->lives[p]) {
                if (session->lives[r]) {
                    results->row0Highlight = session->rings[p] > session->rings[r];
                    results->row1Highlight = session->totalRings[p] > session->totalRings[r];
                    results->row2Highlight = session->score[p] > session->score[r];
                    results->row3Highlight = session->items[p] > session->items[r];
                    results->row4Highlight = times[p] > 0 && times[p] < times[r];
                }
                else {
                    results->row0Highlight = true;
                    results->row1Highlight = true;
                    results->row2Highlight = true;
                    results->row3Highlight = true;
                    results->row4Highlight = true;
                }
            }
        }
    }
}

void MenuSetup_VS_GotoCompetition(void) { UIControl_MatchMenuTag("Competition"); }

void MenuSetup_VS_Total_ProcessButtonCB(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    if (UIControl->anyConfirmPress) {
        int32 mostWins = 0;
        for (int32 p = 0; p < session->playerCount; ++p) {
            if (session->wins[p] > mostWins)
                mostWins = session->wins[p];
        }

        int32 winnerCount = 0;
        for (int32 p = 0; p < session->playerCount; ++p) {
            if (session->wins[p] == mostWins)
                winnerCount++;
        }

        if (winnerCount > 1) {
            MenuSetup_StartTransition(MenuSetup_VS_StartPuyoMatch, 32);
        }
        else {
            CompetitionSession_ResetOptions();
            UITransition_StartTransition(MenuSetup_VS_GotoCompetition, 0);
        }

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;
    }
}

void MenuSetup_VS_Total_MenuSetupCB(void)
{
    EntityUIControl *totalControl     = MenuSetup->competitionTotal;
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    MenuSetup->vsTotalTimer = 120;

    totalControl->targetPos.y = totalControl->startPos.y;
    totalControl->position.y  = totalControl->startPos.y;

    String string;
    INIT_STRING(string);

    EntityUIInfoLabel *label = MenuSetup->totalLabel;
    Localization_GetString(&string, STR_COMPTOTAL);
    UIInfoLabel_SetString(label, &string);

    int32 highestScore = 0;
    for (int32 p = 0; p < session->playerCount; ++p) {
        if (session->wins[p] > highestScore)
            highestScore = session->wins[p];
    }

    EntityUIVsScoreboard *scoreboard = MenuSetup->roundScoreboard;
    UIVsScoreboard_SetScores(scoreboard, session->wins[0], session->wins[1]);
    scoreboard->showWinner = session->wins[0] != session->wins[1];
    scoreboard->winnerID   = session->wins[1] > session->wins[0];

    if (scoreboard->showWinner) {
        LogHelpers_Print("Announce_WinnerIs(%d)", scoreboard->winnerID);
        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceWinner;
        announcer->playerID        = scoreboard->winnerID;
    }

    foreach_all(UIVsResults, results)
    {
        int32 x = totalControl->startPos.x - totalControl->cameraOffset.x;
        int32 y = totalControl->startPos.y - totalControl->cameraOffset.y;

        Hitbox hitbox;
        hitbox.left   = -(totalControl->size.x >> 17);
        hitbox.top    = -(totalControl->size.y >> 17);
        hitbox.right  = totalControl->size.x >> 17;
        hitbox.bottom = totalControl->size.y >> 17;

        if (MathHelpers_PointInHitbox(x, y, results->position.x, results->position.y, FLIP_NONE, &hitbox)) {
            bool32 *highlight = &results->row0Highlight;

            results->numRows  = session->matchCount;
            results->isWinner = results->playerID == scoreboard->winnerID;

            for (int32 r = 0; r < results->numRows; ++r) {
                char buffer[0x40];
                sprintf(buffer, "%d", session->matchWinner[r]);

                if (!SceneInfo->inEditor) {
                    RSDK.InitString(&results->rowText[r], buffer, 0);
                    RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[r]);
                }

                highlight[r] = false;
                if (session->matchWinner[r])
                    highlight[r] = (session->matchWinner[r] - 1) == results->playerID;
            }
        }
    }
}

void MenuSetup_VS_Total_MenuUpdateCB(void)
{
    EntityUIControl *totalControl = MenuSetup->competitionTotal;

    if (totalControl->active == ACTIVE_ALWAYS) {
        if (totalControl->position.y == totalControl->targetPos.y) {
            if (MenuSetup->vsTotalTimer <= 0) {
                int32 targetPos = totalControl->startPos.y;
                if (totalControl->targetPos.y == totalControl->startPos.y) {
                    EntityUIVsResults *button = (EntityUIVsResults *)totalControl->buttons[0];

                    if (button && button->size.y + button->position.y - 0x708000 > totalControl->startPos.y) {
                        targetPos = button->size.y + button->position.y - 0x708000;
                    }
                }

                totalControl->targetPos.y = targetPos;
                MenuSetup->vsTotalTimer   = 120;
            }
            else {
                MenuSetup->vsTotalTimer--;
            }
        }
    }
}

void MenuSetup_VS_ExitComp_TransitionCB(void)
{
    EntityUIControl *control     = MenuSetup->competition;
    EntityUIControl *zoneControl = MenuSetup->competitionZones;

    UIControl_SetInactiveMenu(zoneControl);
    UIControl_SetActiveMenu(control);
    CompetitionSession_ResetOptions();

    zoneControl->childHasFocus = false;

    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !GameProgress_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;
    }
}

void MenuSetup_VS_BackoutFromVsCharSelect_CB(void)
{
    EntityUIControl *control   = UIControl_GetUIControl();
    control->selectionDisabled = false;

    UIControl_HandleMenuChange(&control->parentTag);
}

bool32 MenuSetup_VS_BackoutFromVsCharSelect(void)
{
    LogHelpers_Print("BackoutFromVsCharSelect()");
    EntityUIControl *control = MenuSetup->competition;

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];
        if (button->classID == UIVsCharSelector->classID && button->prevSelected && UIControl->backPress[button->playerID]) {
            UITransition_StartTransition(MenuSetup_VS_BackoutFromVsCharSelect_CB, 0);
            return true;
        }
    }

    if (!control->buttonCount) {
        control->selectionDisabled = true;

        if (!UIControl->backPress[0] && UIControl->anyBackPress) {
            UITransition_StartTransition(MenuSetup_VS_BackoutFromVsCharSelect_CB, 0);
            return true;
        }
    }

    return false;
}

void MenuSetup_VS_ExitComp_YesCB(void) { UITransition_StartTransition(MenuSetup_VS_ExitComp_TransitionCB, 0); }

bool32 MenuSetup_VS_CompZones_BackPressCB(void)
{
    String message;
    INIT_STRING(message);

    Localization_GetString(&message, STR_EXITCOMP);
    UIDialog_CreateDialogYesNo(&message, MenuSetup_VS_ExitComp_YesCB, NULL, true, true);

    return true;
}

void MenuSetup_VS_StartPuyoMatch(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();

    param->puyoSelection = PUYO_SELECTION_TIE_BREAKER;
    globals->gameMode    = MODE_COMPETITION;
    strcpy(param->menuTag, "Competition Total");
    param->menuSelection = 0;

    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

// Options
void MenuSetup_Options_VideoMenuButton_ActionCB(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        UIControl_MatchMenuTag("Video WIN");
    else
        UIControl_MatchMenuTag("Video");
}

void MenuSetup_Options_SoundMenuButton_ActionCB(void) { UIControl_MatchMenuTag("Sound"); }

void MenuSetup_Options_LanguageMenuButton_ActionCB(void) { UIControl_MatchMenuTag("Language"); }

void MenuSetup_Options_ControlsMenuButton_ActionCB(void)
{
    int32 id   = API_GetFilteredInputDeviceID(INPUT_NONE);
    int32 type = API_GetInputDeviceType(id);

    switch (sku_platform) {
        case PLATFORM_DEV:
        case PLATFORM_PC:
            if (sku_platform == PLATFORM_DEV || type)
                UIControl_MatchMenuTag("Controls WIN");
            else
                UIControl_MatchMenuTag("Controls XB1");
            break;

        case PLATFORM_PS4: UIControl_MatchMenuTag("Controls PS4"); break;

        case PLATFORM_XB1: UIControl_MatchMenuTag("Controls XB1"); break;

        case PLATFORM_SWITCH:
            switch (type) {
                case 1: UIControl_MatchMenuTag("Controls NX"); break;
                case 2: UIControl_MatchMenuTag("Controls NX Grip"); break;
                case 3: UIControl_MatchMenuTag("Controls NX Joycon"); break;
                case 4: UIControl_MatchMenuTag("Controls NX Pro"); break;
                default: break;
            }
            break;
    }
}

void MenuSetup_Options_SetDefaultMappings_P1(void)
{
    ControllerInfo[CONT_P1].keyUp.keyMap     = KEYMAP_UP;
    ControllerInfo[CONT_P1].keyDown.keyMap   = KEYMAP_DOWN;
    ControllerInfo[CONT_P1].keyLeft.keyMap   = KEYMAP_LEFT;
    ControllerInfo[CONT_P1].keyRight.keyMap  = KEYMAP_RIGHT;
    ControllerInfo[CONT_P1].keyA.keyMap      = KEYMAP_A;
    ControllerInfo[CONT_P1].keyB.keyMap      = KEYMAP_S;
    ControllerInfo[CONT_P1].keyC.keyMap      = KEYMAP_NO_MAPPING;
    ControllerInfo[CONT_P1].keyX.keyMap      = KEYMAP_Q;
    ControllerInfo[CONT_P1].keyY.keyMap      = KEYMAP_W;
    ControllerInfo[CONT_P1].keyZ.keyMap      = KEYMAP_NO_MAPPING;
    ControllerInfo[CONT_P1].keyStart.keyMap  = KEYMAP_RETURN;
    ControllerInfo[CONT_P1].keySelect.keyMap = KEYMAP_TAB;
}

void MenuSetup_Options_SetDefaultMappings_P2(void)
{
#if GAME_VERSION != VER_100
    ControllerInfo[CONT_P2].keyUp.keyMap     = KEYMAP_NUMPAD8;
    ControllerInfo[CONT_P2].keyDown.keyMap   = KEYMAP_NUMPAD5;
    ControllerInfo[CONT_P2].keyLeft.keyMap   = KEYMAP_NUMPAD4;
    ControllerInfo[CONT_P2].keyRight.keyMap  = KEYMAP_NUMPAD6;
    ControllerInfo[CONT_P2].keyA.keyMap      = KEYMAP_J;
    ControllerInfo[CONT_P2].keyB.keyMap      = KEYMAP_K;
    ControllerInfo[CONT_P2].keyC.keyMap      = KEYMAP_NO_MAPPING;
    ControllerInfo[CONT_P2].keyX.keyMap      = KEYMAP_U;
    ControllerInfo[CONT_P2].keyY.keyMap      = KEYMAP_I;
    ControllerInfo[CONT_P2].keyZ.keyMap      = KEYMAP_NO_MAPPING;
    ControllerInfo[CONT_P2].keyStart.keyMap  = KEYMAP_OEM_4;
    ControllerInfo[CONT_P2].keySelect.keyMap = KEYMAP_OEM_6;
#else
    ControllerInfo[CONT_P2].keyUp.keyMap     = KEYMAP_I;
    ControllerInfo[CONT_P2].keyDown.keyMap   = KEYMAP_K;
    ControllerInfo[CONT_P2].keyLeft.keyMap   = KEYMAP_J;
    ControllerInfo[CONT_P2].keyRight.keyMap  = KEYMAP_L;
    ControllerInfo[CONT_P2].keyA.keyMap      = KEYMAP_V;
    ControllerInfo[CONT_P2].keyB.keyMap      = KEYMAP_B;
    ControllerInfo[CONT_P2].keyC.keyMap      = KEYMAP_NO_MAPPING;
    ControllerInfo[CONT_P2].keyX.keyMap      = KEYMAP_F;
    ControllerInfo[CONT_P2].keyY.keyMap      = KEYMAP_G;
    ControllerInfo[CONT_P2].keyZ.keyMap      = KEYMAP_NO_MAPPING;
    ControllerInfo[CONT_P2].keyStart.keyMap  = KEYMAP_O;
    ControllerInfo[CONT_P2].keySelect.keyMap = KEYMAP_NO_MAPPING;
#endif
}

void MenuSetup_Options_SetupKBControlsMenu(int32 playerID)
{
    EntityUIControl *control = MenuSetup->controls_KB;

    foreach_all(UISubHeading, subHeading)
    {
        Hitbox hitbox;
        int32 x = control->startPos.x - control->cameraOffset.x;
        int32 y = control->startPos.y - control->cameraOffset.y;

        hitbox.right  = control->size.x >> 17;
        hitbox.left   = -(control->size.x >> 17);
        hitbox.bottom = control->size.y >> 17;
        hitbox.top    = -(control->size.y >> 17);

        if (MathHelpers_PointInHitbox(x, y, subHeading->position.x, subHeading->position.y, FLIP_NONE, &hitbox)) {
            subHeading->frameID = playerID + 8;
            foreach_break;
        }
    }

    for (int32 b = 0; b < control->buttonCount; ++b) {
        if (control->buttons[b]->classID == UIKeyBinder->classID) {
            EntityUIKeyBinder *binder = (EntityUIKeyBinder *)control->buttons[b];
            binder->inputID = playerID;
        }
        else if (control->buttons[b]->classID == UIButton->classID) {
            EntityUIButton *button = control->buttons[b];

            if (playerID == 1)
                button->actionCB = MenuSetup_Options_SetDefaultMappings_P2;
            else if (playerID == 0)
                button->actionCB = MenuSetup_Options_SetDefaultMappings_P1;
        }
    }
}

void MenuSetup_OptionsVideo_Win_MenuUpdateCB(void) { MenuSetup_OptionsVideo_Win_InitVideoOptionsMenu(); }

void MenuSetup_OptionsVideo_Win_InitVideoOptionsMenu(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        EntityUIControl *videoControl_Win = MenuSetup->video_win;
        OptionsRAM *optionsRAM            = Options_GetOptionsRAM();
        Options_GetWinSize();

        int32 options[7];

#if GAME_VERSION != VER_100
        options[0] = RSDK.GetVideoSetting(VIDEOSETTING_SHADERID); // filter
        options[1] = optionsRAM->windowSize;                      // window size
        options[2] = RSDK.GetVideoSetting(VIDEOSETTING_BORDERED); // bordered

        options[3] = 0;
        if (!RSDK.GetVideoSetting(VIDEOSETTING_WINDOWED) || optionsRAM->windowSize == 4)
            options[3] = 1;

        options[4] = 0;                                                 // fullscreen res
        options[5] = RSDK.GetVideoSetting(VIDEOSETTING_VSYNC);          // vsync
        options[6] = RSDK.GetVideoSetting(VIDEOSETTING_TRIPLEBUFFERED); // triple buffered
#else
        options[0] = RSDK.GetVideoSetting(VIDEOSETTING_SHADERID); // filter
        options[1] = optionsRAM->windowSize;                      // window size
        options[2] = RSDK.GetVideoSetting(VIDEOSETTING_BORDERED); // bordered

        options[3] = 0;
        if (!RSDK.GetVideoSetting(VIDEOSETTING_WINDOWED) || optionsRAM->windowSize == 4)
            options[3] = 1;

        options[4] = RSDK.GetVideoSetting(VIDEOSETTING_VSYNC);          // vsync
        options[5] = RSDK.GetVideoSetting(VIDEOSETTING_TRIPLEBUFFERED); // triple buffered
#endif

        for (int32 i = 0; i < videoControl_Win->buttonCount; ++i) {
            EntityUIButton *button = videoControl_Win->buttons[i];

#if GAME_VERSION != VER_100
            if (i == 4) {
                EntityUIResPicker *resPicker = (EntityUIResPicker *)UIButton_GetChoicePtr(button, button->selection);
                UIResPicker_GetDisplayInfo(resPicker);
            }
            else if (i == 1) {
                EntityUIWinSize *winSize = (EntityUIWinSize *)UIButton_GetChoicePtr(button, button->selection);
                winSize->selection       = RSDK.GetVideoSetting(VIDEOSETTING_WINDOW_HEIGHT) / SCREEN_YSIZE;
            }
            else if (button->selection != options[i]) {
                UIButton_SetChoiceSelection(button, options[i]);
            }
#else
            if (button->selection != options[i]) {
                UIButton_SetChoiceSelection(button, options[i]);
            }
#endif
        }
    }
}

void MenuSetup_Options_OpenKBControlsMenu(void)
{
    RSDK_THIS(UIButton);

    EntityUIControl *control = MenuSetup->controls_win;

    for (int32 i = 0; i < control->buttonCount; ++i) {
        if (self == control->buttons[i]) {
            MenuSetup_Options_SetupKBControlsMenu(i);
            UIControl_MatchMenuTag("Controls KB");
            break;
        }
    }
}

void MenuSetup_Options_MenuSetupCB(void)
{
    if (Options->changed) {
        UIWaitSpinner_StartWait();

        Options_SaveFile(MenuSetup_Options_SaveOptionsCB_Load);
    }
}

void MenuSetup_Options_SaveOptionsCB_Load(void) { UIWaitSpinner_FinishWait(); }

void MenuSetup_Options_LaunchManual(void)
{
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
    API_LaunchManual();
}

void MenuSetup_OptionsLanguage_LanguageButton_ActionCB(void)
{
    OptionsRAM *options      = Options_GetOptionsRAM();
    EntityUIControl *control = MenuSetup->language;

    if (control->buttonID < 0)
        options->language = -1;
    else
        options->language = control->buttonID;

    options->overrideLanguage = control->buttonID >= 0;

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        RSDK.SetVideoSetting(VIDEOSETTING_LANGUAGE, control->buttonID);

    Options->changed = true;

    Localization->language = control->buttonID;
    control->startingID    = control->buttonID;
    Localization->loaded   = false;

    Localization_LoadStrings();
    UIWidgets_ApplyLanguage();
    UIHeading_LoadSprites();

    UIControl_MatchMenuTag("Options");
}

void MenuSetup_OptionsVideo_ShaderButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    OptionsRAM *options = Options_GetOptionsRAM();

    options->screenShader   = self->selection;
    options->overrideShader = true;
    RSDK.SetVideoSetting(VIDEOSETTING_SHADERID, self->selection);

    Options->changed = true;
}

void MenuSetup_OptionsVideo_WindowScaleButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    OptionsRAM *options = Options_GetOptionsRAM();

    if (self->selection != 4) {
        RSDK.SetVideoSetting(VIDEOSETTING_WINDOW_WIDTH, WIDE_SCR_XSIZE * (self->selection + 1));
        RSDK.SetVideoSetting(VIDEOSETTING_WINDOW_HEIGHT, SCREEN_YSIZE * (self->selection + 1));

        options->windowSize = self->selection;
        Options->changed    = true;
    }
}

void MenuSetup_OptionsVideo_BorderlessButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    OptionsRAM *options = Options_GetOptionsRAM();

    options->windowBorder = self->selection;
    RSDK.SetVideoSetting(VIDEOSETTING_BORDERED, self->selection);

    RSDK.UpdateWindow();
    Options->changed = true;
}

void MenuSetup_OptionsVideo_FullscreenButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    OptionsRAM *options = Options_GetOptionsRAM();

    options->windowed = self->selection ^ 1;
    RSDK.SetVideoSetting(VIDEOSETTING_WINDOWED, options->windowed);

    RSDK.UpdateWindow();
    Options->changed = true;
}

void MenuSetup_OptionsVideo_VSyncButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    OptionsRAM *options = Options_GetOptionsRAM();

    options->vSync = self->selection;
    RSDK.SetVideoSetting(VIDEOSETTING_VSYNC, self->selection);

    Options->changed = true;
}

void MenuSetup_OptionsVideo_TripleBufferButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    OptionsRAM *options = Options_GetOptionsRAM();

    options->tripleBuffering = self->selection;
    RSDK.SetVideoSetting(VIDEOSETTING_TRIPLEBUFFERED, self->selection);

    Options->changed = true;
}

void MenuSetup_OptionsVideo_UISlider_ChangedCB(void)
{
    RSDK_THIS(UISlider);

    OptionsRAM *options = Options_GetOptionsRAM();

    // Bug Details (?):
    // what the hell is up with this???????
    // it'd only ever be 0 or 1 why are 0xF1, 0xF2, 0xF4, 0xF5 & 0xFC options?????
    // this is a CB for the slider why are the boolean values here???

    bool32 value = self->frameID != 1;
    switch (value) {
        case 0xF1:
            options->windowed = self->sliderPos;
            RSDK.SetVideoSetting(VIDEOSETTING_WINDOWED, options->windowed);
            break;

        case 0xF2:
            options->windowBorder = self->sliderPos;
            RSDK.SetVideoSetting(VIDEOSETTING_BORDERED, options->windowBorder);
            break;

        case 0xF4:
            options->vSync = self->sliderPos;
            RSDK.SetVideoSetting(VIDEOSETTING_VSYNC, options->vSync);
            break;

        case 0xF5:
            options->tripleBuffering = self->sliderPos;
            RSDK.SetVideoSetting(VIDEOSETTING_TRIPLEBUFFERED, options->tripleBuffering);
            break;

        case 0xFC:
            options->screenShader   = self->sliderPos;
            options->overrideShader = true;
            RSDK.SetVideoSetting(VIDEOSETTING_SHADERID, options->screenShader);

            RSDK.UpdateWindow();
            break;

        case 0:
            options->volMusic         = self->sliderPos;
            options->overrideMusicVol = true;
            RSDK.SetVideoSetting(VIDEOSETTING_STREAM_VOL, options->volMusic);
            break;

        case 1:
            options->volSfx         = self->sliderPos;
            options->overrideSfxVol = true;
            RSDK.SetVideoSetting(VIDEOSETTING_SFX_VOL, options->volSfx);
            break;

        default: break;
    }
    Options->changed = true;
}

void MenuSetup_OpenExtrasMenu_ActionCB(void) { UIControl_MatchMenuTag("Extras"); }

// Extras
void MenuSetup_Extras_ProcessButtonCB(void) { UIControl_ProcessButtonInput(); }

void MenuSetup_Extras_Start_Puyo_vsAI(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();

    param->puyoSelection = PUYO_SELECTION_VS_CPU;
    strcpy(param->menuTag, "Extras");
    param->menuSelection = EXTRAS_SELECTION_PUYO;

    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Puyo_vsAI_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_Puyo_vsAI, 32); }

void MenuSetup_Extras_Start_Puyo_vs2P(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();

    param->puyoSelection = PUYO_SELECTION_VS_2P;
    strcpy(param->menuTag, "Extras");
    param->menuSelection = EXTRAS_SELECTION_PUYO;

    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Puyo_vs2P_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_Puyo_vs2P, 32); }

void MenuSetup_Extras_Start_Credits(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();

    param->bssSelection = CREDITS_SELECTION_EXTRAS;
    strcpy(param->menuTag, "Extras");
    param->menuSelection       = EXTRAS_SELECTION_CREDITS;
    param->creditsReturnToMenu = true;

    RSDK.SetScene("Presentation", "Credits");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Credits_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_Credits, 32); }

void MenuSetup_Extras_StartDAGarden(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();

    strcpy(param->menuTag, "Extras");
    param->menuSelection = EXTRAS_SELECTION_DAGARDEN;

    RSDK.SetScene("Extras", "D.A. Garden");
    RSDK.LoadScene();
}

void MenuSetup_Extras_DAGarden_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_Extras_StartDAGarden, 32); }

void MenuSetup_Extras_Start_BSS_3K(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();

    param->bssSelection = BSS_SELECTION_EXTRAS;
    strcpy(param->menuTag, "Extras");
    param->menuSelection = EXTRAS_SELECTION_BSS;

    RSDK.SetScene("Blue Spheres", "Random");
    RSDK.LoadScene();
}

void MenuSetup_Extras_BSS_3K_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_BSS_3K, 32); }

void MenuSetup_Extras_Start_BSS_Mania(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();

    param->bssSelection = BSS_SELECTION_EXTRAS;
    strcpy(param->menuTag, "Extras");
    param->menuSelection = EXTRAS_SELECTION_BSS;

    RSDK.SetScene("Blue Spheres", "Random 2");
    RSDK.LoadScene();
}

void MenuSetup_Extras_BSS_Mania_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_BSS_Mania, 32); }

#endif

#if RETRO_INCLUDE_EDITOR
void MenuSetup_EditorDraw(void) {}

void MenuSetup_EditorLoad(void) {}
#endif

void MenuSetup_Serialize(void) {}

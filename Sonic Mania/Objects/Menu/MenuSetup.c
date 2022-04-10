// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MenuSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMenuSetup *MenuSetup;

void MenuSetup_Update(void)
{
    RSDK_THIS(MenuSetup);
    StateMachine_Run(self->state);

#if !RETRO_USE_PLUS
    if (self->state != MenuSetup_State_SetupLeaderboards) {
#endif
        if (self->timer >= self->delay) {
            StateMachine_Run(self->stateDelay);
            destroyEntity(self);
        }
        else {
            self->timer++;
        }
#if !RETRO_USE_PLUS
    }
#endif
}

void MenuSetup_LateUpdate(void) {}

void MenuSetup_StaticUpdate(void)
{
#if RETRO_USE_PLUS
    if (!MenuSetup->initializedAPI) {
        MenuSetup->fxFade->speedOut = 0;
        TextInfo tag;
        INIT_TEXTINFO(tag);
        foreach_all(UIControl, control)
        {
            if (!RSDK.StringCompare(&tag, &control->tag, false)) {
                RSDK.PrependText(&tag, "Main Menu");
            }
        }

        if (!ManiaModeMenu_InitAPI()) {
            control->selectionDisabled = true;
            return;
        }
        else {
            control->selectionDisabled = false;
            MenuSetup->initializedAPI  = true;
            TextInfo info;
            Localization_GetString(&info, STR_RPC_MENU);
            API_SetRichPresence(PRESENCE_MENU, &info);
        }
    }

    if (!MenuSetup->initializedMenu) {
        ManiaModeMenu_Initialize();
        MenuSetup->initializedMenu = true;
    }

    if (!MenuSetup->initializedMenuReturn) {
        ManiaModeMenu_HandleMenuReturn();
        MenuSetup->initializedMenuReturn = true;
        ManiaModeMenu_SetBGColours();
        if (!globals->suppressAutoMusic) {
            ManiaModeMenu_ChangeMenuTrack();
        }
        globals->suppressAutoMusic = false;
    }

    MenuSetup->fxFade->speedOut = 12;
    DialogRunner_GetUserAuthStatus();
#else
    if (!MenuSetup->initializedAPI) {
        TextInfo tag;
        INIT_TEXTINFO(tag);
        foreach_all(UIControl, control)
        {
            if (!RSDK.StringCompare(&tag, &control->tag, false)) {
                RSDK.PrependText(&tag, "Main Menu");
            }
        }
        if (!MenuSetup_InitAPI()) {
            control->selectionDisabled = true;
            return;
        }
        else {
            control->selectionDisabled = false;
            MenuSetup->initializedAPI  = true;
            TextInfo info;
            Localization_GetString(&info, STR_RPC_MENU);
            API_SetRichPresence(PRESENCE_MENU, &info);
        }
    }

    if (!MenuSetup->initializedMenu) {
        MenuSetup_Initialize();
        MenuSetup->initializedMenu = true;
    }

    if (!MenuSetup->initializedMenuReturn) {
        MenuSetup_HandleMenuReturn();
        MenuSetup->initializedMenuReturn = true;
        MenuSetup_SetBGColours();
        if (!globals->suppressAutoMusic) {
            MenuSetup_ChangeMenuTrack();
        }
        globals->suppressAutoMusic = false;
    }

    APICallback_GetUserAuthStatus();
#endif
}

void MenuSetup_Draw(void)
{
    RSDK_THIS(MenuSetup);
    RSDK.FillScreen(self->fadeColour, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);
}

void MenuSetup_Create(void *data)
{
    RSDK_THIS(MenuSetup);
    self->active    = ACTIVE_ALWAYS;
    self->visible   = true;
    self->drawOrder = 14;
}

void MenuSetup_StageLoad(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
#if RETRO_USE_PLUS
    LogHelpers_Print("Menu recall ctrl: %s", param->menuTag);
    MenuSetup->initializedMenuReturn = false;
    MenuSetup->initializedMenu       = false;
    MenuSetup->initializedAPI        = false;
    MenuSetup->initializedSaves      = false;
    MenuSetup->fxFade                = NULL;
    if (!globals->suppressAutoMusic) {
        RSDK.StopChannel(Music->channelID);
        Music->activeTrack = -1;
    }
#else
    RSDK.StopChannel(Music->channelID);
    Music->activeTrack   = -1;
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

    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues = 0;
#if RETRO_USE_PLUS
    globals->recallEntities = false;
#else
    MenuSetup->vsTotalTimer = 120;
#endif
    RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
    foreach_all(FXFade, fade) { MenuSetup->fxFade = fade; }
}

void MenuSetup_StartTransition(void (*callback)(void), int32 delay)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->fadeShift       = 5;
    menuSetup->delay           = delay;
#if RETRO_USE_PLUS
    menuSetup->state = ManiaModeMenu_State_HandleTransition;
#else
    menuSetup->state     = MenuSetup_State_HandleTransition;
#endif
    menuSetup->stateDelay = callback;
}

#if !RETRO_USE_PLUS
void MenuSetup_StartTransitionLB(void (*callback)(void), int32 delay)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->fadeShift       = 5;
    menuSetup->delay           = delay;
    menuSetup->state           = MenuSetup_State_SetupLeaderboards;
    menuSetup->stateDelay      = callback;
}
#endif

// START PRE-PLUS AREA
#if !RETRO_USE_PLUS
void MenuSetup_Initialize(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&info, "Main Menu");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->mainMenu  = control;
            control->backPressCB = MenuSetup_ReturnToTitleOption;
        }
        RSDK.PrependText(&info, "Time Attack");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->timeAttack = control;
        RSDK.PrependText(&info, "Time Attack Zones");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->timeAttackZones = control;
        RSDK.PrependText(&info, "Leaderboards");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->leaderboards = control;
            control->backPressCB    = MenuSetup_TA_Leaderboards_BackPressCB;
        }
        RSDK.PrependText(&info, "Competition");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->competition = control;
            control->backPressCB   = MenuSetup_VS_BackoutFromVsCharSelect;
        }
        RSDK.PrependText(&info, "Competition Rules");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->competitionRules = control;
        RSDK.PrependText(&info, "Competition Zones");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->competitionZones = control;
            control->backPressCB        = MenuSetup_VS_CompZones_BackPressCB;
        }
        RSDK.PrependText(&info, "Competition Round");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->competitionRound = control;
        RSDK.PrependText(&info, "Competition Total");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->competitionTotal = control;
        RSDK.PrependText(&info, "Save Select");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->saveSelect = control;
        RSDK.PrependText(&info, "No Save Mode");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->noSaveMode = control;
        RSDK.PrependText(&info, "Secrets");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->secrets = control;
        RSDK.PrependText(&info, "Extras");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->extras = control;
        RSDK.PrependText(&info, "Options");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->options = control;
        RSDK.PrependText(&info, "Language");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->language = control;
        RSDK.PrependText(&info, "Video");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->video = control;
        RSDK.PrependText(&info, "Video WIN");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->video_win = control;
        RSDK.PrependText(&info, "Sound");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->sound = control;
        RSDK.PrependText(&info, "Controls WIN");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_win = control;
        RSDK.PrependText(&info, "Controls KB");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_KB = control;
        RSDK.PrependText(&info, "Controls PS4");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_PS4 = control;
        RSDK.PrependText(&info, "Controls XB1");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_XB1 = control;
        RSDK.PrependText(&info, "Controls NX");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_NX = control;
        RSDK.PrependText(&info, "Controls NX Grip");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_NX_Grip = control;
        RSDK.PrependText(&info, "Controls NX Joycon");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_NX_JoyCon = control;
        RSDK.PrependText(&info, "Controls NX Pro");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_NX_Pro = control;
    }

    Hitbox hitbox;
    foreach_all(UIButtonPrompt, prompt)
    {

        EntityUIControl *saveControl = MenuSetup->saveSelect;
        hitbox.right                 = saveControl->size.x >> 17;
        hitbox.left                  = -(saveControl->size.x >> 17);
        hitbox.bottom                = saveControl->size.y >> 17;
        hitbox.top                   = -(saveControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, saveControl->startPos.x - saveControl->cameraOffset.x,
                                      saveControl->startPos.y - saveControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 2)
            MenuSetup->delSavePrompt = prompt;

        EntityUIControl *leaderboardsControl = MenuSetup->leaderboards;
        hitbox.right                         = leaderboardsControl->size.x >> 17;
        hitbox.left                          = -(leaderboardsControl->size.x >> 17);
        hitbox.bottom                        = leaderboardsControl->size.y >> 17;
        hitbox.top                           = -(leaderboardsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, leaderboardsControl->startPos.x - leaderboardsControl->cameraOffset.x,
                                      leaderboardsControl->startPos.y - leaderboardsControl->cameraOffset.y, &hitbox, prompt->position.x,
                                      prompt->position.y)
            && prompt->buttonID == 3)
            MenuSetup->leaderboardPrompt = prompt;

        EntityUIControl *optionsControl = MenuSetup->options;
        hitbox.right                    = optionsControl->size.x >> 17;
        hitbox.left                     = -(optionsControl->size.x >> 17);
        hitbox.bottom                   = optionsControl->size.y >> 17;
        hitbox.top                      = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, optionsControl->startPos.x - optionsControl->cameraOffset.x,
                                      optionsControl->startPos.y - optionsControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 3)
            MenuSetup->optionsPrompt = prompt;
    }

    foreach_all(UILeaderboard, leaderboard) { MenuSetup->leaderboardWidget = leaderboard; }

    foreach_all(UIInfoLabel, label)
    {
        EntityUIControl *roundControl = MenuSetup->competitionRound;
        hitbox.right                  = roundControl->size.x >> 17;
        hitbox.left                   = -(roundControl->size.x >> 17);
        hitbox.bottom                 = roundControl->size.y >> 17;
        hitbox.top                    = -(roundControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, roundControl->startPos.x - roundControl->cameraOffset.x,
                                      roundControl->startPos.y - roundControl->cameraOffset.y, &hitbox, label->position.x, label->position.y))
            MenuSetup->roundLabel = label;

        EntityUIControl *totalControl = MenuSetup->competitionTotal;
        hitbox.right                  = totalControl->size.x >> 17;
        hitbox.left                   = -(totalControl->size.x >> 17);
        hitbox.bottom                 = totalControl->size.y >> 17;
        hitbox.top                    = -(totalControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, totalControl->startPos.x - totalControl->cameraOffset.x,
                                      totalControl->startPos.y - totalControl->cameraOffset.y, &hitbox, label->position.x, label->position.y))
            MenuSetup->totalLabel = label;
    }

    foreach_all(UIVsScoreboard, scoreboard)
    {
        EntityUIControl *roundControl = MenuSetup->competitionRound;
        hitbox.right                  = roundControl->size.x >> 17;
        hitbox.left                   = -(roundControl->size.x >> 17);
        hitbox.bottom                 = roundControl->size.y >> 17;
        hitbox.top                    = -(roundControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, roundControl->startPos.x - roundControl->cameraOffset.x,
                                      roundControl->startPos.y - roundControl->cameraOffset.y, &hitbox, scoreboard->position.x,
                                      scoreboard->position.y)) {
            MenuSetup->roundScoreboard = scoreboard;
            scoreboard->parent         = roundControl;
        }

        EntityUIControl *totalControl = MenuSetup->competitionTotal;
        hitbox.right                  = totalControl->size.x >> 17;
        hitbox.left                   = -(totalControl->size.x >> 17);
        hitbox.bottom                 = totalControl->size.y >> 17;
        hitbox.top                    = -(totalControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, totalControl->startPos.x - totalControl->cameraOffset.x,
                                      totalControl->startPos.y - totalControl->cameraOffset.y, &hitbox, scoreboard->position.x,
                                      scoreboard->position.y)) {
            MenuSetup->totalScoreboard = scoreboard;
            scoreboard->parent         = totalControl;
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
            if (!globals->noSave && (APICallback->authStatus != STATUS_OK || storageStatus != STATUS_OK)) {
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
                Options_LoadOptionsBin();
                SaveGame_LoadFile();
                MenuSetup->initializedSaves = true;
            }

            if (MenuSetup->initializedAPI)
                return true;

            if (globals->optionsLoaded == STATUS_OK && globals->saveLoaded == STATUS_OK) {
                if (!globals->noSave && APICallback_NotifyAutosave())
                    return false;
                UIWaitSpinner_FinishWait();
                if (APICallback_CheckUnreadNotifs())
                    return false;
                MenuSetup->initializedAPI = true;
                return true;
            }

            if (globals->noSave) {
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
    EntityUIControl *comp         = MenuSetup->competitionRound;
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

        int32 posX    = controls_win->startPos.x - controls_win->cameraOffset.x;
        int32 posY    = controls_win->startPos.y - controls_win->cameraOffset.y;
        hitbox.right  = controls_win->size.x >> 17;
        hitbox.left   = -(controls_win->size.x >> 17);
        hitbox.bottom = controls_win->size.y >> 17;
        hitbox.top    = -(controls_win->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->actionCB = MenuSetup_Options_OpenKBControlsMenu;

        posX          = compRules->startPos.x - compRules->cameraOffset.x;
        posY          = compRules->startPos.y - compRules->cameraOffset.y;
        hitbox.right  = compRules->size.x >> 17;
        hitbox.left   = -(compRules->size.x >> 17);
        hitbox.bottom = compRules->size.y >> 17;
        hitbox.top    = -(compRules->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 9
            && button->frameID == 2)
            button->actionCB = MenuSetup_VS_RulesButton_ActionCB;

        posX          = secrets->startPos.x - secrets->cameraOffset.x;
        posY          = secrets->startPos.y - secrets->cameraOffset.y;
        hitbox.right  = secrets->size.x >> 17;
        hitbox.left   = -(secrets->size.x >> 17);
        hitbox.bottom = secrets->size.y >> 17;
        hitbox.top    = -(secrets->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 9
            && button->frameID == 2)
            button->actionCB = MenuSetup_OpenSaveSelectMenu;

        posX          = options->startPos.x - options->cameraOffset.x;
        posY          = options->startPos.y - options->cameraOffset.y;
        hitbox.right  = options->size.x >> 17;
        hitbox.left   = -(options->size.x >> 17);
        hitbox.bottom = options->size.y >> 17;
        hitbox.top    = -(options->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3) {
            switch (button->frameID) {
                case 0: button->actionCB = MenuSetup_Options_VideoMenuButton_ActionCB; break;
                case 1: button->actionCB = MenuSetup_Options_SoundMenuButton_ActionCB; break;
                case 2: button->actionCB = MenuSetup_Options_ControlsMenuButton_ActionCB; break;
                case 3: button->actionCB = MenuSetup_Options_LanguageMenuButton_ActionCB; break;
            }
        }

        posX          = language->startPos.x - language->cameraOffset.x;
        posY          = language->startPos.y - language->cameraOffset.y;
        hitbox.right  = language->size.x >> 17;
        hitbox.left   = -(language->size.x >> 17);
        hitbox.bottom = language->size.y >> 17;
        hitbox.top    = -(language->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->actionCB = MenuSetup_OptionsLanguage_LanguageButton_ActionCB;

        posX          = video->startPos.x - video->cameraOffset.x;
        posY          = video->startPos.y - video->cameraOffset.y;
        hitbox.right  = video->size.x >> 17;
        hitbox.left   = -(video->size.x >> 17);
        hitbox.bottom = video->size.y >> 17;
        hitbox.top    = -(video->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && button->frameID == 0)
            button->choiceChangeCB = MenuSetup_OptionsVideo_ShaderButton_ActionCB;

        posX          = video_win->startPos.x - video_win->cameraOffset.x;
        posY          = video_win->startPos.y - video_win->cameraOffset.y;
        hitbox.right  = video_win->size.x >> 17;
        hitbox.left   = -(video_win->size.x >> 17);
        hitbox.bottom = video_win->size.y >> 17;
        hitbox.top    = -(video->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 17) {
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
        if (MathHelpers_PointInHitbox(FLIP_NONE, sound->startPos.x - sound->cameraOffset.x, sound->startPos.y - sound->cameraOffset.y, &hitbox,
                                      slider->position.x, slider->position.y)
            && slider->listID == 5)
            slider->sliderChangedCB = MenuSetup_OptionsVideo_UISlider_ChangedCB;
    }

    comp->processButtonInputCB = MenuSetup_VS_ProcessButtonCB;
    comp->menuSetupCB          = MenuSetup_VS_MenuSetupCB;
    if (comp->active == ACTIVE_ALWAYS) {
        RSDK_THIS(UIControl);
        self->childHasFocus = false;
        // self->menuWasSetup  = false;
        // LODWORD(RSDK_sceneInfo->self->data[13]) = 0;

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
    if (GameInfo->platform == PLATFORM_DEV || GameInfo->platform == PLATFORM_SWITCH)
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
    taButton->disabled       = !GameProgress_CheckUnlock(0);
    EntityUIButton *vsButton = mainMenu->buttons[1];
    vsButton->disabled       = !GameProgress_CheckUnlock(1);

    foreach_all(UITAZoneModule, module) { module->disabled = !GameProgress_GetZoneUnlocked(module->zoneID); }

    int32 maxRounds = 0;
    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !GameProgress_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;
        if (!zoneButton->xOut)
            ++maxRounds;
    }

    EntityUIButton *vsButton1 = (MenuSetup->competitionRules)->buttons[1];
    if (vsButton1->choiceCount >= 1) {
        EntityUIVsRoundPicker *picker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(vsButton1, vsButton1->selection);

        if (picker->objectID == UIVsRoundPicker->objectID) {
            picker->maxVal = maxRounds;
            if (picker->val >= picker->maxVal)
                picker->val = picker->maxVal;
        }
    }

    EntityUIControl *secrets       = MenuSetup->secrets;
    EntityUIButton *secretsButton1 = secrets->buttons[0];
    secretsButton1->disabled       = !GameProgress_CheckUnlock(5);
    if (secretsButton1->disabled)
        UIButton_ManageChoices(secretsButton1);

    EntityUIButton *secretsButton2 = secrets->buttons[1];
    EntityUIButton *option1        = UIButton_GetChoicePtr(secretsButton2, 1);
    EntityUIButton *option2        = UIButton_GetChoicePtr(secretsButton2, 2);
    secretsButton2->disabled       = !GameProgress_CheckUnlock(2);
    if (secretsButton2->disabled)
        UIButton_ManageChoices(secretsButton2);
    option1->disabled = !GameProgress_CheckUnlock(2);
    option2->disabled = !GameProgress_CheckUnlock(3);

    EntityUIButton *secretsButton3 = secrets->buttons[2];
    secretsButton3->disabled       = !GameProgress_CheckUnlock(4);
    if (secretsButton3->disabled)
        UIButton_ManageChoices(secretsButton3);

    EntityUIControl *extras = MenuSetup->extras;

    EntityUIButton *bssButton = extras->buttons[0];
    bssButton->disabled       = !GameProgress_CheckUnlock(8);
    if (bssButton->disabled)
        UIButton_ManageChoices(bssButton);

    EntityUIButton *puyoButton = extras->buttons[1];
    puyoButton->disabled       = !GameProgress_CheckUnlock(6);
    if (puyoButton->disabled)
        UIButton_ManageChoices(puyoButton);

    EntityUIButton *daGardenButton = extras->buttons[2];
    daGardenButton->disabled       = !GameProgress_CheckUnlock(7) && !globals->medallionDebug;
}

void MenuSetup_HandleMenuReturn(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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

            UIButton_SetChoiceSelection(control->buttons[0], (globals->medalMods & getMod(MEDAL_ANDKNUCKLES)) != 0);

            int32 medals = globals->medalMods;
            if (medals & getMod(MEDAL_NODROPDASH)) {
                if (medals & getMod(MEDAL_PEELOUT)) {
                    UIButton_SetChoiceSelection(control->buttons[1], 1);
                }
                else if (medals & getMod(MEDAL_INSTASHIELD)) {
                    UIButton_SetChoiceSelection(control->buttons[1], 2);
                }
            }
            else {
                UIButton_SetChoiceSelection(control->buttons[1], 0);
            }

            UIButton_SetChoiceSelection(control->buttons[2], (globals->medalMods & getMod(MEDAL_ANDKNUCKLES)) != 0);
        }

        if (control == MenuSetup->video) {
            EntityUIControl *control = MenuSetup->video;
            EntityUIButton *button   = control->buttons[0];
            UIButton_SetChoiceSelection(button, RSDK.GetSettingsValue(SETTINGS_SHADERID));
        }

        if (control == MenuSetup->sound) {
            EntityUIControl *control = MenuSetup->sound;

            EntityUISlider *slider = (EntityUISlider *)control->buttons[0];
            slider->sliderPos      = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);

            slider            = (EntityUISlider *)control->buttons[1];
            slider->sliderPos = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
        }

        if (control == MenuSetup->language) {
            EntityUIControl *control = MenuSetup->language;
            control->startingID      = Localization->language;
            control->buttonID        = Localization->language;
        }

        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        if (session->inMatch) {
            if (control == MenuSetup->competition) {
                foreach_all(UIVsCharSelector, selector)
                {
                    switch (session->characterFlags[selector->playerID]) {
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

                EntityUIVsRoundPicker *picker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(control->buttons[1], control->buttons[1]->selection);
                if (picker)
                    picker->val = session->matchCount;
            }

            if (control == MenuSetup->competitionZones) {
                for (int32 i = 0; i < 12; ++i) {
                    EntityUIVsZoneButton *button = (EntityUIVsZoneButton *)control->buttons[i];
                    if (button && session->zoneFlags[i])
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

    if (!RSDK.ChannelActive(Music->channelID))
        Music_PlayTrack(trackID);

    if (Music->activeTrack != trackID)
        Music_TransitionTrack(trackID, 0.12);
}

void MenuSetup_SetBGColours(void)
{
    switch (MenuSetup_GetActiveMenu()) {
        case MAINMENU_MAIN: UIBackground->activeColours = UIBackground->bgColours; break;
        case MAINMENU_TIMEATTACK:
        case MAINMENU_COMPETITION: UIBackground->activeColours = &UIBackground->bgColours[3]; break;
        case MAINMENU_SAVESELECT: UIBackground->activeColours = &UIBackground->bgColours[6]; break;
        default: break;
    }
}

void MenuSetup_MenuButton_ActionCB(void)
{
    RSDK_THIS(UIModeButton);
    switch (self->buttonID) {
        case 0:
            if (globals->noSave) {
                UIControl_MatchMenuTag("No Save Mode");
            }
            else {
                (MenuSetup->saveSelect)->buttonID = 7;
                UIControl_MatchMenuTag("Save Select");
            }
            break;

        case 1: UIControl_MatchMenuTag("Time Attack"); break;
        case 2: UIControl_MatchMenuTag("Competition"); break;
        case 3: UIControl_MatchMenuTag("Options"); break;
        default: return;
    }
}

bool32 MenuSetup_ReturnToTitleOption(void)
{
    MenuSetup_StartReturnToTitle();
    return true;
}

int32 MenuSetup_StartReturnToTitle(void)
{
    EntityUIControl *control = UIControl_GetUIControl();
    if (control)
        control->state = StateMachine_None;
    Music_FadeOut(0.05);
    MenuSetup_StartTransition(MenuSetup_ReturnToTitle, 32);
    return 1;
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
    TextInfo info;
    INIT_TEXTINFO(info);
    Localization_GetString(&info, STR_QUITWARNING);
    UIDialog_CreateDialogYesNo(&info, MenuSetup_ExitGame_CB, NULL, true, true);
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
    self->fadeTimer = self->timer << ((self->fadeShift & 0xFF) - 1);
    if (self->fadeTimer >= 512)
        self->fadeTimer = 512;
    else if (self->fadeTimer < 0)
        self->fadeTimer = 0;
}

// Save Select
int32 MenuSetup_GetMedalMods(void)
{
    EntityUIControl *control = MenuSetup->secrets;

    int32 mods = 0;
    if (control->buttons[0]->selection == 1)
        mods |= getMod(MEDAL_DEBUGMODE);

    if (control->buttons[1]->selection == 1) {
        mods |= getMod(MEDAL_NODROPDASH);
        mods |= getMod(MEDAL_PEELOUT);
    }
    else if (control->buttons[1]->selection == 2) {
        mods |= getMod(MEDAL_NODROPDASH);
        mods |= getMod(MEDAL_INSTASHIELD);
    }

    if (control->buttons[2]->selection == 1)
        mods |= getMod(MEDAL_ANDKNUCKLES);

    return mods;
}

void MenuSetup_OpenSaveSelectMenu(void)
{
    EntityUIControl *control = MenuSetup->saveSelect;
    UIControl_MatchMenuTag("Save Select");
    control->childHasFocus = false;
}

void MenuSetup_SaveFileCB(bool32 success)
{
    UIWaitSpinner_FinishWait();
    RSDK.LoadScene();
}

void MenuSetup_SaveSlot_ActionCB(void)
{
    RSDK_THIS(UISaveSlot);
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *control = (EntityUIControl *)self->parent;

    EntitySaveGame *saveRAM = (EntitySaveGame *)SaveGame_GetDataPtr(self->slotID);
    TimeAttackData_Clear();
    RSDK.GetCString(param->menuTag, &control->tag);
    param->menuSelection = control->lastButtonID;
    param->replayID    = 0;
    globals->gameMode  = MODE_MANIA;

    bool32 loadingSave = false;
    if (self->type) {
        memset(globals->noSaveSlot, 0, 0x400);
        globals->tempFlags  = 0;
        globals->saveSlotID = NO_SAVE_SLOT;
        globals->gameMode   = MODE_NOSAVE;
        globals->medalMods  = MenuSetup_GetMedalMods();
    }
    else {
        globals->saveSlotID = self->slotID;
        globals->medalMods  = 0;
        if (self->isNewSave) {
            int32 *saveData = SaveGame_GetDataPtr(self->slotID % 8);

            memset(saveData, 0, 0x400);
            saveRAM->saveState     = 1;
            saveRAM->characterID   = self->frameID;
            saveRAM->zoneID        = 0;
            saveRAM->lives         = 3;
            saveRAM->chaosEmeralds = self->saveEmeralds;
            saveRAM->continues     = 0;
            UIWaitSpinner_StartWait();
            loadingSave = true;
            SaveGame_SaveFile(MenuSetup_SaveFileCB);
        }
        else {
            if (saveRAM->saveState == 2) {
                saveRAM->collectedSpecialRings = 0;
                saveRAM->score                 = 0;
                saveRAM->score1UP              = 500000;
            }
            loadingSave = true;
            SaveGame_SaveFile(MenuSetup_SaveFileCB);
        }
    }

    switch (self->frameID) {
        case 0:
        case 1: globals->playerID = ID_SONIC; break;
        case 2: globals->playerID = ID_TAILS; break;
        case 3: globals->playerID = ID_KNUCKLES; break;
        default: break;
    }

    if ((globals->medalMods & getMod(MEDAL_ANDKNUCKLES))) {
        globals->playerID |= ID_KNUCKLES_ASSIST;
    }
    else if (!self->frameID) {
        globals->playerID |= ID_TAILS_ASSIST;
    }

    if (self->type == 1 || self->isNewSave) {
        if (((globals->medalMods & getMod(MEDAL_DEBUGMODE)) && (ControllerInfo->keyC.down || ControllerInfo->keyX.down)) && self->type == 1) {
            RSDK.SetScene("Presentation", "Level Select");
        }
        else {
            RSDK.SetScene("Cutscenes", "Angel Island Zone");
        }
    }
    else {
        RSDK.SetScene("Mania Mode", "");
        SceneInfo->listPos += TimeAttackData_GetManiaListPos(self->saveZoneID, self->frameID, 0);
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
        if (control->lastButtonID == 8) {
            prompt->promptID      = 6;
            prompt->headingAnchor = 3;
        }
        else {
            prompt->promptID      = 3;
            prompt->headingAnchor = 2;
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
    if (control->active == ACTIVE_ALWAYS && control->buttonID == 8) {
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
        UIControl->inputLocked = true;

        UITransition_StartTransition(MenuSetup_OpenSecretsMenu, 0);
    }
}

void MenuSetup_TA_OpenZoneList_Sonic(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    sprintf(param->menuTag, "Time Attack Zones");
    param->menuSelection      = param->zoneID;
    param->startedTAAttempt = true;

    SaveGame_ResetPlayerState();
    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues  = 0;
    globals->saveSlotID = 255;
    globals->gameMode   = MODE_TIMEATTACK;
    globals->medalMods  = 0;

    RSDK.SetScene("Mania Mode", "");
    SceneInfo->listPos += TimeAttackData_GetManiaListPos(param->zoneID, param->characterID, param->actID);

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
    EntityUILeaderboard *leaderboards   = (EntityUILeaderboard *)MenuSetup->leaderboardWidget;
    UIControl->forceBackPress           = false;
    leaderboardControl->active          = ACTIVE_NEVER;
    leaderboardControl->visible         = false;
    leaderboardControl->state           = StateMachine_None;

    UIControl_SetActiveMenu(MenuSetup->timeAttackZones);
    UITAZoneModule_SetStartupModule(MenuSetup->timeAttackZones, leaderboards->playerID, leaderboards->zoneID, leaderboards->actID, 0);
}

bool32 MenuSetup_TA_Leaderboards_BackPressCB(void)
{
    UITransition_StartTransition(MenuSetup_TA_Leaderboards_TransitionCB, 0);
    return true;
}

void MenuSetup_TA_Leaderboards_YPressCB(void)
{
    EntityUILeaderboard *leaderboards = MenuSetup->leaderboardWidget;
    UITAZoneModule_ShowLeaderboards(leaderboards->playerID, leaderboards->zoneID, leaderboards->actID, leaderboards->entryIsUser, NULL);
}

void MenuSetup_State_SetupLeaderboards(void)
{
    RSDK_THIS(MenuSetup);
    EntityUIDialog *dialog           = MenuSetup->connectingDlg;
    EntityUILeaderboard *leaderboard = MenuSetup->leaderboardWidget;
    EntityUIButtonPrompt *prompt     = MenuSetup->leaderboardPrompt;

    int32 status = APICallback_LeaderboardStatus();

    if (status == STATUS_CONTINUE) {
        TextInfo info;
        INIT_TEXTINFO(info);
        RSDK.SetText(&info, "", false);
        Localization_GetString(&info, STR_CONNECTING);

        UIDialog_SetupText(dialog, &info);
    }
    else if (status >= STATUS_ERROR) {
        status    = APICallback_LeaderboardStatus();
        int32 str = STR_COMMERROR;
        if (status == 504)
            str = STR_NOWIFI;

        TextInfo info;
        INIT_TEXTINFO(info);
        RSDK.SetText(&info, "", false);
        Localization_GetString(&info, str);

        UIDialog_SetupText(dialog, &info);
        UIDialog_AddButton(DIALOG_OK, dialog, NULL, true);

        EntityUIControl *parent = dialog->parent;
        parent->rowCount        = 1;
        parent->columnCount     = 1;
        parent->buttonID        = 0;

        if (leaderboard->taRecord)
            leaderboard->entryIsUser = !leaderboard->entryIsUser;

        if (leaderboard->entryIsUser)
            prompt->promptID = 14;
        else
            prompt->promptID = 15;
        prompt->unused           = -1;
        prompt->visible          = leaderboard->taRecord != 0;
        self->stateDelay         = StateMachine_None;
        MenuSetup->connectingDlg = NULL;
        destroyEntity(self);
    }
    else if (status == STATUS_OK) {
        if (leaderboard->entryIsUser)
            prompt->promptID = 14;
        else
            prompt->promptID = 15;
        prompt->unused  = -1;
        prompt->visible = leaderboard->taRecord != 0;

        UILeaderboard_InitLeaderboard(leaderboard);
        UIDialog_CloseOnSel_HandleSelection(dialog, self->stateDelay);
        self->stateDelay         = StateMachine_None;
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
        bool32 flag = true;
        for (int32 i = 0; i < control->buttonCount; ++i) {
            EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];

            Entity *entStore  = SceneInfo->entity;
            SceneInfo->entity = (Entity *)button;
            StateMachine_Run(button->processButtonCB);
            SceneInfo->entity = entStore;

            if (flag) {
                if (button->ready)
                    flag = !button->isSelected;
                else
                    flag = false;
            }
        }

        if (flag) {
            control->selectionDisabled = true;
            UITransition_StartTransition(MenuSetup_VS_OpenCompRules, 0);
        }
    }
}

void MenuSetup_VS_MenuSetupCB(void)
{
    RSDK_THIS(UIControl);

    self->childHasFocus = false;
    for (int32 i = 0; i < self->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)self->buttons[i];

        button->isSelected      = true;
        button->ready           = false;
        button->processButtonCB = UIVsCharSelector_ProcessButtonCB;
    }
}

void MenuSetup_VS_StartMatch(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    EntityMenuParam *param            = (EntityMenuParam *)globals->menuParam;

    sprintf(param->menuTag, "Competition Round");
    session->levelIndex = (MenuSetup->competitionZones)->buttonID;
    session->zoneID     = param->vsZoneID;
    session->actID      = param->vsActID;

    RSDK.SetScene("Mania Mode", "");
    SceneInfo->listPos += TimeAttackData_GetManiaListPos(param->vsZoneID, 0, param->vsActID);
    SaveGame_ResetPlayerState();
    memset(globals->noSaveSlot, 0, 0x400);

    globals->continues  = 0;
    globals->saveSlotID = 255;
    globals->gameMode   = MODE_COMPETITION;
    globals->medalMods  = 0;

    globals->playerID = ID_NONE;
    for (int32 i = 0; i < competition_PlayerCount; ++i) globals->playerID |= session->characterFlags[i] << (8 * i);
    globals->itemMode = session->itemMode;
    RSDK.LoadScene();
}

void MenuSetup_VS_StartMatch_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_VS_StartMatch, 32); }

void MenuSetup_VS_RulesButton_ActionCB(void)
{
    EntityUIControl *rulesControl     = MenuSetup->competitionRules;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    int32 matchCount = 0;
    foreach_all(UIVsRoundPicker, picker)
    {
        matchCount = picker->val;
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
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (UIControl->confirmPress[0] || UIControl->confirmPress[1] || UIControl->confirmPress[2] || UIControl->confirmPress[3]) {
        bool32 flag = false;
        int32 count = 0;
        for (int32 p = 0; p < competition_PlayerCount; ++p) {
            if (session->lives[p] > 0)
                count++;
            if (session->wins[p] > (session->matchCount >> 1))
                flag = true;
        }

        if (flag || count < 2 || session->matchID >= session->matchCount)
            UITransition_StartTransition(MenuSetup_VS_OpenCompTotal, 0);
        else
            UITransition_StartTransition(MenuSetup_VS_OpenCompZones, 0);

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;
    }
}

void MenuSetup_VS_Round_MenuSetupCB(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    EntityUIControl *roundControl     = MenuSetup->competitionRound;

    TextInfo info;
    INIT_TEXTINFO(info);
    char textBuffer[0x100];
    memset(textBuffer, 0, sizeof(textBuffer));
    Localization_GetZoneName(&info, session->zoneID);
    if (session->zoneID < 12) {
        char buf[16];
        sprintf(buf, " ZONE %d", session->actID + 1);
        RSDK.AppendText(&info, buf);
    }
    RSDK.GetCString(textBuffer, &info);
    UIInfoLabel_SetText((EntityUIInfoLabel *)MenuSetup->roundLabel, textBuffer);

    // EntityUIVsScoreboard *scoreboard = (EntityUIVsScoreboard*)MenuSetup->roundScoreboard;
    // sub_71000587C4(scoreboard, session->wins[0], session->wins[1]);
    // v11                  = session->wins[0] == session->wins[1];
    // scoreboard->sizeY    = session->wins[0] != session->wins[1];
    // scoreboard->field_A4 = session->wins[1] > session->wins[0];

    uint32 bestRings      = 0;
    uint32 bestTotalRings = 0;
    uint32 bestScore      = 0;
    uint32 bestItems      = 0;
    uint32 bestTime       = 0xFFFFFFFF;
    uint32 times[PLAYER_MAX];

    for (int32 p = 0; p < competition_PlayerCount; ++p) {
        if (session->rings[p] > bestRings)
            bestRings = session->rings[p];

        if (session->totalRings[p] > bestTotalRings)
            bestTotalRings = session->totalRings[p];

        if (session->score[p] > bestScore)
            bestScore = session->score[p];

        if (session->items[p] > bestItems)
            bestItems = session->items[p];

        times[p] = session->time[p].milliseconds + 100 * (session->time[p].seconds + 4 * (16 * session->time[p].minutes - session->time[p].minutes));
        if (times[p] < bestTime)
            bestTime = times[p];
    }

    char buffer[0x40];
    int32 winnerCount = 0;
    int32 match       = session->matchID - 1;

    for (int32 p = 0; p < competition_PlayerCount; ++p) {
        EntityUIVsResults *results = (EntityUIVsResults *)roundControl->buttons[p];

        results->isWinner = session->winnerFlags[match] & (1 << p);
        results->isLoser  = session->winnerFlags[match] & (1 << p);
        if (session->winnerFlags[match] & (1 << p))
            winnerCount++;
        results->trophyCount = session->wins[p];
        memset(buffer, 0, sizeof(buffer));

        sprintf(buffer, "%d", session->rings[p]);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[0], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[0]);
        }

        printf(buffer, "%d", session->totalRings[p]);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[1], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[1]);
        }

        sprintf(buffer, "%d", session->score[p]);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[2], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[2]);
        }

        sprintf(buffer, "%d", session->items[p]);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[3], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[3]);
        }

        sprintf(buffer, "%d'%02d\"%02d", session->time[p].minutes, session->time[p].seconds, session->time[p].milliseconds);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[4], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[4]);
        }

        if (session->finishFlags[p] != FINISHFLAG_TIMEOVER) {
            results->row0Highlight = session->rings[p] == bestRings;
            results->row1Highlight = session->totalRings[p] == bestTotalRings;
            results->row2Highlight = session->score[p] == bestScore;
            results->row3Highlight = session->items[p] == bestItems;
            results->row4Highlight = times[p] > 0 && times[p] == bestTime;
        }
    }

    if (winnerCount == 1) {
        int32 winner = -1;
        for (int32 p = 0; p < competition_PlayerCount; ++p) {
            if ((1 << p) & session->winnerFlags[match]) {
                winner = p;
                break;
            }
        }

        LogHelpers_Print("Announce_CharWins(%d)", session->characterFlags[winner]);
        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceWinPlayer;
        announcer->playerID        = session->characterFlags[winner];
    }
    else {
        LogHelpers_Print("Announce_ItsADraw(%d)", 0);
        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceDraw;
        announcer->playerID        = 0;
        announcer->timer           = 0;
    }
}

void MenuSetup_VS_OpenCompetition(void) { UIControl_MatchMenuTag("Competition"); }

void MenuSetup_VS_Total_ProcessButtonCB(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (UIControl->keyConfirm) {
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
            UITransition_StartTransition(MenuSetup_VS_OpenCompetition, 0);
        }
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;
    }
}

void MenuSetup_VS_Total_MenuSetupCB(void)
{
    EntityUIControl *totalControl     = MenuSetup->competitionTotal;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    MenuSetup->vsTotalTimer = 120;

    totalControl->targetPos.y = totalControl->startPos.y;
    totalControl->position.y  = totalControl->startPos.y;

    TextInfo info;
    INIT_TEXTINFO(info);

    EntityUIInfoLabel *label = (EntityUIInfoLabel *)MenuSetup->totalLabel;
    Localization_GetString(&info, STR_COMPTOTAL);
    UIInfoLabel_SetString(label, &info);

    int32 highestScore = 0;
    for (int32 p = 0; p < session->playerCount; ++p) {
        if (session->wins[p] > highestScore)
            highestScore = session->wins[p];
    }

    int32 count    = 0;
    int32 winnerID = -1;
    for (int32 p = 0; p < session->playerCount; ++p) {
        if (session->wins[p] == highestScore) {
            winnerID = p;
            ++count;
        }
    }

    // EntityUIVsScoreboard *scoreboard = (EntityUIVsScoreboard*)MenuSetup->totalScoreboard;
    // sub_71000587C4(scoreboard, session->wins[0], session->wins[1]);
    // v11                  = session->wins[0] == session->wins[1];
    // scoreboard->sizeY    = session->wins[0] != session->wins[1];
    // scoreboard->field_A4 = session->wins[1] > session->wins[0];

    if (count <= 1) {
        LogHelpers_Print("Announce_WinnerIs(%d)", winnerID);
        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceWinner;
        announcer->playerID        = winnerID;
    }
    else {
        LogHelpers_Print("Announce_ItsADraw(%d)", 1);
        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceDraw;
        announcer->playerID        = 1;
        announcer->timer           = 0;
    }

    for (int32 p = 0; p < session->playerCount; ++p) {
        EntityUIVsResults *results = (EntityUIVsResults *)totalControl->buttons[p];
        bool32 *highlight          = &results->row0Highlight;

        results->numRows     = session->matchCount;
        results->isWinner    = session->wins[p] == highestScore;
        results->trophyCount = session->wins[p];
        for (int32 r = 0; r < results->numRows; ++r) {
            char buffer[0x40];
            sprintf(buffer, "%d", session->winnerFlags[r]);
            if (!SceneInfo->inEditor) {
                RSDK.SetText(&results->rowText[r], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[r]);
            }
            highlight[r] = ((1 << p) & session->winnerFlags[r]);
        }
    }
}

void MenuSetup_VS_Total_MenuUpdateCB(void)
{
    EntityUIControl *totalControl = MenuSetup->competitionTotal;

    if (totalControl->active == ACTIVE_ALWAYS) {
        if (totalControl->position.y == totalControl->targetPos.y) {
            if (MenuSetup->vsTotalTimer <= 0) {
                int32 pos = totalControl->startPos.y;
                if (totalControl->targetPos.y == totalControl->startPos.y) {
                    EntityUIVsResults *button = (EntityUIVsResults *)totalControl->buttons[0];
                    if (button) {
                        if (button->size.y + button->position.y - 0x708000 > totalControl->startPos.y)
                            pos = button->size.y + button->position.y - 0x708000;
                    }
                }
                totalControl->targetPos.y = pos;
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
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttonCount;
        if (button->objectID == UIVsCharSelector->objectID && button->prevSelected && UIControl->backPress[button->playerID]) {
            UITransition_StartTransition(MenuSetup_VS_BackoutFromVsCharSelect_CB, 0);
            return true;
        }
    }

    if (!control->buttonCount) {
        control->selectionDisabled = true;
        if (!UIControl->backPress[0] && UIControl->keyBack) {
            UITransition_StartTransition(MenuSetup_VS_BackoutFromVsCharSelect_CB, 0);
            return true;
        }
    }
    return false;
}

void MenuSetup_VS_ExitComp_YesCB(void) { UITransition_StartTransition(MenuSetup_VS_ExitComp_TransitionCB, 0); }

bool32 MenuSetup_VS_CompZones_BackPressCB(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_EXITCOMP);
    UIDialog_CreateDialogYesNo(&info, MenuSetup_VS_ExitComp_YesCB, NULL, true, true);
    return true;
}

void MenuSetup_VS_StartPuyoMatch(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

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
    int32 id   = API_MostRecentActiveControllerID(0);
    int32 type = API_GetControllerType(id);

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
    ControllerInfo[CONT_P1].keyUp.keyMap     = 38;
    ControllerInfo[CONT_P1].keyDown.keyMap   = 40;
    ControllerInfo[CONT_P1].keyLeft.keyMap   = 37;
    ControllerInfo[CONT_P1].keyRight.keyMap  = 39;
    ControllerInfo[CONT_P1].keyA.keyMap      = 65;
    ControllerInfo[CONT_P1].keyB.keyMap      = 83;
    ControllerInfo[CONT_P1].keyC.keyMap      = 0;
    ControllerInfo[CONT_P1].keyX.keyMap      = 81;
    ControllerInfo[CONT_P1].keyY.keyMap      = 87;
    ControllerInfo[CONT_P1].keyZ.keyMap      = 0;
    ControllerInfo[CONT_P1].keyStart.keyMap  = 13;
    ControllerInfo[CONT_P1].keySelect.keyMap = 9;
}

void MenuSetup_Options_SetDefaultMappings_P2(void)
{
    ControllerInfo[CONT_P2].keyUp.keyMap     = 104;
    ControllerInfo[CONT_P2].keyDown.keyMap   = 101;
    ControllerInfo[CONT_P2].keyLeft.keyMap   = 100;
    ControllerInfo[CONT_P2].keyRight.keyMap  = 102;
    ControllerInfo[CONT_P2].keyA.keyMap      = 74;
    ControllerInfo[CONT_P2].keyB.keyMap      = 75;
    ControllerInfo[CONT_P2].keyC.keyMap      = 0;
    ControllerInfo[CONT_P2].keyX.keyMap      = 85;
    ControllerInfo[CONT_P2].keyY.keyMap      = 73;
    ControllerInfo[CONT_P2].keyZ.keyMap      = 0;
    ControllerInfo[CONT_P2].keyStart.keyMap  = 219;
    ControllerInfo[CONT_P2].keySelect.keyMap = 221;
}

void MenuSetup_Options_SetupKBControlsMenu(int32 playerID)
{
    EntityUIControl *control = MenuSetup->controls_KB;
    foreach_all(UISubHeading, subHeading)
    {
        Hitbox hitbox;
        hitbox.right  = control->size.x >> 17;
        hitbox.left   = -(control->size.x >> 17);
        hitbox.bottom = control->size.y >> 17;
        hitbox.top    = -(control->size.y >> 17);

        if (MathHelpers_PointInHitbox(FLIP_NONE, control->startPos.x - control->cameraOffset.x, control->startPos.y - control->cameraOffset.y,
                                      &hitbox, subHeading->position.x, subHeading->position.y)) {
            subHeading->frameID = playerID + 8;
            foreach_break;
        }
    }

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIKeyBinder *binder = (EntityUIKeyBinder *)control->buttons[i];
        if (binder->objectID == UIKeyBinder->objectID) {
            binder->inputID = playerID;
            if (playerID == 1)
                binder->actionCB = MenuSetup_Options_SetDefaultMappings_P2;
            else if (!playerID)
                binder->actionCB = MenuSetup_Options_SetDefaultMappings_P1;
        }
    }
}

void MenuSetup_OptionsVideo_Win_MenuUpdateCB(void) { MenuSetup_OptionsVideo_Win_InitVideoOptionsMenu(); }

void MenuSetup_OptionsVideo_Win_InitVideoOptionsMenu(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        EntityUIControl *videoControl_Win = MenuSetup->video_win;
        EntityOptions *options            = (EntityOptions *)globals->optionsRAM;
        Options_GetWinSize();

        int32 vals[7];

        vals[0] = RSDK.GetSettingsValue(SETTINGS_SHADERID);
        vals[1] = options->windowSize;
        vals[2] = RSDK.GetSettingsValue(SETTINGS_BORDERED);

        vals[3] = 0;
        if (!RSDK.GetSettingsValue(SETTINGS_WINDOWED) || options->windowSize == 4)
            vals[3] = 1;

        vals[4] = 0;
        vals[5] = RSDK.GetSettingsValue(SETTINGS_VSYNC);
        vals[6] = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);

        for (int32 i = 0; i < videoControl_Win->buttonCount; ++i) {
            EntityUIButton *button = videoControl_Win->buttons[i];

            if (i == 4) {
                EntityUIResPicker *child = (EntityUIResPicker *)UIButton_GetChoicePtr(button, button->selection);
                UIResPicker_GetDisplayInfo(child);
            }
            else if (i == 1) {
                EntityUIWinSize *child = (EntityUIWinSize *)UIButton_GetChoicePtr(button, button->selection);
                child->selection       = RSDK.GetSettingsValue(SETTINGS_WINDOW_HEIGHT) / SCREEN_YSIZE;
            }
            else if (button->selection != vals[i]) {
                UIButton_SetChoiceSelection(button, vals[i]);
            }
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
        Options_SaveOptionsBin(MenuSetup_Options_SaveOptionsCB_Load);
    }
}

void MenuSetup_Options_SaveOptionsCB_Load(bool32 success) { UIWaitSpinner_FinishWait(); }

void MenuSetup_Options_LaunchManual(void)
{
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
    APICallback_LaunchManual();
}

void MenuSetup_OptionsLanguage_LanguageButton_ActionCB(void)
{
    EntityOptions *options   = (EntityOptions *)globals->optionsRAM;
    EntityUIControl *control = MenuSetup->language;

    if (control->buttonID < 0)
        options->language = -1;
    else
        options->language = control->buttonID;
    options->overrideLanguage = control->buttonID >= 0;

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        RSDK.SetSettingsValue(SETTINGS_LANGUAGE, control->buttonID);
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
    EntityOptions *options  = (EntityOptions *)globals->optionsRAM;
    options->screenShader   = self->selection;
    options->overrideShader = true;
    RSDK.SetSettingsValue(SETTINGS_SHADERID, self->selection);
    Options->changed = true;
}

void MenuSetup_OptionsVideo_WindowScaleButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    if (self->selection != 4) {
        RSDK.SetSettingsValue(SETTINGS_WINDOW_WIDTH, WIDE_SCR_XSIZE * (self->selection + 1));
        RSDK.SetSettingsValue(SETTINGS_WINDOW_HEIGHT, SCREEN_YSIZE * (self->selection + 1));

        options->windowSize = self->selection;
        Options->changed    = true;
    }
}

void MenuSetup_OptionsVideo_BorderlessButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->windowBorder = self->selection;
    RSDK.SetSettingsValue(SETTINGS_BORDERED, self->selection);
    RSDK.UpdateWindow();
    Options->changed = true;
}

void MenuSetup_OptionsVideo_FullscreenButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->windowed = self->selection ^ 1;
    RSDK.SetSettingsValue(SETTINGS_WINDOWED, options->windowed);
    RSDK.UpdateWindow();
    Options->changed = true;
}

void MenuSetup_OptionsVideo_VSyncButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->vSync = self->selection;
    RSDK.SetSettingsValue(SETTINGS_VSYNC, self->selection);
    Options->changed = true;
}

void MenuSetup_OptionsVideo_TripleBufferButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->tripleBuffering = self->selection;
    RSDK.SetSettingsValue(SETTINGS_TRIPLEBUFFERED, self->selection);
    Options->changed = true;
}

void MenuSetup_OptionsVideo_UISlider_ChangedCB(void)
{
    RSDK_THIS(UISlider);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    // what the hell is up with this???????
    // it'd only ever be 0 or 1 why are F1,F2,F4,F5 & FC options?????
    // this is a CB for the slider why are the boolean values here???
    bool32 value = self->frameID != 1;
    switch (value) {
        case 0xF1:
            options->windowed = self->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_WINDOWED, options->windowed);
            break;
        case 0xF2:
            options->windowBorder = self->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_BORDERED, options->windowBorder);
            break;
        case 0xF4:
            options->vSync = self->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_VSYNC, options->vSync);
            break;
        case 0xF5:
            options->tripleBuffering = self->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_TRIPLEBUFFERED, options->tripleBuffering);
            break;
        case 0xFC:
            options->screenShader   = self->sliderPos;
            options->overrideShader = true;
            RSDK.SetSettingsValue(SETTINGS_SHADERID, options->screenShader);
            RSDK.UpdateWindow();
            break;
        case 0:
            options->volMusic         = self->sliderPos;
            options->overrideMusicVol = true;
            RSDK.SetSettingsValue(SETTINGS_STREAM_VOL, options->volMusic);
            break;
        case 1:
            options->volSfx         = self->sliderPos;
            options->overrideSfxVol = true;
            RSDK.SetSettingsValue(SETTINGS_SFX_VOL, options->volSfx);
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_Clear();
    param->bssSelection = CREDITS_SELECTION_EXTRAS;
    strcpy(param->menuTag, "Extras");
    param->menuSelection         = EXTRAS_SELECTION_CREDITS;
    param->creditsReturnToMenu = true;
    RSDK.SetScene("Presentation", "Credits");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Credits_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_Credits, 32); }

void MenuSetup_Extras_StartDAGarden(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_Clear();
    strcpy(param->menuTag, "Extras");
    param->menuSelection = EXTRAS_SELECTION_DAGARDEN;
    RSDK.SetScene("Extras", "D.A. Garden");
    RSDK.LoadScene();
}

void MenuSetup_Extras_DAGarden_ActionCB(void) { MenuSetup_StartTransition(MenuSetup_Extras_StartDAGarden, 32); }

void MenuSetup_Extras_Start_BSS_3K(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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

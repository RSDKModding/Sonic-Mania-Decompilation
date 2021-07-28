#include "SonicMania.h"

ObjectMenuSetup *MenuSetup;

void MenuSetup_Update(void)
{
    RSDK_THIS(MenuSetup);
    StateMachine_Run(entity->state);
    if (entity->timer >= entity->timeOut) {
        StateMachine_Run(entity->timedState);
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else {
        entity->timer++;
    }
}

void MenuSetup_LateUpdate(void) {}

void MenuSetup_StaticUpdate(void)
{
    if (!MenuSetup->dword18) {
        MenuSetup->fxFade->speedOut = 0;
        TextInfo tag;
        INIT_TEXTINFO(tag);
        foreach_all(UIControl, control)
        {
            if (!RSDK.StringCompare(&tag, &control->tag, false)) {
                RSDK.PrependText(&tag, "Main Menu");
            }
        }
        if (!MenuSetup_InitUserdata()) {
            control->selectionDisabled = true;
            return;
        }
        else {
            control->selectionDisabled = false;
            MenuSetup->dword18         = 1;
            TextInfo info;
            Localization_GetString(&info, STR_RPC_MENU);
#if RETRO_USE_PLUS
            API.SetRichPresence(1, &info);
#else
            APICallback_SetRichPresence(1, &info);
#endif
        }
    }
    if (!MenuSetup->initialized) {
        MenuSetup_Initialize();
        MenuSetup->initialized = true;
    }
    if (!MenuSetup->dword8) {
        MenuSetup_Unknown7();
        MenuSetup->dword8 = 1;
        MenuSetup_SetBGColours();
        if (!globals->suppressAutoMusic) {
            MenuSetup_ChangeMenuTrack();
        }
        globals->suppressAutoMusic = false;
    }
    MenuSetup->fxFade->speedOut = 12;
#if RETRO_USE_PLUS
    DialogRunner_GetUserAuthStatus();
#else
    APICallback_GetUserAuthStatus();
#endif
}

void MenuSetup_Draw(void)
{
    RSDK_THIS(MenuSetup);
    RSDK.FillScreen(entity->fadeColour, entity->fadeTimer, entity->fadeTimer - 128, entity->fadeTimer - 256);
}

void MenuSetup_Create(void *data)
{
    RSDK_THIS(MenuSetup);
    entity->active    = ACTIVE_ALWAYS;
    entity->visible   = true;
    entity->drawOrder = 14;
}

void MenuSetup_StageLoad(void)
{
    const char *tag = (const char *)(&((char *)&globals->menuParam)[90]);
    LogHelpers_Print("Menu recall ctrl: %s", tag);
    MenuSetup->dword8      = 0;
    MenuSetup->initialized = 0;
    MenuSetup->dword10     = 0;
    MenuSetup->gameLoaded  = 0;
    MenuSetup->dword18     = 0;
    MenuSetup->fxFade      = 0;
    if (!globals->suppressAutoMusic) {
        RSDK.StopChannel(Music->channelID);
        Music->activeTrack = -1;
    }
    if (!RSDK_sceneInfo->inEditor) {
#if RETRO_USE_PLUS
        switch (RSDK_sku->platform) {
#else
        switch (RSDK_info->platform) {
#endif
            case PLATFORM_PC: LogHelpers_Print("PC SKU"); break;
            case PLATFORM_PS4: LogHelpers_Print("PS4 SKU"); break;
            case PLATFORM_XB1: LogHelpers_Print("XB1 SKU"); break;
            case PLATFORM_SWITCH: LogHelpers_Print("NX SKU"); break;
            case PLATFORM_DEV: LogHelpers_Print("DEV SKU"); break;
            default:
#if RETRO_USE_PLUS
                LogHelpers_Print("INVALID PLATFORM: %d", RSDK_sku->platform);
#else
                LogHelpers_Print("INVALID PLATFORM: %d", RSDK_info->platform);
#endif
                break;
        }

#if RETRO_USE_PLUS
        switch (RSDK_sku->region) {
#else
        switch (RSDK_info->region) {
#endif
            case REGION_US: LogHelpers_Print("US REGION"); break;
            case REGION_JP: LogHelpers_Print("JP REGION"); break;
            case REGION_EU: LogHelpers_Print("EU REGION"); break;
            default:
#if RETRO_USE_PLUS
                LogHelpers_Print("INVALID REGION: %d", RSDK_sku->region);
#else
                LogHelpers_Print("INVALID REGION: %d", RSDK_info->region);
#endif
                break;
        }
    }

    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues      = 0;
    globals->recallEntities = false;
    RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
    foreach_all(FXFade, fade) { MenuSetup->fxFade = fade; }
}

void MenuSetup_Initialize(void)
{
#if RETRO_USE_PLUS
    LogHelpers_Print("ManiaModeMenu_Initialize()");
    MainMenu_Initialize();
    UISubHeading_Initialize();
    // TimeAttackMenu_Initialize();
    // CompetitionMenu_Initialize();
    OptionsMenu_Initialize();
    ExtrasMenu_Initialize();
    MenuSetup_Unknown6();
    MainMenu_Unknown3();
    UISubHeading_Unknown3();
    // TimeAttackMenu_Unknown3();
    // CompetitionMenu_Unknown2();
    OptionsMenu_Unknown3();
    ExtrasMenu_Unknown3();
#endif
}

bool32 MenuSetup_InitUserdata(void)
{
#if RETRO_USE_PLUS
    if (!MenuSetup->dword10)
        MenuSetup->fxFade->timer = 512;
    int authStatus = API.GetUserAuthStatus();
    if (!authStatus) {
        API.TryAuth();
    }
    else if (authStatus != STATUS_CONTINUE) {
        int storageStatus = API.UserStorageStatusUnknown1();
        if (!storageStatus) {
            API.TryInitStorage();
        }
        else if (storageStatus != STATUS_CONTINUE) {
            int statusUnknown2 = API.GetSaveStatus();
            if (!API.GetUserStorageNoSave() && (authStatus != STATUS_OK || storageStatus != STATUS_OK)) {
                if (statusUnknown2 != STATUS_CONTINUE) {
                    if (statusUnknown2 != STATUS_FORBIDDEN) {
                        DialogRunner_PromptSavePreference(storageStatus);
                    }
                    else {
                        RSDK.LoadScene("Presentation", "Title Screen");
                        RSDK.InitSceneLoad();
                    }
                }
                return false;
            }

            if (!MenuSetup->gameLoaded) {
                UIWaitSpinner_Wait();
                Options_LoadOptionsBin();
                SaveGame_LoadFile();
                ReplayRecorder_LoadReplayDB(NULL);
                MenuSetup->gameLoaded = true;
            }
            if (MenuSetup->dword10)
                return true;
            if (globals->optionsLoaded == STATUS_OK && globals->saveLoaded == STATUS_OK && globals->replayTableLoaded == STATUS_OK
                && globals->taTableLoaded == STATUS_OK) {
                if (!API.GetUserStorageNoSave() && DialogRunner_NotifyAutosave())
                    return false;
                UIWaitSpinner_Wait2();
                if (DialogRunner_CheckUnreadNotifs())
                    return false;
                MenuSetup->dword10 = 1;
                return true;
            }

            if (API.GetUserStorageNoSave()) {
                UIWaitSpinner_Wait2();
                return true;
            }
            else {
                if (globals->optionsLoaded != STATUS_ERROR && globals->saveLoaded != STATUS_ERROR && globals->replayTableLoaded != STATUS_ERROR
                    && globals->taTableLoaded != STATUS_ERROR) {
                }
                else {
                    int status = API.GetSaveStatus();
                    if (status != STATUS_CONTINUE) {
                        if (status == STATUS_FORBIDDEN) {
                            RSDK.LoadScene("Presentation", "Title Screen");
                            RSDK.InitSceneLoad();
                        }
                        else {
                            DialogRunner_PromptSavePreference(STATUS_CORRUPT);
                        }
                    }
                }
            }
        }
    }
#else
    if (!MenuSetup->dword10)
        MenuSetup->fxFade->timer = 512;
    
    APICallback_GetUserAuthStatus();
    if (!APICallback->authStatus) {
        APICallback_TryAuth();
    }
    else if (APICallback->authStatus != STATUS_CONTINUE) {
        int storageStatus = APICallback_GetStorageStatus();
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
                        RSDK.LoadScene("Presentation", "Title Screen");
                        RSDK.InitSceneLoad();
                    }
                }
                return false;
            }

            if (!MenuSetup->gameLoaded) {
                UIWaitSpinner_Wait();
                Options_LoadOptionsBin();
                SaveGame_LoadFile();
                MenuSetup->gameLoaded = true;
            }
            if (MenuSetup->dword10)
                return true;
            if (globals->optionsLoaded == STATUS_OK && globals->saveLoaded == STATUS_OK) {
                if (!globals->noSave && APICallback_NotifyAutosave())
                    return false;
                UIWaitSpinner_Wait2();
                if (APICallback_CheckUnreadNotifs())
                    return false;
                MenuSetup->dword10 = 1;
                return true;
            }

            if (globals->noSave) {
                UIWaitSpinner_Wait2();
                return true;
            }
            else {
                if (globals->optionsLoaded != STATUS_ERROR && globals->saveLoaded != STATUS_ERROR) {
                }
                else {
                    if (APICallback->saveStatus != STATUS_CONTINUE) {
                        if (APICallback->saveStatus == STATUS_FORBIDDEN) {
                            RSDK.LoadScene("Presentation", "Title Screen");
                            RSDK.InitSceneLoad();
                        }
                        else {
                            APICallback_PromptSavePreference(STATUS_CORRUPT);
                        }
                    }
                }
            }
        }
    }
#endif
    return false;
}

void MenuSetup_InitLocalization(int a1)
{
    if (a1) {
        Localization->loaded = false;
        Localization_LoadStrings();
        UIWidgets_ApplyLanguage();
        UIHeading_LoadSprites();
    }
}

int MenuSetup_GetActiveMenu(void)
{
    Entity *control = (Entity *)UIControl_GetUIControl();
#if RETRO_USE_PLUS
    if (control == MainMenu->menuControlPtr || control == ExtrasMenu->extrasControl
        || control == OptionsMenu->optionsControl || control == OptionsMenu->videoControl || control == OptionsMenu->soundControl
        || control == OptionsMenu->dataOptionsControl || control == OptionsMenu->controlsControl_Windows
        || control == OptionsMenu->controlsControl_KB || control == OptionsMenu->controlsControl_PS4
        || control == OptionsMenu->controlsControl_XB1 || control == OptionsMenu->controlsControl_NX
        || control == OptionsMenu->controlsControl_NXGrip || control == OptionsMenu->controlsControl_NXJoycon
        || control == OptionsMenu->controlsControl_NXPro) {
        return 0;
    }
    // if (control == TimeAttackMenu->menu1 || control == TimeAttackMenu->menu2 || control == TimeAttackMenu->menu3
    //    || control == TimeAttackMenu->menu4 || control == TimeAttackMenu->menu5 || control == TimeAttackMenu->menu6
    //    || control == CompetitionMenu[1] || control == CompetitionMenu[2] || control == CompetitionMenu[3]
    //    || control == CompetitionMenu[4]) {
    //    return 1;
    //}
    // if (control == CompetitionMenu[8] || control == CompetitionMenu[10])
    //    return 2;
    if (control == ManiaModeMenu->saveSelectMenu || control == ManiaModeMenu->noSaveMenu || control == ManiaModeMenu->secretsMenu) {
        return 3;
    }
    if (control == ManiaModeMenu->encoreSaveSelect || control == ManiaModeMenu->noSaveMenuEncore)
        return 4;
#else
    if (control == MenuSetup->mainMenu || control == MenuSetup->extras || control == MenuSetup->options || control == MenuSetup->video
        || control == MenuSetup->sound || control == MenuSetup->controls_win || control == MenuSetup->controls_KB
        || control == MenuSetup->controls_PS4 || control == MenuSetup->controls_XB1 || control == MenuSetup->controls_NX
        || control == MenuSetup->controls_NX_Grip || control == MenuSetup->controls_NX_JoyCon || control == MenuSetup->controls_NX_Pro) {
        return 0;
    }
    if (control == MenuSetup->timeAttack || control == MenuSetup->timeAttackZones || control == MenuSetup->leaderboards
        || control == MenuSetup->competition || control == MenuSetup->competitionRules || control == MenuSetup->competitionZones) {
        return 1;
    }
    if (control == MenuSetup->competitionRound || control == MenuSetup->competitionTotal)
        return 2;
    if (control == MenuSetup->saveSelect || control == MenuSetup->noSaveMode || control == MenuSetup->secrets)
        return 3;
#endif
    return 0;
}

void MenuSetup_ChangeMenuTrack(void)
{
    int trackID = 0;
    switch (MenuSetup_GetActiveMenu()) {
        case 1: trackID = 1; break;
        case 2: trackID = 2; break;
        case 3: trackID = 3; break;
        case 4: trackID = 4; break;
        default: trackID = 0; break;
    }
    if (!RSDK.ChannelActive(Music->channelID))
        Music_PlayTrack(trackID);
    if (Music->activeTrack != trackID)
        Music_TransitionTrack(trackID, 0.12);
}

int MenuSetup_StartReturnToTitle(void)
{
    EntityUIControl *control = UIControl_GetUIControl();
    if (control)
        control->state = StateMachine_None;
    Music_FadeOut(0.05);
    EntityMenuSetup *menuSetup = (EntityMenuSetup *)RSDK.CreateEntity(MenuSetup->objectID, 0, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = 32;
    menuSetup->state           = MenuSetup_Unknown13;
    menuSetup->timedState      = MenuSetup_ReturnToTitle;
    return 1;
}

void MenuSetup_SetBGColours(void)
{
    switch (MenuSetup_GetActiveMenu()) {
        case 0: UIBackground->activeColours = UIBackground->bgColours; break;
        case 1:
        case 2: UIBackground->activeColours = &UIBackground->bgColours[3]; break;
        case 3: UIBackground->activeColours = &UIBackground->bgColours[6]; break;
#if RETRO_USE_PLUS
        case 4: UIBackground->activeColours = &UIBackground->bgColours[15]; break;
#endif
        default: break;
    }
}

void MenuSetup_ReturnToTitle(void)
{
    TimeAttackData_ClearOptions();
    RSDK.LoadScene("Presentation", "Title Screen");
    RSDK.InitSceneLoad();
}

void MenuSetup_Unknown13(void)
{
    RSDK_THIS(MenuSetup);
    entity->fadeTimer = entity->timer << ((entity->field_68 & 0xFF) - 1);
    if (entity->fadeTimer >= 512)
        entity->fadeTimer = 512;
    else if (entity->fadeTimer < 0)
        entity->fadeTimer = 0;
}

void MenuSetup_Unknown6(void)
{
#if RETRO_USE_PLUS
    MainMenu_Unknown2();
    UISubHeading_Unknown2();
    // TimeAttackMenu_Unknown2();
    /*int unknown     = CompetitionMenu_Unknown4();
    EntityUIButton *button = UIButton_Unknown2(*(*&CompetitionMenu[3] + 252), *(*(*&CompetitionMenu[3] + 252) + 348));
    if (button) {
        button->choiceCount = unknown;
        if (button->align < unknown)
            unknown = button->align;
        button->align = unknown;
    }*/
    OptionsMenu_Unknown2();
    ExtrasMenu_Unknown2();
#endif
}

void MenuSetup_Unknown7(void)
{
    char buffer[0x100];
    memset(buffer, 0, 0x100);
    const char *tag = (const char *)&((char *)&globals->menuParam)[90];
    if (strcmp(tag, "") == 0)
        UIUsernamePopup_ShowPopup();
    foreach_all(UIControl, control)
    {
        if (strcmp(tag, "") != 0) {
            RSDK.GetCString(buffer, &control->tag);
            if (strcmp((const char *)buffer, tag) != 0) {
                UIControl_Unknown6(control);
            }
            else {
                control->storedEntityID = globals->menuParam[87];
                control->dwordC4        = 1;
                UIControl_Unknown4(control);
                control->activeEntityID = globals->menuParam[87];
            }
        }
    }

    UISubHeading_Unknown4(0);
    // TimeAttackMenu_Unknown4();
    // CompetitionMenu_Unknown3();
    OptionsMenu_Unknown4();
    if ((byte)(globals->menuParam[22] >> 8) == 2) {
        EntityUIControl *extras = (EntityUIControl *)ExtrasMenu->extrasControl;
        UIButton_Unknown4(extras->entities[1], 1);
    }

    int a, b, c, d;
    if (globals->menuParam[89]) {
        a = globals->menuParam[91];
        b = globals->menuParam[92];
        c = globals->menuParam[93];
        d = globals->menuParam[95];
    }
    else {
        a = 0;
        b = 0;
        c = 0;
        d = 0;
    }
    TimeAttackData_ClearOptions();
    if (globals->menuParam[89]) {
        globals->menuParam[91] = a;
        globals->menuParam[92] = b;
        globals->menuParam[93] = c;
        globals->menuParam[95] = d;
    }
}

//START PRE-PLUS AREA
#if !RETRO_USE_PLUS

#endif

void MenuSetup_EditorDraw(void) {}

void MenuSetup_EditorLoad(void) {}

void MenuSetup_Serialize(void) {}

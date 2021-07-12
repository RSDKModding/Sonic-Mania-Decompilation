#include "../SonicMania.h"

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
            // unsure about this
            if (!RSDK.StringCompare(&tag, &control->tag, false)) {
                RSDK.PrependText(&tag, "Main Menu");
            }
        }
        if (!MenuSetup_InitUserdata()) {
            control->userdataInitialized = true;
            return;
        }
        else {
            control->userdataInitialized = false;
            MenuSetup->dword18           = 1;
            TextInfo info;
            Localization_GetString(&info, STR_RPC_MENU);
            User.SetRichPresence(1, &info);
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
    DialogRunner_GetUserAuthStatus();
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
    LogHelpers_Print("Menu recall ctrl: %s", ((char *)&globals->menuParam)[90]);
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
        switch (RSDK_sku->platform) {
            case PLATFORM_PC: LogHelpers_Print("PC SKU"); break;
            case PLATFORM_PS4: LogHelpers_Print("PS4 SKU"); break;
            case PLATFORM_XB1: LogHelpers_Print("XB1 SKU"); break;
            case PLATFORM_SWITCH: LogHelpers_Print("NX SKU"); break;
            case PLATFORM_DEV: LogHelpers_Print("DEV SKU"); break;
            default: LogHelpers_Print("INVALID PLATFORM: %d", RSDK_sku->platform); break;
        }

        switch (RSDK_sku->region) {
            case REGION_US: LogHelpers_Print("US REGION"); break;
            case REGION_JP: LogHelpers_Print("JP REGION"); break;
            case REGION_EU: LogHelpers_Print("EU REGION"); break;
            default: LogHelpers_Print("INVALID REGION: %d", RSDK_sku->region); break;
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
    LogHelpers_Print("ManiaModeMenu_Initialize()");
    MainMenu_Initialize();
    //UISubHeading_Initialize();
    //TimeAttackMenu_Initialize();
    //CompetitionMenu_Initialize();
    //OptionsMenu_Initialize();
    //ExtrasMenu_Initialize();
    MenuSetup_Unknown6();
    MainMenu_Unknown3();
    //UISubHeading_Unknown3();
    //TimeAttackMenu_Unknown3();
    //CompetitionMenu_Unknown2();
    //OptionsMenu_Unknown3();
    //ExtrasMenu_Unknown3();
}

bool32 MenuSetup_InitUserdata(void)
{
    if (!MenuSetup->dword10)
        MenuSetup->fxFade->timer = 512;
    int authStatus = User.GetUserAuthStatus();
    if (!authStatus) {
        User.TryAuth();
    }
    else if (authStatus != STATUS_CONTINUE) {
        int storageStatus = User.UserStorageStatusUnknown1();
        if (!storageStatus) {
            User.TryInitStorage();
        }
        else if (storageStatus != STATUS_CONTINUE) {
            int statusUnknown2 = User.UserStorageStatusUnknown2();
            if (!User.GetUserStorageNoSave() && (authStatus != STATUS_OK || storageStatus != STATUS_OK)) {
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
                // ReplayRecorder_LoadReplayDB(NULL);
                MenuSetup->gameLoaded = true;
            }
            if (MenuSetup->dword10)
                return true;
            if (globals->optionsLoaded == STATUS_OK && globals->saveLoaded == STATUS_OK && globals->replayTableLoaded == STATUS_OK
                && globals->taTableLoaded == STATUS_OK) {
                if (!User.GetUserStorageNoSave() && DialogRunner_NotifyAutosave())
                    return false;
                UIWaitSpinner_Wait2();
                if (DialogRunner_CheckUnreadNotifs())
                    return false;
                MenuSetup->dword10 = 1;
                return true;
            }

            if (User.GetUserStorageNoSave()) {
                UIWaitSpinner_Wait2();
                return true;
            }
            else {
                if (globals->optionsLoaded != STATUS_ERROR && globals->saveLoaded != STATUS_ERROR && globals->replayTableLoaded != STATUS_ERROR
                    && globals->taTableLoaded != STATUS_ERROR) {
                }
                else {
                    int status = User.UserStorageStatusUnknown2();
                    if (status != STATUS_CONTINUE) {
                        if (status == STATUS_FORBIDDEN) {
                            RSDK.LoadScene("Presentation", "Title Screen");
                            RSDK.InitSceneLoad();
                        }
                        else {
                            DialogRunner_PromptSavePreference(505);
                        }
                    }
                }
            }
        }
    }
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
    if (control == MainMenu->menuControlPtr /*|| control == ExtrasMenu[1] || control == OptionsMenu[1] || control == OptionsMenu[3]
        || control == OptionsMenu[5] || control == OptionsMenu[6] || control == OptionsMenu[7] || control == OptionsMenu[8]
        || control == OptionsMenu[9] || control == OptionsMenu[10] || control == OptionsMenu[11] || control == OptionsMenu[12]
        || control == OptionsMenu[13] || control == OptionsMenu[14]*/) {
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
    EntityMenuSetup *menuSetup = (EntityMenuSetup*)RSDK.CreateEntity(MenuSetup->objectID, 0, -0x100000, -0x100000);
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
        case 4: UIBackground->activeColours = &UIBackground->bgColours[15]; break;
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
    MainMenu_Unknown2();
    // UISubHeading_Unknown2();
    // TimeAttackMenu_Unknown2();
    /*int unknown     = CompetitionMenu_Unknown4();
    EntityUIButton *button = UIButton_Unknown2(*(*&CompetitionMenu[3] + 252), *(*(*&CompetitionMenu[3] + 252) + 348));
    if (button) {
        button->choiceCount = unknown;
        if (button->align < unknown)
            unknown = button->align;
        button->align = unknown;
    }*/
    // OptionsMenu_Unknown2();
    // ExtrasMenu_Unknown2();
}

void MenuSetup_Unknown7(void)
{
    char buffer[0x100];
    memset(buffer, 0, 0x100);
    char *str = (char *)&((char *)&globals->menuParam)[90];
    // if (!str)
    //    UIUsernamePopup_Unknown1();
    foreach_all(UIControl, control)
    {
        if (str) {
            RSDK.GetCString(buffer, &control->tag);
            if (strcmp((const char *)buffer, (const char *)str) == 0) {
                UIControl_Unknown6(control);
            }
            else {
                control->dwordD4 = globals->menuParam[87];
                control->dwordC4 = 1;
                UIControl_Unknown4(control);
                control->activeEntityID = globals->menuParam[87];
            }
        }
    }
    // UISubHeading_Unknown4(0);
    // TimeAttackMenu_Unknown4();
    // CompetitionMenu_Unknown3();
    // OptionsMenu_Unknown4();
    // if ((byte)(globals->menuParam[22] >> 8) == 2)
    //    UIButton_Unknown4(*(*&ExtrasMenu[1] + 252), 1);

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

void MenuSetup_EditorDraw(void) {}

void MenuSetup_EditorLoad(void) {}

void MenuSetup_Serialize(void) {}

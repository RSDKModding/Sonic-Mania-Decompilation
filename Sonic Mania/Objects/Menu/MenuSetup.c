#include "SonicMania.h"

ObjectMenuSetup *MenuSetup;

void MenuSetup_Update(void)
{
    RSDK_THIS(MenuSetup);
    StateMachine_Run(entity->state);

#if !RETRO_USE_PLUS
    if (entity->state != MenuSetup_Leaderboard_Unknown) {
#endif
        if (entity->timer >= entity->timeOut) {
            StateMachine_Run(entity->timedState);
            destroyEntity(entity);
        }
        else {
            entity->timer++;
        }
#if !RETRO_USE_PLUS
    }
#endif
}

void MenuSetup_LateUpdate(void) {}

void MenuSetup_StaticUpdate(void)
{
#if RETRO_USE_PLUS
    if (!MenuSetup->dword10) {
        MenuSetup->fxFade->speedOut = 0;
        TextInfo tag;
        INIT_TEXTINFO(tag);
        foreach_all(UIControl, control)
        {
            if (!RSDK.StringCompare(&tag, &control->tag, false)) {
                RSDK.PrependText(&tag, "Main Menu");
            }
        }
        if (!ManiaModeMenu_InitUserdata()) {
            control->selectionDisabled = true;
            return;
        }
        else {
            control->selectionDisabled = false;
            MenuSetup->dword10         = 1;
            TextInfo info;
            Localization_GetString(&info, STR_RPC_MENU);
            API_SetRichPresence(PRESENCE_MENU, &info);
        }
    }
    if (!MenuSetup->initialized) {
        ManiaModeMenu_Initialize();
        MenuSetup->initialized = true;
    }
    if (!MenuSetup->dword8) {
        ManiaModeMenu_Unknown7();
        MenuSetup->dword8 = 1;
        ManiaModeMenu_SetBGColours();
        if (!globals->suppressAutoMusic) {
            ManiaModeMenu_ChangeMenuTrack();
        }
        globals->suppressAutoMusic = false;
    }
    MenuSetup->fxFade->speedOut = 12;
    DialogRunner_GetUserAuthStatus();
#else
    if (!MenuSetup->dword10) {
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
            MenuSetup->dword10         = 1;
            TextInfo info;
            Localization_GetString(&info, STR_RPC_MENU);
            API_SetRichPresence(PRESENCE_MENU, &info);
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
#if RETRO_USE_PLUS
    LogHelpers_Print("Menu recall ctrl: %s", param->menuTag);
    MenuSetup->dword8      = false;
    MenuSetup->initialized = false;
    MenuSetup->dword10     = false;
    MenuSetup->gameLoaded  = false;
    MenuSetup->dword10     = false;
    MenuSetup->fxFade      = NULL;
    if (!globals->suppressAutoMusic) {
        RSDK.StopChannel(Music->channelID);
        Music->activeTrack = -1;
    }
#else
    RSDK.StopChannel(Music->channelID);
    Music->activeTrack   = -1;
#endif

    if (!RSDK_sceneInfo->inEditor) {
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
            case REGION_EU:
                LogHelpers_Print("EU REGION");
                break;
                LogHelpers_Print("INVALID REGION: %d", sku_region);
                break;
        }
    }

    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues = 0;
#if RETRO_USE_PLUS
    globals->recallEntities = false;
#else
    MenuSetup->field_140 = 120;
#endif
    RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
    foreach_all(FXFade, fade) { MenuSetup->fxFade = fade; }
}

void MenuSetup_StartTransition(void (*callback)(void), int time)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = time;
#if RETRO_USE_PLUS
    menuSetup->state = ManiaModeMenu_Unknown13;
#else
    menuSetup->state     = MenuSetup_Unknown13;
#endif
    menuSetup->timedState = callback;
}

#if !RETRO_USE_PLUS
void MenuSetup_StartTransitionLB(void (*callback)(void), int time)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = time;
    menuSetup->state           = MenuSetup_Leaderboard_Unknown;
    menuSetup->timedState      = callback;
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
            MenuSetup->mainMenu  = (Entity *)control;
            control->backPressCB = MenuSetup_ReturnToTitleOption;
        }
        RSDK.PrependText(&info, "Time Attack");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->timeAttack = (Entity *)control;
        RSDK.PrependText(&info, "Time Attack Zones");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->timeAttackZones = (Entity *)control;
        RSDK.PrependText(&info, "Leaderboards");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->leaderboards = (Entity *)control;
            control->backPressCB    = MenuSetup_TA_Unknown51;
        }
        RSDK.PrependText(&info, "Competition");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->competition = (Entity *)control;
            control->backPressCB   = MenuSetup_VS_BackoutFromVsCharSelect;
        }
        RSDK.PrependText(&info, "Competition Rules");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->competitionRules = (Entity *)control;
        RSDK.PrependText(&info, "Competition Zones");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->competitionZones = (Entity *)control;
            control->backPressCB        = MenuSetup_VS_Unknown60;
        }
        RSDK.PrependText(&info, "Competition Round");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->competitionRound = (Entity *)control;
        RSDK.PrependText(&info, "Competition Total");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->competitionTotal = (Entity *)control;
        RSDK.PrependText(&info, "Save Select");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->saveSelect = (Entity *)control;
        RSDK.PrependText(&info, "No Save Mode");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->noSaveMode = (Entity *)control;
        RSDK.PrependText(&info, "Secrets");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->secrets = (Entity *)control;
        RSDK.PrependText(&info, "Extras");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->extras = (Entity *)control;
        RSDK.PrependText(&info, "Options");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->options = (Entity *)control;
        RSDK.PrependText(&info, "Language");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->language = (Entity *)control;
        RSDK.PrependText(&info, "Video");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->video = (Entity *)control;
        RSDK.PrependText(&info, "Video WIN");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->video_win = (Entity *)control;
        RSDK.PrependText(&info, "Sound");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->sound = (Entity *)control;
        RSDK.PrependText(&info, "Controls WIN");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_win = (Entity *)control;
        RSDK.PrependText(&info, "Controls KB");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_KB = (Entity *)control;
        RSDK.PrependText(&info, "Controls PS4");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_PS4 = (Entity *)control;
        RSDK.PrependText(&info, "Controls XB1");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_XB1 = (Entity *)control;
        RSDK.PrependText(&info, "Controls NX");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_NX = (Entity *)control;
        RSDK.PrependText(&info, "Controls NX Grip");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_NX_Grip = (Entity *)control;
        RSDK.PrependText(&info, "Controls NX Joycon");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_NX_JoyCon = (Entity *)control;
        RSDK.PrependText(&info, "Controls NX Pro");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->controls_NX_Pro = (Entity *)control;
    }

    Hitbox hitbox;
    foreach_all(UIButtonPrompt, prompt)
    {

        EntityUIControl *saveControl = (EntityUIControl *)MenuSetup->saveSelect;
        hitbox.right                 = saveControl->size.x >> 17;
        hitbox.left                  = -(saveControl->size.x >> 17);
        hitbox.bottom                = saveControl->size.y >> 17;
        hitbox.top                   = -(saveControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, saveControl->startPos.x - saveControl->cameraOffset.x,
                                      saveControl->startPos.y - saveControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 2)
            MenuSetup->saveSelPrompt = (Entity *)prompt;

        EntityUIControl *leaderboardsControl = (EntityUIControl *)MenuSetup->leaderboards;
        hitbox.right                         = leaderboardsControl->size.x >> 17;
        hitbox.left                          = -(leaderboardsControl->size.x >> 17);
        hitbox.bottom                        = leaderboardsControl->size.y >> 17;
        hitbox.top                           = -(leaderboardsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, leaderboardsControl->startPos.x - leaderboardsControl->cameraOffset.x,
                                      leaderboardsControl->startPos.y - leaderboardsControl->cameraOffset.y, &hitbox, prompt->position.x,
                                      prompt->position.y)
            && prompt->buttonID == 3)
            MenuSetup->leaderboardPrompt = (Entity *)prompt;

        EntityUIControl *optionsControl = (EntityUIControl *)MenuSetup->options;
        hitbox.right                    = optionsControl->size.x >> 17;
        hitbox.left                     = -(optionsControl->size.x >> 17);
        hitbox.bottom                   = optionsControl->size.y >> 17;
        hitbox.top                      = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, optionsControl->startPos.x - optionsControl->cameraOffset.x,
                                      optionsControl->startPos.y - optionsControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 3)
            MenuSetup->optionsPrompt = (Entity *)prompt;
    }

    foreach_all(UILeaderboard, leaderboard) { MenuSetup->leaderboardWidget = (Entity *)leaderboard; }

    foreach_all(UIInfoLabel, label)
    {
        EntityUIControl *roundControl = (EntityUIControl *)MenuSetup->competitionRound;
        hitbox.right                  = roundControl->size.x >> 17;
        hitbox.left                   = -(roundControl->size.x >> 17);
        hitbox.bottom                 = roundControl->size.y >> 17;
        hitbox.top                    = -(roundControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, roundControl->startPos.x - roundControl->cameraOffset.x,
                                      roundControl->startPos.y - roundControl->cameraOffset.y, &hitbox, label->position.x, label->position.y))
            MenuSetup->roundLabel = (Entity *)label;

        EntityUIControl *totalControl = (EntityUIControl *)MenuSetup->competitionTotal;
        hitbox.right                  = totalControl->size.x >> 17;
        hitbox.left                   = -(totalControl->size.x >> 17);
        hitbox.bottom                 = totalControl->size.y >> 17;
        hitbox.top                    = -(totalControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, totalControl->startPos.x - totalControl->cameraOffset.x,
                                      totalControl->startPos.y - totalControl->cameraOffset.y, &hitbox, label->position.x, label->position.y))
            MenuSetup->totalLabel = (Entity *)label;
    }

    foreach_all(UIVsScoreboard, scoreboard)
    {
        EntityUIControl *roundControl = (EntityUIControl *)MenuSetup->competitionRound;
        hitbox.right                  = roundControl->size.x >> 17;
        hitbox.left                   = -(roundControl->size.x >> 17);
        hitbox.bottom                 = roundControl->size.y >> 17;
        hitbox.top                    = -(roundControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, roundControl->startPos.x - roundControl->cameraOffset.x,
                                      roundControl->startPos.y - roundControl->cameraOffset.y, &hitbox, scoreboard->position.x,
                                      scoreboard->position.y)) {
            MenuSetup->roundScoreboard = (Entity *)scoreboard;
            scoreboard->parent         = roundControl;
        }

        EntityUIControl *totalControl = (EntityUIControl *)MenuSetup->competitionTotal;
        hitbox.right                  = totalControl->size.x >> 17;
        hitbox.left                   = -(totalControl->size.x >> 17);
        hitbox.bottom                 = totalControl->size.y >> 17;
        hitbox.top                    = -(totalControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, totalControl->startPos.x - totalControl->cameraOffset.x,
                                      totalControl->startPos.y - totalControl->cameraOffset.y, &hitbox, scoreboard->position.x,
                                      scoreboard->position.y)) {
            MenuSetup->totalScoreboard = (Entity *)scoreboard;
            scoreboard->parent         = totalControl;
        }
    }

    MenuSetup_Unknown52();
    MenuSetup_Unknown3();
}

bool32 MenuSetup_InitUserdata(void)
{
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
                        RSDK.SetScene("Presentation", "Title Screen");
                        RSDK.LoadScene();
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

void MenuSetup_Unknown3(void)
{
    EntityUIControl *comp         = (EntityUIControl *)MenuSetup->competitionRound;
    EntityUIControl *options      = (EntityUIControl *)MenuSetup->options;
    EntityUIControl *video        = (EntityUIControl *)MenuSetup->video;
    EntityUIControl *video_win    = (EntityUIControl *)MenuSetup->video_win;
    EntityUIControl *controls_win = (EntityUIControl *)MenuSetup->controls_win;
    EntityUIControl *sound        = (EntityUIControl *)MenuSetup->sound;
    EntityUIControl *saveSel      = (EntityUIControl *)MenuSetup->saveSelect;
    EntityUIControl *secrets      = (EntityUIControl *)MenuSetup->secrets;
    EntityUIControl *extras       = (EntityUIControl *)MenuSetup->extras;
    EntityUIControl *compTotal    = (EntityUIControl *)MenuSetup->competitionTotal;
    EntityUIControl *compRound    = (EntityUIControl *)MenuSetup->competitionRound;
    EntityUIControl *compRules    = (EntityUIControl *)MenuSetup->competitionRules;
    EntityUIControl *language     = (EntityUIControl *)MenuSetup->language;

    foreach_all(UIModeButton, modeButton) { modeButton->options2 = MenuSetup_ChangeMenu; }
    foreach_all(UISaveSlot, saveSlot) { saveSlot->options2 = MenuSetup_StartNewSave; }

    Hitbox hitbox;
    foreach_all(UIButton, button)
    {
        switch (button->listID) {
            case 17:
                if (!button->frameID) {
                    if (RSDK_info->platform == PLATFORM_PC || RSDK_info->platform == PLATFORM_DEV) {
                        button->options2 = MenuSetup_ExitGame_Confirm;
                    }
                    else {
                        EntityUIControl *control = (EntityUIControl *)MenuSetup->mainMenu;
                        --control->buttonCount;
                        control->buttons[5] = NULL;
                        destroyEntity(button);
                    }
                }
                break;
            case 1:
                if (button->frameID == 4)
                    button->options2 = MenuSetup_OpenExtrasMenu;
                break;
            case 7:
                if (button->frameID == 4) {
                    button->options2 = MenuSetup_Extras_Callback_DAGarden;
                }
                else if (button->frameID == 8) {
                    button->options2  = MenuSetup_Extras_Callback_Credits;
                    button->field_150 = 1;
                }
                break;
        }

        int posX      = controls_win->startPos.x - controls_win->cameraOffset.x;
        int posY      = controls_win->startPos.y - controls_win->cameraOffset.y;
        hitbox.right  = controls_win->size.x >> 17;
        hitbox.left   = -(controls_win->size.x >> 17);
        hitbox.bottom = controls_win->size.y >> 17;
        hitbox.top    = -(controls_win->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->options2 = MenuSetup_Options_OpenKBControlsMenu;

        posX          = compRules->startPos.x - compRules->cameraOffset.x;
        posY          = compRules->startPos.y - compRules->cameraOffset.y;
        hitbox.right  = compRules->size.x >> 17;
        hitbox.left   = -(compRules->size.x >> 17);
        hitbox.bottom = compRules->size.y >> 17;
        hitbox.top    = -(compRules->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 9
            && button->frameID == 2)
            button->options2 = MenuSetup_VS_Unknown52;

        posX          = secrets->startPos.x - secrets->cameraOffset.x;
        posY          = secrets->startPos.y - secrets->cameraOffset.y;
        hitbox.right  = secrets->size.x >> 17;
        hitbox.left   = -(secrets->size.x >> 17);
        hitbox.bottom = secrets->size.y >> 17;
        hitbox.top    = -(secrets->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 9
            && button->frameID == 2)
            button->options2 = MenuSetup_OpenSaveSelectMenu;

        posX          = options->startPos.x - options->cameraOffset.x;
        posY          = options->startPos.y - options->cameraOffset.y;
        hitbox.right  = options->size.x >> 17;
        hitbox.left   = -(options->size.x >> 17);
        hitbox.bottom = options->size.y >> 17;
        hitbox.top    = -(options->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3) {
            switch (button->frameID) {
                case 0: button->options2 = MenuSetup_Options_OpenVideoMenu; break;
                case 1: button->options2 = MenuSetup_Options_OpenSoundMenu; break;
                case 2: button->options2 = MenuSetup_Options_OpenControlsMenu; break;
                case 3: button->options2 = MenuSetup_Options_OpenLanguageMenu; break;
            }
        }

        posX          = language->startPos.x - language->cameraOffset.x;
        posY          = language->startPos.y - language->cameraOffset.y;
        hitbox.right  = language->size.x >> 17;
        hitbox.left   = -(language->size.x >> 17);
        hitbox.bottom = language->size.y >> 17;
        hitbox.top    = -(language->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->options2 = MenuSetup_Options_Unknown52;

        posX          = video->startPos.x - video->cameraOffset.x;
        posY          = video->startPos.y - video->cameraOffset.y;
        hitbox.right  = video->size.x >> 17;
        hitbox.left   = -(video->size.x >> 17);
        hitbox.bottom = video->size.y >> 17;
        hitbox.top    = -(video->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && button->frameID == 0)
            button->choiceChangeCB = MenuSetup_Options_ShaderIDChanged_CB;

        posX          = video_win->startPos.x - video_win->cameraOffset.x;
        posY          = video_win->startPos.y - video_win->cameraOffset.y;
        hitbox.right  = video_win->size.x >> 17;
        hitbox.left   = -(video_win->size.x >> 17);
        hitbox.bottom = video_win->size.y >> 17;
        hitbox.top    = -(video->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 17) {
            switch (button->frameID) {
                case 2: button->choiceChangeCB = MenuSetup_Options_ShaderIDChanged_CB; break;
                case 7: button->choiceChangeCB = MenuSetup_Options_WinSizeChanged_CB; break;
                case 13: button->choiceChangeCB = MenuSetup_Options_BorderedChanged_CB; break;
                case 14: button->choiceChangeCB = MenuSetup_Options_FullscreenChanged_CB; break;
                case 15: button->choiceChangeCB = MenuSetup_Options_VSyncChanged_CB; break;
                case 16: button->choiceChangeCB = MenuSetup_Options_TripleBufferedChanged_CB; break;
                default: break;
            }
        }
    }

    foreach_all(UIChoice, choice)
    {
        if (choice->listID == 7) {
            switch (choice->frameID) {
                case 2:
                    choice->options2 = MenuSetup_Extras_Callback_Puyo_vsAI;
                    choice->textFlag = true;
                    break;
                case 3:
                    choice->options2 = MenuSetup_Extras_Callback_Puyo_vs2P;
                    choice->textFlag = true;
                    break;
                case 6:
                    choice->options2 = MenuSetup_Extras_Callback_BSS_3K;
                    choice->textFlag = true;
                    break;
                case 7:
                    choice->options2 = MenuSetup_Extras_Callback_BSS_Mania;
                    choice->textFlag = true;
                    break;
                default: break;
            }
        }
    }

    foreach_all(UICharButton, charButton)
    {
        if (charButton->parent == MenuSetup->timeAttack) {
            switch (charButton->characterID) {
                case 0: charButton->options2 = MenuSetup_TA_OpenZoneList_Sonic; break;
                case 1: charButton->options2 = MenuSetup_TA_OpenZoneList_Tails; break;
                case 2: charButton->options2 = MenuSetup_TA_OpenZoneList_Knux; break;
            }
        }
    }

    foreach_all(UITAZoneModule, module) { module->options2 = MenuSetup_TA_StartAttempt_Callback; }

    ((EntityUILeaderboard *)MenuSetup->leaderboardWidget)->yPressCB = MenuSetup_TA_Unknown52;

    foreach_all(UISlider, slider)
    {
        hitbox.right  = sound->size.x >> 17;
        hitbox.left   = -(sound->size.x >> 17);
        hitbox.bottom = sound->size.y >> 17;
        hitbox.top    = -(sound->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, sound->startPos.x - sound->cameraOffset.x, sound->startPos.y - sound->cameraOffset.y, &hitbox,
                                      slider->position.x, slider->position.y)
            && slider->listID == 5)
            slider->sliderChangedCB = MenuSetup_Options_SliderChanged_CB;
    }

    comp->processButtonInputCB = MenuSetup_VS_Unknown50;
    comp->unknownCallback3     = MenuSetup_VS_Unknown51;
    if (comp->active == ACTIVE_ALWAYS) {
        RSDK_THIS(UIControl);
        entity->childHasFocus = false;
        // LODWORD(RSDK_sceneInfo->entity->data[13]) = 0;

        foreach_all(UIVsCharSelector, selector)
        {
            selector->flag            = true;
            selector->ready           = false;
            selector->processButtonCB = UIVsCharSelector_ProcessButtonCB;
        }
    }

    compRound->processButtonInputCB = MenuSetup_VS_Unknown53;
    compRound->unknownCallback3     = MenuSetup_VS_Unknown54;
    if (compRound->active == ACTIVE_ALWAYS)
        MenuSetup_VS_Unknown54();

    compTotal->processButtonInputCB = MenuSetup_VS_Unknown55;
    compTotal->unknownCallback3     = MenuSetup_VS_Unknown56;
    compTotal->unknownCallback4     = MenuSetup_VS_Unknown57;
    compTotal->posUnknown.y         = compTotal->startPos.y;
    compTotal->position.y           = compTotal->startPos.y;
    if (compTotal->active == ACTIVE_ALWAYS)
        MenuSetup_VS_Unknown56();

    saveSel->unknownCallback4 = MenuSetup_CheckNoSaveSelected;
    saveSel->yPressCB         = MenuSetup_SaveSel_YPressCB;

    extras->processButtonInputCB = MenuSetup_Extras_ProcessButtonCB;

    options->unknownCallback3 = MenuSetup_Options_Unknown25;
    if (RSDK_info->platform == PLATFORM_DEV || RSDK_info->platform == PLATFORM_SWITCH)
        options->yPressCB = MenuSetup_Options_LaunchManual;
    else
        MenuSetup->optionsPrompt->visible = false;

    video_win->unknownCallback4 = MenuSetup_Unknown53;

    foreach_all(UIVsZoneButton, zoneButton) { zoneButton->options2 = MenuSetup_VS_StartMatch_CB; }
}

void MenuSetup_Unknown52(void)
{
    EntityUIControl *mainMenu = (EntityUIControl *)MenuSetup->mainMenu;

    EntityUIButton *taButton = mainMenu->buttons[1];
    taButton->disabled       = !GameProgress_CheckUnlock(0);
    EntityUIButton *vsButton = mainMenu->buttons[1];
    vsButton->disabled       = !GameProgress_CheckUnlock(1);

    foreach_all(UITAZoneModule, module) { module->disabled = !GameProgress_GetZoneUnlocked(module->zoneID); }

    int maxRounds = 0;
    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !GameProgress_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;
        if (!zoneButton->xOut)
            ++maxRounds;
    }

    EntityUIButton *vsButton1 = ((EntityUIControl *)MenuSetup->competitionRules)->buttons[1];
    if (vsButton1->choiceCount >= 1) {
        EntityUIVsRoundPicker *picker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(vsButton1, vsButton1->selection);

        if (picker->objectID == UIVsRoundPicker->objectID) {
            picker->maxVal = maxRounds;
            if (picker->val >= picker->maxVal)
                picker->val = picker->maxVal;
        }
    }

    EntityUIControl *secrets       = (EntityUIControl *)MenuSetup->secrets;
    EntityUIButton *secretsButton1 = secrets->buttons[0];
    secretsButton1->disabled       = !GameProgress_CheckUnlock(5);
    if (secretsButton1->disabled)
        UIButton_Unknown1(secretsButton1);

    EntityUIButton *secretsButton2 = secrets->buttons[1];
    EntityUIButton *option1        = UIButton_GetChoicePtr(secretsButton2, 1);
    EntityUIButton *option2        = UIButton_GetChoicePtr(secretsButton2, 2);
    secretsButton2->disabled       = !GameProgress_CheckUnlock(2);
    if (secretsButton2->disabled)
        UIButton_Unknown1(secretsButton2);
    option1->disabled = !GameProgress_CheckUnlock(2);
    option2->disabled = !GameProgress_CheckUnlock(3);

    EntityUIButton *secretsButton3 = secrets->buttons[2];
    secretsButton3->disabled       = !GameProgress_CheckUnlock(4);
    if (secretsButton3->disabled)
        UIButton_Unknown1(secretsButton3);

    EntityUIControl *extras = (EntityUIControl *)MenuSetup->extras;

    EntityUIButton *extrasButton1 = extras->buttons[0];
    extrasButton1->disabled       = !GameProgress_CheckUnlock(8);
    if (extrasButton1->disabled)
        UIButton_Unknown1(extrasButton1);

    EntityUIButton *extrasButton2 = extras->buttons[1];
    extrasButton2->disabled       = !GameProgress_CheckUnlock(6);
    if (extrasButton2->disabled)
        UIButton_Unknown1(extrasButton2);

    EntityUIButton *extrasButton3 = extras->buttons[2];
    extrasButton3->disabled       = !GameProgress_CheckUnlock(7) && !globals->medallionDebug;
}

void MenuSetup_Unknown7(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    char buffer[0x100];
    memset(buffer, 0, 0x100);
    if (strcmp(param->menuTag, "") == 0)
        UIUsernamePopup_ShowPopup();

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        MenuSetup_Unknown53();

    foreach_all(UIControl, control)
    {
        if (strcmp(param->menuTag, "") != 0) {
            RSDK.GetCString(buffer, &control->tag);
            if (strcmp((const char *)buffer, param->menuTag) != 0) {
                UIControl_Unknown6(control);
            }
            else {
                UIControl_Unknown4(control);
                control->activeEntityID = param->selectionID;

                if (control == (EntityUIControl *)MenuSetup->timeAttackZones && param->field_160)
                    UITAZoneModule_Unknown26(control, param->characterID, param->zoneID, param->actID, param->timeScore);
            }
        }

        if (control == (EntityUIControl *)MenuSetup->timeAttack && param->field_160) {
            int charID                     = param->characterID - 1;
            control->activeEntityID        = charID;
            control->buttons[charID]->flag = true;
        }

        if (control == (EntityUIControl *)MenuSetup->extras && param->field_59 == 2) {
            UIButton_SetChoiceSelection(control->buttons[1], 1);
        }

        if (control == (EntityUIControl *)MenuSetup->secrets) {
            EntityUIControl *control = (EntityUIControl *)MenuSetup->secrets;

            UIButton_SetChoiceSelection(control->buttons[0], (globals->medalMods & getMod(MEDAL_ANDKNUCKLES)) != 0);

            int medals = globals->medalMods;
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

        if (control == (EntityUIControl *)MenuSetup->video) {
            EntityUIControl *control = (EntityUIControl *)MenuSetup->video;
            EntityUIButton *button   = control->buttons[0];
            UIButton_SetChoiceSelection(button, RSDK.GetSettingsValue(SETTINGS_SHADERID));
        }

        if (control == (EntityUIControl *)MenuSetup->sound) {
            EntityUIControl *control = (EntityUIControl *)MenuSetup->sound;

            EntityUISlider *slider = (EntityUISlider *)control->buttons[0];
            slider->sliderPos      = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);

            slider            = (EntityUISlider *)control->buttons[1];
            slider->sliderPos = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);
        }

        if (control == (EntityUIControl *)MenuSetup->language) {
            EntityUIControl *control = (EntityUIControl *)MenuSetup->language;
            control->startingID      = Localization->language;
            control->activeEntityID  = Localization->language;
        }

        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        if (session->inMatch) {
            if (control == (EntityUIControl *)MenuSetup->competition) {
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

            if (control == (EntityUIControl *)MenuSetup->competitionRules) {
                switch (session->monitorMode) {
                    case 0: UIButton_SetChoiceSelection(control->buttons[0], 0); break;
                    case 1: UIButton_SetChoiceSelection(control->buttons[0], 2); break;
                    case 2: UIButton_SetChoiceSelection(control->buttons[0], 1); break;
                }

                EntityUIVsRoundPicker *picker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(control->buttons[1], control->buttons[1]->selection);
                if (picker)
                    picker->val = session->matchCount;
            }

            if (control == (EntityUIControl *)MenuSetup->competitionZones) {
                for (int i = 0; i < 12; ++i) {
                    EntityUIVsZoneButton *button = (EntityUIVsZoneButton *)control->buttons[i];
                    if (button && session->zoneFlags[i])
                        button->xOut = true;
                }
            }
        }
    }

    TimeAttackData_ClearOptions();
}

// Main Menu
int MenuSetup_GetActiveMenu(void)
{
    Entity *control = (Entity *)UIControl_GetUIControl();
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

void MenuSetup_SetBGColours(void)
{
    switch (MenuSetup_GetActiveMenu()) {
        case 0: UIBackground->activeColours = UIBackground->bgColours; break;
        case 1:
        case 2: UIBackground->activeColours = &UIBackground->bgColours[3]; break;
        case 3: UIBackground->activeColours = &UIBackground->bgColours[6]; break;
        default: break;
    }
}

void MenuSetup_ChangeMenu(void)
{
    RSDK_THIS(UIModeButton);
    switch (entity->buttonID) {
        case 0:
            if (globals->noSave) {
                UIControl_MatchMenuTag("No Save Mode");
            }
            else {
                ((EntityUIControl *)MenuSetup->saveSelect)->activeEntityID = 7;
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

int MenuSetup_StartReturnToTitle(void)
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
    TimeAttackData_ClearOptions();
    RSDK.SetScene("Presentation", "Title Screen");
    RSDK.LoadScene();
}

void MenuSetup_ExitGame(void) { APICallback_ExitGame(); }

void MenuSetup_ExitGame_Confirm(void)
{
    TextInfo buffer;
    Localization_GetString(&buffer, STR_QUITWARNING);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&buffer);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, NULL, 1);
        UIDialog_AddButton(DIALOG_YES, dialog, MenuSetup_ExitGame_CB, 1);
        UIDialog_Setup(dialog);
    }
}

void MenuSetup_ExitGame_CB(void)
{
    if (UIControl_GetUIControl())
        UIControl_GetUIControl()->state = StateMachine_None;
    Music_FadeOut(0.02);

    MenuSetup_StartTransition(MenuSetup_ExitGame, 64);
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

// Save Select
int MenuSetup_GetMedalMods(void)
{
    EntityUIControl *control = (EntityUIControl *)MenuSetup->secrets;

    int mods = 0;
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
    EntityUIControl *control = (EntityUIControl *)MenuSetup->saveSelect;
    UIControl_MatchMenuTag("Save Select");
    control->childHasFocus = false;
}

void MenuSetup_SaveFileCB(int status)
{
    UIWaitSpinner_Wait2();
    RSDK.LoadScene();
}

void MenuSetup_StartNewSave(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    RSDK_THIS(UISaveSlot);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    EntitySaveGame *saveRAM = (EntitySaveGame *)SaveGame_GetDataPtr(entity->slotID);
    TimeAttackData_ClearOptions();
    RSDK.GetCString(param->menuTag, &control->tag);
    param->selectionID = control->field_D8;
    param->field_168   = 0;
    globals->gameMode  = MODE_MANIA;

    bool32 loadingSave = false;
    if (entity->type) {
        memset(globals->noSaveSlot, 0, 0x400);
        globals->tempFlags  = false;
        globals->saveSlotID = NO_SAVE_SLOT;
        globals->gameMode   = MODE_NOSAVE;
        globals->medalMods  = MenuSetup_GetMedalMods();
    }
    else {
        globals->saveSlotID = entity->slotID;
        globals->medalMods  = 0;
        if (entity->isNewSave) {
            int *saveData = SaveGame_GetDataPtr(entity->slotID % 8);

            memset(saveData, 0, 0x400);
            saveRAM->saveState     = 1;
            saveRAM->characterID   = entity->frameID;
            saveRAM->zoneID        = 0;
            saveRAM->lives         = 3;
            saveRAM->chaosEmeralds = entity->saveEmeralds;
            saveRAM->continues     = 0;
            UIWaitSpinner_Wait();
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

    switch (entity->frameID) {
        case 0:
        case 1: globals->playerID = ID_SONIC; break;
        case 2: globals->playerID = ID_TAILS; break;
        case 3: globals->playerID = ID_KNUCKLES; break;
        default: break;
    }

    if ((globals->medalMods & getMod(MEDAL_ANDKNUCKLES))) {
        globals->playerID |= ID_KNUCKLES_ASSIST;
    }
    else if (!entity->frameID) {
        globals->playerID |= ID_TAILS_ASSIST;
    }

    if (entity->type == 1 || entity->isNewSave) {
        if (((globals->medalMods & getMod(MEDAL_DEBUGMODE)) && (RSDK_controller->keyC.down || RSDK_controller->keyX.down)) && entity->type == 1) {
            RSDK.SetScene("Presentation", "Level Select");
        }
        else {
            RSDK.SetScene("Cutscenes", "Angel Island Zone");
        }
    }
    else {
        RSDK.SetScene("Mania Mode", "");
        RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(entity->saveZoneID, entity->frameID, 0);
    }

    if (!loadingSave) {
        globals->initCoolBonus = false;
        RSDK.LoadScene();
    }
}

void MenuSetup_CheckNoSaveSelected(void)
{
    EntityUIControl *control = (EntityUIControl *)MenuSetup->saveSelect;
    if (control->active == ACTIVE_ALWAYS) {
        EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)MenuSetup->saveSelPrompt;
        if (control->field_D8 == 8) {
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
    EntityUIControl *control = (EntityUIControl *)MenuSetup->secrets;
    control->childHasFocus   = false;
    UIControl_MatchMenuTag("Secrets");
}

void MenuSetup_SaveSel_YPressCB(void)
{
    EntityUIControl *control = (EntityUIControl *)MenuSetup->saveSelect;
    if (control->active == ACTIVE_ALWAYS && control->activeEntityID == 8) {
        RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
        UIControl->inputLocked = true;

        UITransition_StartTransition(MenuSetup_OpenSecretsMenu, 0);
    }
}

void MenuSetup_TA_OpenZoneList_Sonic(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->characterID = 1;

    EntityUIControl *control = (EntityUIControl *)MenuSetup->timeAttackZones;
    for (int i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        button->characterID          = param->characterID;
    }

    UIControl_MatchMenuTag("Time Attack Zones");
}

void MenuSetup_TA_OpenZoneList_Tails(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->characterID = 2;

    EntityUIControl *control = (EntityUIControl *)MenuSetup->timeAttackZones;
    for (int i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        button->characterID          = param->characterID;
    }

    UIControl_MatchMenuTag("Time Attack Zones");
}

void MenuSetup_TA_OpenZoneList_Knux(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->characterID = 3;

    EntityUIControl *control = (EntityUIControl *)MenuSetup->timeAttackZones;
    for (int i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        button->characterID          = param->characterID;
    }

    UIControl_MatchMenuTag("Time Attack Zones");
}

void MenuSetup_TA_StartAttempt_Callback(void) { MenuSetup_StartTransition(MenuSetup_TA_StartAttempt, 32); }

void MenuSetup_TA_StartAttempt(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    sprintf(param->menuTag, "Time Attack Zones");
    param->selectionID = param->zoneID;
    param->field_160   = 1;

    SaveGame_ResetPlayerState();
    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues  = 0;
    globals->saveSlotID = 255;
    globals->gameMode   = MODE_TIMEATTACK;
    globals->medalMods  = 0;

    RSDK.SetScene("Mania Mode", "");
    RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(param->zoneID, param->characterID, param->actID);

    switch (param->characterID) {
        case 1: globals->playerID = ID_SONIC; break;
        case 2: globals->playerID = ID_TAILS; break;
        case 3: globals->playerID = ID_KNUCKLES; break;
        default: break;
    }
    RSDK.LoadScene();
}

void MenuSetup_TA_Unknown50(void)
{
    EntityUIControl *leaderboardControl = (EntityUIControl *)MenuSetup->leaderboards;
    EntityUILeaderboard *leaderboards   = (EntityUILeaderboard *)MenuSetup->leaderboardWidget;
    UIControl->flagA                    = false;
    leaderboardControl->active          = ACTIVE_NEVER;
    leaderboardControl->visible         = false;
    leaderboardControl->state           = StateMachine_None;

    UIControl_Unknown4((EntityUIControl *)MenuSetup->timeAttackZones);
    UITAZoneModule_Unknown26((EntityUIControl *)MenuSetup->timeAttackZones, leaderboards->playerID, leaderboards->zoneID, leaderboards->actID, 0);
}

bool32 MenuSetup_TA_Unknown51(void)
{
    UITransition_StartTransition(MenuSetup_TA_Unknown50, 0);
    return true;
}

void MenuSetup_TA_Unknown52(void)
{
    EntityUILeaderboard *leaderboards = (EntityUILeaderboard *)MenuSetup->leaderboardWidget;
    UITAZoneModule_Unknown25(leaderboards->playerID, leaderboards->zoneID, leaderboards->actID, leaderboards->entryIsUser, NULL);
}

void MenuSetup_Leaderboard_Unknown(void)
{
    RSDK_THIS(MenuSetup);
    EntityUIDialog *dialog           = (EntityUIDialog *)MenuSetup->dialog;
    EntityUILeaderboard *leaderboard = (EntityUILeaderboard *)MenuSetup->leaderboardWidget;
    EntityUIButtonPrompt *prompt     = (EntityUIButtonPrompt *)MenuSetup->leaderboardPrompt;

    int status = APICallback_LeaderboardStatus();

    if (status == STATUS_CONTINUE) {
        TextInfo info;
        INIT_TEXTINFO(info);
        RSDK.SetText(&info, "", false);
        Localization_GetString(&info, STR_CONNECTING);

        UIDialog_SetupText(dialog, &info);
    }
    else if (status >= STATUS_ERROR) {
        status  = APICallback_LeaderboardStatus();
        int str = STR_COMMERROR;
        if (status == 504)
            str = STR_NOWIFI;

        TextInfo info;
        INIT_TEXTINFO(info);
        RSDK.SetText(&info, "", false);
        Localization_GetString(&info, str);

        UIDialog_SetupText(dialog, &info);
        UIDialog_AddButton(DIALOG_OK, dialog, 0, true);

        EntityUIControl *parent = (EntityUIControl *)dialog->parent;
        parent->rowCount        = 1;
        parent->columnCount     = 1;
        parent->activeEntityID  = 0;

        if (leaderboard->taRecord)
            leaderboard->entryIsUser = !leaderboard->entryIsUser;

        if (leaderboard->entryIsUser)
            prompt->promptID = 14;
        else
            prompt->promptID = 15;
        prompt->field_78   = -1;
        prompt->visible    = leaderboard->taRecord != 0;
        entity->timedState = StateMachine_None;
        MenuSetup->dialog  = NULL;
        destroyEntity(entity);
    }
    else if (status == STATUS_OK) {
        if (leaderboard->entryIsUser)
            prompt->promptID = 14;
        else
            prompt->promptID = 15;
        prompt->field_78 = -1;
        prompt->visible  = leaderboard->taRecord != 0;

        UILeaderboard_InitLeaderboard(leaderboard);
        UIDialog_Unknown4(dialog, entity->timedState);
        entity->timedState = StateMachine_None;
        MenuSetup->dialog  = NULL;
        destroyEntity(entity);
    }
}

// Competition
void MenuSetup_VS_OpenCompRules(void) { UIControl_MatchMenuTag("Competition Rules"); }
void MenuSetup_VS_OpenCompZones(void) { UIControl_MatchMenuTag("Competition Zones"); }

void MenuSetup_VS_Unknown50(void)
{
    EntityUIControl *control = (EntityUIControl *)MenuSetup->competition;

    if (control) {
        bool32 flag = true;
        for (int i = 0; i < control->buttonCount; ++i) {
            EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];

            Entity *entStore       = RSDK_sceneInfo->entity;
            RSDK_sceneInfo->entity = (Entity *)button;
            StateMachine_Run(button->processButtonCB);
            RSDK_sceneInfo->entity = entStore;

            if (flag) {
                if (button->ready)
                    flag = !button->flag;
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

void MenuSetup_VS_Unknown51(void)
{
    RSDK_THIS(UIControl);

    entity->childHasFocus = false;
    for (int i = 0; i < entity->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)entity->buttons[i];

        button->flag            = true;
        button->ready           = false;
        button->processButtonCB = UIVsCharSelector_ProcessButtonCB;
    }
}

void MenuSetup_VS_StartMatch(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    EntityMenuParam *param            = (EntityMenuParam *)globals->menuParam;

    sprintf(param->menuTag, "Competition Round");
    session->levelIndex = ((EntityUIControl *)MenuSetup->competitionZones)->activeEntityID;
    session->zoneID     = param->vsZoneID;
    session->actID      = param->vsActID;

    RSDK.SetScene("Mania Mode", "");
    RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(param->vsZoneID, 0, param->vsActID);
    SaveGame_ResetPlayerState();
    memset(globals->noSaveSlot, 0, 0x400);

    globals->continues  = 0;
    globals->saveSlotID = 255;
    globals->gameMode   = MODE_COMPETITION;
    globals->medalMods  = 0;

    globals->playerID = ID_NONE;
    for (int i = 0; i < competition_PlayerCount; ++i) globals->playerID |= session->characterFlags[i] << (8 * i);
    globals->itemMode = session->monitorMode;
    RSDK.LoadScene();
}

void MenuSetup_VS_StartMatch_CB(void) { MenuSetup_StartTransition(MenuSetup_VS_StartMatch, 32); }

void MenuSetup_VS_Unknown52(void)
{
    EntityUIControl *rulesControl     = (EntityUIControl *)MenuSetup->competitionRules;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    int matchCount = 0;
    foreach_all(UIVsRoundPicker, picker)
    {
        matchCount = picker->val;
        foreach_break;
    }

    int monitorMode = 0;
    if (rulesControl->buttons[0]->selection) {
        if (rulesControl->buttons[0]->selection == 1) {
            monitorMode = 2;
        }
        else if (rulesControl->buttons[0]->selection == 2) {
            monitorMode = 1;
        }
    }

    CompetitionSession_ResetOptions();

    session->matchCount  = matchCount;
    session->inMatch     = true;
    session->monitorMode = monitorMode;

    EntityUIControl *zoneControl = (EntityUIControl *)MenuSetup->competitionZones;
    zoneControl->position        = zoneControl->startPos;
    zoneControl->posUnknown.x    = zoneControl->startPos.x;
    zoneControl->posUnknown.y    = zoneControl->startPos.y;
    zoneControl->activeEntityID  = 0;

    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !GameProgress_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;
    }

    UIControl_MatchMenuTag("Competition Zones");
}

void MenuSetup_VS_OpenCompTotal(void) { UIControl_MatchMenuTag("Competition Total"); }

void MenuSetup_VS_Unknown53(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (UIControl->confirmPress[0] || UIControl->confirmPress[1] || UIControl->confirmPress[2] || UIControl->confirmPress[3]) {
        bool32 flag = false;
        int count   = 0;
        for (int p = 0; p < competition_PlayerCount; ++p) {
            if (session->lives[p] > 0)
                count++;
            if (session->wins[p] > (session->matchCount >> 1))
                flag = true;
        }

        if (flag || count < 2 || session->matchID >= session->matchCount)
            UITransition_StartTransition(MenuSetup_VS_OpenCompTotal, 0);
        else
            UITransition_StartTransition(MenuSetup_VS_OpenCompZones, 0);

        RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
        UIControl->inputLocked = true;
    }
}

void MenuSetup_VS_Unknown54(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    EntityUIControl *roundControl     = (EntityUIControl *)MenuSetup->competitionRound;

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

    int bestRings      = 0;
    int bestTotalRings = 0;
    int bestScore      = 0;
    int bestItems      = 0;
    int bestTime       = -1;
    int times[PLAYER_MAX];

    for (int p = 0; p < competition_PlayerCount; ++p) {
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
    int winnerCount = 0;
    for (int p = 0; p < competition_PlayerCount; ++p) {
        EntityUIVsResults *results = (EntityUIVsResults *)roundControl->buttons[p];

        results->field_1D4 = session->winnerFlags[p] & (1 << p);
        results->field_1D8 = session->winnerFlags[p] & (1 << p);
        if (session->winnerFlags[p] & (1 << p))
            winnerCount++;
        results->trophyCount = session->wins[p];
        memset(buffer, 0, sizeof(buffer));

        sprintf(buffer, "%d", session->rings[p]);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[0], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[0]);
        }

        printf(buffer, "%d", session->totalRings[p]);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[1], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[1]);
        }

        sprintf(buffer, "%d", session->score[p]);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[2], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[2]);
        }

        sprintf(buffer, "%d", session->items[p]);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[3], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[3]);
        }

        sprintf(buffer, "%d'%02d\"%02d", session->time[p].minutes, session->time[p].seconds, session->time[p].milliseconds);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[4], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[4]);
        }

        if (session->finishFlags[p] != 1) {
            results->row0Highlight = session->rings[p] == bestRings;
            results->row1Highlight = session->totalRings[p] == bestTotalRings;
            results->row2Highlight = session->score[p] == bestScore;
            results->row3Highlight = session->items[p] == bestItems;
            results->row4Highlight = times[p] > 0 && times[p] == bestTime;
        }
    }

    if (winnerCount == 1) {
        int winner = -1;
        for (int p = 0; p < competition_PlayerCount; ++p) {
            if ((1 << p) & session->winnerFlags[p]) {
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

void MenuSetup_VS_Unknown55(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (UIControl->keyConfirm) {
        int mostWins = 0;
        for (int p = 0; p < session->playerCount; ++p) {
            if (session->wins[p] > mostWins)
                mostWins = session->wins[p];
        }

        int winnerCount = 0;
        for (int p = 0; p < session->playerCount; ++p) {
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
        RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
        UIControl->inputLocked = true;
    }
}

void MenuSetup_VS_Unknown56(void)
{
    EntityUIControl *totalControl     = (EntityUIControl *)MenuSetup->competitionTotal;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    MenuSetup->field_140 = 120;

    totalControl->posUnknown.y = totalControl->startPos.y;
    totalControl->position.y   = totalControl->startPos.y;

    TextInfo info;
    INIT_TEXTINFO(info);

    EntityUIInfoLabel *label = (EntityUIInfoLabel *)MenuSetup->totalLabel;
    Localization_GetString(&info, STR_COMPTOTAL);
    UIInfoLabel_SetString(label, &info);

    int highestScore = 0;
    for (int p = 0; p < session->playerCount; ++p) {
        if (session->wins[p] > highestScore)
            highestScore = session->wins[p];
    }

    int count    = 0;
    int winnerID = -1;
    for (int p = 0; p < session->playerCount; ++p) {
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

    for (int p = 0; p < session->playerCount; ++p) {
        EntityUIVsResults *results = (EntityUIVsResults *)totalControl->buttons[p];
        bool32 *highlight          = &results->row0Highlight;

        results->numRows     = session->matchCount;
        results->field_1D4   = session->wins[p] == highestScore;
        results->trophyCount = session->wins[p];
        for (int r = 0; r < results->numRows; ++r) {
            char buffer[0x40];
            sprintf(buffer, "%d", session->winnerFlags[r]);
            if (!RSDK_sceneInfo->inEditor) {
                RSDK.SetText(&results->rowText[r], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[r]);
            }
            highlight[r] = ((1 << p) & session->winnerFlags[r]);
        }
    }
}

void MenuSetup_VS_Unknown57(void)
{
    EntityUIControl *totalControl = (EntityUIControl *)MenuSetup->competitionTotal;

    if (totalControl->active == ACTIVE_ALWAYS) {
        if (totalControl->position.y == totalControl->posUnknown.y) {
            if (MenuSetup->field_140 <= 0) {
                int pos = totalControl->startPos.y;
                if (totalControl->posUnknown.y == totalControl->startPos.y) {
                    EntityUIVsResults *button = (EntityUIVsResults *)totalControl->buttons[0];
                    if (button) {
                        if (button->field_1D0 + button->position.y - 0x708000 > totalControl->startPos.y)
                            pos = button->field_1D0 + button->position.y - 0x708000;
                    }
                }
                totalControl->posUnknown.y = pos;
                MenuSetup->field_140       = 120;
            }
            else {
                MenuSetup->field_140--;
            }
        }
    }
}

void MenuSetup_VS_Unknown58(void)
{
    EntityUIControl *control     = (EntityUIControl *)MenuSetup->competition;
    EntityUIControl *zoneControl = (EntityUIControl *)MenuSetup->competitionZones;

    UIControl_Unknown6(zoneControl);
    UIControl_Unknown4(control);
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
    UIControl_Unknown11(&control->parentTag);
}

bool32 MenuSetup_VS_BackoutFromVsCharSelect(void)
{
    LogHelpers_Print("BackoutFromVsCharSelect()");
    EntityUIControl *control = (EntityUIControl *)MenuSetup->competition;

    for (int i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttonCount;
        if (button->objectID == UIVsCharSelector->objectID && button->prevFlag && UIControl->backPress[button->playerID]) {
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

void MenuSetup_VS_Unknown59(void) { UITransition_StartTransition(MenuSetup_VS_Unknown58, 0); }

bool32 MenuSetup_VS_Unknown60(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_EXITCOMP);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, 0, true);
        UIDialog_AddButton(DIALOG_YES, dialog, MenuSetup_VS_Unknown59, true);
        UIDialog_Setup(dialog);
    }
    return true;
}

void MenuSetup_VS_StartPuyoMatch(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    TimeAttackData_ClearOptions();
    param->selectionFlag = 3;
    globals->gameMode    = MODE_COMPETITION;
    strcpy(param->menuTag, "Competition Total");
    param->selectionID = 0;
    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

// Options
void MenuSetup_Options_OpenVideoMenu(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        UIControl_MatchMenuTag("Video WIN");
    }
    else {
        UIControl_MatchMenuTag("Video");
    }
}

void MenuSetup_Options_OpenSoundMenu(void) { UIControl_MatchMenuTag("Sound"); }

void MenuSetup_Options_OpenLanguageMenu(void) { UIControl_MatchMenuTag("Language"); }

void MenuSetup_Options_OpenControlsMenu(void)
{
    int id   = API_MostRecentActiveControllerID(0);
    int type = API_GetControllerType(id);

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

void MenuSetup_Options_Unknown22_P1(void)
{
    RSDK_controller[1].keyUp.keyMap     = 38;
    RSDK_controller[1].keyDown.keyMap   = 40;
    RSDK_controller[1].keyLeft.keyMap   = 37;
    RSDK_controller[1].keyRight.keyMap  = 39;
    RSDK_controller[1].keyA.keyMap      = 65;
    RSDK_controller[1].keyB.keyMap      = 83;
    RSDK_controller[1].keyC.keyMap      = 0;
    RSDK_controller[1].keyX.keyMap      = 81;
    RSDK_controller[1].keyY.keyMap      = 87;
    RSDK_controller[1].keyZ.keyMap      = 0;
    RSDK_controller[1].keyStart.keyMap  = 13;
    RSDK_controller[1].keySelect.keyMap = 9;
}

void MenuSetup_Options_Unknown22_P2(void)
{
    RSDK_controller[2].keyUp.keyMap     = 104;
    RSDK_controller[2].keyDown.keyMap   = 101;
    RSDK_controller[2].keyLeft.keyMap   = 100;
    RSDK_controller[2].keyRight.keyMap  = 102;
    RSDK_controller[2].keyA.keyMap      = 74;
    RSDK_controller[2].keyB.keyMap      = 75;
    RSDK_controller[2].keyC.keyMap      = 0;
    RSDK_controller[2].keyX.keyMap      = 85;
    RSDK_controller[2].keyY.keyMap      = 73;
    RSDK_controller[2].keyZ.keyMap      = 0;
    RSDK_controller[2].keyStart.keyMap  = 219;
    RSDK_controller[2].keySelect.keyMap = 221;
}

void MenuSetup_Options_Unknown51(int id)
{
    EntityUIControl *control = (EntityUIControl *)MenuSetup->controls_KB;
    foreach_all(UISubHeading, subHeading)
    {
        Hitbox hitbox;
        hitbox.right  = control->size.x >> 17;
        hitbox.left   = -(control->size.x >> 17);
        hitbox.bottom = control->size.y >> 17;
        hitbox.top    = -(control->size.y >> 17);

        if (MathHelpers_PointInHitbox(FLIP_NONE, control->startPos.x - control->cameraOffset.x, control->startPos.y - control->cameraOffset.y,
                                      &hitbox, subHeading->position.x, subHeading->position.y)) {
            subHeading->frameID = id + 8;
            foreach_break;
        }
    }

    for (int i = 0; i < control->buttonCount; ++i) {
        EntityUIKeyBinder *binder = (EntityUIKeyBinder *)control->buttons[i];
        if (binder->objectID == UIKeyBinder->objectID) {
            binder->inputID = id;
            if (id == 1)
                binder->options2 = MenuSetup_Options_Unknown22_P2;
            else if (!id)
                binder->options2 = MenuSetup_Options_Unknown22_P1;
        }
    }
}

void MenuSetup_Unknown53(void) { MenuSetup_Unknown54(); }

void MenuSetup_Unknown54(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        EntityUIControl *videoControl_Win = (EntityUIControl *)MenuSetup->video_win;
        EntityOptions *options            = (EntityOptions *)globals->optionsRAM;
        Options_GetWinSize();

        int vals[7];

        vals[0] = RSDK.GetSettingsValue(SETTINGS_SHADERID);
        vals[1] = options->windowSize;
        vals[2] = RSDK.GetSettingsValue(SETTINGS_BORDERED);

        vals[3] = 0;
        if (!RSDK.GetSettingsValue(SETTINGS_WINDOWED) || options->windowSize == 4)
            vals[3] = 1;

        vals[4] = 0;
        vals[5] = RSDK.GetSettingsValue(SETTINGS_VSYNC);
        vals[6] = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);

        for (int i = 0; i < videoControl_Win->buttonCount; ++i) {
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
    EntityUIControl *control = (EntityUIControl *)MenuSetup->controls_win;

    for (int i = 0; i < control->buttonCount; ++i) {
        if (entity == control->buttons[i]) {
            MenuSetup_Options_Unknown51(i);
            UIControl_MatchMenuTag("Controls KB");
            break;
        }
    }
}

void MenuSetup_Options_Unknown25(void)
{
    if (Options->state) {
        UIWaitSpinner_Wait();
        Options_SaveOptionsBin(MenuSetup_Options_Unknown27);
    }
}

void MenuSetup_Options_Unknown27(int status) { UIWaitSpinner_Wait2(); }

void MenuSetup_Options_LaunchManual(void)
{
    RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 0xFF);
    APICallback_LaunchManual();
}

void MenuSetup_Options_Unknown52(void)
{
    EntityOptions *options   = (EntityOptions *)globals->optionsRAM;
    EntityUIControl *control = (EntityUIControl *)MenuSetup->language;

    if (control->activeEntityID < 0)
        options->language = -1;
    else
        options->language = control->activeEntityID;
    options->overrideLanguage = control->activeEntityID >= 0;

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        RSDK.SetSettingsValue(SETTINGS_LANGUAGE, control->activeEntityID);
    Options->state = 1;

    Localization->language = control->activeEntityID;
    control->startingID    = control->activeEntityID;
    Localization->loaded   = false;
    Localization_LoadStrings();
    UIWidgets_ApplyLanguage();
    UIHeading_LoadSprites();
    UIControl_MatchMenuTag("Options");
}

void MenuSetup_Options_ShaderIDChanged_CB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    options->screenShader  = entity->selection;
    options->field_60      = true;
    RSDK.SetSettingsValue(SETTINGS_SHADERID, entity->selection);
    Options->state = 1;
}

void MenuSetup_Options_WinSizeChanged_CB(void)
{
    RSDK_THIS(UIButton);

    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    if (entity->selection != 4) {
        RSDK.SetSettingsValue(SETTINGS_WINDOW_WIDTH, 424 * (entity->selection + 1));
        RSDK.SetSettingsValue(SETTINGS_WINDOW_HEIGHT, SCREEN_YSIZE * (entity->selection + 1));

        options->windowSize = entity->selection;
        Options->state      = 1;
    }
}

void MenuSetup_Options_BorderedChanged_CB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->windowBorder = entity->selection;
    RSDK.SetSettingsValue(SETTINGS_BORDERED, entity->selection);
    RSDK.UpdateWindow();
    Options->state = 1;
}

void MenuSetup_Options_FullscreenChanged_CB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->windowed = entity->selection ^ 1;
    RSDK.SetSettingsValue(SETTINGS_WINDOWED, options->windowed);
    RSDK.UpdateWindow();
    Options->state = 1;
}

void MenuSetup_Options_VSyncChanged_CB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->vSync = entity->selection;
    RSDK.SetSettingsValue(SETTINGS_VSYNC, entity->selection);
    Options->state = 1;
}

void MenuSetup_Options_TripleBufferedChanged_CB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->tripleBuffering = entity->selection;
    RSDK.SetSettingsValue(SETTINGS_TRIPLEBUFFERED, entity->selection);
    Options->state = 1;
}

void MenuSetup_Options_SliderChanged_CB(void)
{
    RSDK_THIS(UISlider);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    // what the hell is up with this???????
    // it'd only ever be 0 or 1 why are F1,F2,F4,F5 & FC options?????
    // this is a CB for the slider why are the boolean values here???
    bool32 value = entity->frameID != 1;
    switch (value) {
        case 0xF1:
            options->windowed = entity->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_WINDOWED, options->windowed);
            Options->state = 1;
            break;
        case 0xF2:
            options->windowBorder = entity->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_BORDERED, options->windowBorder);
            Options->state = 1;
            break;
        case 0xF4:
            options->vSync = entity->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_VSYNC, options->vSync);
            Options->state = 1;
            break;
        case 0xF5:
            options->tripleBuffering = entity->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_TRIPLEBUFFERED, options->tripleBuffering);
            Options->state = 1;
            break;
        case 0xFC:
            options->screenShader = entity->sliderPos;
            options->field_60     = true;
            RSDK.SetSettingsValue(SETTINGS_SHADERID, options->screenShader);
            RSDK.UpdateWindow();
            Options->state = 1;
            break;
        case 0:
            options->volMusic = entity->sliderPos;
            options->field_68 = 1;
            RSDK.SetSettingsValue(SETTINGS_STREAM_VOL, options->volMusic);
            Options->state = 1;
            break;
        case 1:
            options->volSfx   = entity->sliderPos;
            options->field_70 = true;
            RSDK.SetSettingsValue(SETTINGS_SFX_VOL, options->volSfx);
            Options->state = 1;
            break;
        default: Options->state = 1; break;
    }
}

void MenuSetup_OpenExtrasMenu(void) { UIControl_MatchMenuTag("Extras"); }

// Extras
void MenuSetup_Extras_ProcessButtonCB(void) { UIControl_ProcessButtonInput(); }

void MenuSetup_Extras_Start_Puyo_vsAI(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 1;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 1;
    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Callback_Puyo_vsAI(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_Puyo_vsAI, 32); }

void MenuSetup_Extras_Start_Puyo_vs2P(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 2;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 1;
    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Callback_Puyo_vs2P(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_Puyo_vs2P, 32); }

void MenuSetup_Extras_Start_Credits(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 1;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 3;
    param->field_1A8   = 1;
    RSDK.SetScene("Presentation", "Credits");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Callback_Credits(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_Credits, 32); }

void MenuSetup_Extras_StartDAGarden(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    strcpy(param->menuTag, "Extras");
    param->selectionID = 2;
    RSDK.SetScene("Extras", "D.A. Garden");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Callback_DAGarden(void) { MenuSetup_StartTransition(MenuSetup_Extras_StartDAGarden, 32); }

void MenuSetup_Extras_Start_BSS_3K(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 1;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 0;
    RSDK.SetScene("Blue Spheres", "Random");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Callback_BSS_3K(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_BSS_3K, 32); }

void MenuSetup_Extras_Start_BSS_Mania(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 1;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 0;
    RSDK.SetScene("Blue Spheres", "Random 2");
    RSDK.LoadScene();
}

void MenuSetup_Extras_Callback_BSS_Mania(void) { MenuSetup_StartTransition(MenuSetup_Extras_Start_BSS_Mania, 32); }

#endif

void MenuSetup_EditorDraw(void) {}

void MenuSetup_EditorLoad(void) {}

void MenuSetup_Serialize(void) {}

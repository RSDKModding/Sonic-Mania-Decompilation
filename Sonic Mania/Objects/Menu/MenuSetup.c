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
    Music->activeTrack = -1;
#endif

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
    menuSetup->state   = MenuSetup_Unknown13;
#endif
    menuSetup->timedState      = callback;
}

//START PRE-PLUS AREA
#if !RETRO_USE_PLUS
void MenuSetup_Initialize(void) {
    TextInfo info;
    INIT_TEXTINFO(info);

    foreach_all(UIControl, control) {
        RSDK.PrependText(&info, "Main Menu");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->mainMenu = (Entity*)control;
            control->backPressCB    = MenuSetup_ReturnToTitleOption;
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
            control->backPressCB        = MenuSetup_TA_Unknown51;
        }
        RSDK.PrependText(&info, "Competition");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->competition = (Entity *)control;
            //control->backPressCB       = MenuSetup_VS_BackoutFromVsCharSelect;
        }
        RSDK.PrependText(&info, "Competition Rules");
        if (RSDK.StringCompare(&info, &control->tag, false))
            MenuSetup->competitionRules = (Entity *)control;
        RSDK.PrependText(&info, "Competition Zones");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            MenuSetup->competitionZones = (Entity *)control;
            //control->backPressCB            = MenuSetup_VS_Unknown60;
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
    foreach_all(UIButtonPrompt, prompt) {

        EntityUIControl *saveControl = (EntityUIControl *)MenuSetup->saveSelect;
        hitbox.right                  = saveControl->size.x >> 17;
        hitbox.left                   = -(saveControl->size.x >> 17);
        hitbox.bottom                 = saveControl->size.y >> 17;
        hitbox.top                    = -(saveControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, saveControl->startPos.x - saveControl->cameraOffset.x,
                                      saveControl->startPos.y - saveControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y))
            MenuSetup->saveSelPrompt = (Entity *)prompt;

        EntityUIControl *leaderboardsControl = (EntityUIControl *)MenuSetup->leaderboards;
        hitbox.right                         = leaderboardsControl->size.x >> 17;
        hitbox.left                          = -(leaderboardsControl->size.x >> 17);
        hitbox.bottom                        = leaderboardsControl->size.y >> 17;
        hitbox.top                           = -(leaderboardsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, leaderboardsControl->startPos.x - leaderboardsControl->cameraOffset.x,
                                      leaderboardsControl->startPos.y - leaderboardsControl->cameraOffset.y, &hitbox, prompt->position.x,
                                      prompt->position.y))
            MenuSetup->leaderboardPrompt = (Entity *)prompt;

        EntityUIControl *optionsControl = (EntityUIControl *)MenuSetup->options;
        hitbox.right                    = optionsControl->size.x >> 17;
        hitbox.left                     = -(optionsControl->size.x >> 17);
        hitbox.bottom                   = optionsControl->size.y >> 17;
        hitbox.top                      = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, optionsControl->startPos.x - optionsControl->cameraOffset.x,
                                      optionsControl->startPos.y - optionsControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y))
            MenuSetup->optionsPrompt = (Entity *)prompt;
    }

    foreach_all(UILeaderboard, leaderboard) { MenuSetup->leaderboardWidget = (Entity *)leaderboard; }

    foreach_all(UIInfoLabel, label) {
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
                if (globals->optionsLoaded != STATUS_ERROR && globals->saveLoaded != STATUS_ERROR) {
                }
                else {
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
    foreach_all(UIButton, button) {
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
        //if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
        //    button->options2 = MenuSetup_Options_Unknown24;

        posX      = compRules->startPos.x - compRules->cameraOffset.x;
        posY      = compRules->startPos.y - compRules->cameraOffset.y;
        hitbox.right  = compRules->size.x >> 17;
        hitbox.left   = -(compRules->size.x >> 17);
        hitbox.bottom = compRules->size.y >> 17;
        hitbox.top    = -(compRules->size.y >> 17);
        //if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 9
        //    && button->frameID == 2)
        //    button->options2 = MenuSetup_VS_Unknown52;

        posX      = secrets->startPos.x - secrets->cameraOffset.x;
        posY      = secrets->startPos.y - secrets->cameraOffset.y;
        hitbox.right  = secrets->size.x >> 17;
        hitbox.left   = -(secrets->size.x >> 17);
        hitbox.bottom = secrets->size.y >> 17;
        hitbox.top    = -(secrets->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 9
            && button->frameID == 2)
            button->options2 = MenuSetup_OpenSaveSelectMenu;

        posX      = options->startPos.x - options->cameraOffset.x;
        posY      = options->startPos.y - options->cameraOffset.y;
        hitbox.right  = options->size.x >> 17;
        hitbox.left   = -(options->size.x >> 17);
        hitbox.bottom = options->size.y >> 17;
        hitbox.top    = -(options->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && button->frameID == 3) {
            switch (button->frameID) {
                //case 0: button->options2 = MenuSetup_Options_Unknown19; break;
                //case 1: button->options2 = MenuSetup_Options_Unknown16; break;
                //case 2: button->options2 = MenuSetup_Options_Unknown21; break;
                //case 3: button->options2 = MenuSetup_Options_Unknown17; break;
            }
        }

        posX          = language->startPos.x - language->cameraOffset.x;
        posY          = language->startPos.y - language->cameraOffset.y;
        hitbox.right  = language->size.x >> 17;
        hitbox.left   = -(language->size.x >> 17);
        hitbox.bottom = language->size.y >> 17;
        hitbox.top    = -(language->size.y >> 17);
        //if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
        //    button->options2 = MenuSetup_Options_Unknown52;

        posX          = video->startPos.x - video->cameraOffset.x;
        posY          = video->startPos.y - video->cameraOffset.y;
        hitbox.right  = video->size.x >> 17;
        hitbox.left   = -(video->size.x >> 17);
        hitbox.bottom = video->size.y >> 17;
        hitbox.top    = -(video->size.y >> 17);
        //if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
        //    && button->frameID == 0)
        //    button->options2 = MenuSetup_Options_Unknown53;

        posX          = video_win->startPos.x - video_win->cameraOffset.x;
        posY          = video_win->startPos.y - video_win->cameraOffset.y;
        hitbox.right  = video_win->size.x >> 17;
        hitbox.left   = -(video_win->size.x >> 17);
        hitbox.bottom = video_win->size.y >> 17;
        hitbox.top    = -(video->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 17
            && button->frameID == 0) {
            switch (button->frameID) {
                //case 2: button->callbackUnknown1 = MenuSetup_Options_Unknown53; break;
                //case 7: button->callbackUnknown1 = MenuSetup_Options_Unknown54; break;
                //case 13: button->callbackUnknown1 = MenuSetup_Options_Unknown55; break;
                //case 14: button->callbackUnknown1 = MenuSetup_Options_Unknown56; break;
                //case 15: button->callbackUnknown1 = MenuSetup_Options_Unknown57; break;
                //case 16: button->callbackUnknown1 = MenuSetup_Options_Unknown58; break;
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
                    choice->textFlag = 1;
                    break;
                case 3:
                    choice->options2 = MenuSetup_Extras_Callback_Puyo_vs2P;
                    choice->textFlag = 1;
                    break;
                case 6:
                    choice->options2 = MenuSetup_Extras_Callback_BSS_3K;
                    choice->textFlag = 1;
                    break;
                case 7:
                    choice->options2 = MenuSetup_Extras_Callback_BSS_Mania;
                    choice->textFlag = 1;
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

    foreach_active(UITAZoneModule, module) { module->options2 = MenuSetup_TA_StartAttempt_Callback; }
    
    ((EntityUILeaderboard*)MenuSetup->leaderboardWidget)->yPressCB = MenuSetup_TA_Unknown52;

    foreach_all(UISlider, slider)
    {
        hitbox.right  = sound->size.x >> 17;
        hitbox.left   = -(sound->size.x >> 17);
        hitbox.bottom = sound->size.y >> 17;
        hitbox.top    = -(sound->size.y >> 17);
        //if (MathHelpers_PointInHitbox(FLIP_NONE, sound->startPos.x - sound->cameraOffset.x, sound->startPos.y - sound->cameraOffset.y, &hitbox,
        //                              slider->position.x, slider->position.y)
        //    && slider->listID == 5)
        //    slider->sliderChangedCB = MenuSetup_Options_Unknown59;
    }

    //comp->processButtonInputCB = MenuSetup_VS_Unknown50;
    //comp->unknownCallback3     = MenuSetup_VS_Unknown51;
    if (comp->active == ACTIVE_ALWAYS) {
        RSDK_THIS(UIControl);
        entity->childHasFocus = false;
        //LODWORD(RSDK_sceneInfo->entity->data[13]) = 0;

        foreach_all(UIVsCharSelector, selector) {
            selector->flag  = true;
            selector->ready = false;
            selector->processButtonCB = UIVsCharSelector_ProcessButtonCB;
        }
    }

    //compRound->processButtonInputCB = MenuSetup_VS_Unknown53;
    //compRound->unknownCallback3     = MenuSetup_VS_Unknown54;
    //if (compRound->active == ACTIVE_ALWAYS)
    //    MenuSetup_VS_Unknown54();
    
    //compTotal->processButtonInputCB = MenuSetup_VS_Unknown55;
    //compTotal->unknownCallback3     = MenuSetup_VS_Unknown56;
    //compTotal->unknownCallback4     = MenuSetup_VS_Unknown57;
    compTotal->posUnknown.y         = compTotal->startPos.y;
    compTotal->position.y           = compTotal->startPos.y;
    // if (compTotal->active == ACTIVE_ALWAYS)
    //     MenuSetup_VS_Unknown56();

    saveSel->unknownCallback4             = MenuSetup_CheckNoSaveSelected;
    saveSel->yPressCB                     = MenuSetup_SaveSel_YPressCB;

    extras->processButtonInputCB = MenuSetup_Extras_ProcessButtonCB;

    //options->unknownCallback3 = MenuSetup_Options_Unknown25;
    if (RSDK_info->platform == PLATFORM_DEV || RSDK_info->platform == PLATFORM_SWITCH)
        options->yPressCB = MenuSetup_Options_LaunchManual;
    else
        MenuSetup->optionsPrompt->visible = false;

    //video_win->unknownCallback4  = MenuSetup_Unknown53;

    //foreach_all(UIVsZoneButton, zoneButton) { zoneButton->options2 = MenuSetup_VS_StartMatch_CB; }
}

void MenuSetup_Unknown52(void)
{
    EntityUIControl *mainMenu = (EntityUIControl *)MenuSetup->mainMenu;
    
    EntityUIButton *taButton = mainMenu->buttons[1];
    taButton->disabled       = !SaveGame_CheckUnlock(0);
    EntityUIButton *vsButton = mainMenu->buttons[1];
    vsButton->disabled       = !SaveGame_CheckUnlock(1);

    foreach_all(UITAZoneModule, module) { module->disabled = !SaveGame_GetZoneUnlocked(module->zoneID); }

    int maxRounds = 0;
    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !SaveGame_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;
        if (!zoneButton->xOut)
            ++maxRounds;
    }

    EntityUIButton *vsButton1 = ((EntityUIControl*)MenuSetup->competitionRules)->buttons[1];
    if (vsButton1->choiceCount >= 1) {
        EntityUIVsRoundPicker *picker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(vsButton1, vsButton1->selection);

        if (picker->objectID == UIVsRoundPicker->objectID) {
            picker->maxVal = maxRounds;
            if (picker->val >= picker->maxVal)
                picker->val = picker->maxVal;
        }
    }

    EntityUIControl *secrets = (EntityUIControl *)MenuSetup->secrets;
    EntityUIButton *secretsButton1 = secrets->buttons[0];
    secretsButton1->disabled       = !SaveGame_CheckUnlock(5);
    if (secretsButton1->disabled)
        UIButton_Unknown1(secretsButton1);

    EntityUIButton *secretsButton2 = secrets->buttons[1];
    EntityUIButton *option1        = UIButton_GetChoicePtr(secretsButton2, 1);
    EntityUIButton *option2        = UIButton_GetChoicePtr(secretsButton2, 2);
    secretsButton2->disabled       = !SaveGame_CheckUnlock(2);
    if (secretsButton2->disabled)
        UIButton_Unknown1(secretsButton2);
    option1->disabled = !SaveGame_CheckUnlock(2);
    option2->disabled = !SaveGame_CheckUnlock(3);

    EntityUIButton *secretsButton3 = secrets->buttons[2];
    secretsButton3->disabled       = !SaveGame_CheckUnlock(4);
    if (secretsButton3->disabled)
        UIButton_Unknown1(secretsButton3);

    EntityUIControl *extras = (EntityUIControl *)MenuSetup->extras;

    EntityUIButton *extrasButton1 = extras->buttons[0];
    extrasButton1->disabled       = !SaveGame_CheckUnlock(8);
    if (extrasButton1->disabled)
        UIButton_Unknown1(extrasButton1);

    EntityUIButton *extrasButton2 = extras->buttons[1];
    extrasButton2->disabled       = !SaveGame_CheckUnlock(6);
    if (extrasButton2->disabled)
        UIButton_Unknown1(extrasButton2);

    EntityUIButton *extrasButton3 = extras->buttons[2];
    extrasButton3->disabled       = !SaveGame_CheckUnlock(7) && !globals->medallionDebug;
}

void MenuSetup_Unknown7(void) {}

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
                ((EntityUIControl*)MenuSetup->saveSelect)->activeEntityID = 7;
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

int MenuSetup_GetMedalMods(void)
{
    EntityUIControl *control = (EntityUIControl *)MenuSetup->secrets;

    int mods = 0;
    if (control->buttons[0]->selection == 1)
        mods |= getMod(MEDAL_NOTIMEOVER);

    if (control->buttons[1]->selection == 1)
        mods |= getMod(MEDAL_DEBUGMODE);

    if (control->buttons[2]->selection == 1) {
        mods |= getMod(MEDAL_NODROPDASH);
        mods |= getMod(MEDAL_PEELOUT);
    }
    else if (control->buttons[2]->selection == 2) {
        mods |= getMod(MEDAL_NODROPDASH);
        mods |= getMod(MEDAL_INSTASHIELD);
    }

    if (control->buttons[3]->selection == 1)
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
        globals->gameMode  = MODE_NOSAVE;
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
            prompt->promptID = 3;
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
        RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 255);
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
    UIControl->flagA             = false;
    leaderboardControl->active  = ACTIVE_NEVER;
    leaderboardControl->visible = false;
    leaderboardControl->state        = StateMachine_None;

    UIControl_Unknown4((EntityUIControl *)MenuSetup->timeAttackZones);
    //sub_7100017054(MenuSetup->timeAttackZones, *&MenuSetup->leaderboardWidget->playerID,
    //                     BYTE1(*&MenuSetup->leaderboardWidget->playerID), BYTE2(*&MenuSetup->leaderboardWidget->playerID), 0);
}

bool32 MenuSetup_TA_Unknown51(void)
{
    UITransition_StartTransition(MenuSetup_TA_Unknown50, 0);
    return true;
}

void MenuSetup_TA_Unknown52(void)
{
    //sub_7100016B74(*&Obj_MenuSetup->leaderboardWidget->playerID, BYTE1(*&Obj_MenuSetup->leaderboardWidget->playerID),
    //                      BYTE2(*&Obj_MenuSetup->leaderboardWidget->playerID), Obj_MenuSetup->leaderboardWidget->entryIsUser, 0LL);
}

void MenuSetup_Leaderboard_Unknown(void) {}

void MenuSetup_VS_OpenCompRules(void) { UIControl_MatchMenuTag("Competition Rules"); }
void MenuSetup_VS_OpenCompZones(void) { UIControl_MatchMenuTag("Competition Zones"); }

void MenuSetup_Options_LaunchManual(void)
{
    RSDK.PlaySFX(UIWidgets->sfx_Accept, 0LL, 255LL);
    APICallback_LaunchManual();
}

void MenuSetup_OpenExtrasMenu(void) { UIControl_MatchMenuTag("Extras"); }

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

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
        if (!ManiaModeMenu_InitUserdata()) {
            control->selectionDisabled = true;
            return;
        }
        else {
            control->selectionDisabled = false;
            MenuSetup->dword18         = 1;
            TextInfo info;
            Localization_GetString(&info, STR_RPC_MENU);
            API.SetRichPresence(1, &info);
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
            APICallback_SetRichPresence(1, &info);
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
    LogHelpers_Print("Menu recall ctrl: %s", param->menuTag);
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
#if RETRO_USE_PLUS
    globals->continues      = 0;
    globals->recallEntities = false;
#else
    globals->tempFlags = false;
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
    menuSetup->state           = ManiaModeMenu_Unknown13;
    menuSetup->timedState      = callback;
}

//START PRE-PLUS AREA
#if !RETRO_USE_PLUS
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

void MenuSetup_SaveFileCB(int status)
{
    UIWaitSpinner_Wait2();
    RSDK.InitSceneLoad();
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
            RSDK.LoadScene("Presentation", "Level Select");
        }
        else {
            RSDK.LoadScene("Cutscenes", "Angel Island Zone");
        }
    }
    else {
        RSDK.LoadScene("Mania Mode", "");
        RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(entity->saveZoneID, entity->frameID, 0);
    }

    if (!loadingSave) {
        globals->initCoolBonus = false;
        RSDK.InitSceneLoad();
    }
}

#endif

void MenuSetup_EditorDraw(void) {}

void MenuSetup_EditorLoad(void) {}

void MenuSetup_Serialize(void) {}

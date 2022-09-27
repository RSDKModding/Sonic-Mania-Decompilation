// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimeAttackMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectTimeAttackMenu *TimeAttackMenu;

void TimeAttackMenu_Update(void)
{
    RSDK_THIS(TimeAttackMenu);

    StateMachine_Run(self->state);
}

void TimeAttackMenu_LateUpdate(void) {}

void TimeAttackMenu_StaticUpdate(void)
{
#if MANIA_USE_EGS
    if (TimeAttackMenu->taDetailsControl && !API.CheckLeaderboardsEnabled()) {
        for (int32 p = 0; p < TimeAttackMenu->taDetailsControl->promptCount; ++p) {
            EntityUIButtonPrompt *prompt = TimeAttackMenu->taDetailsControl->prompts[p];
            if (!prompt)
                continue;

            if (prompt->promptID == 5)
                prompt->visible = false;
        }
    }
#endif

    EntityUIButtonPrompt *switchModePrompt = TimeAttackMenu->switchModePrompt;

    if (switchModePrompt) {
        if (API.CheckDLC(DLC_PLUS)) {
            switchModePrompt->visible = true;
        }
        else {
            if (TimeAttackMenu->encoreMode)
                TimeAttackMenu_SetEncoreLayouts(false);

            switchModePrompt->visible = false;
        }
    }
}

void TimeAttackMenu_Draw(void) {}

void TimeAttackMenu_Create(void *data) {}

void TimeAttackMenu_StageLoad(void)
{
    TimeAttackMenu->encoreMode = false;
    TimeAttackMenu->prevIsUser = false;
    TimeAttackMenu->isUser     = false;
}

void TimeAttackMenu_Initialize(void)
{
    String tag;
    INIT_STRING(tag);

    foreach_all(UIControl, control)
    {
        RSDK.SetString(&tag, "Time Attack");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            TimeAttackMenu->timeAttackControl = control;

        RSDK.SetString(&tag, "Time Attack Legacy");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            TimeAttackMenu->timeAttackControl_Legacy = control;

        RSDK.SetString(&tag, "Time Attack Zones");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            TimeAttackMenu->taZoneSelControl = control;

        RSDK.SetString(&tag, "Time Attack Detail");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            TimeAttackMenu->taDetailsControl = control;

        RSDK.SetString(&tag, "Leaderboards");
        if (RSDK.CompareStrings(&tag, &control->tag, false)) {
            TimeAttackMenu->leaderboardsControl = control;
            control->backPressCB                = TimeAttackMenu_LeaderboardsBackPressCB;
        }

        RSDK.SetString(&tag, "Replays");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            TimeAttackMenu->replaysControl = control;
    }

    EntityUIControl *zoneControl    = TimeAttackMenu->taZoneSelControl;
    EntityUIControl *lbControl      = TimeAttackMenu->leaderboardsControl;
    EntityUIControl *replayControl  = TimeAttackMenu->replaysControl;
    EntityUIControl *detailsControl = TimeAttackMenu->taDetailsControl;

    foreach_all(UIButtonPrompt, prompt)
    {
        if (UIControl_ContainsPos(zoneControl, &prompt->position) && prompt->buttonID == 3)
            TimeAttackMenu->switchModePrompt = prompt;

        if (UIControl_ContainsPos(lbControl, &prompt->position) && prompt->buttonID == 3)
            TimeAttackMenu->topRankPrompt = prompt;

        if (UIControl_ContainsPos(replayControl, &prompt->position) && prompt->buttonID == 2)
            TimeAttackMenu->replayPrompt = prompt;
    }

    foreach_all(UILeaderboard, leaderboard) { TimeAttackMenu->leaderboard = leaderboard; }

    foreach_all(UIReplayCarousel, carousel) { TimeAttackMenu->replayCarousel = carousel; }

    foreach_all(UITABanner, banner)
    {
        if (UIControl_ContainsPos(detailsControl, &banner->position)) {
            TimeAttackMenu->detailsBanner = banner;
            banner->parent                = TimeAttackMenu->taDetailsControl;
        }

        if (UIControl_ContainsPos(lbControl, &banner->position)) {
            TimeAttackMenu->leaderboardsBanner = banner;
            banner->parent                     = TimeAttackMenu->leaderboardsControl;
        }
    }
}

void TimeAttackMenu_HandleUnlocks(void)
{
    foreach_all(UITAZoneModule, module) { module->disabled = !GameProgress_GetZoneUnlocked(module->zoneID); }
}

void TimeAttackMenu_SetupActions(void)
{
    EntityUIControl *control             = TimeAttackMenu->timeAttackControl;
    EntityUIControl *leaderboardsControl = TimeAttackMenu->leaderboardsControl;
    EntityUIControl *replayControl       = TimeAttackMenu->replaysControl;
    EntityUIControl *zoneSelControl      = TimeAttackMenu->taZoneSelControl;
    EntityUIControl *detailsControl      = TimeAttackMenu->taDetailsControl;

    control->menuUpdateCB = TimeAttackMenu_MenuUpdateCB;

    EntityUIButton *replaysButton = control->buttons[control->buttonCount - 1];
    replaysButton->actionCB       = TimeAttackMenu_ReplayButton_ActionCB;

    int32 newCount = (control->buttonCount - 1) + control->columnCount;
    for (int32 i = control->buttonCount; i < newCount; ++i) {
        control->buttons[i] = control->buttons[i - 1];
    }
    control->buttonCount = newCount;

    foreach_all(UICharButton, charButton)
    {
        if (charButton->parent == (Entity *)TimeAttackMenu->timeAttackControl
            || charButton->parent == (Entity *)TimeAttackMenu->timeAttackControl_Legacy)
            charButton->actionCB = TimeAttackMenu_CharButton_ActionCB;
    }

    foreach_all(UITAZoneModule, module) { module->actionCB = TimeAttackMenu_TAModule_ActionCB; }

    for (int32 i = 0; i < leaderboardsControl->buttonCount; ++i) {
        EntityUIRankButton *rankButton = (EntityUIRankButton *)leaderboardsControl->buttons[i];

        UIRankButton_SetRankText(rankButton, i);
    }

    leaderboardsControl->menuUpdateCB = TimeAttackMenu_MenuUpdateCB_LB;
    leaderboardsControl->yPressCB     = TimeAttackMenu_YPressCB_LB;

    replayControl->menuSetupCB  = TimeAttackMenu_MenuSetupCB_Replay;
    replayControl->menuUpdateCB = TimeAttackMenu_MenuUpdateCB_Replay;
    replayControl->xPressCB     = TimeAttackMenu_DeleteReplayActionCB;
    replayControl->yPressCB     = TimeAttackMenu_YPressCB_Replay;

    zoneSelControl->yPressCB    = TimeAttackMenu_YPressCB_ZoneSel;
    zoneSelControl->backPressCB = TimeAttackMenu_BackPressCB_ZoneSel;

#if MANIA_USE_EGS
    if (API.CheckLeaderboardsEnabled())
#endif
        detailsControl->yPressCB = TimeAttackMenu_YPressCB_Details;
    detailsControl->xPressCB    = TimeAttackMenu_XPressCB_Details;
    detailsControl->menuSetupCB = TimeAttackMenu_MenuSetupCB_Details;

    EntityUIButton *replayButton = detailsControl->buttons[0];
    // Actually a UIChoice not a TAZoneModule but it fits better with pre-plus this way
    replayButton->actionCB       = TimeAttackMenu_TAZoneModule_ActionCB;
    replayButton->choiceChangeCB = TimeAttackMenu_TAZoneModule_ChoiceChangeCB;

    EntityUIReplayCarousel *replayCarousel    = TimeAttackMenu->replayCarousel;
    replayCarousel->actionCB                  = TimeAttackMenu_ReplayCarousel_ActionCB;
    replayControl->buttons[0]->choiceChangeCB = TimeAttackMenu_SortReplayChoiceCB;
}

void TimeAttackMenu_HandleMenuReturn(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    if (param->inTimeAttack)
        TimeAttackMenu_SetEncoreLayouts(param->isEncoreMode);

    EntityUIControl *control = TimeAttackMenu->timeAttackControl;
    if (param->inTimeAttack) {
        int32 charID                         = param->characterID - 1;
        control->buttonID                    = charID;
        control->buttons[charID]->isSelected = true;
    }

    EntityUIControl *legacyControl = TimeAttackMenu->timeAttackControl_Legacy;
    if (param->inTimeAttack) {
        int32 charID = param->characterID - 1;
        if (param->characterID - 1 >= UICHARBUTTON_MIGHTY)
            charID = UICHARBUTTON_SONIC;

        legacyControl->buttonID                    = charID;
        legacyControl->buttons[charID]->isSelected = true;
    }

    EntityUIControl *zoneControl = TimeAttackMenu->taZoneSelControl;
    if (param->inTimeAttack) {
        zoneControl->buttonID                           = param->zoneID;
        zoneControl->buttons[param->zoneID]->isSelected = true;
    }

    EntityUIControl *detailsControl = TimeAttackMenu->taDetailsControl;
    if (param->inTimeAttack) {
        UITABanner_SetupDetails(TimeAttackMenu->detailsBanner, param->zoneID, param->actID, param->characterID, param->isEncoreMode);
        TimeAttackMenu_SetupDetailsView();

        UIButton_SetChoiceSelectionWithCB(detailsControl->buttons[0], param->actID);
        detailsControl->buttonID = param->menuSelection;

        TimeAttackMenu_TAZoneModule_ChoiceChangeCB();
    }

    EntityUIControl *replayControl = TimeAttackMenu->replaysControl;
    if (replayControl->active == ACTIVE_ALWAYS) {
        replayControl->buttonID = 1; // Select Replay Carousel
        UIButton_SetChoiceSelectionWithCB(replayControl->buttons[0], param->selectedReplay & 0xFF);

        EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;
        int32 replayCount                = API.GetSortedUserDBRowCount(globals->replayTableID);
        int32 targetID                   = API.GetUserDBRowByID(globals->replayTableID, param->replayUUID);

        int32 replayID = 0;
        for (; replayID < replayCount; ++replayID) {
            if (API.GetSortedUserDBRowID(globals->replayTableID, replayID) == targetID)
                break;
        }

        if (replayCount <= 0 || replayID >= replayCount)
            carousel->curReplayID = param->replayID;
        else
            carousel->curReplayID = replayID;
    }
}

void TimeAttackMenu_SetEncoreLayouts(bool32 enabled)
{
    LogHelpers_Print("SetEncoreLayouts(%d)", enabled);
    TimeAttackMenu->encoreMode = enabled;

    EntityUIButtonPrompt *prompt = TimeAttackMenu->switchModePrompt;
    prompt->promptID             = 21 - (enabled != false);

    foreach_all(UITAZoneModule, module) { module->isEncore = enabled; }
}

void TimeAttackMenu_DeleteReplayActionCB(void)
{
    EntityUIControl *replayControl   = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    if (replayControl->buttonID == 1 && carousel->stateDraw == UIReplayCarousel_Draw_Carousel
        && API.GetSortedUserDBRowCount(globals->replayTableID)) {
        String string;
        INIT_STRING(string);

        Localization_GetString(&string, STR_DELETEREPLAY);
        UIDialog_CreateDialogYesNo(&string, TimeAttackMenu_ConfirmDeleteReplay_Yes_CB, StateMachine_None, true, true);
    }
}

void TimeAttackMenu_ConfirmDeleteReplay_Yes_CB(void)
{
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    int32 row = API.GetSortedUserDBRowID(globals->replayTableID, carousel->curReplayID);
    ReplayDB_DeleteReplay(row, TimeAttackMenu_DeleteReplayCB, false);
}

void TimeAttackMenu_DeleteReplayCB(bool32 success)
{
    TimeAttackMenu_SortReplayChoiceCB();

    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    int32 count = API.GetSortedUserDBRowCount(globals->replayTableID) - 1;
    if (carousel->curReplayID > count)
        carousel->curReplayID = count;
}

void TimeAttackMenu_MenuUpdateCB_LB(void)
{
    EntityUIControl *control = TimeAttackMenu->leaderboardsControl;

    if (control->active == ACTIVE_ALWAYS) {
        EntityUICarousel *carousel = control->carousel;
        LeaderboardAvail avail     = API.LeaderboardEntryViewSize();

        if (avail.start <= 1 || carousel->scrollOffset >= avail.start + 2) {
            if (carousel->scrollOffset > avail.length - control->buttonCount + avail.start - 2) {
                // Load Down
                API.LoadLeaderboardEntries(avail.start, avail.length + 20, LEADERBOARD_LOAD_NEXT);
            }
        }
        else {
            // Load Up
            API.LoadLeaderboardEntries(avail.start - 20, avail.length + 20, LEADERBOARD_LOAD_PREV);
        }

        // Load the new entry count after (possibly) reloading new entries
        avail               = API.LeaderboardEntryViewSize();
        carousel->minOffset = MAX(avail.start, 1);
        carousel->maxOffset = MAX(avail.start + avail.length, carousel->minOffset + 5);

        for (int32 i = 0; i < control->buttonCount; ++i) {
            EntityUIRankButton *button = (EntityUIRankButton *)control->buttons[i];
            EntityUIControl *parent    = carousel->parent;

            int32 max = (i - carousel->scrollOffset) % parent->buttonCount;
            if (max < 0)
                max += parent->buttonCount;

            LeaderboardEntry *entry = API.ReadLeaderboardEntry(max + carousel->scrollOffset);
            if (entry != button->leaderboardEntry)
                UIRankButton_SetupLeaderboardRank(button, entry);
        }
    }
}

void TimeAttackMenu_SetupLeaderboards(int32 zoneID, int32 characterID, int32 act, bool32 isEncore, bool32 isUser, void (*callback)(void))
{
    String string;
    INIT_STRING(string);

    Localization_GetString(&string, STR_CONNECTING);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&string);

    if (dialog) {
        UIDialog_Setup(dialog);
        TimeAttackMenu->connectingDlg = dialog;

        EntityTimeAttackMenu *entity = CREATE_ENTITY(TimeAttackMenu, NULL, -0x100000, -0x100000);
        entity->active               = ACTIVE_NORMAL;
        entity->visible              = true;
        entity->delay                = 120;
        entity->state                = TimeAttackMenu_State_SetupLeaderboards;
        entity->callback             = callback;
        TimeAttackMenu->isUser       = API.GetSortedUserDBRowCount(globals->taTableID) != 0;
        TimeAttackMenu->prevIsUser   = isUser ? false : TimeAttackMenu->isUser;

        LeaderboardID *leaderboardInfo = TimeAttackData_GetLeaderboardInfo(zoneID, act, characterID, isEncore);
        API.FetchLeaderboard(leaderboardInfo, TimeAttackMenu->prevIsUser);

        UITABanner_SetupDetails(TimeAttackMenu->leaderboardsBanner, zoneID, act, characterID, isEncore);
    }
}

void TimeAttackMenu_ReplayCarousel_ActionCB(void)
{
    RSDK_THIS(UIReplayCarousel);

    EntityUIPopover *popover = UIPopover_CreatePopover();
    if (popover) {
        int32 y = self->position.y;
        if (!self->curReplayID)
            y += self->popoverPos;

        UIPopover_AddButton(popover, POPOVER_WATCH, TimeAttackMenu_WatchReplayActionCB_ReplaysMenu, false);
        UIPopover_AddButton(popover, POPOVER_CHALLENGE, TimeAttackMenu_ChallengeReplayActionCB_ReplaysMenu, false);
        UIPopover_AddButton(popover, POPOVER_DELETE, TimeAttackMenu_DeleteReplayActionCB, true);
        UIPopover_Setup(popover, self->position.x, y);
    }
}

void TimeAttackMenu_WatchReplay(int32 row, bool32 showGhost)
{
    EntityMenuParam *param = MenuParam_GetParam();

    int32 id = API_GetFilteredInputDeviceID(false, false, 0);
    API_ResetInputSlotAssignments();
    API_AssignInputSlotToDevice(CONT_P1, id);

    uint32 uuid = API.GetUserDBRowUUID(globals->replayTableID, row);
    LogHelpers_Print("Go_Replay(%d, %d)", row, showGhost);
    LogHelpers_Print("uuid: %08X", uuid);

    int32 score       = 0;
    uint8 zoneID      = 0;
    uint8 act         = 0;
    uint8 characterID = 0;
    uint8 encore      = 0;
    API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT32, "score", &score);
    API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "zoneID", &zoneID);
    API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "act", &act);
    API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "characterID", &characterID);
    API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "encore", &encore);

    param->viewReplay   = true;
    param->showGhost    = showGhost;
    param->replayUUID   = uuid;
    param->zoneID       = zoneID;
    param->actID        = act;
    param->characterID  = characterID;
    param->isEncoreMode = encore;

    int32 replayID = 0;
    if (!showGhost) {
        if (!TimeAttackData->loaded || characterID != TimeAttackData->characterID || zoneID != TimeAttackData->zoneID || act != TimeAttackData->act
            || encore != (int32)TimeAttackData->encore) {
            TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);
        }

        int32 entryCount = API.GetSortedUserDBRowCount(globals->taTableID);
        for (int32 rank = 1; rank < entryCount; ++rank) {
            if (uuid == (uint32)TimeAttackData_GetReplayID(zoneID, act, characterID, encore, rank))
                break;

            ++replayID;
        }
    }
    param->selectedReplay = replayID;

    EntityUIPopover *popover = UIPopover->activePopover;
    if (popover)
        popover->parent->selectionDisabled = true;

    UIWaitSpinner_StartWait();

    char fileName[0x20];
    sprintf_s(fileName, (int32)sizeof(fileName), "Replay_%08X.bin", uuid);

    memset(globals->replayTempRBuffer, 0, sizeof(globals->replayTempRBuffer));
    memset(globals->replayReadBuffer, 0, sizeof(globals->replayReadBuffer));
    ReplayRecorder_Buffer_LoadFile(fileName, globals->replayTempRBuffer, TimeAttackMenu_ReplayLoad_CB);
}

void TimeAttackMenu_ReplayLoad_CB(bool32 success)
{
    UIWaitSpinner_FinishWait();

    int32 strID = 0;
    if (success) {
        Replay *replayPtr = (Replay *)globals->replayTempRBuffer;

        if (replayPtr->header.version == GAME_VERSION) {
            LogHelpers_Print("WARNING: Replay Load OK");
            ReplayRecorder_Buffer_Unpack(globals->replayReadBuffer, globals->replayTempRBuffer);
            TimeAttackMenu_LoadScene_Fadeout();
            return;
        }

        strID = STR_CANNOTLOADREPLAY;
    }
    else {
        strID = STR_ERRORLOADINGREPLAY;
    }

    String message;
    INIT_STRING(message);
    Localization_GetString(&message, strID);

    EntityUIDialog *dialog = UIDialog_CreateDialogOk(&message, StateMachine_None, true);
    if (dialog) {
        EntityUIPopover *popover = UIPopover->activePopover;
        if (popover)
            popover->parent->selectionDisabled = false;
    }
}

void TimeAttackMenu_WatchReplayActionCB_ReplaysMenu(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    EntityUIControl *control         = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;
    EntityUIButton *button           = control->buttons[0];

    sprintf_s(param->menuTag, (int32)sizeof(param->menuTag), "Replays");
    param->replayRankID = button->selection;
    param->replayID     = carousel->curReplayID;

    int32 id = API.GetSortedUserDBRowID(globals->replayTableID, carousel->curReplayID);
    TimeAttackMenu_WatchReplay(id, false);
}

void TimeAttackMenu_ChallengeReplayActionCB_ReplaysMenu(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    EntityUIControl *control         = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;
    EntityUIButton *button           = control->buttons[0];

    sprintf_s(param->menuTag, (int32)sizeof(param->menuTag), "Replays");
    param->replayRankID = button->selection;
    param->replayID     = carousel->curReplayID;

    int32 id = API.GetSortedUserDBRowID(globals->replayTableID, carousel->curReplayID);
    TimeAttackMenu_WatchReplay(id, true);
}

void TimeAttackMenu_WatchReplayCB_RanksMenu(void)
{
    EntityMenuParam *param     = MenuParam_GetParam();
    EntityUIPopover *popover   = UIPopover->activePopover;
    EntityUIRankButton *button = (EntityUIRankButton *)popover->storedEntity;
    EntityUIControl *parent    = (EntityUIControl *)button->parent;

    int32 uuid = API.GetUserDBRowByID(globals->replayTableID, button->replayID);
    if (uuid != -1) {
        RSDK.GetCString(param->menuTag, &parent->tag);
        param->menuSelection = parent->buttonID;
        param->inTimeAttack  = true;
        TimeAttackMenu_WatchReplay(uuid, false);
    }
}

void TimeAttackMenu_ChallengeReplayCB_RanksMenu(void)
{
    EntityMenuParam *param     = MenuParam_GetParam();
    EntityUIPopover *popover   = UIPopover->activePopover;
    EntityUIRankButton *button = (EntityUIRankButton *)popover->storedEntity;
    EntityUIControl *parent    = (EntityUIControl *)button->parent;

    int32 uuid = API.GetUserDBRowByID(globals->replayTableID, button->replayID);
    if (uuid != -1) {
        RSDK.GetCString(param->menuTag, &parent->tag);
        param->menuSelection = parent->buttonID;
        param->inTimeAttack  = true;
        TimeAttackMenu_WatchReplay(uuid, true);
    }
}

void TimeAttackMenu_LoadScene_Fadeout(void)
{
    Music_FadeOut(0.05);
    MenuSetup_StartTransition(TimeAttackMenu_LoadScene, 32);
}

void TimeAttackMenu_MenuSetupCB_Replay(void)
{
    if (API.GetUserDBRowsChanged(globals->replayTableID))
        TimeAttackMenu_SortReplayChoiceCB();
}

void TimeAttackMenu_MenuUpdateCB_Replay(void)
{
    EntityUIControl *replayControl   = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    if (replayControl->lastButtonID != 1)
        carousel->curReplayID = -1;

    EntityUIButtonPrompt *prompt = TimeAttackMenu->replayPrompt;
    if (prompt)
        prompt->visible = API.GetSortedUserDBRowCount(globals->replayTableID) != 0;
}

void TimeAttackMenu_MenuUpdateCB(void)
{
    EntityUIControl *control = TimeAttackMenu->timeAttackControl;

    // Keep "Replays" button centered
    control->buttons[control->buttonCount - 1]->position.x = control->position.x;
}

void TimeAttackMenu_ReplayButton_ActionCB(void)
{
    EntityUIControl *control         = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    control->buttonID     = 1;
    carousel->curReplayID = 0;
    UIControl_MatchMenuTag("Replays");

    UIButton_SetChoiceSelectionWithCB(control->buttons[0], 0);
}

void TimeAttackMenu_YPressCB_Replay(void)
{
    EntityUIControl *control         = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    if (carousel->stateDraw == UIReplayCarousel_Draw_Carousel) {
        control->buttonID = 0;

        carousel->curReplayID      = -1;
        carousel->curViewOffset    = 0;
        carousel->targetViewOffset = 0;

        UIButton_SetChoiceSelectionWithCB(control->buttons[0], control->buttons[0]->selection ^ 1);

        RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
    }
}

void TimeAttackMenu_SortReplayChoiceCB(void)
{
    EntityUIControl *control         = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    API.SetupUserDBRowSorting(globals->replayTableID);

    if (control->buttons[0]->selection == 1)
        API.SortDBRows(globals->replayTableID, DBVAR_UINT32, "zoneSortVal", false);
    else
        API.SortDBRows(globals->replayTableID, 0, NULL, true);

    carousel->stateDraw = UIReplayCarousel_Draw_Loading;
}

void TimeAttackMenu_SetupDetailsView(void)
{
    EntityMenuParam *param   = MenuParam_GetParam();
    EntityUIControl *control = TimeAttackMenu->taDetailsControl;
    EntityUITABanner *banner = TimeAttackMenu->detailsBanner;

    EntityUIChoice *act1 = (EntityUIChoice *)UIButton_GetChoicePtr(control->buttons[0], 0);
    EntityUIChoice *act2 = (EntityUIChoice *)UIButton_GetChoicePtr(control->buttons[0], 1);

    if (!TimeAttackMenu->encoreMode && banner->zoneID == 7 && param->characterID != 3) {
        // Prevent us from selecting MSZ1 in mania mode if we're not knux
        act1->disabled = true;
        UIButton_SetChoiceSelectionWithCB(control->buttons[0], 1);
        act2->arrowWidth = 0;
    }
    else {
        act1->disabled = false;
        UIButton_SetChoiceSelectionWithCB(control->buttons[0], 0);
        act2->arrowWidth = 48;
    }
}

void TimeAttackMenu_TAModule_ActionCB(void)
{
    RSDK_THIS(UITAZoneModule);

    EntityMenuParam *param   = MenuParam_GetParam();
    EntityUITABanner *banner = TimeAttackMenu->detailsBanner;
    EntityUIControl *control = TimeAttackMenu->taDetailsControl;

    param->zoneID     = self->zoneID;
    control->buttonID = 0;

    UITABanner_SetupDetails(banner, self->zoneID, 0, self->characterID, TimeAttackMenu->encoreMode);
    TimeAttackMenu_SetupDetailsView();

    UITransition_SetNewTag("Time Attack Detail");
}

void TimeAttackMenu_StartTAAttempt(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    sprintf_s(param->menuTag, (int32)sizeof(param->menuTag), "Time Attack Detail");
    param->menuSelection = 0;
    param->inTimeAttack  = true;
    param->isEncoreMode  = TimeAttackMenu->encoreMode;

    Replay *replayPtr = (Replay *)globals->replayReadBuffer;
    if (replayPtr->header.isNotEmpty && replayPtr->header.signature == REPLAY_SIGNATURE)
        memset(globals->replayReadBuffer, 0, sizeof(globals->replayReadBuffer));

    TimeAttackMenu_LoadScene();
}

void TimeAttackMenu_LoadScene(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

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

    if (param->isEncoreMode)
        RSDK.SetScene("Encore Mode", "");
    else
        RSDK.SetScene("Mania Mode", "");

    if (param->isEncoreMode)
        SceneInfo->listPos += TimeAttackData_GetEncoreListPos(param->zoneID, param->actID, param->characterID);
    else
        SceneInfo->listPos += TimeAttackData_GetManiaListPos(param->zoneID, param->actID, param->characterID);

    switch (param->characterID) {
        case 1: globals->playerID = ID_SONIC; break;
        case 2: globals->playerID = ID_TAILS; break;
        case 3: globals->playerID = ID_KNUCKLES; break;
        case 4: globals->playerID = ID_MIGHTY; break;
        case 5: globals->playerID = ID_RAY; break;
        default: break;
    }

    RSDK.LoadScene();
}

void TimeAttackMenu_YPressCB_ZoneSel(void)
{
    if (API.CheckDLC(DLC_PLUS)) {
        TimeAttackMenu_SetEncoreLayouts(!TimeAttackMenu->encoreMode);
        RSDK.PlaySfx(UIWidgets->sfxWoosh, false, 255);
    }
}

bool32 TimeAttackMenu_BackPressCB_ZoneSel(void)
{
    if (!API.CheckDLC(DLC_PLUS))
        UITransition_SetNewTag("Time Attack Legacy");
    else
        UITransition_SetNewTag("Time Attack");

    return true;
}

void TimeAttackMenu_YPressCB_Details(void)
{
    EntityUITABanner *banner = TimeAttackMenu->detailsBanner;

    int32 rowCount = API.GetSortedUserDBRowCount(globals->taTableID);
    TimeAttackMenu_SetupLeaderboards(banner->zoneID, banner->characterID, banner->actID, TimeAttackMenu->encoreMode, !rowCount,
                                     UITAZoneModule_ShowLeaderboards_CB);
}

void TimeAttackMenu_ResetTimes_YesCB(void)
{
    EntityUITABanner *banner = TimeAttackMenu->detailsBanner;
    EntityUIControl *control = TimeAttackMenu->taDetailsControl;

    int32 act = control->buttons[0]->selection;
    while (API.GetSortedUserDBRowCount(globals->taTableID) > 0) {
        int32 rowID = API.GetSortedUserDBRowID(globals->taTableID, 0);
        API.RemoveDBRow(globals->taTableID, rowID);

        TimeAttackData_ConfigureTableView(banner->zoneID, act, banner->characterID, TimeAttackMenu->encoreMode);
    }

    control->buttonID = 0;
    TimeAttackData_SaveDB(NULL);

    TimeAttackMenu_TAZoneModule_ChoiceChangeCB();
}

void TimeAttackMenu_XPressCB_Details(void)
{
    String message;
    INIT_STRING(message);

    Localization_GetString(&message, STR_RESETTIMESWARNING);
    UIDialog_CreateDialogYesNo(&message, TimeAttackMenu_ResetTimes_YesCB, StateMachine_None, true, true);
}

void TimeAttackMenu_TAZoneModule_ActionCB(void)
{
    RSDK_THIS(UIButton);

    EntityMenuParam *param  = MenuParam_GetParam();
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    parent->selectionDisabled = true;
    param->actID              = self->selection;

    MenuSetup_StartTransition(TimeAttackMenu_StartTAAttempt, 32);
}

void TimeAttackMenu_RankButton_ActionCB(void)
{
    RSDK_THIS(UIRankButton);

    EntityUIPopover *popover = UIPopover_CreatePopover();
    if (popover) {
        popover->storedEntity = (Entity *)self;

        UIPopover_AddButton(popover, POPOVER_WATCH, TimeAttackMenu_WatchReplayCB_RanksMenu, false);
        UIPopover_AddButton(popover, POPOVER_CHALLENGE, TimeAttackMenu_ChallengeReplayCB_RanksMenu, false);
        UIPopover_Setup(popover, self->popoverPos.x, self->popoverPos.y);
    }
}

void TimeAttackMenu_MenuSetupCB_Details(void) { TimeAttackMenu_TAZoneModule_ChoiceChangeCB(); }

void TimeAttackMenu_TAZoneModule_ChoiceChangeCB(void)
{
    EntityMenuParam *param   = MenuParam_GetParam();
    EntityUIControl *control = TimeAttackMenu->taDetailsControl;

    int32 act = control->buttons[0]->selection;

    UITABanner_SetupDetails(TimeAttackMenu->detailsBanner, param->zoneID, act, param->characterID, TimeAttackMenu->encoreMode);
    TimeAttackData_ConfigureTableView(param->zoneID, act, param->characterID, TimeAttackMenu->encoreMode);

    int32 rowCount = 1;
    for (int32 rank = 1; rank < 4; ++rank) {
        EntityUIRankButton *rankButton = (EntityUIRankButton *)control->buttons[rank];

        int32 score    = TimeAttackData_GetScore(param->zoneID, act, param->characterID, TimeAttackMenu->encoreMode, rank);
        int32 replayID = TimeAttackData_GetReplayID(param->zoneID, act, param->characterID, TimeAttackMenu->encoreMode, rank);

        UIRankButton_SetTimeAttackRank(rankButton, rank, score, replayID);

        if (score)
            ++rowCount;

        if (API.CheckDLC(DLC_PLUS) && replayID)
            rankButton->actionCB = TimeAttackMenu_RankButton_ActionCB;
        else
            rankButton->actionCB = StateMachine_None;
    }

    control->rowCount = rowCount;
}

void TimeAttackMenu_CharButton_ActionCB(void)
{
    RSDK_THIS(UICharButton);

    EntityMenuParam *param   = MenuParam_GetParam();
    EntityUIControl *control = TimeAttackMenu->taZoneSelControl;

    TimeAttackData_Clear();

    int32 characterID  = self->characterID + 1;
    param->characterID = characterID;

    API.InitLeaderboards();

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *charButton = (EntityUITAZoneModule *)control->buttons[i];
        charButton->characterID          = characterID;
    }

    UIControl_MatchMenuTag("Time Attack Zones");
}

void TimeAttackMenu_TransitionToDetailsCB(void)
{
    UIControl_SetInactiveMenu(TimeAttackMenu->leaderboardsControl);
    UIControl_SetActiveMenu(TimeAttackMenu->taDetailsControl);
}

bool32 TimeAttackMenu_LeaderboardsBackPressCB(void)
{
    UITransition_StartTransition(TimeAttackMenu_TransitionToDetailsCB, 0);

    return true;
}

void TimeAttackMenu_YPressCB_LB(void)
{
    EntityUITABanner *banner = TimeAttackMenu->leaderboardsBanner;

    if (TimeAttackMenu->isUser)
        TimeAttackMenu_SetupLeaderboards(banner->zoneID, banner->characterID, banner->actID, banner->isEncore, TimeAttackMenu->prevIsUser,
                                         StateMachine_None);
}

void TimeAttackMenu_State_SetupLeaderboards(void)
{
    RSDK_THIS(TimeAttackMenu);

    String string;
    INIT_STRING(string);

    EntityUIDialog *dialog = TimeAttackMenu->connectingDlg;
    int32 status           = API.GetLeaderboardsStatus();

    switch (status) {
        case STATUS_CONTINUE:
            Localization_GetString(&string, STR_CONNECTING);
            UIDialog_SetupText(dialog, &string);
            break;

        default:
        case STATUS_ERROR: {
            if (status < STATUS_ERROR) {
                int32 strID = status == STATUS_TIMEOUT ? STR_COMMERROR : STR_NOWIFI;
                Localization_GetString(&string, strID);

                UIDialog_SetupText(dialog, &string);
                UIDialog_AddButton(DIALOG_OK, dialog, StateMachine_None, true);

                EntityUIControl *control   = dialog->parent;
                control->rowCount          = 1;
                control->columnCount       = 1;
                control->buttonID          = 0;
                TimeAttackMenu->prevIsUser = !TimeAttackMenu->prevIsUser;

                EntityUIButtonPrompt *topRankPrompt = TimeAttackMenu->topRankPrompt;
                int32 id                            = -(TimeAttackMenu->prevIsUser != 0);
                topRankPrompt->prevPrompt           = -1;
                topRankPrompt->promptID             = (id + 15);
                topRankPrompt->visible              = !TimeAttackMenu->isUser;

                self->callback                = StateMachine_None;
                TimeAttackMenu->connectingDlg = NULL;
                destroyEntity(self);
                break;
            }
            // otherwise, fallthrough and do this
        }

        case STATUS_OK: {
            EntityUIButtonPrompt *topRankPrompt = TimeAttackMenu->topRankPrompt;
            int32 id                            = -(TimeAttackMenu->prevIsUser != false);
            topRankPrompt->prevPrompt           = -1;
            topRankPrompt->promptID             = id + 15;
            topRankPrompt->visible              = TimeAttackMenu->isUser;

            EntityUIControl *leaderboardsControl = TimeAttackMenu->leaderboardsControl;
            TimeAttackMenu_SetupLeaderboardsCarousel(leaderboardsControl->carousel);
            UIDialog_CloseOnSel_HandleSelection(dialog, self->callback);

            self->callback                = StateMachine_None;
            TimeAttackMenu->connectingDlg = NULL;
            destroyEntity(self);
            break;
        }
    }
}

void TimeAttackMenu_SetupLeaderboardsCarousel(EntityUICarousel *carousel)
{
    EntityUIControl *parent = (EntityUIControl *)carousel->parent;

    LeaderboardAvail avail = API.LeaderboardEntryViewSize();
    carousel->minOffset    = MAX(avail.start, 1);
    carousel->maxOffset    = MAX(avail.start + avail.length, carousel->minOffset + 5);

    if (TimeAttackMenu->prevIsUser && avail.length) {
        int32 userID = 0;

        int32 end = avail.start + avail.length;
        for (int32 entryID = avail.start; entryID < end; ++entryID) {
            LeaderboardEntry *entry = API.ReadLeaderboardEntry(entryID);
            if (entry->isUser) {
                userID = entryID;
                break;
            }
        }

        carousel->scrollOffset = userID != parent->buttonCount >> 1 ? userID - (parent->buttonCount >> 1) : 0;
        carousel->virtualIndex = !userID ? avail.start : userID;
    }
    else {
        carousel->virtualIndex = 1;
        carousel->scrollOffset = 0;
    }

    for (int32 i = 0; i < parent->buttonCount; ++i) {
        int32 max = (i - carousel->scrollOffset) % parent->buttonCount;
        if (max < 0)
            max += parent->buttonCount;

        int32 vIndex = max + carousel->scrollOffset;
        LogHelpers_Print("i = %d, vIndex = %d", i, vIndex);

        if (vIndex == carousel->virtualIndex) {
            parent->buttonID = i;
            break;
        }
    }

    LogHelpers_Print("avail.start = %d, length = %d", avail.start, avail.length);
    LogHelpers_Print("scrollOffset = %d", carousel->scrollOffset);
    LogHelpers_Print("virtualIndex = %d", carousel->virtualIndex);
    LogHelpers_Print("minOffset = %d", carousel->minOffset);
    LogHelpers_Print("maxOffset = %d", carousel->maxOffset);

    carousel->buttonID = -1;
}

#if RETRO_INCLUDE_EDITOR
void TimeAttackMenu_EditorDraw(void) {}

void TimeAttackMenu_EditorLoad(void) {}
#endif

void TimeAttackMenu_Serialize(void) {}
#endif

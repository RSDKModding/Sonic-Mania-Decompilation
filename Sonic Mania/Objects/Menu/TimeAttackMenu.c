// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimeAttackMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectTimeAttackMenu *TimeAttackMenu;

void TimeAttackMenu_Update(void)
{
    RSDK_THIS(TimeAttackMenu);
    StateMachine_Run(self->state);
}

void TimeAttackMenu_LateUpdate(void) {}

void TimeAttackMenu_StaticUpdate(void)
{
    EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->switchModePrompt;
    if (prompt) {
        if (API.CheckDLC(DLC_PLUS)) {
            prompt->visible = true;
        }
        else {
            if (TimeAttackMenu->encoreMode)
                TimeAttackMenu_SetEncoreLayouts(false);
            prompt->visible = false;
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
    TextInfo info;
    INIT_TEXTINFO(info);

    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&info, "Time Attack");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->timeAttackControl = control;

        RSDK.PrependText(&info, "Time Attack Legacy");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->timeAttackControl_Legacy = control;

        RSDK.PrependText(&info, "Time Attack Zones");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->taZoneSelControl = control;

        RSDK.PrependText(&info, "Time Attack Detail");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->taDetailsControl = control;

        RSDK.PrependText(&info, "Leaderboards");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            TimeAttackMenu->leaderboardsControl = control;
            control->backPressCB                = TimeAttackMenu_LeaderboardsBackPressCB;
        }

        RSDK.PrependText(&info, "Replays");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->replaysControl = control;
    }

    EntityUIControl *zoneControl    = TimeAttackMenu->taZoneSelControl;
    EntityUIControl *lbControl      = TimeAttackMenu->leaderboardsControl;
    EntityUIControl *replayControl  = TimeAttackMenu->replaysControl;
    EntityUIControl *detailsControl = TimeAttackMenu->taDetailsControl;

    Hitbox hitbox;
    foreach_all(UIButtonPrompt, prompt)
    {
        hitbox.right  = zoneControl->size.x >> 17;
        hitbox.left   = -(zoneControl->size.x >> 17);
        hitbox.bottom = zoneControl->size.y >> 17;
        hitbox.top    = -(zoneControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(zoneControl->startPos.x - zoneControl->cameraOffset.x, zoneControl->startPos.y - zoneControl->cameraOffset.y,
                                      prompt->position.x, prompt->position.y, FLIP_NONE, &hitbox)
            && prompt->buttonID == 3) {
            TimeAttackMenu->switchModePrompt = prompt;
        }

        hitbox.right  = lbControl->size.x >> 17;
        hitbox.left   = -(lbControl->size.x >> 17);
        hitbox.bottom = lbControl->size.y >> 17;
        hitbox.top    = -(lbControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(lbControl->startPos.x - lbControl->cameraOffset.x, lbControl->startPos.y - lbControl->cameraOffset.y,
                                      prompt->position.x, prompt->position.y, FLIP_NONE, &hitbox)
            && prompt->buttonID == 3) {
            TimeAttackMenu->topRankPrompt = prompt;
        }

        hitbox.right  = replayControl->size.x >> 17;
        hitbox.left   = -(replayControl->size.x >> 17);
        hitbox.bottom = replayControl->size.y >> 17;
        hitbox.top    = -(replayControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(replayControl->startPos.x - replayControl->cameraOffset.x,
                                      replayControl->startPos.y - replayControl->cameraOffset.y, prompt->position.x, prompt->position.y, FLIP_NONE,
                                      &hitbox)
            && prompt->buttonID == 2)
            TimeAttackMenu->replayPrompt = prompt;
    }

    foreach_all(UILeaderboard, leaderboard) { TimeAttackMenu->leaderboard = leaderboard; }

    foreach_all(UIReplayCarousel, carousel) { TimeAttackMenu->replayCarousel = carousel; }

    foreach_all(UITABanner, banner)
    {
        hitbox.right  = detailsControl->size.x >> 17;
        hitbox.left   = -(detailsControl->size.x >> 17);
        hitbox.bottom = detailsControl->size.y >> 17;
        hitbox.top    = -(detailsControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(detailsControl->startPos.x - detailsControl->cameraOffset.x,
                                      detailsControl->startPos.y - detailsControl->cameraOffset.y, banner->position.x, banner->position.y, FLIP_NONE,
                                      &hitbox)) {
            TimeAttackMenu->detailsBanner = banner;
            banner->parent                = TimeAttackMenu->taDetailsControl;
        }

        hitbox.right  = lbControl->size.x >> 17;
        hitbox.left   = -(lbControl->size.x >> 17);
        hitbox.bottom = lbControl->size.y >> 17;
        hitbox.top    = -(lbControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(lbControl->startPos.x - lbControl->cameraOffset.x, lbControl->startPos.y - lbControl->cameraOffset.y,
                                      banner->position.x, banner->position.y, FLIP_NONE, &hitbox)) {
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

    EntityUIButton *button = control->buttons[control->buttonCount - 1];
    button->actionCB       = TimeAttackMenu_ReplayButton_ActionCB;
    int32 newCount         = (control->buttonCount - 1) + control->columnCount;
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
        EntityUILeaderboard *button = (EntityUILeaderboard *)leaderboardsControl->buttons[i];
        char buffer[0x10];
        if (i)
            sprintf(buffer, "%d", i);
        else
            sprintf(buffer, "-");

        RSDK.PrependText(&button->text1[0], buffer);
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &button->text1[0]);
    }

    leaderboardsControl->menuUpdateCB = TimeAttackMenu_MenuUpdateCB_LB;
    leaderboardsControl->yPressCB     = TimeAttackMenu_YPressCB_LB;

    replayControl->menuSetupCB  = TimeAttackMenu_MenuSetupCB_Replay;
    replayControl->menuUpdateCB = TimeAttackMenu_MenuUpdateCB_Replay;
    replayControl->xPressCB     = TimeAttackMenu_DeleteReplayActionCB;
    replayControl->yPressCB     = TimeAttackMenu_YPressCB_Replay;

    zoneSelControl->yPressCB    = TimeAttackMenu_YPressCB_ZoneSel;
    zoneSelControl->backPressCB = TimeAttackMenu_BackPressCB_ZoneSel;

    detailsControl->yPressCB    = TimeAttackMenu_YPressCB_Details;
    detailsControl->xPressCB    = TimeAttackMenu_XPressCB_Details;
    detailsControl->menuSetupCB = TimeAttackMenu_MenuSetupCB_Details;

    EntityUIButton *replayButton = detailsControl->buttons[0];
    replayButton->actionCB       = TimeAttackMenu_Replays_ActionCB;
    replayButton->choiceChangeCB = TimeAttackMenu_Replays_ChoiceChangeCB;

    EntityUIReplayCarousel *replayCarousel    = TimeAttackMenu->replayCarousel;
    replayCarousel->actionCB                  = TimeAttackMenu_ReplayCarousel_ActionCB;
    replayControl->buttons[0]->choiceChangeCB = TimeAttackMenu_SortReplayChoiceCB;
}

void TimeAttackMenu_HandleMenuReturn(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
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
        int32 charID = 0;
        if (param->characterID - 1 <= 2)
            charID = param->characterID - 1;

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
        UITABanner_SetupDetails(param->characterID, TimeAttackMenu->detailsBanner, param->zoneID, param->actID, param->isEncoreMode);
        TimeAttackMenu_SetupDetailsView();
        UIButton_SetChoiceSelectionWithCB(detailsControl->buttons[0], param->actID);
        detailsControl->buttonID = param->menuSelection;
        TimeAttackMenu_Replays_ChoiceChangeCB();
    }

    EntityUIControl *replayControl = TimeAttackMenu->replaysControl;
    if (replayControl->active == ACTIVE_ALWAYS) {
        replayControl->buttonID = 1;
        UIButton_SetChoiceSelectionWithCB(replayControl->buttons[0], param->selectedReplay & 0xFF);
        EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;
        int32 count                      = API.GetSortedUserDBRowCount(globals->replayTableID);
        int32 id                         = API.GetUserDBRowByID(globals->replayTableID, param->replayUUID);

        int32 i = 0;
        for (; i < count; ++i) {
            if (API.GetSortedUserDBRowID(globals->replayTableID, i) == id)
                break;
        }
        if (count <= 0 || i >= count)
            carousel->curReplayID = param->replayID;
        else
            carousel->curReplayID = i;
    }
}

void TimeAttackMenu_SetEncoreLayouts(bool32 enabled)
{
    LogHelpers_Print("SetEncoreLayouts(%d)", enabled);
    TimeAttackMenu->encoreMode   = enabled;
    EntityUIButtonPrompt *prompt = TimeAttackMenu->switchModePrompt;
    prompt->promptID             = 21 - (enabled != false);
    foreach_active(UITAZoneModule, module) { module->isEncore = enabled; }
}

void TimeAttackMenu_DeleteReplayActionCB(void)
{
    EntityUIControl *replayControl   = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    if (replayControl->buttonID == 1 && carousel->stateDraw == UIReplayCarousel_Draw_Carousel
        && API.GetSortedUserDBRowCount(globals->replayTableID)) {
        TextInfo info;
        INIT_TEXTINFO(info);
        Localization_GetString(&info, STR_DELETEREPLAY);
        UIDialog_CreateDialogYesNo(&info, TimeAttackMenu_ConfirmDeleteReplay_Yes_CB, StateMachine_None, true, true);
    }
}

void TimeAttackMenu_ConfirmDeleteReplay_Yes_CB(void)
{
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;
    int32 row                        = API.GetSortedUserDBRowID(globals->replayTableID, carousel->curReplayID);
    ReplayRecorder_DeleteReplay(row, TimeAttackMenu_DeleteReplayCB, false);
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
        Vector2 entryCount         = API.LeaderboardEntryCount();
        if (entryCount.x <= 1 || carousel->scrollOffset >= (entryCount.x + 2)) {
            if (carousel->scrollOffset > entryCount.y - control->buttonCount + entryCount.x - 2) {
                // Load Down
                API.LoadNewLeaderboardEntries(entryCount.x, entryCount.y + 20, 2);
            }
        }
        else {
            // Load Up
            API.LoadNewLeaderboardEntries(entryCount.x - 20, entryCount.y + 20, 1);
        }

        entryCount          = API.LeaderboardEntryCount();
        int32 count         = maxVal(1, entryCount.x);
        carousel->minOffset = count;

        if (entryCount.y + entryCount.x > count + 5)
            carousel->maxOffset = entryCount.y + entryCount.x;
        else
            carousel->maxOffset = count + 5;

        for (int32 i = 0; i < control->buttonCount; ++i) {
            EntityUIRankButton *button = (EntityUIRankButton *)control->buttons[i];
            EntityUIControl *parent    = (EntityUIControl *)carousel->parent;

            int32 max = (i - carousel->scrollOffset) % parent->buttonCount;
            if (max < 0)
                max += parent->buttonCount;

            LeaderboardEntry *entry = API.ReadLeaderboardEntry(max + carousel->scrollOffset);
            if (entry != button->leaderboardEntry)
                UIRankButton_SetupLeaderboardRank(button, entry);
        }
    }
}

void TimeAttackMenu_SetupLeaderboards(int32 zoneID, int32 characterID, int32 act, bool32 isEncore, bool32 curIsUser, void (*callback)(void))
{
    TextInfo info;
    INIT_TEXTINFO(info);
    Localization_GetString(&info, STR_CONNECTING);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_Setup(dialog);
        TimeAttackMenu->connectingDlg = dialog;
        EntityTimeAttackMenu *entity  = CREATE_ENTITY(TimeAttackMenu, NULL, -0x100000, -0x100000);
        entity->active                = ACTIVE_NORMAL;
        entity->visible               = true;
        entity->delay                 = 120;
        entity->state                 = TimeAttackMenu_State_SetupLeaderboards;
        entity->callback              = callback;
        TimeAttackMenu->isUser        = API.GetSortedUserDBRowCount(globals->taTableID) != 0;
        if (curIsUser)
            TimeAttackMenu->prevIsUser = false;
        else
            TimeAttackMenu->prevIsUser = TimeAttackMenu->isUser;

        LeaderboardID *leaderboardInfo = NULL;
        if (zoneID > 11 || act > 1 || (characterID - 1) <= 4) {
            int32 pos = act + 2 * zoneID - 1 + characterID + 4 * (act + 2 * zoneID);
            if (isEncore)
                pos += 120;
            leaderboardInfo = &maniaLeaderboardInfo[pos];
        }
        API.FetchLeaderboard(leaderboardInfo, TimeAttackMenu->prevIsUser);
        UITABanner_SetupDetails(characterID, TimeAttackMenu->leaderboardsBanner, zoneID, act, isEncore);
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
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    int32 id               = RSDK.MostRecentActiveControllerID(0, 0, 0);
    RSDK.ResetControllerAssignments();
    RSDK.AssignControllerID(CONT_P1, id);
    LogHelpers_Print("Go_Replay(%d, %d)", row, showGhost);
    uint32 uuid = API.GetUserDBRowUUID(globals->replayTableID, row);
    LogHelpers_Print("uuid: %08X", uuid);

    int32 score       = 0;
    int32 zoneID      = 0;
    int32 act         = 0;
    int32 characterID = 0;
    int32 encore      = 0;
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
            || encore != TimeAttackData->encore) {
            TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);
        }

        int32 count = API.GetSortedUserDBRowCount(globals->taTableID);

        for (int32 i = 1; i < count; ++i) {
            if (uuid == TimeAttackData_GetReplayID(zoneID, act, characterID, encore, i)) {
                break;
            }
            ++replayID;
        }
    }

    param->selectedReplay    = replayID;
    EntityUIPopover *popover = UIPopover->activeEntity;
    if (popover)
        popover->parent->selectionDisabled = true;

    UIWaitSpinner_StartWait();

    char fileBuffer[0x20];
    sprintf(fileBuffer, "Replay_%08X.bin", uuid);

    memset(globals->replayTempRBuffer, 0, sizeof(globals->replayTempRBuffer));
    memset(globals->replayReadBuffer, 0, sizeof(globals->replayReadBuffer));
    ReplayRecorder_Buffer_LoadFile(fileBuffer, globals->replayTempRBuffer, TimeAttackMenu_ReplayLoad_CB);
}

void TimeAttackMenu_ReplayLoad_CB(bool32 success)
{
    UIWaitSpinner_FinishWait();

    int32 strID = 0;
    if (success) {
        Replay *replayPtr = (Replay *)globals->replayTempRBuffer;
        if (replayPtr->header.version == RETRO_GAMEVER) {
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

    TextInfo info;
    INIT_TEXTINFO(info);
    Localization_GetString(&info, strID);

    EntityUIDialog *dialog = UIDialog_CreateDialogOk(&info, StateMachine_None, true);
    if (dialog) {
        EntityUIPopover *popover = UIPopover->activeEntity;
        if (popover)
            popover->parent->selectionDisabled = false;
    }
}

void TimeAttackMenu_WatchReplayActionCB_ReplaysMenu(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    EntityUIControl *control         = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    EntityUIButton *button = control->buttons[0];
    sprintf(param->menuTag, "Replays");
    param->replayRankID = button->selection;
    param->replayID     = carousel->curReplayID;
    int32 id            = API.GetSortedUserDBRowID(globals->replayTableID, carousel->curReplayID);
    TimeAttackMenu_WatchReplay(id, false);
}

void TimeAttackMenu_ChallengeReplayActionCB_ReplaysMenu(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    EntityUIControl *control         = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    EntityUIButton *button = control->buttons[0];
    sprintf(param->menuTag, "Replays");
    param->replayRankID = button->selection;
    param->replayID     = carousel->curReplayID;
    int32 id            = API.GetSortedUserDBRowID(globals->replayTableID, carousel->curReplayID);
    TimeAttackMenu_WatchReplay(id, true);
}

void TimeAttackMenu_WatchReplayCB_RanksMenu(void)
{
    EntityMenuParam *param     = (EntityMenuParam *)globals->menuParam;
    EntityUIPopover *popover   = UIPopover->activeEntity;
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
    EntityMenuParam *param     = (EntityMenuParam *)globals->menuParam;
    EntityUIPopover *popover   = UIPopover->activeEntity;
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
    int32 status = API.GetUserDBRowsChanged(globals->replayTableID);
    if (status)
        TimeAttackMenu_SortReplayChoiceCB();
}

void TimeAttackMenu_MenuUpdateCB_Replay(void)
{
    EntityUIControl *replayControl   = TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = TimeAttackMenu->replayCarousel;

    if (replayControl->lastButtonID != 1) {
        carousel->curReplayID = -1;
    }

    EntityUIButtonPrompt *prompt = TimeAttackMenu->replayPrompt;
    if (prompt)
        prompt->visible = API.GetSortedUserDBRowCount(globals->replayTableID) != 0;
}

void TimeAttackMenu_MenuUpdateCB(void)
{
    EntityUIControl *control                               = TimeAttackMenu->timeAttackControl;
    control->prompts[control->buttonCount - 1]->position.x = control->position.x;
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
        control->buttonID          = 0;
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
        API.SortDBRows(globals->replayTableID, DBVAR_NONE, NULL, true);
    carousel->stateDraw = UIReplayCarousel_Draw_Loading;
}

void TimeAttackMenu_SetupDetailsView(void)
{
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *control = TimeAttackMenu->taDetailsControl;
    EntityUITABanner *banner = TimeAttackMenu->detailsBanner;

    EntityUIChoice *choice1 = (EntityUIChoice *)UIButton_GetChoicePtr(control->buttons[0], 0);
    EntityUIChoice *choice2 = (EntityUIChoice *)UIButton_GetChoicePtr(control->buttons[0], 1);
    if (TimeAttackMenu->encoreMode || banner->zoneID != 7 || param->characterID == 3) {
        choice1->disabled = false;
        UIButton_SetChoiceSelectionWithCB(control->buttons[0], 0);
        choice2->arrowWidth = 48;
    }
    else {
        choice1->disabled = true;
        UIButton_SetChoiceSelectionWithCB(control->buttons[0], 1);
        choice2->arrowWidth = 0;
    }
}

void TimeAttackMenu_TAModule_ActionCB(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUITABanner *banner = TimeAttackMenu->detailsBanner;
    EntityUIControl *control = TimeAttackMenu->taDetailsControl;

    param->zoneID     = self->zoneID;
    control->buttonID = 0;

    UITABanner_SetupDetails(self->characterID, banner, self->zoneID, 0, TimeAttackMenu->encoreMode);
    TimeAttackMenu_SetupDetailsView();
    UITransition_SetNewTag("Time Attack Detail");
}

void TimeAttackMenu_StartTAAttempt(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    sprintf(param->menuTag, "Time Attack Detail");
    param->menuSelection = 0;
    param->inTimeAttack  = true;
    param->isEncoreMode  = TimeAttackMenu->encoreMode;

    Replay *replayPtr = (Replay *)globals->replayReadBuffer;

    if (replayPtr->header.isNotEmpty && replayPtr->header.signature == Replay_Signature)
        memset(globals->replayReadBuffer, 0, sizeof(globals->replayReadBuffer));
    TimeAttackMenu_LoadScene();
}

void TimeAttackMenu_LoadScene(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    SaveGame_ResetPlayerState();
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
    int32 count              = API.GetSortedUserDBRowCount(globals->taTableID);
    TimeAttackMenu_SetupLeaderboards(banner->zoneID, banner->characterID, banner->actID, TimeAttackMenu->encoreMode, !count,
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
    if (!checkNoSave && globals->taTableID != (uint16)-1 && globals->taTableLoaded == STATUS_OK) {
        LogHelpers_Print("Saving Time Attack DB");
        TimeAttackData->saveEntityPtr = SceneInfo->entity;
        TimeAttackData->saveCallback  = NULL;
        API.SaveUserDB(globals->taTableID, TimeAttackData_SaveTimeAttackDB_CB);
    }
    TimeAttackMenu_Replays_ChoiceChangeCB();
}

void TimeAttackMenu_XPressCB_Details(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);
    Localization_GetString(&info, STR_RESETTIMESWARNING);
    UIDialog_CreateDialogYesNo(&info, TimeAttackMenu_ResetTimes_YesCB, StateMachine_None, true, true);
}

void TimeAttackMenu_Replays_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityMenuParam *param  = (EntityMenuParam *)globals->menuParam;
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

void TimeAttackMenu_MenuSetupCB_Details(void) { TimeAttackMenu_Replays_ChoiceChangeCB(); }

void TimeAttackMenu_Replays_ChoiceChangeCB(void)
{
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *control = TimeAttackMenu->taDetailsControl;

    int32 act = control->buttons[0]->selection;
    UITABanner_SetupDetails(param->characterID, TimeAttackMenu->detailsBanner, param->zoneID, act, TimeAttackMenu->encoreMode);
    TimeAttackData_ConfigureTableView(param->zoneID, act, param->characterID, TimeAttackMenu->encoreMode);

    int32 count = 1;
    for (int32 i = 1; i < 4; ++i) {
        EntityUIRankButton *button = (EntityUIRankButton *)control->buttons[i];
        int32 score                = TimeAttackData_GetScore(param->zoneID, act, param->characterID, TimeAttackMenu->encoreMode, i);
        int32 replayID             = TimeAttackData_GetReplayID(param->zoneID, act, param->characterID, TimeAttackMenu->encoreMode, i);
        UIRankButton_SetTimeAttackRank(button, i, score, replayID);
        if (score)
            ++count;
        if (API.CheckDLC(DLC_PLUS) && replayID)
            button->actionCB = TimeAttackMenu_RankButton_ActionCB;
        else
            button->actionCB = StateMachine_None;
    }

    control->rowCount = count;
}

void TimeAttackMenu_CharButton_ActionCB(void)
{
    RSDK_THIS(UICharButton);
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *control = TimeAttackMenu->taZoneSelControl;

    TimeAttackData_Clear();
    int32 characterID  = self->characterID + 1;
    param->characterID = characterID;
    API.LeaderboardsUnknown4();

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        button->characterID          = characterID;
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
    TextInfo info;
    INIT_TEXTINFO(info);

    EntityUIDialog *dialog = TimeAttackMenu->connectingDlg;
    int32 status           = API.GetLeaderboardsStatus();

    switch (status) {
        case STATUS_CONTINUE:
            Localization_GetString(&info, STR_CONNECTING);
            UIDialog_SetupText(dialog, &info);
            break;
        default:
        case STATUS_ERROR: {
            if (status < STATUS_ERROR) {
                int32 strID = STR_COMMERROR;
                if (status == STATUS_NOWIFI)
                    strID = STR_NOWIFI;
                Localization_GetString(&info, strID);
                UIDialog_SetupText(dialog, &info);
                UIDialog_AddButton(DIALOG_OK, dialog, 0, true);

                EntityUIControl *control   = dialog->parent;
                control->rowCount          = 1;
                control->columnCount       = 1;
                control->buttonID          = 0;
                TimeAttackMenu->prevIsUser = !TimeAttackMenu->prevIsUser;

                EntityUIButtonPrompt *prompt = TimeAttackMenu->topRankPrompt;
                int32 val                    = -(TimeAttackMenu->prevIsUser != 0);
                prompt->prevPrompt           = -1;
                prompt->promptID             = (val + 15);
                prompt->visible              = !TimeAttackMenu->isUser;

                self->callback                = StateMachine_None;
                TimeAttackMenu->connectingDlg = NULL;
                destroyEntity(self);
                break;
            }
            // otherwise, fallthrough and do this
        }
        case STATUS_OK: {
            EntityUIButtonPrompt *prompt = TimeAttackMenu->topRankPrompt;
            int32 val                    = -(TimeAttackMenu->prevIsUser != false);
            prompt->prevPrompt           = -1;
            prompt->promptID             = val + 15;
            prompt->visible              = TimeAttackMenu->isUser;

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

void TimeAttackMenu_SetupLeaderboardsCarousel(void *c)
{
    EntityUICarousel *carousel = c;
    EntityUIControl *parent    = (EntityUIControl *)carousel->parent;
    Vector2 entryCount         = API.LeaderboardEntryCount();
    int32 start                = entryCount.x;
    carousel->minOffset        = maxVal(1, entryCount.x);

    if ((entryCount.y + entryCount.x) > carousel->minOffset + 5)
        carousel->maxOffset = entryCount.y + entryCount.x;
    else
        carousel->maxOffset = carousel->minOffset + 5;
    if (TimeAttackMenu->prevIsUser && entryCount.y) {
        int32 userID = 0;
        for (; entryCount.x < entryCount.y; ++entryCount.x) {
            LeaderboardEntry *entry = API.ReadLeaderboardEntry(entryCount.x);
            if (entry->isUser) {
                userID = entryCount.x;
                break;
            }
        }

        carousel->scrollOffset = userID != parent->buttonCount >> 1 ? userID - (parent->buttonCount >> 1) : 0;
        if (!userID)
            userID = start;
        carousel->virtualIndex = userID;
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

    LogHelpers_Print("avail.start = %d, length = %d", entryCount.x, entryCount.y);
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

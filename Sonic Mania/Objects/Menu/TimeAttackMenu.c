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
    TimeAttackMenu->hasRows    = false;
    TimeAttackMenu->rowCount   = 0;
}

void TimeAttackMenu_Initialize(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&info, "Time Attack");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->timeAttackControl = (Entity *)control;

        RSDK.PrependText(&info, "Time Attack Legacy");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->timeAttackControl_Legacy = (Entity *)control;

        RSDK.PrependText(&info, "Time Attack Zones");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->taZoneSelControl = (Entity *)control;

        RSDK.PrependText(&info, "Time Attack Detail");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->taDetailsControl = (Entity *)control;

        RSDK.PrependText(&info, "Leaderboards");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            TimeAttackMenu->leaderboardsControl = (Entity *)control;
            control->backPressCB                = TimeAttackMenu_LeaderboardsBackPressCB;
        }

        RSDK.PrependText(&info, "Replays");
        if (RSDK.StringCompare(&info, &control->tag, false))
            TimeAttackMenu->replaysControl = (Entity *)control;
    }

    EntityUIControl *zoneControl    = (EntityUIControl *)TimeAttackMenu->taZoneSelControl;
    EntityUIControl *lbControl      = (EntityUIControl *)TimeAttackMenu->leaderboardsControl;
    EntityUIControl *replayControl  = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIControl *detailsControl = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

    Hitbox hitbox;
    foreach_all(UIButtonPrompt, prompt)
    {
        hitbox.right  = zoneControl->size.x >> 17;
        hitbox.left   = -(zoneControl->size.x >> 17);
        hitbox.bottom = zoneControl->size.y >> 17;
        hitbox.top    = -(zoneControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(FLIP_NONE, zoneControl->startPos.x - zoneControl->cameraOffset.x,
                                      zoneControl->startPos.y - zoneControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 3) {
            TimeAttackMenu->switchModePrompt = (Entity *)prompt;
        }

        hitbox.right  = lbControl->size.x >> 17;
        hitbox.left   = -(lbControl->size.x >> 17);
        hitbox.bottom = lbControl->size.y >> 17;
        hitbox.top    = -(lbControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(FLIP_NONE, lbControl->startPos.x - lbControl->cameraOffset.x, lbControl->startPos.y - lbControl->cameraOffset.y,
                                      &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 3) {
            TimeAttackMenu->topRankPrompt = (Entity *)prompt;
        }

        hitbox.right  = replayControl->size.x >> 17;
        hitbox.left   = -(replayControl->size.x >> 17);
        hitbox.bottom = replayControl->size.y >> 17;
        hitbox.top    = -(replayControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, replayControl->startPos.x - replayControl->cameraOffset.x,
                                      replayControl->startPos.y - replayControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 2)
            TimeAttackMenu->replayPrompt = (Entity *)prompt;
    }

    foreach_all(UILeaderboard, leaderboard) { TimeAttackMenu->leaderboard = (Entity *)leaderboard; }

    foreach_all(UIReplayCarousel, carousel) { TimeAttackMenu->replayCarousel = (Entity *)carousel; }

    foreach_all(UITABanner, banner)
    {
        hitbox.right  = detailsControl->size.x >> 17;
        hitbox.left   = -(detailsControl->size.x >> 17);
        hitbox.bottom = detailsControl->size.y >> 17;
        hitbox.top    = -(detailsControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(FLIP_NONE, detailsControl->startPos.x - detailsControl->cameraOffset.x,
                                      detailsControl->startPos.y - detailsControl->cameraOffset.y, &hitbox, banner->position.x, banner->position.y)) {
            TimeAttackMenu->detailsBanner = (Entity *)banner;
            banner->parent                = (EntityUIControl *)TimeAttackMenu->taDetailsControl;
        }

        hitbox.right  = lbControl->size.x >> 17;
        hitbox.left   = -(lbControl->size.x >> 17);
        hitbox.bottom = lbControl->size.y >> 17;
        hitbox.top    = -(lbControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, lbControl->startPos.x - lbControl->cameraOffset.x, lbControl->startPos.y - lbControl->cameraOffset.y,
                                      &hitbox, banner->position.x, banner->position.y)) {
            TimeAttackMenu->leaderboardsBanner = (Entity *)banner;
            banner->parent                     = (EntityUIControl *)TimeAttackMenu->leaderboardsControl;
        }
    }
}

void TimeAttackMenu_HandleUnlocks(void)
{
    foreach_all(UITAZoneModule, module) { module->disabled = !GameProgress_GetZoneUnlocked(module->zoneID); }
}

void TimeAttackMenu_SetupActions(void)
{
    EntityUIControl *control             = (EntityUIControl *)TimeAttackMenu->timeAttackControl;
    EntityUIControl *leaderboardsControl = (EntityUIControl *)TimeAttackMenu->leaderboardsControl;
    EntityUIControl *replayControl       = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIControl *zoneSelControl      = (EntityUIControl *)TimeAttackMenu->taZoneSelControl;
    EntityUIControl *detailsControl      = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

    control->menuUpdateCB = TimeAttackMenu_MenuUpdateCB;

    EntityUIButton *button = (EntityUIButton *)control->buttons[control->buttonCount - 1];
    button->actionCB       = TimeAttackMenu_ReplayButton_ActionCB;
    int32 newCount         = (control->buttonCount - 1) + control->columnCount;
    for (int32 i = control->buttonCount; i < newCount; ++i) {
        control->buttons[i] = control->buttons[i - 1];
    }
    control->buttonCount = newCount;

    foreach_all(UICharButton, charButton)
    {
        Entity *parent = charButton->parent;
        if (parent == TimeAttackMenu->timeAttackControl || parent == TimeAttackMenu->timeAttackControl_Legacy)
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

    EntityUIReplayCarousel *replayCarousel    = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;
    replayCarousel->actionCB                  = TimeAttackMenu_ReplayCarousel_ActionCB;
    replayControl->buttons[0]->choiceChangeCB = TimeAttackMenu_SortReplayChoiceCB;
}

void TimeAttackMenu_HandleMenuReturn(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    if (param->inTimeAttack)
        TimeAttackMenu_SetEncoreLayouts(param->isEncoreMode);

    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->timeAttackControl;
    if (param->inTimeAttack) {
        int32 charID                         = param->characterID - 1;
        control->buttonID                    = charID;
        control->buttons[charID]->isSelected = true;
    }

    EntityUIControl *legacyControl = (EntityUIControl *)TimeAttackMenu->timeAttackControl_Legacy;
    if (param->inTimeAttack) {
        int32 charID = 0;
        if (param->characterID - 1 <= 2)
            charID = param->characterID - 1;

        legacyControl->buttonID                    = charID;
        legacyControl->buttons[charID]->isSelected = true;
    }

    EntityUIControl *zoneControl = (EntityUIControl *)TimeAttackMenu->taZoneSelControl;
    if (param->inTimeAttack) {
        zoneControl->buttonID                           = param->zoneID;
        zoneControl->buttons[param->zoneID]->isSelected = true;
    }

    EntityUIControl *detailsControl = (EntityUIControl *)TimeAttackMenu->taDetailsControl;
    if (param->inTimeAttack) {
        UITABanner_SetupDetails(param->characterID, (EntityUITABanner *)TimeAttackMenu->detailsBanner, param->zoneID, param->actID,
                                param->isEncoreMode);
        TimeAttackMenu_SetupDetailsView();
        UIButton_SetChoiceSelectionWithCB(detailsControl->buttons[0], param->actID);
        detailsControl->buttonID = param->selectionID;
        TimeAttackMenu_Replays_ChoiceChangeCB();
    }

    EntityUIControl *replayControl = (EntityUIControl *)TimeAttackMenu->replaysControl;
    if (replayControl->active == ACTIVE_ALWAYS) {
        replayControl->buttonID = 1;
        UIButton_SetChoiceSelectionWithCB(replayControl->buttons[0], param->selectedReplay & 0xFF);
        EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;
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
    EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->switchModePrompt;
    prompt->promptID             = 21 - (enabled != false);
    foreach_active(UITAZoneModule, module) { module->isEncore = enabled; }
}

void TimeAttackMenu_DeleteReplayActionCB(void)
{
    EntityUIControl *replayControl   = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

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
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;
    int32 row                        = API.GetSortedUserDBRowID(globals->replayTableID, carousel->curReplayID);
    ReplayRecorder_DeleteReplay(row, TimeAttackMenu_DeleteReplayCB, false);
}

void TimeAttackMenu_DeleteReplayCB(bool32 success)
{
    TimeAttackMenu_SortReplayChoiceCB();
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    int32 count = API.GetSortedUserDBRowCount(globals->replayTableID) - 1;
    if (carousel->curReplayID > count)
        carousel->curReplayID = count;
}

void TimeAttackMenu_MenuUpdateCB_LB(void)
{
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->leaderboardsControl;
    if (control->active == ACTIVE_ALWAYS) {
        EntityUICarousel *carousel = control->carousel;
        Vector2 entryCount         = API.LeaderboardEntryCount();
        if (entryCount.x <= 1 || carousel->scrollOffset >= (entryCount.x + 2)) {
            if (carousel->scrollOffset > entryCount.y - control->buttonCount + entryCount.x - 2)
                API.Unknown12(entryCount.x, entryCount.y + 20, 2);
        }
        else {
            API.Unknown12(entryCount.x - 20, entryCount.y + 20, 1);
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

void TimeAttackMenu_SetupLeaderboards(int32 zoneID, int32 characterID, int32 act, bool32 isEncore, bool32 noRows, void (*callback)(void))
{
    TextInfo info;
    INIT_TEXTINFO(info);
    Localization_GetString(&info, STR_CONNECTING);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_Setup(dialog);
        TimeAttackMenu->connectingDlg = (Entity *)dialog;
        EntityTimeAttackMenu *entity  = CREATE_ENTITY(TimeAttackMenu, NULL, -0x100000, -0x100000);
        entity->active                = ACTIVE_NORMAL;
        entity->visible               = true;
        entity->delay                 = 120;
        entity->state                 = TimeAttackMenu_State_SetupLeaderboards;
        entity->callback              = callback;
        int32 rowCount                = API.GetSortedUserDBRowCount(globals->taTableID);
        if (noRows)
            rowCount = 0;
        TimeAttackMenu->hasRows  = rowCount;
        TimeAttackMenu->rowCount = rowCount;

        const char *name = "";
        if (zoneID > 11 || act > 1 || (characterID - 1) > 4) {
            name = 0;
        }
        else {
            int32 pos = act + 2 * zoneID - 1 + characterID + 4 * (act + 2 * zoneID);
            if (isEncore)
                pos += 120;
            name = LeaderboardNames[pos];
        }
        API.FetchLeaderboard(name, rowCount);
        UITABanner_SetupDetails(characterID, (EntityUITABanner *)TimeAttackMenu->leaderboardsBanner, zoneID, act, isEncore);
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
            TimeAttackData_ConfigureTableView(zoneID, characterID, act, encore);
        }

        int32 count = API.GetSortedUserDBRowCount(globals->taTableID);

        for (int32 i = 1; i < count; ++i) {
            if (uuid == TimeAttackData_GetReplayID(zoneID, characterID, act, encore, i)) {
                break;
            }
            ++replayID;
        }
    }

    param->selectedReplay    = replayID;
    EntityUIPopover *popover = (EntityUIPopover *)UIPopover->activeEntity;
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
        EntityUIPopover *popover = (EntityUIPopover *)UIPopover->activeEntity;
        if (popover)
            popover->parent->selectionDisabled = false;
    }
}

void TimeAttackMenu_WatchReplayActionCB_ReplaysMenu(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

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

    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

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
    EntityUIPopover *popover   = (EntityUIPopover *)UIPopover->activeEntity;
    EntityUIRankButton *button = (EntityUIRankButton *)popover->storedEntity;
    EntityUIControl *parent    = (EntityUIControl *)button->parent;

    int32 uuid = API.GetUserDBRowByID(globals->replayTableID, button->replayID);
    if (uuid != -1) {
        RSDK.GetCString(param->menuTag, &parent->tag);
        param->selectionID  = parent->buttonID;
        param->inTimeAttack = true;
        TimeAttackMenu_WatchReplay(uuid, false);
    }
}

void TimeAttackMenu_ChallengeReplayCB_RanksMenu(void)
{
    EntityMenuParam *param     = (EntityMenuParam *)globals->menuParam;
    EntityUIPopover *popover   = (EntityUIPopover *)UIPopover->activeEntity;
    EntityUIRankButton *button = (EntityUIRankButton *)popover->storedEntity;
    EntityUIControl *parent    = (EntityUIControl *)button->parent;

    int32 uuid = API.GetUserDBRowByID(globals->replayTableID, button->replayID);
    if (uuid != -1) {
        RSDK.GetCString(param->menuTag, &parent->tag);
        param->selectionID  = parent->buttonID;
        param->inTimeAttack = true;
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
    EntityUIControl *replayControl   = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    if (replayControl->lastButtonID != 1) {
        carousel->curReplayID = -1;
    }

    EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->replayPrompt;
    if (prompt)
        prompt->visible = API.GetSortedUserDBRowCount(globals->replayTableID) != 0;
}

void TimeAttackMenu_MenuUpdateCB(void)
{
    EntityUIControl *control                               = (EntityUIControl *)TimeAttackMenu->timeAttackControl;
    control->prompts[control->buttonCount - 1]->position.x = control->position.x;
}

void TimeAttackMenu_ReplayButton_ActionCB(void)
{
    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    control->buttonID     = 1;
    carousel->curReplayID = 0;
    UIControl_MatchMenuTag("Replays");
    UIButton_SetChoiceSelectionWithCB(control->buttons[0], 0);
}

void TimeAttackMenu_YPressCB_Replay(void)
{
    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;
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
    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

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
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taDetailsControl;
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->detailsBanner;

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
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->detailsBanner;
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

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
    param->selectionID  = 0;
    param->inTimeAttack = true;
    param->isEncoreMode = TimeAttackMenu->encoreMode;

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
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->detailsBanner;
    int32 count              = API.GetSortedUserDBRowCount(globals->taTableID);
    TimeAttackMenu_SetupLeaderboards(banner->zoneID, banner->characterID, banner->actID, TimeAttackMenu->encoreMode, !count,
                                     UITAZoneModule_ShowLeaderboards_CB);
}

void TimeAttackMenu_ResetTimes_YesCB(void)
{
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->detailsBanner;
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

    int32 act = control->buttons[0]->selection;
    while (API.GetSortedUserDBRowCount(globals->taTableID) > 0) {
        int32 value = API.GetSortedUserDBRowID(globals->taTableID, 0);
        API.RemoveDBRow(globals->taTableID, value);
        TimeAttackData_ConfigureTableView(banner->zoneID, banner->characterID, act, TimeAttackMenu->encoreMode);
    }

    control->buttonID = 0;
    if (!API.GetUserStorageNoSave() && globals->taTableID != 0xFFFF && globals->taTableLoaded == STATUS_OK) {
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
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);

    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, NULL, true);
        UIDialog_AddButton(DIALOG_YES, dialog, TimeAttackMenu_ResetTimes_YesCB, true);
        UIDialog_Setup(dialog);
    }
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
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

    int32 act = control->buttons[0]->selection;
    UITABanner_SetupDetails(param->characterID, (EntityUITABanner *)TimeAttackMenu->detailsBanner, param->zoneID, act, TimeAttackMenu->encoreMode);
    TimeAttackData_ConfigureTableView(param->zoneID, param->characterID, act, TimeAttackMenu->encoreMode);

    int32 count = 1;
    for (int32 i = 1; i < 4; ++i) {
        EntityUIRankButton *button = (EntityUIRankButton *)control->buttons[i];
        int32 score                = TimeAttackData_GetScore(param->zoneID, param->characterID, act, TimeAttackMenu->encoreMode, i);
        int32 replayID             = TimeAttackData_GetReplayID(param->zoneID, param->characterID, act, TimeAttackMenu->encoreMode, i);
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
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taZoneSelControl;

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
    UIControl_SetInactiveMenu((EntityUIControl *)TimeAttackMenu->leaderboardsControl);
    UIControl_SetActiveMenu((EntityUIControl *)TimeAttackMenu->taDetailsControl);
}

bool32 TimeAttackMenu_LeaderboardsBackPressCB(void)
{
    UITransition_StartTransition(TimeAttackMenu_TransitionToDetailsCB, 0);
    return true;
}

void TimeAttackMenu_YPressCB_LB(void)
{
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->leaderboardsBanner;
    if (TimeAttackMenu->rowCount)
        TimeAttackMenu_SetupLeaderboards(banner->zoneID, banner->characterID, banner->actID, banner->isEncore, TimeAttackMenu->hasRows,
                                         StateMachine_None);
}

void TimeAttackMenu_State_SetupLeaderboards(void)
{
    RSDK_THIS(TimeAttackMenu);
    TextInfo info;
    INIT_TEXTINFO(info);

    EntityUIDialog *dialog = (EntityUIDialog *)TimeAttackMenu->connectingDlg;
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

                EntityUIControl *control = (EntityUIControl *)dialog->parent;
                control->rowCount        = 1;
                control->columnCount     = 1;
                control->buttonID        = 0;
                TimeAttackMenu->hasRows  = TimeAttackMenu->hasRows == 0;

                EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->topRankPrompt;
                int32 val                    = -(TimeAttackMenu->hasRows != 0);
                prompt->prevPrompt           = -1;
                prompt->promptID             = (val + 15);
                prompt->visible              = TimeAttackMenu->rowCount != 0;

                self->callback                = StateMachine_None;
                TimeAttackMenu->connectingDlg = NULL;
                destroyEntity(self);
                break;
            }
            // otherwise, fallthrough and do this
        }
        case STATUS_OK: {
            EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->topRankPrompt;
            int32 val                    = -(TimeAttackMenu->hasRows != 0);
            prompt->prevPrompt           = -1;
            prompt->promptID             = (val + 15);
            prompt->visible              = TimeAttackMenu->rowCount != 0;

            EntityUIControl *leaderboardsControl = (EntityUIControl *)TimeAttackMenu->leaderboardsControl;
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
    EntityUICarousel *carousel = (EntityUICarousel *)c;
    EntityUIControl *parent    = (EntityUIControl *)carousel->parent;
    Vector2 entryCount         = API.LeaderboardEntryCount();
    int32 start                = entryCount.x;
    carousel->minOffset        = maxVal(1, entryCount.x);

    if ((entryCount.y + entryCount.x) > carousel->minOffset + 5)
        carousel->maxOffset = entryCount.y + entryCount.x;
    else
        carousel->maxOffset = carousel->minOffset + 5;
    if (TimeAttackMenu->hasRows && entryCount.y) {
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
    carousel->field_98 = -1;
}

#if RETRO_INCLUDE_EDITOR
void TimeAttackMenu_EditorDraw(void) {}

void TimeAttackMenu_EditorLoad(void) {}
#endif

void TimeAttackMenu_Serialize(void) {}
#endif

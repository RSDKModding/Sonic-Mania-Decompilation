#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectTimeAttackMenu *TimeAttackMenu;

void TimeAttackMenu_Update(void)
{
    RSDK_THIS(TimeAttackMenu);
    StateMachine_Run(entity->state);
}

void TimeAttackMenu_LateUpdate(void) {}

void TimeAttackMenu_StaticUpdate(void)
{
    EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->prompt1;
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
    TimeAttackMenu->field_40   = false;
    TimeAttackMenu->field_44   = false;
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
            TimeAttackMenu->prompt1 = (Entity *)prompt;
        }

        hitbox.right  = lbControl->size.x >> 17;
        hitbox.left   = -(lbControl->size.x >> 17);
        hitbox.bottom = lbControl->size.y >> 17;
        hitbox.top    = -(lbControl->size.y >> 17);

        if (MathHelpers_PointInHitbox(FLIP_NONE, lbControl->startPos.x - lbControl->cameraOffset.x, lbControl->startPos.y - lbControl->cameraOffset.y,
                                      &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 3) {
            TimeAttackMenu->prompt2 = (Entity *)prompt;
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
            TimeAttackMenu->banner = (Entity *)banner;
            banner->parent         = (EntityUIControl *)TimeAttackMenu->taDetailsControl;
        }

        hitbox.right  = lbControl->size.x >> 17;
        hitbox.left   = -(lbControl->size.x >> 17);
        hitbox.bottom = lbControl->size.y >> 17;
        hitbox.top    = -(lbControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, lbControl->startPos.x - lbControl->cameraOffset.x, lbControl->startPos.y - lbControl->cameraOffset.y,
                                      &hitbox, banner->position.x, banner->position.y)) {
            TimeAttackMenu->banner2 = (Entity *)banner;
            banner->parent          = (EntityUIControl *)TimeAttackMenu->leaderboardsControl;
        }
    }
}

void TimeAttackMenu_Unknown2(void)
{
    foreach_all(UITAZoneModule, module) { module->disabled = !SaveGame_GetZoneUnlocked(module->zoneID); }
}

void TimeAttackMenu_Unknown3(void)
{
    EntityUIControl *control             = (EntityUIControl *)TimeAttackMenu->timeAttackControl;
    EntityUIControl *leaderboardsControl = (EntityUIControl *)TimeAttackMenu->leaderboardsControl;
    EntityUIControl *replayControl       = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIControl *zoneSelControl      = (EntityUIControl *)TimeAttackMenu->taZoneSelControl;
    EntityUIControl *detailsControl      = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

    control->unknownCallback4 = TimeAttackMenu_Unknown20;

    EntityUIButton *button = (EntityUIButton *)control->buttons[control->buttonCount - 1];
    button->options2       = TimeAttackMenu_Unknown21;
    int newCount           = (control->buttonCount - 1) + control->columnCount;
    for (int i = control->buttonCount; i < newCount; ++i) {
        control->buttons[i] = control->buttons[i - 1];
    }
    control->buttonCount = newCount;

    foreach_all(UICharButton, charButton)
    {
        Entity *parent = charButton->parent;
        if (parent == TimeAttackMenu->timeAttackControl || parent == TimeAttackMenu->timeAttackControl_Legacy)
            charButton->options2 = TimeAttackMenu_Options2CB_CharButton;
    }

    foreach_all(UITAZoneModule, module) { module->options2 = TimeAttackMenu_TAModuleCB; }

    for (int i = 0; i < leaderboardsControl->buttonCount; ++i) {
        EntityUILeaderboard *button = (EntityUILeaderboard *)leaderboardsControl->buttons[i];
        char buffer[0x10];
        if (i)
            sprintf(buffer, "%d", i);
        else
            sprintf(buffer, "-");

        RSDK.PrependText(&button->text1[0], buffer);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &button->text1[0]);
    }

    leaderboardsControl->unknownCallback4 = TimeAttackMenu_UnknownCB4_LB;
    leaderboardsControl->yPressCB         = TimeAttackMenu_YPressCB_LB;

    replayControl->unknownCallback3 = TimeAttackMenu_UnknownCB3_Replay;
    replayControl->unknownCallback4 = TimeAttackMenu_UnknownCB4_Replay;
    replayControl->xPressCB         = TimeAttackMenu_XPressCB_Replay;
    replayControl->yPressCB         = TimeAttackMenu_YPressCB_Replay;

    zoneSelControl->yPressCB    = TimeAttackMenu_YPressCB_ZoneSel;
    zoneSelControl->backPressCB = TimeAttackMenu_BackPressCB_ZoneSel;

    detailsControl->yPressCB         = TimeAttackMenu_YPressCB_Details;
    detailsControl->xPressCB         = TimeAttackMenu_XPressCB_Details;
    detailsControl->unknownCallback3 = TimeAttackMenu_UnknownCB3_Details;

    EntityUIButton *replayButton   = detailsControl->buttons[0];
    replayButton->options2         = TimeAttackMenu_Options2CB_Replays;
    replayButton->callbackUnknown1 = TimeAttackMenu_UnknownCB1_Replays;

    EntityUIReplayCarousel *replayCarousel      = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;
    replayCarousel->options2                    = TimeAttackMenu_Options2CB_ReplayCarousel;
    replayControl->buttons[0]->callbackUnknown1 = TimeAttackMenu_UnknownCB1_ReplayCarousel;
}

void TimeAttackMenu_Unknown4(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    if (param->clearFlag)
        TimeAttackMenu_SetEncoreLayouts(param->isEncoreMode);

    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->timeAttackControl;
    if (param->clearFlag) {
        int charID                     = param->characterID - 1;
        control->activeEntityID        = charID;
        control->buttons[charID]->flag = true;
    }

    EntityUIControl *legacyControl = (EntityUIControl *)TimeAttackMenu->timeAttackControl_Legacy;
    if (param->clearFlag) {
        int charID = 0;
        if (param->characterID - 1 <= 2)
            charID = param->characterID - 1;

        legacyControl->activeEntityID        = charID;
        legacyControl->buttons[charID]->flag = true;
    }

    EntityUIControl *zoneControl = (EntityUIControl *)TimeAttackMenu->taZoneSelControl;
    if (param->clearFlag) {
        zoneControl->activeEntityID               = param->zoneID;
        zoneControl->buttons[param->zoneID]->flag = true;
    }

    EntityUIControl *detailsControl = (EntityUIControl *)TimeAttackMenu->taDetailsControl;
    if (param->clearFlag) {
        UITABanner_Unknown1(param->characterID, (EntityUITABanner *)TimeAttackMenu->banner, param->zoneID, param->actID, param->isEncoreMode);
        TimeAttackMenu_Unknown24();
        UIButton_SetChoiceSelectionWithCB(detailsControl->buttons[0], param->actID);
        detailsControl->activeEntityID = param->selectionID;
        TimeAttackMenu_UnknownCB1_Replays();
    }

    EntityUIControl *replayControl = (EntityUIControl *)TimeAttackMenu->replaysControl;
    if (replayControl->active == ACTIVE_ALWAYS) {
        replayControl->activeEntityID = 1;
        UIButton_SetChoiceSelectionWithCB(replayControl->buttons[0], param->field_18C & 0xFF);
        EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;
        int count                        = API.GetUserDBUnknownCount(globals->replayTableID);
        int id                           = API.GetUserDBByID(globals->replayTableID, param->field_180);

        int i = 0;
        for (; i < count; ++i) {
            if (API.GetUserDBUnknown(globals->replayTableID, i) == id)
                break;
        }
        // if (count <= 0 || i >= count)
        //    carousel->field_164 = param->field_168;
        // else
        //    carousel->field_164 = i;
    }
}

void TimeAttackMenu_SetEncoreLayouts(bool32 enabled)
{
    LogHelpers_Print("SetEncoreLayouts(%d)", enabled);
    TimeAttackMenu->encoreMode   = enabled;
    EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->prompt1;
    prompt->promptID             = 21 - (enabled != false);
    foreach_active(UITAZoneModule, module) { module->isEncore = enabled; }
}

void TimeAttackMenu_XPressCB_Replay(void)
{
    EntityUIControl *replayControl   = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    if (replayControl->activeEntityID == 1 /*&& carousel->field_160 == UIReplayCarousel_Unknown15*/
        && API.GetUserDBUnknownCount(globals->replayTableID)) {
        TextInfo info;
        INIT_TEXTINFO(info);
        Localization_GetString(&info, STR_DELETEREPLAY);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
        if (dialog) {
            UIDialog_AddButton(DIALOG_NO, dialog, NULL, true);
            UIDialog_AddButton(DIALOG_YES, dialog, TimeAttackMenu_Unknown7, true);
            UIDialog_Setup(dialog);
        }
    }
}

void TimeAttackMenu_Unknown7(void)
{
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;
    // int unknown                      = API.GetUserDBUnknown(globals->replayTableID, carousel->field_164);
    // ReplayRecorder_DeleteTimeAttackRow(unknown, TimeAttackMenu_Unknown8, 0);
}

void TimeAttackMenu_Unknown8(void)
{
    TimeAttackMenu_UnknownCB1_ReplayCarousel();
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    int count = API.GetUserDBUnknownCount(globals->replayTableID) - 1;
    // if (carousel->field_164 > count)
    //     carousel->field_164 = count;
}

void TimeAttackMenu_UnknownCB4_LB(void)
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
        int count           = maxVal(1, entryCount.x);
        carousel->minOffset = count;

        if (entryCount.y + entryCount.x > count + 5)
            carousel->maxOffset = entryCount.y + entryCount.x;
        else
            carousel->maxOffset = count + 5;

        for (int i = 0; i < control->buttonCount; ++i) {
            EntityUILeaderboard *button = (EntityUILeaderboard *)control->buttons[i];
            EntityUIControl *parent     = (EntityUIControl *)carousel->parent;

            int max = (i - carousel->scrollOffset) % parent->buttonCount;
            if (max < 0)
                max += parent->buttonCount;

            LeaderboardEntry *entry = API.ReadLeaderboardEntry(max + carousel->scrollOffset);
            // if (entry != button->leaderboardEntry)
            //     UIRankButton_Unknown3(button, entry);
        }
    }
}

void TimeAttackMenu_Unknown10(signed int zone, int playerID, signed int act, int encore, int a5, void (*callback)(void))
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
        entity->timeOut               = 120;
        entity->state                 = TimeAttackMenu_Unknown41;
        entity->callback              = callback;
        int unknown                   = API.GetUserDBUnknownCount(globals->taTableID);
        if (a5)
            unknown = 0;
        TimeAttackMenu->field_40 = unknown;
        TimeAttackMenu->field_44 = unknown;

        const char *name = "";
        if (zone > 11 || act > 1 || (playerID - 1) > 4) {
            name = 0;
        }
        else {
            int pos = act + 2 * zone - 1 + playerID + 4 * (act + 2 * zone);
            if (encore)
                pos += 120;
            name = LeaderboardNames[pos];
        }
        API.FetchLeaderboard(name, unknown);
        UITABanner_Unknown1(playerID, (EntityUITABanner *)TimeAttackMenu->banner2, zone, act, encore);
    }
}

void TimeAttackMenu_Options2CB_ReplayCarousel(void)
{
    RSDK_THIS(UIReplayCarousel);
    /*if (UIPopover_Unknown1()) {
        int y = entity->position.y;
        if (!entity->field_164)
            y += entity->field_17C;

        UIPopover_Unknown2(7, TimeAttackMenu_Unknown13, 0);
        UIPopover_Unknown2(8, TimeAttackMenu_Unknown14, 0);
        UIPopover_Unknown2(9, TimeAttackMenu_XPressCB_Replay, 1);
        UIPopover_Unknown3(entity->position.x, y);
    }*/
}

void TimeAttackMenu_AddReplayEntry(int a1, int a2)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    int id                 = RSDK.MostRecentActiveControllerID(0, 0, 0);
    RSDK.ResetControllerAssignments();
    RSDK.AssignControllerID(CONT_P1, id);
    LogHelpers_Print("Go_Replay(%d, %d)", a1, a2);
    uint uuid = API.GetUserDBEntryUUID(globals->replayTableID, a1);
    LogHelpers_Print("uuid: %08X", uuid);

    int score       = 0;
    int zoneID      = 0;
    int act         = 0;
    int characterID = 0;
    int encore      = 0;
    API.GetUserDBValue(globals->replayTableID, a1, 4, "score", &score);
    API.GetUserDBValue(globals->replayTableID, a1, 2, "zoneID", &zoneID);
    API.GetUserDBValue(globals->replayTableID, a1, 2, "act", &act);
    API.GetUserDBValue(globals->replayTableID, a1, 2, "characterID", &characterID);
    API.GetUserDBValue(globals->replayTableID, a1, 2, "encore", &encore);
    param->field_184   = 1;
    param->field_188   = a2;
    param->field_180   = uuid;
    param->zoneID      = zoneID;
    param->actID       = act;
    param->characterID = characterID;
    param->isEncoreMode   = encore;

    int dbID = 0;
    if (!a2) {
        if (!TimeAttackData->status || characterID != TimeAttackData->characterID || zoneID != TimeAttackData->zoneID || act != TimeAttackData->act
            || encore != TimeAttackData->encore) {
            TimeAttackData_ConfigureTableView(zoneID, characterID, act, encore);
        }

        int count = API.GetUserDBUnknownCount(globals->taTableID);

        for (int i = 1; i < count; ++i) {
            if (uuid == TimeAttackData_SetReplayID(zoneID, characterID, act, encore, i)) {
                break;
            }
            ++dbID;
        }
    }

    param->field_18C = dbID;
    // EntityUIPopover *popover         = (EntityUIPopover *)UIPopover->activeEntity;
    // if (popover)
    //    *(*&popover[16].base + 200) = 1;

    UIWaitSpinner_Wait();

    char fileBuffer[0x20];
    sprintf(fileBuffer, "Replay_%08X.bin", uuid);

    memset(globals->replayTempRBuffer, 0, sizeof(globals->replayTempRBuffer));
    memset(globals->replayReadBuffer, 0, sizeof(globals->replayReadBuffer));
    ReplayRecorder_Buffer_LoadFile(fileBuffer, globals->replayTempRBuffer, TimeAttackMenu_ReplayLoad_CB);
}

void TimeAttackMenu_ReplayLoad_CB(bool32 a1)
{
    UIWaitSpinner_Wait2();

    int strID = 0;
    if (a1) {
        if (globals->replayTempRBuffer[1] == 6) {
            LogHelpers_Print("WARNING: Replay Load OK");
            ReplayRecorder_Buffer_Unpack(globals->replayReadBuffer, globals->replayTempRBuffer);
            TimeAttackMenu_Unknown17();
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
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_AddButton(DIALOG_OK, dialog, 0, true);
        UIDialog_Setup(dialog);
        // EntityUIPopover *popover         = (EntityUIPopover *)UIPopover->activeEntity;
        // if (popover)
        //*(*&popover->field_80 + 200) = 0;
    }
}

void TimeAttackMenu_Unknown13(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    EntityUIButton *button = control->buttons[0];
    sprintf(param->menuTag, "Replays");
    // param->field_18D = button->selection;
    // param->field_168 = carousel->field_164;
    // int id           = API.GetUserDBUnknown(globals->replayTableID, carousel->field_164);
    // TimeAttackMenu_AddReplayEntry(id, 0);
}

void TimeAttackMenu_Unknown14(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    EntityUIButton *button = control->buttons[0];
    sprintf(param->menuTag, "Replays");
    // param->field_18D = button->selection;
    // param->field_168 = carousel->field_164;
    // int id           = API.GetUserDBUnknown(globals->replayTableID, carousel->field_164);
    // TimeAttackMenu_AddReplayEntry(id, 1);
}

void TimeAttackMenu_Unknown15(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    // EntityUIPopover *popover         = (EntityUIPopover *)UIPopover->activeEntity;
    // v1     = popover->field_84

    /* EntityUIControl *parent     = *(v1 + 140);
     int uuid = API.GetUserDBByID(globals->replayTableID, *(v1 + 312));
     if (uuid != -1) {
         RSDK.GetCString(param->menuTag, &parent->tag);
         param->selectionID = parent->activeEntityID;
         param->clearFlag   = 1;
         TimeAttackMenu_AddReplayEntry(uuid, 0);
     }*/
}

void TimeAttackMenu_Unknown16(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    // EntityUIPopover *popover         = (EntityUIPopover *)UIPopover->activeEntity;
    // v1     = popover->field_84

    /* EntityUIControl *parent     = *(v1 + 140);
     int uuid = API.GetUserDBByID(globals->replayTableID, *(v1 + 312));
     if (uuid != -1) {
         RSDK.GetCString(param->menuTag, &parent->tag);
         param->selectionID = parent->activeEntityID;
         param->clearFlag   = 1;
         TimeAttackMenu_AddReplayEntry(uuid, 1);
     }*/
}

void TimeAttackMenu_Unknown17(void)
{
    Music_FadeOut(0.05);
    MenuSetup_StartTransition(TimeAttackMenu_Unknown27, 32);
}

void TimeAttackMenu_UnknownCB3_Replay(void)
{
    int status = API.GetUserDBStatus(globals->replayTableID);
    if (status)
        TimeAttackMenu_UnknownCB1_ReplayCarousel();
}

void TimeAttackMenu_UnknownCB4_Replay(void)
{
    EntityUIControl *replayControl = (EntityUIControl *)TimeAttackMenu->replaysControl;
    // if (replayControl->touchPos2[2].y != 1) {
    //    replayControl->field_164 = -1;
    //}

    EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->replayPrompt;
    if (prompt)
        prompt->visible = API.GetUserDBUnknownCount(globals->replayTableID) != 0;
}

void TimeAttackMenu_Unknown20(void)
{
    EntityUIControl *control                                = (EntityUIControl *)TimeAttackMenu->timeAttackControl;
    control->unknown2[control->buttonCount - 1]->position.x = control->position.x;
}

void TimeAttackMenu_Unknown21(void)
{
    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    control->activeEntityID = 1;
    // carousel->field_164     = 0;
    UIControl_MatchMenuTag("Replays");
    UIButton_SetChoiceSelectionWithCB(control->buttons[0], 0);
}

void TimeAttackMenu_YPressCB_Replay(void)
{
    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;
    /*if (carousel->field_160 == UIReplayCarousel_Unknown15) {
        control->activeEntityID = 0;
        carousel->field_164  = -1;
        carousel->field_16C  = 0;
        carousel->field_170  = 0;
        UIButton_SetChoiceSelectionWithCB(control->buttons[0], control->buttons[0]->selection ^ 1);
        RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
    }*/
}

void TimeAttackMenu_UnknownCB1_ReplayCarousel(void)
{
    EntityUIControl *control         = (EntityUIControl *)TimeAttackMenu->replaysControl;
    EntityUIReplayCarousel *carousel = (EntityUIReplayCarousel *)TimeAttackMenu->replayCarousel;

    API.SetupRowUnknown(globals->replayTableID);
    if (control->buttons[0]->selection == 1)
        API.Unknown34(globals->replayTableID, 4, "zoneSortVal", intToVoid(0));
    else
        API.Unknown34(globals->replayTableID, 0, 0, intToVoid(1));
    // carousel->field_160 = UIReplayCarousel_Unknown13;
}

void TimeAttackMenu_Unknown24(void)
{
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taDetailsControl;
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->banner;

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

void TimeAttackMenu_TAModuleCB(void)
{
    RSDK_THIS(UITAZoneModule);
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->banner;
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

    param->zoneID           = entity->zoneID;
    control->activeEntityID = 0;

    UITABanner_Unknown1(entity->characterID, banner, entity->zoneID, 0, TimeAttackMenu->encoreMode);
    TimeAttackMenu_Unknown24();
    UITransition_SetNewTag("Time Attack Detail");
}

void TimeAttackMenu_Unknown26(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    sprintf(param->menuTag, "Time Attack Detail");
    param->selectionID = 0;
    param->clearFlag   = 1;
    param->isEncoreMode   = TimeAttackMenu->encoreMode;
    if (globals->replayReadBuffer[3] && globals->replayReadBuffer[0] == 0xF6057BED)
        memset(globals->replayReadBuffer, 0, sizeof(globals->replayReadBuffer));
    TimeAttackMenu_Unknown27();
}

void TimeAttackMenu_Unknown27(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    SaveGame_ResetPlayerState();
    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues  = 0;
    globals->saveSlotID = 255;
    globals->gameMode   = MODE_TIMEATTACK;
    globals->medalMods  = 0;

    if (param->isEncoreMode)
        RSDK.SetScene("Encore Mode", "");
    else
        RSDK.SetScene("Mania Mode", "");

    if (param->isEncoreMode)
        RSDK_sceneInfo->listPos += TimeAttackData_GetEncoreListPos(param->zoneID, param->characterID, param->actID);
    else
        RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(param->zoneID, param->characterID, param->actID);

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
        RSDK.PlaySFX(UIWidgets->sfx_Woosh, false, 255);
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
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->banner;
    int count                = API.GetUserDBUnknownCount(globals->taTableID);
    TimeAttackMenu_Unknown10(banner->zoneID, banner->characterID, banner->actID, TimeAttackMenu->encoreMode, !count, UITAZoneModule_Unknown14);
}

void TimeAttackMenu_ResetTimes_YesCB(void)
{
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->banner;
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

    int act = control->buttons[0]->selection;
    while (API.GetUserDBUnknownCount(globals->taTableID) > 0) {
        int value = API.GetUserDBUnknown(globals->taTableID, 0);
        API.RemoveDBEntry(globals->taTableID, value);
        TimeAttackData_ConfigureTableView(banner->zoneID, banner->characterID, act, TimeAttackMenu->encoreMode);
    }

    control->activeEntityID = 0;
    if (!API.GetUserStorageNoSave() && globals->taTableID != 0xFFFF && globals->taTableLoaded == STATUS_OK) {
        LogHelpers_Print("Saving Time Attack DB");
        TimeAttackData->saveEntityPtr = RSDK_sceneInfo->entity;
        TimeAttackData->saveCallback  = NULL;
        API.SaveUserDB(globals->taTableID, TimeAttackData_SaveTimeAttackDB_CB);
    }
    TimeAttackMenu_UnknownCB1_Replays();
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

void TimeAttackMenu_Options2CB_Replays(void)
{
    RSDK_THIS(UIButton);
    EntityMenuParam *param  = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    parent->selectionDisabled = true;
    param->actID              = entity->selection;

    MenuSetup_StartTransition(TimeAttackMenu_Unknown26, 32);
}

void TimeAttackMenu_Unknown34(void)
{
    /*RSDK_THIS(UIButton); //TODO: what is this
    v1     = UIPopover_Unknown1();
    if (v1) {
        v1->field_84 = entity;
        UIPopover_Unknown2(7u, TimeAttackMenu_Unknown15, 0);
        UIPopover_Unknown2(8u, TimeAttackMenu_Unknown16, 0);
        UIPopover_Unknown3(entity->assignsP1, entity->freeBindP2);
    }*/
}

void TimeAttackMenu_UnknownCB3_Details(void) { TimeAttackMenu_UnknownCB1_Replays(); }

void TimeAttackMenu_UnknownCB1_Replays(void)
{
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taDetailsControl;

    int encore = TimeAttackMenu->encoreMode;
    int act    = control->buttons[0]->selection;
    UITABanner_Unknown1(param->characterID, (EntityUITABanner *)TimeAttackMenu->banner, param->zoneID, act, TimeAttackMenu->encoreMode);
    TimeAttackData_ConfigureTableView(param->zoneID, param->characterID, act, TimeAttackMenu->encoreMode);

    int count = 1;
    for (int i = 1; i < 4; ++i) {
        EntityUIButton *button = control->buttons[i];
        int id1                = TimeAttackData_SetScore(param->zoneID, param->characterID, act, TimeAttackMenu->encoreMode, i);
        int id2                = TimeAttackData_SetReplayID(param->zoneID, param->characterID, act, TimeAttackMenu->encoreMode, i);
        //UIRankButton_Unknown2(i, button, id1, id2);
        if (id1)
            ++count;
        if (API.CheckDLC(DLC_PLUS) && id2)
            button->options2 = TimeAttackMenu_Unknown34;
        else
            button->options2 = NULL;
    }

    control->rowCount = count;
}

void TimeAttackMenu_Options2CB_CharButton(void)
{
    RSDK_THIS(UICharButton);
    EntityMenuParam *param   = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *control = (EntityUIControl *)TimeAttackMenu->taZoneSelControl;

    TimeAttackData_ClearOptions();
    int characterID    = entity->characterID + 1;
    param->characterID = entity->characterID + 1;
    API.LeaderboardsUnknown4();

    for (int i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        button->characterID        = characterID;
    }

    UIControl_MatchMenuTag("Time Attack Zones");
}

void TimeAttackMenu_Unknown38(void)
{
    UIControl_Unknown6((EntityUIControl *)TimeAttackMenu->leaderboardsControl);
    UIControl_Unknown4((EntityUIControl *)TimeAttackMenu->taDetailsControl);
}

bool32 TimeAttackMenu_LeaderboardsBackPressCB(void)
{
    UITransition_StartTransition(TimeAttackMenu_Unknown38, 0);
    return true;
}

void TimeAttackMenu_YPressCB_LB(void)
{
    EntityUITABanner *banner = (EntityUITABanner *)TimeAttackMenu->banner2;
    if (TimeAttackMenu->field_44)
        TimeAttackMenu_Unknown10(banner->zoneID, banner->characterID, banner->actID, banner->isEncore, TimeAttackMenu->field_40, 0);
}

void TimeAttackMenu_Unknown41(void)
{
    RSDK_THIS(TimeAttackMenu);
    TextInfo info;
    INIT_TEXTINFO(info);

    EntityUIDialog *dialog = (EntityUIDialog *)TimeAttackMenu->connectingDlg;
    int status             = API.GetLeaderboardsStatus();
    if (status == STATUS_CONTINUE) {
        Localization_GetString(&info, STR_CONNECTING);
        UIDialog_SetupText(dialog, &info);
    }
    else if (status < STATUS_ERROR) {
        if (status == STATUS_OK) {
            EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->prompt2;
            int val                      = -(TimeAttackMenu->field_40 != false);
            prompt->prevPrompt           = -1;
            prompt->promptID             = (val + 15);
            prompt->visible              = TimeAttackMenu->field_44 != false;

            EntityUIControl *leaderboardsControl = (EntityUIControl *)TimeAttackMenu->leaderboardsControl;
            TimeAttackMenu_Unknown42(leaderboardsControl->carousel);
            UIDialog_Unknown4(dialog, entity->callback);
            entity->callback              = StateMachine_None;
            TimeAttackMenu->connectingDlg = NULL;
            destroyEntity(entity);
        }
    }
    else {
        int strID = STR_COMMERROR;
        if (status == STATUS_NOWIFI)
            strID = STR_NOWIFI;
        Localization_GetString(&info, strID);
        UIDialog_SetupText(dialog, &info);
        UIDialog_AddButton(DIALOG_OK, dialog, 0, true);

        EntityUIControl *control = (EntityUIControl *)dialog->parent;
        control->rowCount        = 1;
        control->columnCount     = 1;
        control->activeEntityID  = 0;
        TimeAttackMenu->field_40 = !TimeAttackMenu->field_40;

        EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)TimeAttackMenu->prompt2;
        int val                      = -(TimeAttackMenu->field_40 != false);
        prompt->prevPrompt           = -1;
        prompt->promptID             = (val + 15);
        prompt->visible              = TimeAttackMenu->field_44 != false;

        entity->callback              = StateMachine_None;
        TimeAttackMenu->connectingDlg = NULL;
        destroyEntity(entity);
    }
}

void TimeAttackMenu_Unknown42(void *c)
{
    EntityUICarousel *carousel = (EntityUICarousel *)c;
    EntityUIControl *parent    = (EntityUIControl *)carousel->parent;
    Vector2 entryCount         = API.LeaderboardEntryCount();
    int start                  = entryCount.x;
    carousel->minOffset        = maxVal(1, entryCount.x);

    if ((entryCount.y + entryCount.x) > carousel->minOffset + 5)
        carousel->maxOffset = entryCount.y + entryCount.x;
    else
        carousel->maxOffset = carousel->minOffset + 5;
    if (TimeAttackMenu->field_40 && entryCount.y) {
        int userID = 0;
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

    for (int i = 0; i < parent->buttonCount; ++i) {
        int max = (i - carousel->scrollOffset) % parent->buttonCount;
        if (max < 0)
            max += parent->buttonCount;
        int vIndex = max + carousel->scrollOffset;
        LogHelpers_Print("i = %d, vIndex = %d", i, vIndex);
        if (vIndex == carousel->virtualIndex) {
            parent->activeEntityID = i;
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

void TimeAttackMenu_EditorDraw(void) {}

void TimeAttackMenu_EditorLoad(void) {}

void TimeAttackMenu_Serialize(void) {}
#endif

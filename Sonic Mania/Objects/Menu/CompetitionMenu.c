#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectCompetitionMenu *CompetitionMenu;

void CompetitionMenu_Update(void) {}

void CompetitionMenu_LateUpdate(void) {}

void CompetitionMenu_StaticUpdate(void) { CompetitionMenu_Unknown5(); }

void CompetitionMenu_Draw(void) {}

void CompetitionMenu_Create(void *data) {}

void CompetitionMenu_StageLoad(void) { CompetitionMenu->timer = 120; }

void CompetitionMenu_Initialize(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&info, "Competition");
        if (RSDK.StringCompare(&info, &control->tag, false))
            CompetitionMenu->competitionControl = (Entity *)control;

        RSDK.PrependText(&info, "Competition Legacy");
        if (RSDK.StringCompare(&info, &control->tag, false))
            CompetitionMenu->competitionControl_Legacy = (Entity *)control;

        RSDK.PrependText(&info, "Competition Rules");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            CompetitionMenu->compRulesControl = (Entity *)control;
            control->backPressCB              = CompetitionMenu_Unknown29;
        }

        RSDK.PrependText(&info, "Competition Zones");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            CompetitionMenu->compZoneControl = (Entity *)control;
            control->backPressCB             = CompetitionMenu_Unknown30;
        }

        RSDK.PrependText(&info, "Competition Round");
        if (RSDK.StringCompare(&info, &control->tag, false))
            CompetitionMenu->compRoundControl = (Entity *)control;

        RSDK.PrependText(&info, "Competition Total");
        if (RSDK.StringCompare(&info, &control->tag, false))
            CompetitionMenu->compTotalControl = (Entity *)control;
    }

    Hitbox hitbox;
    foreach_all(UIInfoLabel, label)
    {
        EntityUIControl *roundControl = (EntityUIControl *)CompetitionMenu->compRoundControl;
        hitbox.right                  = roundControl->size.x >> 17;
        hitbox.left                   = -(roundControl->size.x >> 17);
        hitbox.bottom                 = roundControl->size.y >> 17;
        hitbox.top                    = -(roundControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, roundControl->startPos.x - roundControl->cameraOffset.x,
                                      roundControl->startPos.y - roundControl->cameraOffset.y, &hitbox, label->position.x, label->position.y))
            CompetitionMenu->label1 = (Entity*)label;

        EntityUIControl *totalControl = (EntityUIControl *)CompetitionMenu->compTotalControl;
        hitbox.right                  = totalControl->size.x >> 17;
        hitbox.left                   = -(totalControl->size.x >> 17);
        hitbox.bottom                 = totalControl->size.y >> 17;
        hitbox.top                    = -(totalControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, totalControl->startPos.x - totalControl->cameraOffset.x,
                                      totalControl->startPos.y - totalControl->cameraOffset.y, &hitbox, label->position.x, label->position.y))
            CompetitionMenu->label2 = (Entity *)label;
    }

    foreach_all(UIButtonPrompt, prompt)
    {
        EntityUIControl *compControl = (EntityUIControl *)CompetitionMenu->competitionControl;
        hitbox.right                 = compControl->size.x >> 17;
        hitbox.left                  = -(compControl->size.x >> 17);
        hitbox.bottom                = compControl->size.y >> 17;
        hitbox.top                   = -(compControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, compControl->startPos.x - compControl->cameraOffset.x,
                                      compControl->startPos.y - compControl->cameraOffset.y, &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 5) {
            CompetitionMenu->prompt1 = (Entity *)prompt;
        }
        EntityUIControl *compControl_Legacy = (EntityUIControl *)CompetitionMenu->competitionControl_Legacy;

        hitbox.right  = compControl_Legacy->size.x >> 17;
        hitbox.left   = -(compControl_Legacy->size.x >> 17);
        hitbox.bottom = compControl_Legacy->size.y >> 17;
        hitbox.top    = -(compControl_Legacy->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, compControl_Legacy->startPos.x - compControl_Legacy->cameraOffset.x,
                                      compControl_Legacy->startPos.y - compControl_Legacy->cameraOffset.y, &hitbox, prompt->position.x,
                                      prompt->position.y)) {
            if (prompt->buttonID == 5)
                CompetitionMenu->prompt2 = (Entity *)prompt;
        }
    }
}

void CompetitionMenu_Unknown2(void)
{
    EntityUIControl *compControl        = (EntityUIControl *)CompetitionMenu->competitionControl;
    EntityUIControl *compControl_Legacy = (EntityUIControl *)CompetitionMenu->competitionControl_Legacy;
    EntityUIControl *rulesControl       = (EntityUIControl *)CompetitionMenu->compRulesControl;
    EntityUIControl *roundControl       = (EntityUIControl *)CompetitionMenu->compRoundControl;
    EntityUIControl *totalControl       = (EntityUIControl *)CompetitionMenu->compTotalControl;

    foreach_all(UIButton, button)
    {
        Hitbox hitbox;
        hitbox.right  = rulesControl->size.x >> 17;
        hitbox.left   = -(rulesControl->size.x >> 17);
        hitbox.bottom = rulesControl->size.y >> 17;
        hitbox.top    = -(rulesControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, rulesControl->startPos.x - rulesControl->cameraOffset.x,
                                      rulesControl->startPos.y - rulesControl->cameraOffset.y, &hitbox, button->position.x, button->position.y)
            && button->listID == 9 && button->frameID == 2)
            button->options2 = CompetitionMenu_RulesButtonCB;
    }

    compControl->processButtonInputCB = CompetitionMenu_VS_ProcessInputCB;
    compControl->unknownCallback3     = CompetitionMenu_VS_UnknownCB3;
    if (compControl->active == ACTIVE_ALWAYS)
        CompetitionMenu_VS_UnknownCB3();

    compControl_Legacy->processButtonInputCB = CompetitionMenu_VS_ProcessInputCB;
    compControl_Legacy->unknownCallback3     = CompetitionMenu_VS_UnknownCB3;
    if (compControl_Legacy->active == ACTIVE_ALWAYS)
        CompetitionMenu_VS_UnknownCB3();

    rulesControl->unknownCallback3 = CompetitionMenu_Rules_UnknownCB3;
    if (rulesControl->active == ACTIVE_ALWAYS)
        CompetitionMenu_Rules_UnknownCB3();

    roundControl->processButtonInputCB = CompetitionMenu_Round_ProcessInputCB;
    roundControl->unknownCallback3     = CompetitionMenu_Round_UnknownCB3;
    if (roundControl->active == ACTIVE_ALWAYS)
        CompetitionMenu_Round_UnknownCB3();

    totalControl->processButtonInputCB = CompetitionMenu_Results_ProcessInputCB;
    totalControl->unknownCallback3     = CompetitionMenu_Results_UnknownCB3;
    totalControl->unknownCallback4     = CompetitionMenu_Results_UnknownCB4;
    totalControl->posUnknown.y         = totalControl->startPos.y;
    totalControl->position.y           = totalControl->startPos.y;
    if (totalControl->active == ACTIVE_ALWAYS)
        CompetitionMenu_Results_UnknownCB3();

    foreach_all(UIVsZoneButton, zoneButton) { zoneButton->options2 = CompetitionMenu_ZoneButtonCB; }
}

void CompetitionMenu_Unknown3(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    if (session->inMatch) {
        foreach_all(UIControl, control)
        {
            Entity *compControl = CompetitionMenu->competitionControl;
            if (!API.CheckDLC(DLC_PLUS))
                compControl = CompetitionMenu->competitionControl_Legacy;

            if (control == (EntityUIControl *)compControl) {
                foreach_all(UIVsCharSelector, selector)
                {
                    switch (session->characterFlags[selector->playerID]) {
                        case ID_SONIC: selector->frameID = 0; break;
                        case ID_TAILS: selector->frameID = 1; break;
                        case ID_KNUCKLES: selector->frameID = 2; break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY: selector->frameID = 3; break;
                        case ID_RAY: selector->frameID = 4; break;
#endif
                        default: break;
                    }
                }
            }

            if (control == (EntityUIControl *)CompetitionMenu->compRulesControl) {
                switch (session->monitorMode) {
                    case 0: UIButton_SetChoiceSelection(control->buttons[0], 0); break;
                    case 1: UIButton_SetChoiceSelection(control->buttons[0], 2); break;
                    case 2: UIButton_SetChoiceSelection(control->buttons[0], 1); break;
                }

                EntityUIVsRoundPicker *picker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(control->buttons[1], control->buttons[1]->selection);
                if (picker)
                    picker->val = session->matchCount;
                UIButton_SetChoiceSelectionWithCB(control->buttons[2], session->splitScreenMode);
                CompetitionMenu_Unknown9(session->playerCount);
            }

            if (control == (EntityUIControl*)CompetitionMenu->compZoneControl) {
                for (int32 i = 0; i < 12; ++i) {
                    EntityUIVsZoneButton *button = (EntityUIVsZoneButton *)control->buttons[i];
                    if (button && session->zoneFlags[i])
                        button->xOut = true;
                }
            }
        }
    }
}

int32 CompetitionMenu_Unknown4(void)
{
    int32 count = 0;
    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !GameProgress_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;

        if (!zoneButton->xOut)
            ++count;
    }
    return count;
}

void CompetitionMenu_Unknown5(void)
{
    int32 count1                    = CompetitionMenu_GetReadyPlayerCount();
    int32 count2                    = CompetitionMenu_GetFlaggedReadyPlayerCount();
    EntityUIButtonPrompt *prompt1 = (EntityUIButtonPrompt *)CompetitionMenu->prompt1;
    if (prompt1)
        prompt1->visible = count1 > 1 && count1 == count2;
    EntityUIButtonPrompt *prompt2 = (EntityUIButtonPrompt *)CompetitionMenu->prompt2;
    if (prompt2)
        prompt2->visible = count1 > 1 && count1 == count2;
}

int32 CompetitionMenu_GetReadyPlayerCount(void)
{
    EntityUIControl *control       = (EntityUIControl *)CompetitionMenu->competitionControl;
    EntityUIControl *legacyControl = (EntityUIControl *)CompetitionMenu->competitionControl_Legacy;
    if (!API.CheckDLC(DLC_PLUS))
        control = legacyControl;

    if (!control)
        return 0;

    int32 count = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];
        if (button->ready && !button->flag)
            ++count;
    }
    return count;
}

int32 CompetitionMenu_GetFlaggedReadyPlayerCount(void)
{
    EntityUIControl *control       = (EntityUIControl *)CompetitionMenu->competitionControl;
    EntityUIControl *legacyControl = (EntityUIControl *)CompetitionMenu->competitionControl_Legacy;
    if (!API.CheckDLC(DLC_PLUS))
        control = legacyControl;

    if (!control)
        return 0;

    int32 count = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector*)control->buttons[i];
        if (button->ready || button->flag)
            ++count;
    }
    return count;
}

void CompetitionMenu_Unknown8(void)
{
    EntityUIVsCharSelector *buttons[PLAYER_MAX];
    EntityUIControl *control = (EntityUIControl *)CompetitionMenu->competitionControl;

    int32 playerID = 0;
    for (int32 i = 0; i < PLAYER_MAX; ++i) buttons[i] = (EntityUIVsCharSelector *)control->buttons[i];

    for (int32 i = 0; i < PLAYER_MAX; ++i) {
        EntityUIVsCharSelector *selector = buttons[i];
        int32 id                           = RSDK.ControllerIDForInputID(i + 1);

        if (id && id != CONT_AUTOASSIGN) {
            if (playerID < i)
                RSDK.AssignControllerID(i, CONT_ANY);
            RSDK.AssignControllerID(playerID + 1, id);
            buttons[playerID]->frameID = selector->frameID;
            if (playerID < i)
                selector->frameID = selector->playerID;
            ++playerID;
        }
    }

    for (; playerID < PLAYER_MAX; ++playerID) RSDK.AssignControllerID(playerID + 1, CONT_ANY);
}

void CompetitionMenu_Unknown9(int32 playerCount)
{
    EntityUIButton *button = ((EntityUIControl *)CompetitionMenu->compRulesControl)->buttons[2];

    bool32 flags[5];
    flags[0] = playerCount == 2;
    flags[1] = flags[0];
    flags[2] = playerCount == 3;
    flags[3] = flags[2];
    flags[4] = playerCount == 4;

    for (int32 c = 0; c < button->choiceCount; ++c) {
        EntityUIChoice *choice = (EntityUIChoice *)UIButton_GetChoicePtr(button, c);
        if (choice) {
            choice->disabled = !flags[c];
        }
    }

    EntityUIChoice *choice = (EntityUIChoice *)UIButton_GetChoicePtr(button, button->selection);
    if (choice->disabled) {
        if (playerCount == 2)
            UIButton_SetChoiceSelectionWithCB(button, 0);
        else if (playerCount == 3)
            UIButton_SetChoiceSelectionWithCB(button, 2);
        else
            UIButton_SetChoiceSelectionWithCB(button, 4);

        UIButton_Unknown1(button);
    }
}
void CompetitionMenu_SetupSplitScreen(int32 mode)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    int32 startVert_3P[15];
    char startVert_2P[10];

    memset(startVert_2P, 0, sizeof(startVert_2P));
    memset(startVert_3P, 0, sizeof(startVert_3P));

    startVert_2P[0] = 18;
    startVert_2P[1] = 6;
    startVert_2P[5] = 24;
    startVert_2P[6] = 12;

    startVert_3P[2]  = 30;
    startVert_3P[3]  = 6;
    startVert_3P[7]  = 36;
    startVert_3P[8]  = 42;
    startVert_3P[12] = 12;
    startVert_3P[13] = 48;
    RSDK.SetScreenSplitVerticies(startVert_2P[mode], startVert_2P[mode + 5], startVert_3P[mode], startVert_3P[mode + 5], startVert_3P[mode + 10]);

    switch (mode) {
        case 0:
        case 1:
            session->unknown84 = 2;
            session->unknown85 = 5;
            break;
        case 2:
            session->splitScreenMode = 5;
            session->unknown85       = 3;
            session->unknown84       = 1;
            break;
        case 3:
            session->unknown84       = 2;
            session->unknown85       = 4;
            session->splitScreenMode = 6;
            break;
        case 4:
            session->splitScreenMode = 4;
            session->unknown87       = 6;
            session->unknown85       = 3;
            session->unknown84       = 1;
            break;
        default: break;
    }
}

void CompetitionMenu_Unknown11(void *control)
{
    EntityUIControl *roundControl     = (EntityUIControl *)control;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    int32 offsets[] = { -580000, 0x580000, 0x0, 0x0, -0x7A0000, 0x000000, 0x7A0000, 0x000000, -0x9C0000, -0x340000, 0x340000, 0x9C0000 };

    int32 offsetPtr = 4 * (session->playerCount - 2);
    for (int32 i = 0; i < PLAYER_MAX; ++i) {
        EntityUIButton *button = roundControl->buttons[i];
        if (session->playerCount <= i) {
            button->visible = false;
        }
        else {
            button->visible    = true;
            button->position.x = roundControl->position.x + offsets[offsetPtr + i];
        }
    }
}

void CompetitionMenu_GotoCompRules(void)
{
    CompetitionMenu_Unknown9(CompetitionMenu_GetFlaggedReadyPlayerCount());
    UIControl_MatchMenuTag("Competition Rules");
}

void CompetitionMenu_GotoCompZones(void) { UIControl_MatchMenuTag("Competition Zones"); }

void CompetitionMenu_VS_ProcessInputCB(void)
{
    EntityUIControl *control       = (EntityUIControl *)CompetitionMenu->competitionControl;
    EntityUIControl *legacyControl = (EntityUIControl *)CompetitionMenu->competitionControl_Legacy;
    if (!API.CheckDLC(DLC_PLUS))
        control = legacyControl;

    if (control) {
        for (int32 i = 0; i < control->buttonCount; ++i) {
            EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];

            Entity *entStore       = RSDK_sceneInfo->entity;
            RSDK_sceneInfo->entity = (Entity *)button;
            StateMachine_Run(button->processButtonCB);
            RSDK_sceneInfo->entity = entStore;

            RSDK.ControllerIDForInputID(button->playerID + 1);
        }

        EntityUIButtonPrompt *promptPtr = NULL;
        for (int32 i = 0; i < control->promptCount; ++i) {
            EntityUIButtonPrompt *prompt = control->prompts[i];
            if (prompt->buttonID == 4)
                promptPtr = prompt;
        }

        int32 count1 = CompetitionMenu_GetReadyPlayerCount();
        if (count1 > 1 && count1 == CompetitionMenu_GetFlaggedReadyPlayerCount()) {
            if (UIControl->startPress[0] || UIControl->startPress[1] || UIControl->startPress[2] || UIControl->startPress[3]) {
                control->selectionDisabled = true;
                UITransition_StartTransition(CompetitionMenu_GotoCompRules, 0);
            }
            promptPtr->visible = true;
        }
        else {
            promptPtr->visible = false;
        }
    }
}

void CompetitionMenu_VS_UnknownCB3(void)
{
    RSDK_THIS(UIControl);

    entity->childHasFocus = false;
    for (int32 i = 0; i < entity->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)entity->buttons[i];

        button->flag            = true;
        button->ready           = false;
        button->processButtonCB = UIVsCharSelector_ProcessButtonCB;

        if (!RSDK.ControllerIDForInputID(button->playerID + 1))
            RSDK.AssignControllerID(button->playerID + 1, CONT_AUTOASSIGN);
    }

    for (int32 i = 0; i < entity->promptCount; ++i) {
        EntityUIButtonPrompt *prompt = entity->prompts[i];
        if (prompt->buttonID == 4)
            prompt->visible = false;
    }
}

void CompetitionMenu_Rules_UnknownCB3(void)
{
    if (API.CheckDLC(DLC_PLUS))
        CompetitionMenu_Unknown8();
}

void CompetitionMenu_StartMatch(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    EntityMenuParam *param            = (EntityMenuParam *)globals->menuParam;

    sprintf(param->menuTag, "Competition Round");
    session->levelIndex  = ((EntityUIControl *)CompetitionMenu->compZoneControl)->activeEntityID;
    session->zoneID      = param->vsZoneID;
    session->actID       = param->vsActID;
    session->prevMatchID = session->matchID;
    for (int32 i = 0; i < session->playerCount; ++i) {
        if (!session->lives[i])
            session->lives[i] = 1;
    }

    if (globals->gameMode == MODE_ENCORE)
        RSDK.SetScene("Encore Mode", "");
    else
        RSDK.SetScene("Mania Mode", "");
    RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(param->vsZoneID, 0, param->vsActID);
    SaveGame_ResetPlayerState();
    memset(globals->noSaveSlot, 0, 0x400);

    globals->continues  = 0;
    globals->saveSlotID = 255;
    globals->gameMode   = MODE_COMPETITION;
    globals->medalMods  = 0;

    globals->playerID = ID_NONE;
    for (int32 i = 0; i < session->playerCount; ++i) globals->playerID |= session->characterFlags[i] << (8 * i);
    globals->itemMode = session->monitorMode;
    RSDK.LoadScene();
}

void CompetitionMenu_ZoneButtonCB(void) { MenuSetup_StartTransition(CompetitionMenu_StartMatch, 32); }

void CompetitionMenu_RulesButtonCB(void)
{
    EntityUIControl *rulesControl     = (EntityUIControl *)CompetitionMenu->compRulesControl;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    int32 matchCount = 0;
    foreach_all(UIVsRoundPicker, picker)
    {
        matchCount = picker->val;
        foreach_break;
    }

    int32 monitorMode = 0;
    if (rulesControl->buttons[0]->selection) {
        if (rulesControl->buttons[0]->selection == 1) {
            monitorMode = 2;
        }
        else if (rulesControl->buttons[0]->selection == 2) {
            monitorMode = 1;
        }
    }
    Competition_ResetOptions();

    session->matchCount  = matchCount;
    session->inMatch     = true;
    session->monitorMode = monitorMode;
    CompetitionMenu_SetupSplitScreen(rulesControl->buttons[2]->selection);
    session->readOnlyDisplayMode = rulesControl->buttons[2]->selection;

    EntityUIControl *control       = (EntityUIControl *)CompetitionMenu->competitionControl;
    EntityUIControl *legacyControl = (EntityUIControl *)CompetitionMenu->competitionControl_Legacy;
    if (!API.CheckDLC(DLC_PLUS))
        control = legacyControl;

    session->playerCount = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];
        int32 id                         = RSDK.ControllerIDForInputID(i + 1);
        if (id && id != CONT_UNASSIGNED) {
            switch (button->frameID) {
                case 0: session->characterFlags[i] = ID_SONIC; break;
                case 1: session->characterFlags[i] = ID_TAILS; break;
                case 2: session->characterFlags[i] = ID_KNUCKLES; break;
#if RETRO_USE_PLUS
                case 3: session->characterFlags[i] = ID_MIGHTY; break;
                case 4: session->characterFlags[i] = ID_RAY; break;
#endif
                default: break;
            }
            session->controllerIDs[i] = id;
            session->playerCount++;
        }
    }

    EntityUIControl *zoneControl = (EntityUIControl *)CompetitionMenu->compZoneControl;
    zoneControl->position        = zoneControl->startPos;
    zoneControl->posUnknown.x    = zoneControl->startPos.x;
    zoneControl->posUnknown.y    = zoneControl->startPos.y;
    zoneControl->activeEntityID  = 0;
    CompetitionMenu_Unknown4();
    UIControl_MatchMenuTag("Competition Zones");
}

void CompetitionMenu_GotoCompTotal(void) { UIControl_MatchMenuTag("Competition Total"); }

void CompetitionMenu_Round_ProcessInputCB(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (UIControl->keyConfirm) {
        bool32 flag = false;
        int32 count   = 0;
        for (int32 p = 0; p < session->playerCount; ++p) {
            if (session->lives[p] > 0)
                count++;
            if (session->wins[p] > (session->matchCount >> 1))
                flag = true;
        }

        EntityUIControl *zoneControl = (EntityUIControl *)CompetitionMenu->compZoneControl;
        int32 count2                    = 0;
        for (int32 i = 0; i < 12; ++i) {
            if (zoneControl->buttons[i]) {
                EntityUIVsZoneButton *zoneButton = (EntityUIVsZoneButton *)zoneControl->buttons[i];
                if (!session->zoneFlags[i] && GameProgress_GetZoneUnlocked(zoneButton->zoneID))
                    ++count2;
            }
        }

        if (flag || count < 2 || session->matchID >= session->matchCount || !count2) {
            UITransition_StartTransition(CompetitionMenu_GotoCompTotal, 0);
        }
        else {
            UITransition_StartTransition(CompetitionMenu_GotoCompZones, 0);
        }

        RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
        UIControl->inputLocked = true;
    }
}

void CompetitionMenu_Round_UnknownCB3(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    EntityUIControl *roundControl     = (EntityUIControl *)CompetitionMenu->compRoundControl;
    CompetitionMenu_Unknown11(roundControl);

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
    UIInfoLabel_SetText((EntityUIInfoLabel *)CompetitionMenu->label1, textBuffer);

    int32 bestRings      = 0;
    int32 bestTotalRings = 0;
    int32 bestScore      = 0;
    int32 bestItems      = 0;
    int32 bestTime       = -1;
    int32 times[PLAYER_MAX];

    for (int32 p = 0; p < session->playerCount; ++p) {
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
    for (int32 p = 0; p < session->playerCount; ++p) {
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
        int32 winner = -1;
        for (int32 p = 0; p < session->playerCount; ++p) {
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

void CompetitionMenu_GotoCompetition(void)
{
    const char *tag = "Competition";
    if (!API.CheckDLC(DLC_PLUS))
        tag = "Competition Legacy";
    UIControl_MatchMenuTag(tag);
}

void CompetitionMenu_Results_ProcessInputCB(void)
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

        if (winnerCount > 1 && session->playerCount == 2) {
            MenuSetup_StartTransition(CompetitionMenu_GotoPuyoVS, 32);
        }
        else {
            Competition_ResetOptions();
            UITransition_StartTransition(CompetitionMenu_GotoCompetition, 0);
        }
        RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
        UIControl->inputLocked = true;
    }
}

void CompetitionMenu_Results_UnknownCB3(void)
{
    EntityUIControl *totalControl     = (EntityUIControl *)CompetitionMenu->compTotalControl;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    CompetitionMenu_Unknown11(totalControl);
    CompetitionMenu->timer = 120;

    totalControl->posUnknown.y = totalControl->startPos.y;
    totalControl->position.y   = totalControl->startPos.y;

    TextInfo info;
    INIT_TEXTINFO(info);

    EntityUIInfoLabel *label = (EntityUIInfoLabel *)CompetitionMenu->label2;
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
        results->field_1D4   = session->wins[p] == highestScore;
        results->trophyCount = session->wins[p];
        for (int32 r = 0; r < results->numRows; ++r) {
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

void CompetitionMenu_Results_UnknownCB4(void)
{
    EntityUIControl *totalControl = (EntityUIControl *)CompetitionMenu->compTotalControl;

    if (totalControl->active == ACTIVE_ALWAYS) {
        if (totalControl->position.y == totalControl->posUnknown.y) {
            if (CompetitionMenu->timer <= 0) {
                int32 pos = totalControl->startPos.y;
                if (totalControl->posUnknown.y == totalControl->startPos.y) {
                    EntityUIVsResults *button = (EntityUIVsResults *)totalControl->buttons[0];
                    if (button) {
                        if (button->field_1D0 + button->position.y - 0x708000 > totalControl->startPos.y)
                            pos = button->field_1D0 + button->position.y - 0x708000;
                    }
                }
                totalControl->posUnknown.y = pos;
                CompetitionMenu->timer     = 120;
            }
            else {
                CompetitionMenu->timer--;
            }
        }
    }
}

void CompetitionMenu_ExitComp_TransitionCB(void)
{
    EntityUIControl *control       = (EntityUIControl *)CompetitionMenu->competitionControl;
    EntityUIControl *legacyControl = (EntityUIControl *)CompetitionMenu->competitionControl_Legacy;
    EntityUIControl *zoneControl   = (EntityUIControl *)CompetitionMenu->compZoneControl;
    if (!API.CheckDLC(DLC_PLUS))
        control = legacyControl;

    UIControl_Unknown6(zoneControl);
    UIControl_Unknown4(control);
    Competition_ResetOptions();
    zoneControl->childHasFocus = false;
    CompetitionMenu_Unknown4();
}

void CompetitionMenu_ExitComp_YesCB(void) { UITransition_StartTransition(CompetitionMenu_ExitComp_TransitionCB, 0); }

bool32 CompetitionMenu_Unknown29(void)
{
    if (!API.CheckDLC(DLC_PLUS)) {
        UITransition_SetNewTag("Competition Legacy");
        return true;
    }
    else {
        UITransition_SetNewTag("Competition");
        return true;
    }
    return false;
}

bool32 CompetitionMenu_Unknown30(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_EXITCOMP);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, 0, true);
        UIDialog_AddButton(DIALOG_YES, dialog, CompetitionMenu_ExitComp_YesCB, true);
        UIDialog_Setup(dialog);
    }
    return true;
}

void CompetitionMenu_GotoPuyoVS(void)
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

#if RETRO_INCLUDE_EDITOR
void CompetitionMenu_EditorDraw(void) {}

void CompetitionMenu_EditorLoad(void) {}
#endif

void CompetitionMenu_Serialize(void) {}
#endif

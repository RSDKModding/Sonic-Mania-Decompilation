// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CompetitionMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectCompetitionMenu *CompetitionMenu;

void CompetitionMenu_Update(void) {}

void CompetitionMenu_LateUpdate(void) {}

void CompetitionMenu_StaticUpdate(void) { CompetitionMenu_HandleStartCompPrompt(); }

void CompetitionMenu_Draw(void) {}

void CompetitionMenu_Create(void *data) {}

void CompetitionMenu_StageLoad(void) { CompetitionMenu->timer = 120; }

void CompetitionMenu_Initialize(void)
{
    String tag;
    INIT_STRING(tag);

    foreach_all(UIControl, control)
    {
        RSDK.SetString(&tag, "Competition");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            CompetitionMenu->competitionControl = control;

        RSDK.SetString(&tag, "Competition Legacy");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            CompetitionMenu->competitionControl_Legacy = control;

        RSDK.SetString(&tag, "Competition Rules");
        if (RSDK.CompareStrings(&tag, &control->tag, false)) {
            CompetitionMenu->compRulesControl = control;
            control->backPressCB              = CompetitionMenu_CompRules_BackPressCB;
        }

        RSDK.SetString(&tag, "Competition Zones");
        if (RSDK.CompareStrings(&tag, &control->tag, false)) {
            CompetitionMenu->compZoneControl = control;
            control->backPressCB             = CompetitionMenu_CompZones_BackPressCB;
        }

        RSDK.SetString(&tag, "Competition Round");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            CompetitionMenu->compRoundControl = control;

        RSDK.SetString(&tag, "Competition Total");
        if (RSDK.CompareStrings(&tag, &control->tag, false))
            CompetitionMenu->compTotalControl = control;
    }

    foreach_all(UIInfoLabel, label)
    {
        EntityUIControl *roundControl = CompetitionMenu->compRoundControl;
        EntityUIControl *totalControl = CompetitionMenu->compTotalControl;

        if (UIControl_ContainsPos(roundControl, &label->position))
            CompetitionMenu->resultsLabel_Round = label;

        if (UIControl_ContainsPos(totalControl, &label->position))
            CompetitionMenu->resultsLabel_Total = label;
    }

    foreach_all(UIButtonPrompt, prompt)
    {
        EntityUIControl *compControl        = CompetitionMenu->competitionControl;
        EntityUIControl *compControl_Legacy = CompetitionMenu->competitionControl_Legacy;

        if (UIControl_ContainsPos(compControl, &prompt->position) && prompt->buttonID == 5)
            CompetitionMenu->startCompPrompt = prompt;

        if (UIControl_ContainsPos(compControl_Legacy, &prompt->position) && prompt->buttonID == 5)
            CompetitionMenu->startCompPrompt_Legacy = prompt;
    }
}

void CompetitionMenu_SetupActions(void)
{
    EntityUIControl *compControl        = CompetitionMenu->competitionControl;
    EntityUIControl *compControl_Legacy = CompetitionMenu->competitionControl_Legacy;
    EntityUIControl *rulesControl       = CompetitionMenu->compRulesControl;
    EntityUIControl *roundControl       = CompetitionMenu->compRoundControl;
    EntityUIControl *totalControl       = CompetitionMenu->compTotalControl;

    foreach_all(UIButton, button)
    {
        if (UIControl_ContainsPos(rulesControl, &button->position) && button->listID == 9 && button->frameID == 2)
            button->actionCB = CompetitionMenu_RulesButton_ActionCB;
    }

    compControl->processButtonInputCB = CompetitionMenu_VS_ProcessInputCB;
    compControl->menuSetupCB          = CompetitionMenu_VS_MenuSetupCB;
    if (compControl->active == ACTIVE_ALWAYS)
        CompetitionMenu_VS_MenuSetupCB();

    compControl_Legacy->processButtonInputCB = CompetitionMenu_VS_ProcessInputCB;
    compControl_Legacy->menuSetupCB          = CompetitionMenu_VS_MenuSetupCB;
    if (compControl_Legacy->active == ACTIVE_ALWAYS)
        CompetitionMenu_VS_MenuSetupCB();

    rulesControl->menuSetupCB = CompetitionMenu_Rules_MenuSetupCB;
    if (rulesControl->active == ACTIVE_ALWAYS)
        CompetitionMenu_Rules_MenuSetupCB();

    roundControl->processButtonInputCB = CompetitionMenu_Round_ProcessInputCB;
    roundControl->menuSetupCB          = CompetitionMenu_Round_MenuSetupCB;
    if (roundControl->active == ACTIVE_ALWAYS)
        CompetitionMenu_Round_MenuSetupCB();

    totalControl->processButtonInputCB = CompetitionMenu_Results_ProcessInputCB;
    totalControl->menuSetupCB          = CompetitionMenu_Results_MenuSetupCB;
    totalControl->menuUpdateCB         = CompetitionMenu_Results_MenuUpdateCB;
    totalControl->targetPos.y          = totalControl->startPos.y;
    totalControl->position.y           = totalControl->startPos.y;
    if (totalControl->active == ACTIVE_ALWAYS)
        CompetitionMenu_Results_MenuSetupCB();

    foreach_all(UIVsZoneButton, zoneButton) { zoneButton->actionCB = CompetitionMenu_ZoneButtonActionCB; }
}

void CompetitionMenu_HandleMenuReturn(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    if (session->inMatch) {
        foreach_all(UIControl, control)
        {
            EntityUIControl *compControl = CompetitionMenu->competitionControl;
            if (!API.CheckDLC(DLC_PLUS))
                compControl = CompetitionMenu->competitionControl_Legacy;

            if (control == compControl) {
                foreach_all(UIVsCharSelector, selector)
                {
                    switch (session->playerID[selector->playerID]) {
                        case ID_SONIC: selector->frameID = 0; break;
                        case ID_TAILS: selector->frameID = 1; break;
                        case ID_KNUCKLES: selector->frameID = 2; break;
                        case ID_MIGHTY: selector->frameID = 3; break;
                        case ID_RAY: selector->frameID = 4; break;
                        default: break;
                    }
                }
            }

            if (control == CompetitionMenu->compRulesControl) {
                switch (session->itemMode) {
                    case ITEMS_FIXED: UIButton_SetChoiceSelection(control->buttons[0], 0); break;
                    case ITEMS_RANDOM: UIButton_SetChoiceSelection(control->buttons[0], 2); break;
                    case ITEMS_TELEPORT: UIButton_SetChoiceSelection(control->buttons[0], 1); break;
                }

                EntityUIVsRoundPicker *picker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(control->buttons[1], control->buttons[1]->selection);
                if (picker)
                    picker->val = session->matchCount;

                UIButton_SetChoiceSelectionWithCB(control->buttons[2], session->screenBorderType[2]);
                CompetitionMenu_SetupSplitScreenChoices(session->playerCount);
            }

            if (control == CompetitionMenu->compZoneControl) {
                for (int32 i = 0; i < 12; ++i) {
                    EntityUIVsZoneButton *button = (EntityUIVsZoneButton *)control->buttons[i];
                    if (button && session->completedStages[i])
                        button->xOut = true;
                }
            }
        }
    }
}

int32 CompetitionMenu_HandleUnlocks(void)
{
    int32 maxRounds = 0;

    foreach_all(UIVsZoneButton, zoneButton)
    {
        zoneButton->xOut      = !GameProgress_GetZoneUnlocked(zoneButton->zoneID);
        zoneButton->obfuscate = zoneButton->xOut;

        if (!zoneButton->xOut)
            ++maxRounds;
    }

    return maxRounds;
}

void CompetitionMenu_HandleStartCompPrompt(void)
{
    int32 readyCount  = CompetitionMenu_GetReadyPlayerCount();
    int32 activeCount = CompetitionMenu_GetTotalPlayerCount();

    EntityUIButtonPrompt *startComp = CompetitionMenu->startCompPrompt;
    if (startComp)
        startComp->visible = readyCount > 1 && readyCount == activeCount;

    EntityUIButtonPrompt *startCompLegacy = CompetitionMenu->startCompPrompt_Legacy;
    if (startCompLegacy)
        startCompLegacy->visible = readyCount > 1 && readyCount == activeCount;
}

int32 CompetitionMenu_GetReadyPlayerCount(void)
{
    EntityUIControl *control = CompetitionMenu->competitionControl;
    if (!API.CheckDLC(DLC_PLUS))
        control = CompetitionMenu->competitionControl_Legacy;

    if (!control)
        return 0;

    int32 readyPlayers = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];
        if (button->ready && !button->isSelected)
            ++readyPlayers;
    }

    return readyPlayers;
}

int32 CompetitionMenu_GetTotalPlayerCount(void)
{
    EntityUIControl *control = CompetitionMenu->competitionControl;
    if (!API.CheckDLC(DLC_PLUS))
        control = CompetitionMenu->competitionControl_Legacy;

    if (!control)
        return 0;

    int32 activePlayers = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];
        if (button->ready || button->isSelected)
            ++activePlayers;
    }

    return activePlayers;
}

void CompetitionMenu_ResetControllerAssignments(void)
{
    EntityUIVsCharSelector *buttons[PLAYER_COUNT];
    EntityUIControl *control = CompetitionMenu->competitionControl;

    int32 playerID = 0;
    for (int32 p = 0; p < PLAYER_COUNT; ++p) buttons[p] = (EntityUIVsCharSelector *)control->buttons[p];

    for (int32 p = 0; p < PLAYER_COUNT; ++p) {
        EntityUIVsCharSelector *selector = buttons[p];
        int32 id                         = API_GetInputDeviceID(p + 1);

        if (id && id != INPUT_AUTOASSIGN) {
            if (playerID < p)
                API_AssignInputSlotToDevice(p, INPUT_NONE);

            API_AssignInputSlotToDevice(CONT_P1 + playerID, id);

            buttons[playerID]->frameID = selector->frameID;
            if (playerID < p)
                selector->frameID = selector->playerID;

            ++playerID;
        }
    }

    for (; playerID < PLAYER_COUNT; ++playerID) API_AssignInputSlotToDevice(CONT_P1 + playerID, INPUT_NONE);
}

void CompetitionMenu_SetupSplitScreenChoices(int32 playerCount)
{
    EntityUIButton *screenLayoutChoice = CompetitionMenu->compRulesControl->buttons[2];

    bool32 choices[5];

    choices[0] = playerCount == 2;
    choices[1] = playerCount == 2;

    choices[2] = playerCount == 3;
    choices[3] = playerCount == 3;

    choices[4] = playerCount == 4;

    for (int32 c = 0; c < screenLayoutChoice->choiceCount; ++c) {
        EntityUIChoice *choice = (EntityUIChoice *)UIButton_GetChoicePtr(screenLayoutChoice, c);

        if (choice)
            choice->disabled = !choices[c];
    }

    EntityUIChoice *choice = (EntityUIChoice *)UIButton_GetChoicePtr(screenLayoutChoice, screenLayoutChoice->selection);
    if (choice->disabled) {
        if (playerCount == 2)
            UIButton_SetChoiceSelectionWithCB(screenLayoutChoice, 0);
        else if (playerCount == 3)
            UIButton_SetChoiceSelectionWithCB(screenLayoutChoice, 2);
        else
            UIButton_SetChoiceSelectionWithCB(screenLayoutChoice, 4);

        UIButton_ManageChoices(screenLayoutChoice);
    }
}
void CompetitionMenu_SetupSplitScreen(int32 mode)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    int32 startVert_3P[15];
    uint8 startVert_2P[10];

    memset(startVert_2P, 0, sizeof(startVert_2P));
    memset(startVert_3P, 0, sizeof(startVert_3P));

    startVert_2P[(0 * 5) + 0] = 18;
    startVert_2P[(0 * 5) + 1] = 6;
    startVert_2P[(0 * 5) + 2] = 0;
    startVert_2P[(0 * 5) + 3] = 0;
    startVert_2P[(0 * 5) + 4] = 0;

    startVert_2P[(1 * 5) + 0] = 24;
    startVert_2P[(1 * 5) + 1] = 12;
    startVert_2P[(1 * 5) + 2] = 0;
    startVert_2P[(1 * 5) + 3] = 0;
    startVert_2P[(1 * 5) + 4] = 0;

    startVert_3P[(0 * 5) + 0] = 0;
    startVert_3P[(0 * 5) + 1] = 0;
    startVert_3P[(0 * 5) + 2] = 30;
    startVert_3P[(0 * 5) + 3] = 6;
    startVert_3P[(0 * 5) + 4] = 0;

    startVert_3P[(1 * 5) + 0] = 0;
    startVert_3P[(1 * 5) + 1] = 0;
    startVert_3P[(1 * 5) + 2] = 36;
    startVert_3P[(1 * 5) + 3] = 42;
    startVert_3P[(1 * 5) + 4] = 0;

    startVert_3P[(2 * 5) + 0] = 0;
    startVert_3P[(2 * 5) + 1] = 0;
    startVert_3P[(2 * 5) + 2] = 12;
    startVert_3P[(2 * 5) + 3] = 48;
    startVert_3P[(2 * 5) + 4] = 0;

    RSDK.SetScreenVertices(startVert_2P[mode], startVert_2P[5 + mode], startVert_3P[mode], startVert_3P[5 + mode], startVert_3P[10 + mode]);

    switch (mode) {
        case 0: // 2P (Streched)
        case 1: // 2P (Bordered)
            session->screenBorderType[0] = VS_BORDER_BOTTOM;
            session->screenBorderType[1] = VS_BORDER_TOP;
            break;

        case 2: // 3P (P1 & P2 top, P3 bottom)
            session->screenBorderType[0] = VS_BORDER_BOTTOMRIGHT;
            session->screenBorderType[1] = VS_BORDER_BOTTOMLEFT;
            session->screenBorderType[2] = VS_BORDER_TOP;
            break;

        case 3: // 3P (P1 top, P2 & P3 bottom)
            session->screenBorderType[0] = VS_BORDER_BOTTOM;
            session->screenBorderType[1] = VS_BORDER_TOPRIGHT;
            session->screenBorderType[2] = VS_BORDER_TOPLEFT;
            break;

        case 4: // 4P
            session->screenBorderType[0] = VS_BORDER_BOTTOMRIGHT;
            session->screenBorderType[1] = VS_BORDER_BOTTOMLEFT;
            session->screenBorderType[2] = VS_BORDER_TOPRIGHT;
            session->screenBorderType[3] = VS_BORDER_TOPLEFT;
            break;

        default: break;
    }
}

void CompetitionMenu_SetupResultsUI(EntityUIControl *roundControl)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    int32 offsets[] = { -580000, 0x580000, 0x0, 0x0, -0x7A0000, 0x000000, 0x7A0000, 0x000000, -0x9C0000, -0x340000, 0x340000, 0x9C0000 };

    int32 offsetPtr = 4 * (session->playerCount - 2);
    for (int32 p = 0; p < PLAYER_COUNT; ++p) {
        EntityUIButton *button = roundControl->buttons[p];
        if (session->playerCount <= p) {
            button->visible = false;
        }
        else {
            button->visible    = true;
            button->position.x = roundControl->position.x + offsets[offsetPtr + p];
        }
    }
}

void CompetitionMenu_GotoCompRules(void)
{
    CompetitionMenu_SetupSplitScreenChoices(CompetitionMenu_GetTotalPlayerCount());
    UIControl_MatchMenuTag("Competition Rules");
}

void CompetitionMenu_GotoCompZones(void) { UIControl_MatchMenuTag("Competition Zones"); }

void CompetitionMenu_VS_ProcessInputCB(void)
{
    EntityUIControl *control = CompetitionMenu->competitionControl;
    if (!API.CheckDLC(DLC_PLUS))
        control = CompetitionMenu->competitionControl_Legacy;

    if (control) {
        for (int32 i = 0; i < control->buttonCount; ++i) {
            EntityUIVsCharSelector *charSel = (EntityUIVsCharSelector *)control->buttons[i];

            Entity *entStore  = SceneInfo->entity;
            SceneInfo->entity = (Entity *)charSel;
            StateMachine_Run(charSel->processButtonCB);
            SceneInfo->entity = entStore;

            API_GetInputDeviceID(charSel->playerID + 1);
        }

        EntityUIButtonPrompt *goPrompt = NULL;
        for (int32 i = 0; i < control->promptCount; ++i) {
            EntityUIButtonPrompt *prompt = control->prompts[i];

            if (prompt->buttonID == 4)
                goPrompt = prompt;
        }

        int32 readyPlayers = CompetitionMenu_GetReadyPlayerCount();
        if (readyPlayers > 1 && readyPlayers == CompetitionMenu_GetTotalPlayerCount()) {
            if (UIControl->startPress[0] || UIControl->startPress[1] || UIControl->startPress[2] || UIControl->startPress[3]) {
                control->selectionDisabled = true;
                UITransition_StartTransition(CompetitionMenu_GotoCompRules, 0);
            }

            goPrompt->visible = true;
        }
        else {
            goPrompt->visible = false;
        }
    }
}

void CompetitionMenu_VS_MenuSetupCB(void)
{
    RSDK_THIS(UIControl);

    self->childHasFocus = false;
    for (int32 i = 0; i < self->buttonCount; ++i) {
        EntityUIVsCharSelector *charSel = (EntityUIVsCharSelector *)self->buttons[i];

        charSel->isSelected      = true;
        charSel->ready           = false;
        charSel->processButtonCB = UIVsCharSelector_ProcessButtonCB;

        if (!API_GetInputDeviceID(CONT_P1 + charSel->playerID))
            API_AssignInputSlotToDevice(CONT_P1 + charSel->playerID, INPUT_AUTOASSIGN);
    }

    for (int32 i = 0; i < self->promptCount; ++i) {
        EntityUIButtonPrompt *prompt = self->prompts[i];

        if (prompt->buttonID == 4)
            prompt->visible = false;
    }
}

void CompetitionMenu_Rules_MenuSetupCB(void)
{
    if (API.CheckDLC(DLC_PLUS))
        CompetitionMenu_ResetControllerAssignments();
}

void CompetitionMenu_StartMatch(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    EntityMenuParam *param            = MenuParam_GetParam();

    sprintf_s(param->menuTag, (int32)sizeof(param->menuTag), "Competition Round");
    session->stageIndex  = CompetitionMenu->compZoneControl->buttonID;
    session->zoneID      = param->vsZoneID;
    session->actID       = param->vsActID;
    session->prevMatchID = session->matchID;

    for (int32 p = 0; p < session->playerCount; ++p) {
        if (!session->lives[p])
            session->lives[p] = 1;
    }

    if (globals->gameMode == MODE_ENCORE)
        RSDK.SetScene("Encore Mode", "");
    else
        RSDK.SetScene("Mania Mode", "");
    SceneInfo->listPos += TimeAttackData_GetManiaListPos(param->vsZoneID, param->vsActID, CHAR_SONIC);

    SaveGame_ResetPlayerState();

    // Bug Details(?):
    // sizeof(globals->noSaveSlot) and sizeof(saveData) is 4096 (sizeof(int32) * 0x400)
    // but the memset size is only 1024 (sizeof(uint8) * 0x400)
    // so only about 1/4th of the save slot is cleared, though nothin uses the extra space so it's not a big deal
    memset(globals->noSaveSlot, 0, 0x400);

    globals->continues  = 0;
    globals->saveSlotID = NO_SAVE_SLOT;
    globals->gameMode   = MODE_COMPETITION;
    globals->medalMods  = 0;

    globals->playerID = ID_NONE;
    for (int32 p = 0; p < session->playerCount; ++p) globals->playerID |= session->playerID[p] << (8 * p);

    globals->itemMode = session->itemMode;

    RSDK.LoadScene();
}

void CompetitionMenu_ZoneButtonActionCB(void) { MenuSetup_StartTransition(CompetitionMenu_StartMatch, 32); }

void CompetitionMenu_RulesButton_ActionCB(void)
{
    EntityUIControl *control = CompetitionMenu->competitionControl;
    if (!API.CheckDLC(DLC_PLUS))
        control = CompetitionMenu->competitionControl_Legacy;

    EntityUIControl *rulesControl     = CompetitionMenu->compRulesControl;
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    int32 matchCount = 0;
    foreach_all(UIVsRoundPicker, picker)
    {
        matchCount = picker->val;
        foreach_break;
    }

    int32 itemMode = ITEMS_FIXED;
    switch (rulesControl->buttons[0]->selection) {
        default: break;
        case 0: itemMode = ITEMS_FIXED; break;
        case 1: itemMode = ITEMS_TELEPORT; break;
        case 2: itemMode = ITEMS_RANDOM; break;
    }

    Competition_ResetOptions();

    session->matchCount = matchCount;
    session->inMatch    = true;
    session->itemMode   = itemMode;

    CompetitionMenu_SetupSplitScreen(rulesControl->buttons[2]->selection);
    session->displayMode = rulesControl->buttons[2]->selection;

    session->playerCount = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *charSel = (EntityUIVsCharSelector *)control->buttons[i];
        int32 id                        = API_GetInputDeviceID(CONT_P1 + i);
        if (id && id != INPUT_UNASSIGNED) {
            switch (charSel->frameID) {
                case 0: session->playerID[i] = ID_SONIC; break;
                case 1: session->playerID[i] = ID_TAILS; break;
                case 2: session->playerID[i] = ID_KNUCKLES; break;
#if MANIA_USE_PLUS
                case 3: session->playerID[i] = ID_MIGHTY; break;
                case 4: session->playerID[i] = ID_RAY; break;
#endif
                default: break;
            }

            session->inputSlots[i] = id;
            session->playerCount++;
        }
    }

    EntityUIControl *zoneControl = CompetitionMenu->compZoneControl;
    zoneControl->position        = zoneControl->startPos;
    zoneControl->targetPos.x     = zoneControl->startPos.x;
    zoneControl->targetPos.y     = zoneControl->startPos.y;
    zoneControl->buttonID        = 0;

    CompetitionMenu_HandleUnlocks();

    UIControl_MatchMenuTag("Competition Zones");
}

void CompetitionMenu_GotoCompTotal(void) { UIControl_MatchMenuTag("Competition Total"); }

void CompetitionMenu_Round_ProcessInputCB(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    if (UIControl->anyConfirmPress) {
        bool32 toCompTotal = false;

        int32 activePlayers = 0;
        for (int32 p = 0; p < session->playerCount; ++p) {
            if (session->lives[p] > 0)
                activePlayers++;

            if (session->wins[p] > (session->matchCount >> 1))
                toCompTotal = true;
        }

        EntityUIControl *zoneControl = CompetitionMenu->compZoneControl;
        int32 remainingZones         = 0;
        for (int32 i = 0; i < 12; ++i) {
            if (zoneControl->buttons[i]) {
                EntityUIVsZoneButton *zoneButton = (EntityUIVsZoneButton *)zoneControl->buttons[i];
                if (!session->completedStages[i] && GameProgress_GetZoneUnlocked(zoneButton->zoneID))
                    ++remainingZones;
            }
        }

        if (toCompTotal || activePlayers < 2 || session->matchID >= session->matchCount || !remainingZones)
            UITransition_StartTransition(CompetitionMenu_GotoCompTotal, 0);
        else
            UITransition_StartTransition(CompetitionMenu_GotoCompZones, 0);

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;
    }
}

void CompetitionMenu_Round_MenuSetupCB(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    EntityUIControl *roundControl     = CompetitionMenu->compRoundControl;
    CompetitionMenu_SetupResultsUI(roundControl);

    String roundLabelStr;
    INIT_STRING(roundLabelStr);

    char roundLabel[0x100];
    memset(roundLabel, 0, sizeof(roundLabel));

    Localization_GetZoneName(&roundLabelStr, session->zoneID);
    if (session->zoneID < 12) {
        char buf[16];
        sprintf_s(buf, (int32)sizeof(buf), " ZONE %d", session->actID + 1);
        RSDK.AppendText(&roundLabelStr, buf);
    }

    RSDK.GetCString(roundLabel, &roundLabelStr);
    UIInfoLabel_SetText(CompetitionMenu->resultsLabel_Round, roundLabel);

    int32 bestRings      = 0;
    int32 bestTotalRings = 0;
    int32 bestScore      = 0;
    int32 bestItems      = 0;
    uint32 bestTime       = 0xFFFFFFFF;
    uint32 times[PLAYER_COUNT];

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
    int32 match       = session->matchID - 1;

    for (int32 p = 0; p < session->playerCount; ++p) {
        EntityUIVsResults *results = (EntityUIVsResults *)roundControl->buttons[p];

        results->isWinner = session->matchWinner[match] & (1 << p);
        results->isLoser  = session->matchWinner[match] & (1 << p);

        if (session->matchWinner[match] & (1 << p))
            winnerCount++;

        results->trophyCount = session->wins[p];
        memset(buffer, 0, sizeof(buffer));

        sprintf_s(buffer, (int32)sizeof(buffer), "%d", session->rings[p]);
        if (!SceneInfo->inEditor) {
            RSDK.InitString(&results->rowText[0], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[0]);
        }

        sprintf_s(buffer, (int32)sizeof(buffer), "%d", session->totalRings[p]);
        if (!SceneInfo->inEditor) {
            RSDK.InitString(&results->rowText[1], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[1]);
        }

        sprintf_s(buffer, (int32)sizeof(buffer), "%d", session->score[p]);
        if (!SceneInfo->inEditor) {
            RSDK.InitString(&results->rowText[2], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[2]);
        }

        sprintf_s(buffer, (int32)sizeof(buffer), "%d", session->items[p]);
        if (!SceneInfo->inEditor) {
            RSDK.InitString(&results->rowText[3], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[3]);
        }

        sprintf_s(buffer, (int32)sizeof(buffer), "%d'%02d\"%02d", session->time[p].minutes, session->time[p].seconds, session->time[p].milliseconds);
        if (!SceneInfo->inEditor) {
            RSDK.InitString(&results->rowText[4], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[4]);
        }

        if (session->finishState[p] != FINISHTYPE_GAMEOVER) {
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
            if ((1 << p) & session->matchWinner[match]) {
                winner = p;
                break;
            }
        }

        LogHelpers_Print("Announce_CharWins(%d)", session->playerID[winner]);

        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceWinPlayer;
        announcer->playerID        = session->playerID[winner];
    }
    else {
        LogHelpers_Print("Announce_ItsADraw(%d)", 0);

        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceDraw;
        announcer->playerID        = 0;
        announcer->timer           = 0;
    }
}

void CompetitionMenu_GotoCompetition(void) { UIControl_MatchMenuTag(API.CheckDLC(DLC_PLUS) ? "Competition" : "Competition Legacy"); }

void CompetitionMenu_Results_ProcessInputCB(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    if (UIControl->anyConfirmPress) {
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

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;
    }
}

void CompetitionMenu_Results_MenuSetupCB(void)
{
    EntityUIControl *totalControl     = CompetitionMenu->compTotalControl;
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    CompetitionMenu_SetupResultsUI(totalControl);
    CompetitionMenu->timer = 120;

    totalControl->targetPos.y = totalControl->startPos.y;
    totalControl->position.y  = totalControl->startPos.y;

    String message;
    INIT_STRING(message);

    EntityUIInfoLabel *label = CompetitionMenu->resultsLabel_Total;
    Localization_GetString(&message, STR_COMPTOTAL);
    UIInfoLabel_SetString(label, &message);

    int32 highestScore = 0;
    for (int32 p = 0; p < session->playerCount; ++p) {
        if (session->wins[p] > highestScore)
            highestScore = session->wins[p];
    }

    int32 winnerCount = 0;
    int32 winnerID    = -1;
    for (int32 p = 0; p < session->playerCount; ++p) {
        if (session->wins[p] == highestScore) {
            winnerID = p;
            ++winnerCount;
        }
    }

    if (winnerCount >= 2) {
        LogHelpers_Print("Announce_ItsADraw(%d)", 1);

        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceDraw;
        announcer->playerID        = 1;
        announcer->timer           = 0;
    }
    else {
        LogHelpers_Print("Announce_WinnerIs(%d)", winnerID);

        EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
        announcer->state           = Announcer_State_AnnounceWinner;
        announcer->playerID        = winnerID;
    }

    for (int32 p = 0; p < session->playerCount; ++p) {
        EntityUIVsResults *results = (EntityUIVsResults *)totalControl->buttons[p];
        bool32 *highlight          = &results->row0Highlight;

        results->numRows     = session->matchCount;
        results->isWinner    = session->wins[p] == highestScore;
        results->trophyCount = session->wins[p];

        for (int32 r = 0; r < results->numRows; ++r) {
            char buffer[0x40];
            sprintf_s(buffer, (int32)sizeof(buffer), "%d", session->matchWinner[r]);

            if (!SceneInfo->inEditor) {
                RSDK.InitString(&results->rowText[r], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[r]);
            }

            highlight[r] = ((1 << p) & session->matchWinner[r]);
        }
    }
}

void CompetitionMenu_Results_MenuUpdateCB(void)
{
    EntityUIControl *totalControl = CompetitionMenu->compTotalControl;

    if (totalControl->active == ACTIVE_ALWAYS) {
        if (totalControl->position.y == totalControl->targetPos.y) {
            if (CompetitionMenu->timer <= 0) {
                int32 targetPos = totalControl->startPos.y;
                if (totalControl->targetPos.y == totalControl->startPos.y) {
                    EntityUIVsResults *button = (EntityUIVsResults *)totalControl->buttons[0];

                    if (button && button->size.y + button->position.y - 0x708000 > totalControl->startPos.y) {
                        targetPos = button->size.y + button->position.y - 0x708000;
                    }
                }

                totalControl->targetPos.y = targetPos;
                CompetitionMenu->timer    = 120;
            }
            else {
                CompetitionMenu->timer--;
            }
        }
    }
}

void CompetitionMenu_ExitComp_TransitionCB(void)
{
    EntityUIControl *zoneControl = CompetitionMenu->compZoneControl;
    EntityUIControl *control     = CompetitionMenu->competitionControl;
    if (!API.CheckDLC(DLC_PLUS))
        control = CompetitionMenu->competitionControl_Legacy;

    UIControl_SetInactiveMenu(zoneControl);
    UIControl_SetActiveMenu(control);
    Competition_ResetOptions();

    zoneControl->childHasFocus = false;

    CompetitionMenu_HandleUnlocks();
}

void CompetitionMenu_ExitComp_YesCB(void) { UITransition_StartTransition(CompetitionMenu_ExitComp_TransitionCB, 0); }

bool32 CompetitionMenu_CompRules_BackPressCB(void)
{
    if (API.CheckDLC(DLC_PLUS))
        UITransition_SetNewTag("Competition");
    else
        UITransition_SetNewTag("Competition Legacy");

    return true;
}

bool32 CompetitionMenu_CompZones_BackPressCB(void)
{
    String message;
    INIT_STRING(message);

    Localization_GetString(&message, STR_EXITCOMP);
    UIDialog_CreateDialogYesNo(&message, CompetitionMenu_ExitComp_YesCB, StateMachine_None, true, true);
    return true;
}

void CompetitionMenu_GotoPuyoVS(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();

    param->puyoSelection = PUYO_SELECTION_TIE_BREAKER;
    globals->gameMode    = MODE_COMPETITION;
    strcpy(param->menuTag, "Competition Total");
    param->menuSelection = 0;

    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

#if GAME_INCLUDE_EDITOR
void CompetitionMenu_EditorDraw(void) {}

void CompetitionMenu_EditorLoad(void) {}
#endif

void CompetitionMenu_Serialize(void) {}
#endif

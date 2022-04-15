// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CompetitionMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectCompetitionMenu *CompetitionMenu;

void CompetitionMenu_Update(void) {}

void CompetitionMenu_LateUpdate(void) {}

void CompetitionMenu_StaticUpdate(void) { CompetitionMenu_HandleStartCompPrompt(); }

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
            CompetitionMenu->competitionControl = control;

        RSDK.PrependText(&info, "Competition Legacy");
        if (RSDK.StringCompare(&info, &control->tag, false))
            CompetitionMenu->competitionControl_Legacy = control;

        RSDK.PrependText(&info, "Competition Rules");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            CompetitionMenu->compRulesControl = control;
            control->backPressCB              = CompetitionMenu_CompRules_BackPressCB;
        }

        RSDK.PrependText(&info, "Competition Zones");
        if (RSDK.StringCompare(&info, &control->tag, false)) {
            CompetitionMenu->compZoneControl = control;
            control->backPressCB             = CompetitionMenu_CompZones_BackPressCB;
        }

        RSDK.PrependText(&info, "Competition Round");
        if (RSDK.StringCompare(&info, &control->tag, false))
            CompetitionMenu->compRoundControl = control;

        RSDK.PrependText(&info, "Competition Total");
        if (RSDK.StringCompare(&info, &control->tag, false))
            CompetitionMenu->compTotalControl = control;
    }

    Hitbox hitbox;
    foreach_all(UIInfoLabel, label)
    {
        EntityUIControl *roundControl = (EntityUIControl *)CompetitionMenu->compRoundControl;
        hitbox.right                  = roundControl->size.x >> 17;
        hitbox.left                   = -(roundControl->size.x >> 17);
        hitbox.bottom                 = roundControl->size.y >> 17;
        hitbox.top                    = -(roundControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(roundControl->startPos.x - roundControl->cameraOffset.x,
                                      roundControl->startPos.y - roundControl->cameraOffset.y, label->position.x, label->position.y, FLIP_NONE,
                                      &hitbox))
            CompetitionMenu->resultsLabel_Round = label;

        EntityUIControl *totalControl = (EntityUIControl *)CompetitionMenu->compTotalControl;
        hitbox.right                  = totalControl->size.x >> 17;
        hitbox.left                   = -(totalControl->size.x >> 17);
        hitbox.bottom                 = totalControl->size.y >> 17;
        hitbox.top                    = -(totalControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(totalControl->startPos.x - totalControl->cameraOffset.x,
                                      totalControl->startPos.y - totalControl->cameraOffset.y, label->position.x, label->position.y, FLIP_NONE,
                                      &hitbox))
            CompetitionMenu->resultsLabel_Total = label;
    }

    foreach_all(UIButtonPrompt, prompt)
    {
        EntityUIControl *compControl = (EntityUIControl *)CompetitionMenu->competitionControl;
        hitbox.right                 = compControl->size.x >> 17;
        hitbox.left                  = -(compControl->size.x >> 17);
        hitbox.bottom                = compControl->size.y >> 17;
        hitbox.top                   = -(compControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(compControl->startPos.x - compControl->cameraOffset.x, compControl->startPos.y - compControl->cameraOffset.y,
                                      prompt->position.x, prompt->position.y, FLIP_NONE, &hitbox)
            && prompt->buttonID == 5) {
            CompetitionMenu->startCompPrompt = prompt;
        }
        EntityUIControl *compControl_Legacy = (EntityUIControl *)CompetitionMenu->competitionControl_Legacy;

        hitbox.right  = compControl_Legacy->size.x >> 17;
        hitbox.left   = -(compControl_Legacy->size.x >> 17);
        hitbox.bottom = compControl_Legacy->size.y >> 17;
        hitbox.top    = -(compControl_Legacy->size.y >> 17);
        if (MathHelpers_PointInHitbox(compControl_Legacy->startPos.x - compControl_Legacy->cameraOffset.x,
                                      compControl_Legacy->startPos.y - compControl_Legacy->cameraOffset.y, prompt->position.x, prompt->position.y,
                                      FLIP_NONE, &hitbox)) {
            if (prompt->buttonID == 5)
                CompetitionMenu->startCompPrompt_Legacy = prompt;
        }
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
        Hitbox hitbox;
        hitbox.right  = rulesControl->size.x >> 17;
        hitbox.left   = -(rulesControl->size.x >> 17);
        hitbox.bottom = rulesControl->size.y >> 17;
        hitbox.top    = -(rulesControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(rulesControl->startPos.x - rulesControl->cameraOffset.x,
                                      rulesControl->startPos.y - rulesControl->cameraOffset.y, button->position.x, button->position.y, FLIP_NONE,
                                      &hitbox)
            && button->listID == 9 && button->frameID == 2)
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
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    if (session->inMatch) {
        foreach_all(UIControl, control)
        {
            EntityUIControl *compControl = CompetitionMenu->competitionControl;
            if (!API.CheckDLC(DLC_PLUS))
                compControl = CompetitionMenu->competitionControl_Legacy;

            if (control == compControl) {
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

            if (control == CompetitionMenu->compRulesControl) {
                switch (session->itemMode) {
                    case ITEMS_FIXED: UIButton_SetChoiceSelection(control->buttons[0], 0); break;
                    case ITEMS_RANDOM: UIButton_SetChoiceSelection(control->buttons[0], 2); break;
                    case ITEMS_TELEPORT: UIButton_SetChoiceSelection(control->buttons[0], 1); break;
                }

                EntityUIVsRoundPicker *picker = (EntityUIVsRoundPicker *)UIButton_GetChoicePtr(control->buttons[1], control->buttons[1]->selection);
                if (picker)
                    picker->val = session->matchCount;
                UIButton_SetChoiceSelectionWithCB(control->buttons[2], session->splitScreenMode[2]);
                CompetitionMenu_SetupSplitScreenChoices(session->playerCount);
            }

            if (control == CompetitionMenu->compZoneControl) {
                for (int32 i = 0; i < 12; ++i) {
                    EntityUIVsZoneButton *button = (EntityUIVsZoneButton *)control->buttons[i];
                    if (button && session->zoneFlags[i])
                        button->xOut = true;
                }
            }
        }
    }
}

int32 CompetitionMenu_HandleUnlocks(void)
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

void CompetitionMenu_HandleStartCompPrompt(void)
{
    int32 readyCount  = CompetitionMenu_GetReadyPlayerCount();
    int32 activeCount = CompetitionMenu_GetActivePlayerCount();

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

    int32 count = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];
        if (button->ready && !button->isSelected)
            ++count;
    }
    return count;
}

int32 CompetitionMenu_GetActivePlayerCount(void)
{
    EntityUIControl *control = CompetitionMenu->competitionControl;
    if (!API.CheckDLC(DLC_PLUS))
        control = CompetitionMenu->competitionControl_Legacy;

    if (!control)
        return 0;

    int32 count = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];
        if (button->ready || button->isSelected)
            ++count;
    }
    return count;
}

void CompetitionMenu_ResetControllerAssignments(void)
{
    EntityUIVsCharSelector *buttons[PLAYER_MAX];
    EntityUIControl *control = CompetitionMenu->competitionControl;

    int32 playerID = 0;
    for (int32 i = 0; i < PLAYER_MAX; ++i) buttons[i] = (EntityUIVsCharSelector *)control->buttons[i];

    for (int32 i = 0; i < PLAYER_MAX; ++i) {
        EntityUIVsCharSelector *selector = buttons[i];
        int32 id                         = RSDK.ControllerIDForInputID(i + 1);

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

void CompetitionMenu_SetupSplitScreenChoices(int32 playerCount)
{
    EntityUIButton *screenLayoutChoice = CompetitionMenu->compRulesControl->buttons[2];

    bool32 flags[5];
    flags[0] = playerCount == 2;
    flags[1] = flags[0];
    flags[2] = playerCount == 3;
    flags[3] = flags[2];
    flags[4] = playerCount == 4;

    for (int32 c = 0; c < screenLayoutChoice->choiceCount; ++c) {
        EntityUIChoice *choice = (EntityUIChoice *)UIButton_GetChoicePtr(screenLayoutChoice, c);
        if (choice) {
            choice->disabled = !flags[c];
        }
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
            session->splitScreenMode[0] = 2;
            session->splitScreenMode[1] = 5;
            break;
        case 2:
            session->splitScreenMode[2] = 5;
            session->splitScreenMode[1] = 3;
            session->splitScreenMode[0] = 1;
            break;
        case 3:
            session->splitScreenMode[0] = 2;
            session->splitScreenMode[1] = 4;
            session->splitScreenMode[2] = 6;
            break;
        case 4:
            session->splitScreenMode[0] = 1;
            session->splitScreenMode[1] = 3;
            session->splitScreenMode[2] = 4;
            session->splitScreenMode[3] = 6;
            break;
        default: break;
    }
}

void CompetitionMenu_SetupResultsUI(void *control)
{
    EntityUIControl *roundControl     = control;
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
    CompetitionMenu_SetupSplitScreenChoices(CompetitionMenu_GetActivePlayerCount());
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

            Entity *entStore  = SceneInfo->entity;
            SceneInfo->entity = (Entity *)button;
            StateMachine_Run(button->processButtonCB);
            SceneInfo->entity = entStore;

            RSDK.ControllerIDForInputID(button->playerID + 1);
        }

        EntityUIButtonPrompt *promptPtr = NULL;
        for (int32 i = 0; i < control->promptCount; ++i) {
            EntityUIButtonPrompt *prompt = control->prompts[i];
            if (prompt->buttonID == 4)
                promptPtr = prompt;
        }

        int32 count1 = CompetitionMenu_GetReadyPlayerCount();
        if (count1 > 1 && count1 == CompetitionMenu_GetActivePlayerCount()) {
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

void CompetitionMenu_VS_MenuSetupCB(void)
{
    RSDK_THIS(UIControl);

    self->childHasFocus = false;
    for (int32 i = 0; i < self->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)self->buttons[i];

        button->isSelected      = true;
        button->ready           = false;
        button->processButtonCB = UIVsCharSelector_ProcessButtonCB;

        if (!RSDK.ControllerIDForInputID(button->playerID + 1))
            RSDK.AssignControllerID(button->playerID + 1, CONT_AUTOASSIGN);
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
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    EntityMenuParam *param            = (EntityMenuParam *)globals->menuParam;

    sprintf(param->menuTag, "Competition Round");
    session->levelIndex  = ((EntityUIControl *)CompetitionMenu->compZoneControl)->buttonID;
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
    SceneInfo->listPos += TimeAttackData_GetManiaListPos(param->vsZoneID, param->vsActID, 0);
    SaveGame_ResetPlayerState();
    memset(globals->noSaveSlot, 0, 0x400);

    globals->continues  = 0;
    globals->saveSlotID = 255;
    globals->gameMode   = MODE_COMPETITION;
    globals->medalMods  = 0;

    globals->playerID = ID_NONE;
    for (int32 i = 0; i < session->playerCount; ++i) globals->playerID |= session->characterFlags[i] << (8 * i);
    globals->itemMode = session->itemMode;
    RSDK.LoadScene();
}

void CompetitionMenu_ZoneButtonActionCB(void) { MenuSetup_StartTransition(CompetitionMenu_StartMatch, 32); }

void CompetitionMenu_RulesButton_ActionCB(void)
{
    EntityUIControl *rulesControl     = CompetitionMenu->compRulesControl;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

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

    EntityUIControl *control       = CompetitionMenu->competitionControl;
    EntityUIControl *legacyControl = CompetitionMenu->competitionControl_Legacy;
    if (!API.CheckDLC(DLC_PLUS))
        control = legacyControl;

    session->playerCount = 0;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)control->buttons[i];
        int32 id                       = RSDK.ControllerIDForInputID(i + 1);
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
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (UIControl->keyConfirm) {
        bool32 flag = false;
        int32 count = 0;
        for (int32 p = 0; p < session->playerCount; ++p) {
            if (session->lives[p] > 0)
                count++;
            if (session->wins[p] > (session->matchCount >> 1))
                flag = true;
        }

        EntityUIControl *zoneControl = CompetitionMenu->compZoneControl;
        int32 count2                 = 0;
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

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;
    }
}

void CompetitionMenu_Round_MenuSetupCB(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    EntityUIControl *roundControl     = CompetitionMenu->compRoundControl;
    CompetitionMenu_SetupResultsUI(roundControl);

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
    UIInfoLabel_SetText(CompetitionMenu->resultsLabel_Round, textBuffer);

    uint32 bestRings      = 0;
    uint32 bestTotalRings = 0;
    uint32 bestScore      = 0;
    uint32 bestItems      = 0;
    uint32 bestTime       = 0xFFFFFFFF;
    uint32 times[PLAYER_MAX];

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

        results->isWinner = session->winnerFlags[match] & (1 << p);
        results->isLoser  = session->winnerFlags[match] & (1 << p);
        if (session->winnerFlags[match] & (1 << p))
            winnerCount++;
        results->trophyCount = session->wins[p];
        memset(buffer, 0, sizeof(buffer));

        sprintf(buffer, "%d", session->rings[p]);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[0], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[0]);
        }

        printf(buffer, "%d", session->totalRings[p]);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[1], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[1]);
        }

        sprintf(buffer, "%d", session->score[p]);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[2], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[2]);
        }

        sprintf(buffer, "%d", session->items[p]);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[3], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[3]);
        }

        sprintf(buffer, "%d'%02d\"%02d", session->time[p].minutes, session->time[p].seconds, session->time[p].milliseconds);
        if (!SceneInfo->inEditor) {
            RSDK.SetText(&results->rowText[4], buffer, 0);
            RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[4]);
        }

        if (session->finishFlags[p] != FINISHFLAG_TIMEOVER) {
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
            if ((1 << p) & session->winnerFlags[match]) {
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
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIControl->inputLocked = true;
    }
}

void CompetitionMenu_Results_MenuSetupCB(void)
{
    EntityUIControl *totalControl     = CompetitionMenu->compTotalControl;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    CompetitionMenu_SetupResultsUI(totalControl);
    CompetitionMenu->timer = 120;

    totalControl->targetPos.y = totalControl->startPos.y;
    totalControl->position.y  = totalControl->startPos.y;

    TextInfo info;
    INIT_TEXTINFO(info);

    EntityUIInfoLabel *label = CompetitionMenu->resultsLabel_Total;
    Localization_GetString(&info, STR_COMPTOTAL);
    UIInfoLabel_SetString(label, &info);

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

    if (winnerCount <= 1) {
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
        results->isWinner    = session->wins[p] == highestScore;
        results->trophyCount = session->wins[p];
        for (int32 r = 0; r < results->numRows; ++r) {
            char buffer[0x40];
            sprintf(buffer, "%d", session->winnerFlags[r]);
            if (!SceneInfo->inEditor) {
                RSDK.SetText(&results->rowText[r], buffer, 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &results->rowText[r]);
            }
            highlight[r] = ((1 << p) & session->winnerFlags[r]);
        }
    }
}

void CompetitionMenu_Results_MenuUpdateCB(void)
{
    EntityUIControl *totalControl = CompetitionMenu->compTotalControl;

    if (totalControl->active == ACTIVE_ALWAYS) {
        if (totalControl->position.y == totalControl->targetPos.y) {
            if (CompetitionMenu->timer <= 0) {
                int32 pos = totalControl->startPos.y;
                if (totalControl->targetPos.y == totalControl->startPos.y) {
                    EntityUIVsResults *button = (EntityUIVsResults *)totalControl->buttons[0];
                    if (button) {
                        if (button->size.y + button->position.y - 0x708000 > totalControl->startPos.y)
                            pos = button->size.y + button->position.y - 0x708000;
                    }
                }
                totalControl->targetPos.y = pos;
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
    EntityUIControl *control       = CompetitionMenu->competitionControl;
    EntityUIControl *legacyControl = CompetitionMenu->competitionControl_Legacy;
    EntityUIControl *zoneControl   = CompetitionMenu->compZoneControl;
    if (!API.CheckDLC(DLC_PLUS))
        control = legacyControl;

    UIControl_SetInactiveMenu(zoneControl);
    UIControl_SetActiveMenu(control);
    Competition_ResetOptions();
    zoneControl->childHasFocus = false;
    CompetitionMenu_HandleUnlocks();
}

void CompetitionMenu_ExitComp_YesCB(void) { UITransition_StartTransition(CompetitionMenu_ExitComp_TransitionCB, 0); }

bool32 CompetitionMenu_CompRules_BackPressCB(void)
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

bool32 CompetitionMenu_CompZones_BackPressCB(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_EXITCOMP);
    UIDialog_CreateDialogYesNo(&info, CompetitionMenu_ExitComp_YesCB, StateMachine_None, true, true);
    return true;
}

void CompetitionMenu_GotoPuyoVS(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;

    TimeAttackData_Clear();
    param->puyoSelection = PUYO_SELECTION_TIE_BREAKER;
    globals->gameMode    = MODE_COMPETITION;
    strcpy(param->menuTag, "Competition Total");
    param->menuSelection = 0;
    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

#if RETRO_INCLUDE_EDITOR
void CompetitionMenu_EditorDraw(void) {}

void CompetitionMenu_EditorLoad(void) {}
#endif

void CompetitionMenu_Serialize(void) {}
#endif

// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PuyoGame Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"
#include <time.h>

ObjectPuyoGame *PuyoGame;

void PuyoGame_Update(void)
{
    RSDK_THIS(PuyoGame);

    if (!self->started) {
        PuyoGame_SetupStartingEntities();
        self->started = true;
    }

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->animator);

    if ((ControllerInfo->keyStart.press || Unknown_pausePress) && !RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID) {
        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
        RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->triggerPlayer = 1;
        RSDK.PlaySfx(PauseMenu->sfxAccept, false, 0xFF);
        RSDK.SetEngineState(ENGINESTATE_FROZEN);
    }
}

void PuyoGame_LateUpdate(void) {}

void PuyoGame_StaticUpdate(void) {}

void PuyoGame_Draw(void) {}

void PuyoGame_Create(void *data)
{
    RSDK_THIS(PuyoGame);

    self->active        = ACTIVE_NORMAL;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = false;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (!SceneInfo->inEditor)
        RSDK.AddCamera(&self->position, ScreenInfo->center.x << 16, ScreenInfo->center.y << 16, true);

    self->state = PuyoGame_State_Init;
}

void PuyoGame_StageLoad(void)
{
    PuyoGame->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE);

    foreach_all(FXFade, fxFade) { PuyoGame->fxFade = fxFade; }

    String text;
    Localization_GetString(&text, STR_RPC_PLAYING);
    API_SetRichPresence(PRESENCE_GENERIC, &text);
    destroyEntitySlot(SLOT_ZONE);
}

void PuyoGame_SetupStartingEntities(void)
{
    RSDK_THIS(PuyoGame);

    EntityMenuParam *param            = MenuParam_GetParam();
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    ScreenInfo->position.x = (self->position.x >> 16) - ScreenInfo->center.x;
    ScreenInfo->position.y = (self->position.y >> 16) - ScreenInfo->center.y;

    foreach_all(PuyoScore, score)
    {
        int32 playerID = score->playerID;
        if (score->counter) {
            PuyoGame->score2[playerID] = score;

            if (param->puyoSelection == PUYO_SELECTION_TIE_BREAKER)
                score->score = session->wins[playerID];
        }
        else {
            PuyoGame->score1[playerID] = score;
        }
    }

    foreach_all(PuyoLevelSelect, levelSel) { PuyoGame->levelSel[levelSel->playerID] = levelSel; }

    if (param->puyoSelection == PUYO_SELECTION_TIE_BREAKER) {
        self->selectedLevels[0] = 1;
        self->selectedLevels[1] = 1;
        self->state             = PuyoGame_State_SetupRound;
    }
    else {
        self->state = PuyoGame_State_SelectingLevel;
    }
}

void PuyoGame_SetLoser(uint8 loser)
{
    RSDK_THIS(PuyoGame);

    EntityPuyoMatch *manager           = PuyoGame->managers[loser];
    EntityCollapsingPlatform *platform = PuyoGame->platforms[loser];

    int32 winner          = loser ^ 1;
    self->determinedLoser = true;
    self->roundWinner     = winner;
    platform->stoodPos.x  = manager->position.x;
    manager->timer        = 7;
    PuyoAI->isAI[0]       = false;
    PuyoAI->isAI[1]       = false;

    EntityPuyoMatch *foeManager = PuyoGame->managers[winner];
    EntityPuyoScore *foeScore   = PuyoGame->score2[winner];
    ++foeScore->score;
    foeScore->flashing = true;
    foeManager->state  = StateMachine_None;

    PuyoBean_DestroyPuyoBeans();
    PuyoGame_CheckMatchFinish();

    self->timer = 0;
    self->state = self->finishedMatch ? PuyoGame_State_ShowMatchResults : PuyoGame_State_ShowRoundResults;
}

void PuyoGame_DestroyPuyoBeans(void)
{
    foreach_all(PuyoBean, bean) { destroyEntity(bean); }
}

void PuyoGame_SetupGameState(void)
{
    int32 fgHighRebuild = RSDK.GetTileLayerID("FG High Rebuild");
    int32 fgHigh        = RSDK.GetTileLayerID("FG High");

    RSDK.CopyTileLayer(fgHigh, 0, 0, fgHighRebuild, 0, 0, 32, 16);

    EntityPuyoScore *scoreP1 = PuyoGame->score2[0];
    EntityPuyoScore *scoreP2 = PuyoGame->score2[1];

    PuyoBean->comboChainCount[0] = 0;
    PuyoBean->disableBeanLink[0] = false;
    scoreP1->flashing            = false;

    PuyoBean->comboChainCount[1] = 0;
    PuyoBean->disableBeanLink[1] = false;
    scoreP2->flashing            = false;

    for (int32 i = 0; i < 0x100; ++i) PuyoBean->playfield[i] = NULL;

    foreach_all(PuyoMatch, match)
    {
        match->score           = 0;
        match->comboScore      = 0;
        match->comboCount      = 0;
        match->comboBeanCount  = 0;
        match->comboBonusTable = PuyoMatch->comboBonus;
        match->concurrentBonus = 0;
        match->beanBonus       = 0;
        match->timer           = false;
        match->junkDropCount   = 0;
        match->junkBeanCount   = 0;

        PuyoGame->score1[0]->score = match->score;
    }

    foreach_all(CollapsingPlatform, platform) { platform->stoodPos.x = 0; }
}

void PuyoGame_CheckMatchFinish(void)
{
    RSDK_THIS(PuyoGame);

    EntityPuyoScore *scoreP1 = PuyoGame->score2[0];
    EntityPuyoScore *scoreP2 = PuyoGame->score2[1];

    int32 score = MAX(scoreP1->score, scoreP2->score);

    if (score >= 2) {
        self->matchWinner                                  = scoreP1->score <= scoreP2->score;
        self->finishedMatch                                = true;
        PuyoGame->indicators[self->matchWinner]->state     = PuyoIndicator_ShowWinner;
        PuyoGame->indicators[self->matchWinner ^ 1]->state = PuyoIndicator_ShowLoser;
    }
}

void PuyoGame_State_Init(void)
{
    RSDK_THIS(PuyoGame);

    self->state = PuyoGame_State_Wait;
}

void PuyoGame_State_Wait(void)
{
    // Gaming(?)
}

void PuyoGame_State_SelectingLevel(void)
{
    RSDK_THIS(PuyoGame);

    EntityMenuParam *param = MenuParam_GetParam();

    EntityPuyoLevelSelect *levelSelP1 = PuyoGame->levelSel[0];
    EntityPuyoLevelSelect *levelSelP2 = PuyoGame->levelSel[1];

    levelSelP1->canSelectLevels = true;
    if (param->puyoSelection == PUYO_SELECTION_VS_CPU || param->puyoSelection == PUYO_SELECTION_NONE)
        levelSelP2->ready = true;

    levelSelP2->canSelectLevels = true;

    if (levelSelP1->ready && levelSelP2->ready) {
        levelSelP1->canSelectLevels = false;
        levelSelP2->canSelectLevels = false;
        self->selectedLevels[0]     = levelSelP1->optionID;
        self->selectedLevels[1]     = levelSelP2->optionID;
        self->state                 = PuyoGame_State_SetupRound;
    }
}

void PuyoGame_State_SetupRound(void)
{
    RSDK_THIS(PuyoGame);

    if (self->timer >= 60) {
        self->timer = 0;
        self->state = PuyoGame_State_SetupEntities;
    }
    else {
        if (!self->timer) {
            PuyoGame_SetupGameState();
            foreach_all(PuyoIndicator, indicator) { indicator->state = PuyoIndicator_ShowReady; }
            int32 key      = (int32)time(NULL);
            int32 matchKey = RSDK.RandSeeded(0, 512, &key);

            foreach_all(PuyoMatch, match)
            {
                RSDK.SetSpriteAnimation(-1, 0, &match->beanLAnimator, true, 0);
                RSDK.SetSpriteAnimation(-1, 0, &match->beanRAnimator, true, 0);
                match->active        = ACTIVE_NORMAL;
                match->matchKey      = matchKey;
                match->selectedLevel = self->selectedLevels[match->playerID];
                PuyoMatch_SetupNextBeans(match);
            }
        }

        ++self->timer;
    }
}

void PuyoGame_State_SetupEntities(void)
{
    RSDK_THIS(PuyoGame);

    EntityMenuParam *param = MenuParam_GetParam();

    self->determinedLoser = false;
    self->roundWinner     = 0;

    int32 matchID = 0;
    foreach_all(PuyoMatch, match)
    {
        match->timer  = true;
        match->active = ACTIVE_NORMAL;
        match->state  = PuyoMatch_State_HandleMatch;

        if (match->playerID) {
            if (param->puyoSelection == PUYO_SELECTION_VS_CPU || param->puyoSelection == PUYO_SELECTION_NONE) {
                match->stateInput                        = PuyoAI_Input_AI;
                PuyoAI->lastBeanY[match->playerID]       = PUYO_PLAYFIELD_H;
                PuyoAI->desiredColumn[match->playerID]   = 0;
                PuyoAI->desiredRotation[match->playerID] = 0;
                PuyoAI->isAI[match->playerID]            = true;
                PuyoAI->controlInterval[match->playerID] = PuyoAI->controlIntervals[self->selectedLevels[0]];
                PuyoAI->controlChance[match->playerID]   = PuyoAI->controlChances[self->selectedLevels[0]];
            }
        }

        PuyoGame->managers[matchID++] = match;
    }

    int32 indicatorID = 0;
    foreach_all(PuyoIndicator, indicator)
    {
        indicator->state                    = StateMachine_None;
        PuyoGame->indicators[indicatorID++] = indicator;
    }

    int32 platformID = 0;
    foreach_all(CollapsingPlatform, platform) { PuyoGame->platforms[platformID++] = platform; }

    self->state = PuyoGame_State_HandleRound;
    PuyoGame_State_HandleRound();
}

void PuyoGame_State_HandleRound(void)
{
    for (int32 p = 0; p < 2; ++p) {
        EntityPuyoMatch *match = PuyoGame->managers[p];

        PuyoGame->score1[p]->score = match->score;
        if (match->state == PuyoMatch_State_Lose)
            PuyoGame_SetLoser(p);
    }
}

void PuyoGame_State_ShowRoundResults(void)
{
    RSDK_THIS(PuyoGame);
    EntityMenuParam *param = MenuParam_GetParam();

    if (self->timer >= 60) {
        int32 count          = param->puyoSelection >= PUYO_SELECTION_VS_2P ? 2 : 1;
        bool32 buttonPressed = false;

        RSDKControllerState *controller = &ControllerInfo[CONT_P1];
        for (int32 i = 0; i < count; ++i) {
            buttonPressed |= controller[i].keyA.down || controller[i].keyB.down || controller[i].keyC.down || controller[i].keyX.down
                             || controller[i].keyY.down || controller[i].keyZ.down;
        }

        if (buttonPressed) {
            PuyoGame_DestroyPuyoBeans();
            self->timer = 0;
            self->state = PuyoGame_State_SetupRound;
        }
    }
    else {
        self->timer++;
    }
}

void PuyoGame_State_ShowMatchResults(void)
{
    RSDK_THIS(PuyoGame);

    EntityMenuParam *param = MenuParam_GetParam();

    if (self->timer >= 60) {
        int32 count          = param->puyoSelection >= PUYO_SELECTION_VS_2P ? 2 : 1;
        bool32 buttonPressed = false;

        RSDKControllerState *controller = &ControllerInfo[CONT_P1];
        for (int32 i = 0; i < count; ++i) {
            buttonPressed |= controller[i].keyA.down || controller[i].keyB.down || controller[i].keyC.down || controller[i].keyX.down
                             || controller[i].keyY.down || controller[i].keyZ.down;
        }

        if (buttonPressed) {
            EntityFXFade *fxFade = PuyoGame->fxFade;
            fxFade->timer        = 0;
            fxFade->speedIn      = 16;
            fxFade->drawGroup    = 15;
            fxFade->state        = FXFade_State_FadeOut;
            self->state          = PuyoGame_State_FadeToMenu;
        }
    }
    else {
        self->timer++;
    }
}

void PuyoGame_State_FadeToMenu(void)
{
    RSDK_THIS(PuyoGame);

    EntityCompetitionSession *session = CompetitionSession_GetSession();
    EntityMenuParam *param            = MenuParam_GetParam();

    EntityFXFade *fxFade = PuyoGame->fxFade;
    if (fxFade->timer == 512) {
        if (param->puyoSelection == PUYO_SELECTION_TIE_BREAKER)
            session->wins[self->matchWinner]++;

        destroyEntity(self);

        RSDK.SetScene("Presentation", "Menu");
        RSDK.LoadScene();
    }
}

#if RETRO_INCLUDE_EDITOR
void PuyoGame_EditorDraw(void)
{
    RSDK_THIS(PuyoGame);

    RSDK.SetSpriteAnimation(PuyoGame->aniFrames, 24, &self->animator, false, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PuyoGame_EditorLoad(void) { PuyoGame->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE); }
#endif

void PuyoGame_Serialize(void) {}

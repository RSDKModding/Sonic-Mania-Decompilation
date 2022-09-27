// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Competition Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCompetition *Competition;

void Competition_Update(void)
{
    RSDK_THIS(Competition);

    StateMachine_Run(self->state);
}

void Competition_LateUpdate(void) {}

void Competition_StaticUpdate(void) {}

void Competition_Draw(void)
{
    RSDK_THIS(Competition);

#if MANIA_USE_PLUS
    if (RSDK_GET_ENTITY(SceneInfo->currentScreenID, Player)->classID == Player->classID) {
#endif
        if (!self->playerFinished[SceneInfo->currentScreenID]) {
            Vector2 drawPos;
            drawPos.x              = TO_FIXED(ScreenInfo[SceneInfo->currentScreenID].center.x - 4);
            drawPos.y              = TO_FIXED(26);
            self->animator.frameID = self->timer / 10;
            RSDK.DrawSprite(&self->animator, &drawPos, true);

            drawPos.x += TO_FIXED(8);
            self->animator.frameID = self->timer % 10;
            RSDK.DrawSprite(&self->animator, &drawPos, true);
        }
#if MANIA_USE_PLUS
    }
#endif
}

void Competition_Create(void *data)
{
    RSDK_THIS(Competition);

    if (!SceneInfo->inEditor) {
        self->isPermanent = true;
        self->active      = ACTIVE_NORMAL;
        self->visible     = true;
        self->drawGroup   = Zone->hudDrawGroup - 1;

        self->seconds = SceneInfo->seconds;
        if (SceneInfo->minutes == 9) {
            self->timer = 0;

            if (60 - SceneInfo->seconds > 0)
                self->timer = 60 - SceneInfo->seconds;

            if (!SceneInfo->milliseconds)
                SceneInfo->milliseconds = 1;
        }
        else {
            self->timer = 60;
        }

        self->state = Competition_State_Manager;
        RSDK.SetSpriteAnimation(Competition->aniFrames, 1, &self->animator, true, 0);
    }
}

void Competition_StageLoad(void)
{
    Competition->sessionManager = NULL;

    Competition->aniFrames = RSDK.LoadSpriteAnimation("Global/HUD.bin", SCOPE_STAGE);
}

void Competition_State_Manager(void)
{
    RSDK_THIS(Competition);

#if MANIA_USE_PLUS
    if (self->timer <= 0) {
        Zone->gotTimeOver      = true;
        SceneInfo->timeEnabled = false;
        for (int32 p = 0; p < Player->playerCount; ++p) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
            if (player->classID == Player->classID && player->state == Player_State_Death)
                player->visible = true;
        }
        self->state = StateMachine_None;
    }
    else {
        if (self->seconds != SceneInfo->seconds) {
            self->seconds = SceneInfo->seconds;

            self->timer--;
            if (self->timer == 12) {
                Music_PlayJingle(TRACK_DROWNING);
            }
            else if (!self->timer) {
                Zone->gotTimeOver      = true;
                SceneInfo->timeEnabled = false;
                for (int32 p = 0; p < Player->playerCount; ++p) {
                    if (!self->playerFinished[p]) {
                        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
                        if (player->classID == Player->classID)
                            player->deathType = PLAYER_DEATH_DIE_USESFX;
                    }
                }
            }
        }
    }
#else
    if (self->timer > 0) {
        if (self->seconds != SceneInfo->seconds) {
            self->seconds = SceneInfo->seconds;

            self->timer--;
            if (!self->timer) {
                Zone->gotTimeOver = true;
                for (int32 p = 0; p < Player->playerCount; ++p) {
                    if (!self->playerFinished[p]) {
                        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
                        player->deathType    = PLAYER_DEATH_DIE_USESFX;
                        self->state          = StateMachine_None;
                    }
                }
            }
        }
    }
#endif
}

#if MANIA_USE_PLUS
void Competition_ResetOptions(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    session->inMatch                  = false;
    session->matchID                  = 0;
    session->matchCount               = 0;
    session->stageIndex               = 0;

    for (int32 i = 0; i < COMPETITION_STAGE_COUNT; ++i) {
        session->completedStages[i] = false;
        session->matchWinner[i]     = 0;
    }

    for (int32 p = 0; p < PLAYER_COUNT; ++p) {
        session->finishState[p]       = FINISHTYPE_NOTFINISHED;
        session->playerID[p]          = ID_NONE;
        session->time[p].minutes      = 0;
        session->time[p].seconds      = 0;
        session->time[p].milliseconds = 0;
        session->rings[p]             = 0;
        session->score[p]             = 0;
        session->items[p]             = 0;
        session->totalRings[p]        = 0;
        session->wins[p]              = 0;
        session->lives[p]             = 0;
    }
}

void Competition_ClearMatchData(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    session->matchWinner[session->matchID] = 0;

    for (int32 p = 0; p < PLAYER_COUNT; ++p) {
        session->finishState[p]       = FINISHTYPE_NOTFINISHED;
        session->time[p].minutes      = 0;
        session->time[p].seconds      = 0;
        session->time[p].milliseconds = 0;
        session->rings[p]             = 0;
        session->score[p]             = 0;
        session->items[p]             = 0;
        session->totalRings[p]        = 0;
    }
}

void Competition_DeriveWinner(int32 playerID, uint8 finishType)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();
    session->finishState[playerID]    = finishType;

    if (finishType == FINISHTYPE_GAMEOVER) {
        session->totalRings[playerID]        = 0;
        session->items[playerID]             = 0;
        session->rings[playerID]             = 0;
        session->score[playerID]             = 0;
        session->time[playerID].minutes      = 9;
        session->time[playerID].seconds      = 59;
        session->time[playerID].milliseconds = 59;
    }

    int32 deathCount          = 0;
    bool32 allPlayersFinished = true;
    for (int32 p = 0; p < session->playerCount; ++p) {
        if (!session->lives[p] || session->finishState[p] == FINISHTYPE_GAMEOVER)
            ++deathCount;
    }

    if (deathCount < session->playerCount - 1) {
        for (int32 p = 0; p < session->playerCount; ++p) allPlayersFinished = allPlayersFinished && (!session->lives[p] || session->finishState[p]);
    }

    if (allPlayersFinished) {
        if (!session->matchWinner[session->matchID]) {
            uint32 winnerTime       = 0xFFFFFFFF;
            int32 winnerRings      = 0;
            int32 winnerScore      = 0;
            int32 winnerItems      = 0;
            int32 winnerTotalRings = 0;

            uint32 times[PLAYER_COUNT];
            for (int32 p = 0; p < session->playerCount; ++p) {
                int32 mins = session->time[p].minutes;
                int32 secs = session->time[p].seconds;
                int32 ms   = session->time[p].milliseconds;
                uint32 time = ms + 100 * (secs + 60 * mins);
                times[p]   = time;

                if (session->rings[p] > winnerRings)
                    winnerRings = session->rings[p];

                if (session->totalRings[p] > winnerTotalRings)
                    winnerTotalRings = session->totalRings[p];

                if (session->score[p] > winnerScore)
                    winnerScore = session->score[p];

                if (session->items[p] > winnerItems)
                    winnerItems = session->items[p];

                if (time < winnerTime)
                    winnerTime = time;
            }

            int32 scores[PLAYER_COUNT];
            memset(scores, 0, PLAYER_COUNT * sizeof(int32));

            int32 winner = 0;
            for (int32 p = 0; p < session->playerCount; ++p) {
                if (session->finishState[p] == FINISHTYPE_PASSEDSIGNPOST) {
                    int32 score = 0;
                    if (session->rings[p] == winnerRings)
                        score++;

                    if (session->totalRings[p] == winnerTotalRings)
                        ++score;

                    if (session->score[p] == winnerScore)
                        ++score;

                    if (times[p] == winnerTime)
                        ++score;

                    if (session->items[p] == winnerItems)
                        ++score;

                    LogHelpers_Print("player %d => score %d", p, score);

                    scores[p] = score;
                    if (score > winner)
                        winner = score;
                }
            }

            for (int32 p = 0; p < session->playerCount; ++p) {
                bool32 isWinner = session->lives[p] > 0 && session->finishState[p] != FINISHTYPE_GAMEOVER && scores[p] == winner;
                if (isWinner) {
                    Competition_WinMatchFor(p);
                }
            }
        }
    }
}

void Competition_WinMatchFor(int32 playerID)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    ++session->wins[playerID];
    session->matchWinner[session->matchID] |= 1 << playerID;
}
#endif

#if RETRO_INCLUDE_EDITOR
void Competition_EditorDraw(void) {}

void Competition_EditorLoad(void) {}
#endif

void Competition_Serialize(void) {}

// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CompetitionSession Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCompetitionSession *CompetitionSession = NULL;

void CompetitionSession_Update(void) {}

void CompetitionSession_LateUpdate(void) {}

void CompetitionSession_StaticUpdate(void) {}

void CompetitionSession_Draw(void) {}

void CompetitionSession_Create(void *data) {}

void CompetitionSession_StageLoad(void) {}

EntityCompetitionSession *CompetitionSession_GetSession(void) { return (EntityCompetitionSession *)globals->competitionSession; }

#if !MANIA_USE_PLUS
void CompetitionSession_ResetOptions(void)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    session->inMatch    = false;
    session->matchID    = 0;
    session->matchCount = 0;
    session->stageIndex = 0;

    for (int32 r = 0; r < 12; ++r) {
        session->completedStages[r] = 0;
        session->matchWinner[r]     = 0;
    }

    for (int32 p = 0; p < 4; ++p) {
        session->finishState[p]       = FINISHTYPE_NOTFINISHED;
        session->playerID[p]          = 0;
        session->time[p].minutes      = 0;
        session->time[p].seconds      = 0;
        session->time[p].milliseconds = 0;
        session->rings[p]             = 0;
        session->score[p]             = 0;
        session->items[p]             = 0;
        session->totalRings[p]        = 0;
        session->wins[p]              = 0;
        session->lives[p]             = 3;
    }
}

void CompetitionSession_ClearMatchData(void)
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

void CompetitionSession_DeriveWinner(int32 playerID, int32 finishType)
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
        for (int32 p = 0; p < session->playerCount; ++p) {
            allPlayersFinished = allPlayersFinished && (!session->lives[p] || session->finishState[p]);
        }
    }

    if (allPlayersFinished) {
        if (!session->matchWinner[session->matchID]) {
            uint32 winnerTime       = 0xFFFFFFFF;
            uint32 winnerRings      = 0;
            uint32 winnerScore      = 0;
            uint32 winnerItems      = 0;
            uint32 winnerTotalRings = 0;

            int32 times[PLAYER_COUNT];
            for (int32 p = 0; p < session->playerCount; ++p) {
                int32 mins = session->time[p].minutes;
                int32 secs = session->time[p].seconds;
                int32 ms   = session->time[p].milliseconds;
                times[p]   = ms + 100 * (secs + 60 * mins);

                if (session->rings[p] > winnerRings)
                    winnerRings = session->rings[p];

                if (session->totalRings[p] > winnerTotalRings)
                    winnerTotalRings = session->totalRings[p];

                if (session->score[p] > winnerScore)
                    winnerScore = session->score[p];

                if (session->items[p] > winnerItems)
                    winnerItems = session->items[p];

                if (times[p] < winnerTime)
                    winnerTime = times[p];
            }

            int32 scores[PLAYER_COUNT];
            memset(scores, 0, 4 * sizeof(int32));
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

            session->matchWinner[session->matchID] = 0;
            for (int32 p = 0; p < session->playerCount; ++p) {
                bool32 isWinner = session->lives[p] > 0 && session->finishState[p] != FINISHTYPE_GAMEOVER && scores[p] == winner;
                if (isWinner) {
                    LogHelpers_Print("DeriveWinner() -> %d", p);
                    CompetitionSession_WinMatchFor(p);
                }
            }
        }
    }
    else {
        LogHelpers_Print("DeriveWinner() -> %d", -1);
    }
}

void CompetitionSession_WinMatchFor(int32 player)
{
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    LogHelpers_Print("CompetitionSession_WinMatchFor(%d)", player);
    ++session->wins[player];
    session->matchWinner[session->matchID] |= (1 << player);
}
#endif

#if GAME_INCLUDE_EDITOR
void CompetitionSession_EditorDraw(void) {}

void CompetitionSession_EditorLoad(void) {}
#endif

void CompetitionSession_Serialize(void) {}

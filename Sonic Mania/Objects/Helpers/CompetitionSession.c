#include "SonicMania.h"

ObjectCompetitionSession *CompetitionSession = NULL;

void CompetitionSession_Update(void)
{

}

void CompetitionSession_LateUpdate(void)
{

}

void CompetitionSession_StaticUpdate(void)
{

}

void CompetitionSession_Draw(void)
{

}

void CompetitionSession_Create(void* data)
{

}

void CompetitionSession_StageLoad(void)
{

}

#if !RETRO_USE_PLUS
void CompetitionSession_ResetOptions(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    session->inMatch                  = false;
    session->matchID                  = 0;
    session->matchCount               = 0;
    session->levelIndex               = 0;

    for (int i = 0; i < 12; ++i) {
        session->zoneFlags[i]   = 0;
        session->winnerFlags[i] = 0;
    }

    for (int i = 0; i < 4; ++i) {
        session->finishFlags[i]       = 0;
        session->characterFlags[i]    = 0;
        session->time[i].minutes      = 0;
        session->time[i].seconds      = 0;
        session->time[i].milliseconds = 0;
        session->rings[i]             = 0;
        session->score[i]             = 0;
        session->items[i]             = 0;
        session->totalRings[i]        = 0;
        session->wins[i]              = 0;
        session->lives[i]             = 0;
    }
}

void CompetitionSession_ClearMatchData(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    session->winnerFlags[session->matchID] = 0;
    for (int i = 0; i < 4; ++i) {
        session->finishFlags[i]       = 0;
        session->time[i].minutes      = 0;
        session->time[i].seconds      = 0;
        session->time[i].milliseconds = 0;
        session->rings[i]             = 0;
        session->score[i]             = 0;
        session->items[i]             = 0;
        session->totalRings[i]        = 0;
    }
}

void CompetitionSession_DeriveWinner(int playerID, int flags)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    session->finishFlags[playerID]    = flags;

    if (flags == 1) {
        session->totalRings[playerID]        = 0;
        session->items[playerID]             = 0;
        session->rings[playerID]             = 0;
        session->score[playerID]             = 0;
        session->time[playerID].minutes      = 9;
        session->time[playerID].seconds      = 59;
        session->time[playerID].milliseconds = 59;
    }

    int deathCount = 0;
    bool32 flag    = true;
    for (int p = 0; p < session->playerCount; ++p) {
        if (!session->lives[p] || session->finishFlags[p] == 1)
            ++deathCount;
    }

    if (deathCount < session->playerCount - 1) {
        for (int p = 0; p < session->playerCount; ++p) {
            flag = flag && (!session->lives[p] || session->finishFlags[p] == 1);
        }
    }

    if (flag) {
        if (!session->finishFlags[session->matchID]) {
            int winnerTime       = 0;
            int winnerRings      = 0;
            int winnerScore      = 0;
            int winnerItems      = 0;
            int winnerTotalRings = 0;

            int times[4];
            for (int p = 0; p < session->playerCount; ++p) {
                int mins = session->time[p].minutes;
                int secs = session->time[p].seconds;
                int ms   = session->time[p].milliseconds;
                int time = ms + 100 * (secs + 60 * mins);
                times[p] = time;

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

            int scores[4];
            memset(scores, 0, 4 * sizeof(int));
            int winner = 0;
            for (int p = 0; p < session->playerCount; ++p) {
                if (session->finishFlags[p] == 2) {
                    int score = 0;
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

            session->winnerFlags[session->matchID] = 0;
            for (int p = 0; p < session->playerCount; ++p) {
                bool32 flag = session->wins[p] > 0 && session->finishFlags[p] != 1 && scores[p] == winner;
                if (flag) {
                    ++session->wins[p];
                    session->winnerFlags[session->matchID] |= (1 << p);
                    LogHelpers_Print("DeriveWinner() -> %d", p);
                    CompetitionSession_WinMatchFor(p);
                }
            }
        }
    }
    else  {
        LogHelpers_Print("DeriveWinner() -> %d", -1);
    }
}
void CompetitionSession_WinMatchFor(int player)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    LogHelpers_Print("CompetitionSession_WinMatchFor(%d)", player);
    ++session->wins[player];
    session->winnerFlags[session->matchID] |= (1 << player);
}
#endif


void CompetitionSession_EditorDraw(void)
{

}

void CompetitionSession_EditorLoad(void)
{

}

void CompetitionSession_Serialize(void)
{

}

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectCompetition *Competition;

void Competition_Update(void)
{
    RSDK_THIS(Competition);
    StateMachine_Run(entity->state);
}

void Competition_LateUpdate(void) {}

void Competition_StaticUpdate(void) {}

void Competition_Draw(void)
{
    RSDK_THIS(Competition);
    if (RSDK_GET_ENTITY(RSDK_sceneInfo->currentScreenID, Player)->objectID == Player->objectID) {
        if (!entity->playerFlags[RSDK_sceneInfo->currentScreenID]) {
            Vector2 drawPos;
            drawPos.y                = 0x1A0000;
            drawPos.x                = (RSDK_screens[RSDK_sceneInfo->currentScreenID].centerX - 4) << 16;
            entity->animator.frameID = entity->timer / 10;
            RSDK.DrawSprite(&entity->animator, &drawPos, true);
            drawPos.x += 0x80000;
            entity->animator.frameID = entity->timer % 10;
            RSDK.DrawSprite(&entity->animator, &drawPos, true);
        }
    }
}

void Competition_Create(void *data)
{
    RSDK_THIS(Competition);
    if (!RSDK_sceneInfo->inEditor) {
        entity->isPermanent = true;
        entity->active      = ACTIVE_NORMAL;
        entity->visible     = 1;
        entity->drawOrder   = Zone->hudDrawOrder - 1;
        entity->seconds     = RSDK_sceneInfo->seconds;
        if (RSDK_sceneInfo->minutes == 9) {
            entity->timer = 0;
            if (60 - RSDK_sceneInfo->seconds > 0)
                entity->timer = 60 - RSDK_sceneInfo->seconds;
            if (!RSDK_sceneInfo->milliseconds)
                RSDK_sceneInfo->milliseconds = 1;
        }
        else {
            entity->timer = 60;
        }
        entity->state = Competition_State_Manager;
        RSDK.SetSpriteAnimation(Competition->aniFrames, 1, &entity->animator, true, 0);
    }
}

void Competition_StageLoad(void)
{
    Competition->activeEntity = NULL;
    Competition->aniFrames    = RSDK.LoadSpriteAnimation("Global/HUD.bin", SCOPE_STAGE);
}

void Competition_State_Manager(void)
{
    RSDK_THIS(Competition);

    if (entity->timer <= 0) {
        Zone->field_15C             = true;
        RSDK_sceneInfo->timeEnabled = false;
        for (int32 p = 0; p < Player->playerCount; ++p) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
            if (player->objectID == Player->objectID && player->state == Player_State_Die) {
                player->visible = true;
            }
        }
        entity->state = StateMachine_None;
    }
    else {
        if (entity->seconds != RSDK_sceneInfo->seconds) {
            entity->seconds = RSDK_sceneInfo->seconds;
            entity->timer--;
            if (entity->timer == 12) {
                Music_PlayMusicTrack(TRACK_DROWNING);
            }
            else if (!entity->timer) {
                Zone->field_15C             = true;
                RSDK_sceneInfo->timeEnabled = false;
                for (int32 p = 0; p < Player->playerCount; ++p) {
                    if (!entity->playerFlags[p]) {
                        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
                        if (player->objectID == Player->objectID) {
                            player->hurtFlag = true;
                        }
                    }
                }
            }
        }
    }
}

void Competition_ResetOptions(void)
{
    EntityCompetitionSession *session          = (EntityCompetitionSession *)globals->competitionSession;
    session->inMatch    = false;
    session->matchID    = 0;
    session->matchCount = 0;
    session->levelIndex = 0;

    for (int32 i = 0; i < 12; ++i) {
        session->zoneFlags[i]   = 0;
        session->winnerFlags[i] = 0;
    }

    for (int32 i = 0; i < 4; ++i) {
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

void Competition_ClearMatchData(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    session->winnerFlags[session->matchID] = 0;
    for (int32 i = 0; i < 4; ++i) {
        session->finishFlags[i]                = 0;
        session->time[i].minutes               = 0;
        session->time[i].seconds               = 0;
        session->time[i].milliseconds          = 0;
        session->rings[i]                      = 0;
        session->score[i]                      = 0;
        session->items[i]                      = 0;
        session->totalRings[i]                 = 0;
    }
}

void Competition_CalculateScore(int32 playerID, uint8 flags)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    session->finishFlags[playerID] = flags;

    if (flags == 1) {
        session->totalRings[playerID]                 = 0;
        session->items[playerID]                      = 0;
        session->rings[playerID]                      = 0;
        session->score[playerID]                      = 0;
        session->time[playerID].minutes               = 9;
        session->time[playerID].seconds               = 59;
        session->time[playerID].milliseconds          = 59;
    }

    int32 deathCount = 0;
    bool32 flag    = true;
    for (int32 p = 0; p < session->playerCount; ++p) {
        if (!session->lives[p] || session->finishFlags[p] == 1)
            ++deathCount;
    }

    if (deathCount < session->playerCount - 1) {
        for (int32 p = 0; p < session->playerCount; ++p) {
            flag = flag && (!session->lives[p] || session->finishFlags[p] == 1);
        }
    }

    if (flag) {
        if (!session->finishFlags[session->matchID]) {
            int32 winnerTime       = 0;
            int32 winnerRings      = 0;
            int32 winnerScore      = 0;
            int32 winnerItems      = 0;
            int32 winnerTotalRings = 0;

            int32 times[4];
            for (int32 p = 0; p < session->playerCount; ++p) {
                int32 mins = session->time[p].minutes;
                int32 secs = session->time[p].seconds;
                int32 ms   = session->time[p].milliseconds;
                int32 time = ms + 100 * (secs + 60 * mins);
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

            int32 scores[4];
            memset(scores, 0, 4 * sizeof(int32));
            int32 winner = 0;
            for (int32 p = 0; p < session->playerCount; ++p) {
                if (session->finishFlags[p] == 2) {
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
                bool32 flag = session->wins[p] > 0 && session->finishFlags[p] != 1 && scores[p] == winner;
                if (flag) {
                    ++session->wins[p];
                    session->winnerFlags[session->matchID] |= (1 << p);
                }
            }
        }
    }
}

void Competition_EditorDraw(void) {}

void Competition_EditorLoad(void) {}

void Competition_Serialize(void) {}
#endif

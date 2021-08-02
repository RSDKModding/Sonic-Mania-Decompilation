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
        for (int p = 0; p < Player->playerCount; ++p) {
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
                for (int p = 0; p < Player->playerCount; ++p) {
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
    globals->competitionSession[CS_inMatch]    = 0;
    globals->competitionSession[CS_MatchID]    = 0;
    globals->competitionSession[CS_MatchCount] = 0;
    globals->competitionSession[CS_LevelIndex] = 0;

    byte *zoneFlags   = (byte *)&globals->competitionSession[CS_ZoneUnknown30];
    byte *winnerFlags = (byte *)&globals->competitionSession[CS_WinnerFlag1];
    for (int i = 0; i < 12; ++i) {
        zoneFlags[i]   = 0;
        winnerFlags[i] = 0;
    }

    byte *finishFlags = (byte *)&globals->competitionSession[CS_FinishFlags];
    byte *charFlags   = (byte *)&globals->competitionSession[CS_CharacterFlags];

    for (int i = 0; i < 4; ++i) {
        finishFlags[i]                                               = 0;
        charFlags[i]                                                 = 0;
        globals->competitionSession[CS_TimeMinutesP1 + (i * 3)]      = 0;
        globals->competitionSession[CS_TimeSecondsP1 + (i * 3)]      = 0;
        globals->competitionSession[CS_TimeMillisecondsP1 + (i * 3)] = 0;
        globals->competitionSession[CS_RingsP1 + i]                  = 0;
        globals->competitionSession[CS_ScoreP1 + i]                  = 0;
        globals->competitionSession[CS_ItemsP1 + i]                  = 0;
        globals->competitionSession[CS_TotalRingsP1 + i]             = 0;
        globals->competitionSession[CS_WinsP1 + i]                   = 0;
        globals->competitionSession[CS_LivesP1 + i]                  = 0;
    }
}

void Competition_ClearMatchData(void)
{
    byte *winnerFlags = (byte *)&globals->competitionSession[CS_WinnerFlag1];
    byte *finishFlags = (byte *)&globals->competitionSession[CS_FinishFlags];

    winnerFlags[globals->competitionSession[CS_MatchID]] = 0;
    for (int i = 0; i < 4; ++i) {
        finishFlags[i]                                               = 0;
        globals->competitionSession[CS_TimeMinutesP1 + (i * 3)]      = 0;
        globals->competitionSession[CS_TimeSecondsP1 + (i * 3)]      = 0;
        globals->competitionSession[CS_TimeMillisecondsP1 + (i * 3)] = 0;
        globals->competitionSession[CS_RingsP1 + i]                  = 0;
        globals->competitionSession[CS_ScoreP1 + i]                  = 0;
        globals->competitionSession[CS_ItemsP1 + i]                  = 0;
        globals->competitionSession[CS_TotalRingsP1 + i]             = 0;
    }
}

void Competition_CalculateScore(int playerID, byte flags)
{
    byte *finishFlags     = (byte *)&globals->competitionSession[CS_FinishFlags];
    byte *winnerFlags     = (byte *)&globals->competitionSession[CS_WinnerFlag1];
    finishFlags[playerID] = flags;

    if (flags == 1) {
        globals->competitionSession[playerID + CS_TotalRingsP1]             = 0;
        globals->competitionSession[playerID + CS_ItemsP1]                  = 0;
        globals->competitionSession[playerID + CS_RingsP1]                  = 0;
        globals->competitionSession[playerID + CS_ScoreP1]                  = 0;
        globals->competitionSession[CS_TimeMinutesP1 + (3 * playerID)]      = 9;
        globals->competitionSession[CS_TimeSecondsP1 + (3 * playerID)]      = 59;
        globals->competitionSession[CS_TimeMillisecondsP1 + (3 * playerID)] = 99;
    }

    int deathCount = 0;
    bool32 flag    = true;
    for (int p = 0; p < globals->competitionSession[CS_PlayerCount]; ++p) {
        if (!globals->competitionSession[CS_LivesP1 + p] || finishFlags[p] == 1)
            ++deathCount;
    }

    if (deathCount < globals->competitionSession[CS_PlayerCount] - 1) {
        for (int p = 0; p < globals->competitionSession[CS_PlayerCount]; ++p) {
            flag = flag && (!globals->competitionSession[CS_LivesP1 + p] || finishFlags[p] == 1);
        }
    }

    if (flag) {
        if (!finishFlags[globals->competitionSession[CS_MatchID]]) {
            int winnerTime       = 0;
            int winnerRings      = 0;
            int winnerScore      = 0;
            int winnerItems      = 0;
            int winnerTotalRings = 0;

            int times[4];
            for (int p = 0; p < globals->competitionSession[CS_PlayerCount]; ++p) {
                int mins = 0;
                int secs = 0;
                int ms   = 0;
                int time = ms + 100 * (secs + 60 * mins);
                times[p] = time;

                if (globals->competitionSession[CS_RingsP1 + p] > winnerRings)
                    winnerRings = globals->competitionSession[CS_RingsP1 + p];
                if (globals->competitionSession[CS_TotalRingsP1 + p] > winnerTotalRings)
                    winnerTotalRings = globals->competitionSession[CS_TotalRingsP1 + p];
                if (globals->competitionSession[CS_ScoreP1 + p] > winnerScore)
                    winnerScore = globals->competitionSession[CS_ScoreP1 + p];
                if (globals->competitionSession[CS_ItemsP1 + p] > winnerItems)
                    winnerItems = globals->competitionSession[CS_ItemsP1 + p];
                if (time < winnerTime)
                    winnerTime = time;
            }

            int scores[4];
            int winner = 0;
            for (int p = 0; p < globals->competitionSession[CS_PlayerCount]; ++p) {
                if (finishFlags[p] == 2) {
                    int score = 0;
                    if (globals->competitionSession[CS_RingsP1 + p] == winnerRings)
                        score++;
                    if (globals->competitionSession[CS_TotalRingsP1 + p] == winnerTotalRings)
                        ++score;
                    if (globals->competitionSession[CS_ScoreP1 + p] == winnerScore)
                        ++score;
                    if (times[p] == winnerTime)
                        ++score;
                    if (globals->competitionSession[CS_ItemsP1 + p] == winnerItems)
                        ++score;
                    LogHelpers_Print("player %d => score %d", p, score);
                    scores[p] = score;
                    if (score > winner)
                        winner = score;
                }
            }

            for (int p = 0; p < globals->competitionSession[CS_PlayerCount]; ++p) {
                bool32 flag = globals->competitionSession[CS_LivesP1 + p] > 0 && finishFlags[p] != 1 && scores[p] == winner;
                if (flag) {
                    ++globals->competitionSession[CS_LivesP1 + p];
                    winnerFlags[globals->competitionSession[CS_MatchID]] |= (1 << p);
                }
            }
        }
    }
}

void Competition_EditorDraw(void) {}

void Competition_EditorLoad(void) {}

void Competition_Serialize(void) {}
#endif

#ifndef OBJ_COMPETITIONSESSION_H
#define OBJ_COMPETITIONSESSION_H

#include "SonicMania.h"

#if !RETRO_USE_PLUS
//preplus was always 2P
#define competition_PlayerCount (2)
#endif

//created so I can easily set up times in an array
typedef struct {
    int32 minutes;
    int32 seconds;
    int32 milliseconds;
} vsTime;

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCompetitionSession;

// Entity Class
typedef struct {
	RSDK_ENTITY
    bool32 inMatch;
    int32 playerCount;
    int32 levelIndex;
    int32 zoneID;
    int32 actID;
    int32 matchID;
    int32 matchCount;
    int32 monitorMode;
#if RETRO_USE_PLUS
    int32 unknown29;
#endif
    int32 zoneFlags[12];
    uint8 characterFlags[PLAYER_MAX];
    uint8 winnerFlags[12];
    int32 rings[PLAYER_MAX];
    int32 score[PLAYER_MAX];
    int32 items[PLAYER_MAX];
    vsTime time[PLAYER_MAX];
    uint8 finishFlags[PLAYER_MAX];
    int32 totalRings[PLAYER_MAX];
    int32 wins[PLAYER_MAX];
    int32 lives[PLAYER_MAX];
#if RETRO_USE_PLUS
    int32 unknown84;
    int32 unknown85;
    int32 splitScreenMode;
    int32 unknown87;
    bool32 readOnlyDisplayMode;
    int32 controllerIDs[PLAYER_MAX];
    int32 prevMatchID;
#endif
} EntityCompetitionSession;

// Object Entity
extern ObjectCompetitionSession *CompetitionSession;

// Standard Entity Events
void CompetitionSession_Update(void);
void CompetitionSession_LateUpdate(void);
void CompetitionSession_StaticUpdate(void);
void CompetitionSession_Draw(void);
void CompetitionSession_Create(void* data);
void CompetitionSession_StageLoad(void);
void CompetitionSession_EditorDraw(void);
void CompetitionSession_EditorLoad(void);
void CompetitionSession_Serialize(void);

// Extra Entity Functions
#if !RETRO_USE_PLUS
void CompetitionSession_ResetOptions(void);
void CompetitionSession_ClearMatchData(void);
void CompetitionSession_DeriveWinner(int32 playerID, int32 flags);
void CompetitionSession_WinMatchFor(int32 player);
#endif


#endif //!OBJ_COMPETITIONSESSION_H

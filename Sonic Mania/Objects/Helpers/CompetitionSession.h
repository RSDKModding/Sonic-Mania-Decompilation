#ifndef OBJ_COMPETITIONSESSION_H
#define OBJ_COMPETITIONSESSION_H

#include "SonicMania.h"

#if !RETRO_USE_PLUS
//preplus was always 2P
#define competition_PlayerCount (2)
#endif

//created so I can easily set up times in an array
typedef struct {
    int minutes;
    int seconds;
    int milliseconds;
} vsTime;

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCompetitionSession;

// Entity Class
typedef struct {
	RSDK_ENTITY
    bool32 inMatch;
    int playerCount;
    int levelIndex;
    int zoneID;
    int actID;
    int matchID;
    int matchCount;
    int monitorMode;
#if RETRO_USE_PLUS
    int unknown29;
#endif
    int zoneFlags[12];
    uint8 characterFlags[PLAYER_MAX];
    uint8 winnerFlags[12];
    int rings[PLAYER_MAX];
    int score[PLAYER_MAX];
    int items[PLAYER_MAX];
    vsTime time[PLAYER_MAX];
    uint8 finishFlags[PLAYER_MAX];
    int totalRings[PLAYER_MAX];
    int wins[PLAYER_MAX];
    int lives[PLAYER_MAX];
#if RETRO_USE_PLUS
    int unknown84;
    int unknown85;
    int splitScreenMode;
    int unknown87;
    bool32 readOnlyDisplayMode;
    int controllerIDs[PLAYER_MAX];
    int prevMatchID;
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
void CompetitionSession_DeriveWinner(int playerID, int flags);
void CompetitionSession_WinMatchFor(int player);
#endif


#endif //!OBJ_COMPETITIONSESSION_H

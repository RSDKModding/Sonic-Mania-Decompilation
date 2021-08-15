#ifndef OBJ_COMPETITIONSESSION_H
#define OBJ_COMPETITIONSESSION_H

#include "SonicMania.h"

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
    int unknown29;
    int zoneFlags[12];
    byte characterFlags[PLAYER_MAX];
    byte winnerFlags[12];
    int rings[PLAYER_MAX];
    int score[PLAYER_MAX];
    int items[PLAYER_MAX];
    vsTime time[PLAYER_MAX];
    byte finishFlags[PLAYER_MAX];
    int totalRings[PLAYER_MAX];
    int wins[PLAYER_MAX];
    int lives[PLAYER_MAX];
    int unknown84;
    int unknown85;
    int splitScreenMode;
    int unknown87;
    bool32 readOnlyDisplayMode;
    int controllerIDs[PLAYER_MAX];
    int prevMatchID;
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


#endif //!OBJ_COMPETITIONSESSION_H

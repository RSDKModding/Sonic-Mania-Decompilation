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
    byte characterFlags[4];
    byte winnerFlags[12];
    int rings[4];
    int score[4];
    int items[4];
    int timeMinutesP1;
    int timeSecondsP1;
    vsTime time[4];
    byte finishFlags[4];
    int totalRings[4];
    int wins[4];
    int lives[4];
    int unknown84;
    int unknown85;
    int unknown86;
    int unknown87;
    bool32 readOnlyDisplayMode;
    int unknown89;
    int unknown90;
    int unknown91;
    int unknown92;
    int unknown93;
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

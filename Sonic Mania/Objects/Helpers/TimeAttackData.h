#ifndef OBJ_TIMEATTACKDATA_H
#define OBJ_TIMEATTACKDATA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    int status;
    uint8 zoneID;
    uint8 act;
    uint8 characterID;
    bool32 encore;
    int uuid;
    int rowID;
    int dbRank;
    int rank;
    int dword1C;
    Entity *loadEntityPtr;
    void (*loadCallback)(int);
    Entity* saveEntityPtr;
    void (*saveCallback)(int);
} ObjectTimeAttackData;

#if !RETRO_USE_PLUS
// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityTimeAttackData;
#endif

// Object Entity
extern ObjectTimeAttackData *TimeAttackData;

// Standard Entity Events
void TimeAttackData_Update(void);
void TimeAttackData_LateUpdate(void);
void TimeAttackData_StaticUpdate(void);
void TimeAttackData_Draw(void);
void TimeAttackData_Create(void *data);
void TimeAttackData_StageLoad(void);
void TimeAttackData_EditorDraw(void);
void TimeAttackData_EditorLoad(void);
void TimeAttackData_Serialize(void);

//Extra Entity FUnctions
void TimeAttackData_TrackActClear(uint8 act, uint8 zone, StatInfo *stat, uint8 charID, int time, int rings, int score);
void TimeAttackData_TrackTAClear(uint8 actID, uint8 zone, StatInfo *stat, uint8 charID, int gameMode, int time);
void TimeAttackData_TrackEnemyDefeat(uint8 actID, uint8 zoneID, StatInfo *stat, uint8 charID, bool32 encore, int x, int y);
void TimeAttackData_ClearOptions(void);
int TimeAttackData_GetManiaListPos(int zoneID, int playerID, int act);
int TimeAttackData_GetEncoreListPos(int zoneID, int playerID, int act);
void TimeAttackData_GetTimeFromValue(int time, int *minsPtr, int *secsPtr, int *millisecsPtr);
#if RETRO_USE_PLUS
int TimeAttackData_LoadCB(int statusCode);
void TimeAttackData_ResetTimeAttackDB(void);
void TimeAttackData_MigrateLegacyTADB(void);
int TimeAttackData_AddTimeAttackDBEntry(char zone, char charID, int act, char mode, int time);
int TimeAttackData_AddTADBEntry(char zone, char charID, int act, int mode, int time, void (*callback)(int));
int TimeAttackData_SaveTimeAttackDB(void (*callback)(int));
void TimeAttackData_SaveTimeAttackDB_CB(int statusCode);
int TimeAttackData_GetScore(uint8 zone, uint8 charID, uint8 act, int encore, int val);
int TimeAttackData_GetReplayID(uint8 zone, uint8 charID, uint8 act, int encore, int val);
void TimeAttackData_ConfigureTableView(uint8 zoneID, uint8 characterID, uint8 act, int encore);

void TimeAttackData_GetLeaderboardRank_CB(int status, int rank);
void TimeAttackData_AddLeaderboardEntry(uint8 zone, char playerID, uint8 act, int mode, int time);
#else
void TimeAttackData_SaveTATime(uint8 zone, uint8 act, uint8 player, uint8 rank, uint16 time);
uint16 *TimeAttackData_GetRecordedTime(uint8 zone, uint8 act, uint8 player, uint8 rank);
#endif

#endif //!OBJ_TIMEATTACKDATA_H

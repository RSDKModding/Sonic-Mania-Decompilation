#ifndef OBJ_TIMEATTACKDATA_H
#define OBJ_TIMEATTACKDATA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    int status;
    byte zoneID;
    byte act;
    byte characterID;
    byte field_7;
    bool32 encore;
    int uuid;
    int unknown;
    int field_14;
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
void TimeAttackData_TrackActClear(byte act, byte zone, StatInfo *stat, byte charID, int time, int rings, int score);
void TimeAttackData_TrackTAClear(byte actID, byte zone, StatInfo *stat, byte charID, int gameMode, int time);
void TimeAttackData_TrackEnemyDefeat(byte actID, byte zoneID, StatInfo *stat, byte charID, bool32 encore, int x, int y);
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
int TimeAttackData_SaveTimeAttackDB_CB(int statusCode);
int TimeAttackData_SetScore(byte zone, byte charID, byte act, int encore, int dst);
void TimeAttackData_ConfigureTableView(byte zoneID, byte characterID, byte act, int encore);

void TimeAttackData_GetLeaderboardRank_CB(int status, int rank);
void TimeAttackData_AddLeaderboardEntry(byte zone, char playerID, byte act, int mode, int time);
#endif

#endif //!OBJ_TIMEATTACKDATA_H

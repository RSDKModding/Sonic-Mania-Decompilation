#ifndef OBJ_TIMEATTACKDATA_H
#define OBJ_TIMEATTACKDATA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    int32 status;
    uint8 zoneID;
    uint8 act;
    uint8 characterID;
    bool32 encore;
    int32 uuid;
    int32 rowID;
    int32 dbRank;
    int32 rank;
    int32 dword1C;
    Entity *loadEntityPtr;
    void (*loadCallback)(int32);
    Entity* saveEntityPtr;
    void (*saveCallback)(int32);
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
#if RETRO_INCLUDE_EDITOR
void TimeAttackData_EditorDraw(void);
void TimeAttackData_EditorLoad(void);
#endif
void TimeAttackData_Serialize(void);

//Extra Entity FUnctions
void TimeAttackData_TrackActClear(uint8 act, uint8 zone, StatInfo *stat, uint8 charID, int32 time, int32 rings, int32 score);
void TimeAttackData_TrackTAClear(uint8 actID, uint8 zone, StatInfo *stat, uint8 charID, int32 gameMode, int32 time);
void TimeAttackData_TrackEnemyDefeat(uint8 actID, uint8 zoneID, StatInfo *stat, uint8 charID, bool32 encore, int32 x, int32 y);
void TimeAttackData_ClearOptions(void);
int32 TimeAttackData_GetManiaListPos(int32 zoneID, int32 playerID, int32 act);
int32 TimeAttackData_GetEncoreListPos(int32 zoneID, int32 playerID, int32 act);
void TimeAttackData_GetTimeFromValue(int32 time, int32 *minsPtr, int32 *secsPtr, int32 *millisecsPtr);
#if RETRO_USE_PLUS
int32 TimeAttackData_LoadCB(int32 statusCode);
void TimeAttackData_ResetTimeAttackDB(void);
void TimeAttackData_MigrateLegacyTADB(void);
int32 TimeAttackData_AddTimeAttackDBEntry(char zone, char charID, int32 act, char mode, int32 time);
int32 TimeAttackData_AddTADBEntry(char zone, char charID, int32 act, int32 mode, int32 time, void (*callback)(int32));
int32 TimeAttackData_SaveTimeAttackDB(void (*callback)(int32));
void TimeAttackData_SaveTimeAttackDB_CB(int32 statusCode);
int32 TimeAttackData_GetScore(uint8 zone, uint8 charID, uint8 act, int32 encore, int32 val);
int32 TimeAttackData_GetReplayID(uint8 zone, uint8 charID, uint8 act, int32 encore, int32 val);
void TimeAttackData_ConfigureTableView(uint8 zoneID, uint8 characterID, uint8 act, int32 encore);

void TimeAttackData_GetLeaderboardRank_CB(int32 status, int32 rank);
void TimeAttackData_AddLeaderboardEntry(uint8 zone, char playerID, uint8 act, int32 mode, int32 time);
#else
void TimeAttackData_SaveTATime(uint8 zone, uint8 act, uint8 player, uint8 rank, uint16 time);
uint16 *TimeAttackData_GetRecordedTime(uint8 zone, uint8 act, uint8 player, uint8 rank);
#endif

#endif //!OBJ_TIMEATTACKDATA_H

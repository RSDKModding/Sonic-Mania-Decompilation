#ifndef OBJ_TIMEATTACKDATA_H
#define OBJ_TIMEATTACKDATA_H

#include "SonicMania.h"

// Object Class
struct ObjectTimeAttackData {
#if !RETRO_USE_PLUS
    RSDK_OBJECT
#endif
    bool32 loaded;
    uint8 zoneID;
    uint8 act;
    uint8 characterID;
    bool32 encore;
    int32 uuid;
    int32 rowID;
    int32 personalRank;
    int32 leaderboardRank;
    bool32 isMigratingData;
    Entity *loadEntityPtr;
    void (*loadCallback)(bool32 success);
    Entity* saveEntityPtr;
    void (*saveCallback)(bool32 success);
};

#if !RETRO_USE_PLUS
// Entity Class
struct EntityTimeAttackData {
    RSDK_ENTITY
};
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
#if RETRO_USE_PLUS
void TimeAttackData_TrackActClear(StatInfo *stat, uint8 zone, uint8 act, uint8 charID, int32 time, int32 rings, int32 score);
void TimeAttackData_TrackTAClear(StatInfo *stat, uint8 zone, uint8 actID, uint8 charID, int32 gameMode, int32 time);
void TimeAttackData_TrackEnemyDefeat(StatInfo *stat, uint8 zoneID, uint8 actID, uint8 charID, bool32 encore, int32 x, int32 y);
#endif
void TimeAttackData_Clear(void);
int32 TimeAttackData_GetManiaListPos(int32 zoneID, int32 act, int32 characterID);
int32 TimeAttackData_GetEncoreListPos(int32 zoneID, int32 act, int32 characterID);
void TimeAttackData_GetTimeFromValue(int32 time, int32 *minsPtr, int32 *secsPtr, int32 *millisecsPtr);
#if RETRO_USE_PLUS
uint16 TimeAttackData_LoadTimeAttackDB(void (*callback)(bool32 success));
void TimeAttackData_LoadTimeAttackDB_CB(int32 statusCode);
void TimeAttackData_ResetTimeAttackDB(void);
void TimeAttackData_MigrateLegacySaves(void);
int32 TimeAttackData_AddTimeAttackDBEntry(uint8 zoneID, uint8 act, uint8 characterID, uint8 encore, int32 score);
int32 TimeAttackData_AddTADBEntry(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 score, void (*callback)(bool32 success));
void TimeAttackData_SaveTimeAttackDB(void (*callback)(bool32 success));
void TimeAttackData_SaveTimeAttackDB_CB(int32 statusCode);
int32 TimeAttackData_GetScore(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 rank);
int32 TimeAttackData_GetReplayID(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 rank);
void TimeAttackData_ConfigureTableView(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore);

void TimeAttackData_GetLeaderboardRank_CB(bool32 success, int32 rank);
void TimeAttackData_AddLeaderboardEntry(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 score);
#else
void TimeAttackData_SaveTATime(uint8 zoneID, uint8 act, uint8 characterID, uint8 rank, uint16 score);
uint16 *TimeAttackData_GetRecordedTime(uint8 zoneID, uint8 act, uint8 characterID, uint8 rank);
#endif

#endif //!OBJ_TIMEATTACKDATA_H

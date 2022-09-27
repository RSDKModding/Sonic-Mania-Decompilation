#ifndef OBJ_TIMEATTACKDATA_H
#define OBJ_TIMEATTACKDATA_H

#include "Game.h"

typedef enum {
    CHAR_SONIC_AND_TAILS,
    CHAR_SONIC,
    CHAR_TAILS,
    CHAR_KNUX,
#if MANIA_USE_PLUS
    CHAR_MIGHTY,
    CHAR_RAY,
#endif
} CharacterIDs;

typedef enum {
    ACT_1,
    ACT_2,
    ACT_3,
    ACT_NONE,
} ActIDs;

// Using a seperate TimeAttackRAM struct
// Normally (and officially) the ObjectGameProgress struct was used here
// but due to v5U updating the entity (and thus the TimeAttackRAM "spec")
// ObjectGameProgress is no longer easily compatible across versions
// so I gave it dummy data and will be using this struct to interact with TimeAttackRAM
// this one was also broken with plus since medals[] were aligned by 1 byte
// so when "filter" was added, all medals were offset by 1 without this fix
typedef struct {
    uint8 padding[0x56]; // aka sizeof(Entity) for pre-plus
    uint16 records[3][12][2][3];
} TimeAttackRAM;

// Object Class
struct ObjectTimeAttackData {
#if !MANIA_USE_PLUS
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
    Entity *saveEntityPtr;
    void (*saveCallback)(bool32 success);
};

#if !MANIA_USE_PLUS
// Entity Class
struct EntityTimeAttackData {
    RSDK_ENTITY
    // padding to match whatever it would be normally
    // not required, but its for safety :)
    uint8 padding[sizeof(TimeAttackRAM) - sizeof(Entity)];
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

// Extra Entity Functions
#if MANIA_USE_PLUS
void TimeAttackData_TrackActClear(StatInfo *stat, uint8 zone, uint8 act, uint8 charID, int32 time, int32 rings, int32 score);
void TimeAttackData_TrackTAClear(StatInfo *stat, uint8 zone, uint8 actID, uint8 charID, int32 gameMode, int32 time);
void TimeAttackData_TrackEnemyDefeat(StatInfo *stat, uint8 zoneID, uint8 actID, uint8 charID, bool32 encore, int32 x, int32 y);
#endif

TimeAttackRAM *TimeAttackData_GetTimeAttackRAM(void);
void TimeAttackData_Clear(void);
int32 TimeAttackData_GetManiaListPos(int32 zoneID, int32 act, int32 characterID);
#if MANIA_USE_PLUS
int32 TimeAttackData_GetEncoreListPos(int32 zoneID, int32 act, int32 characterID);
#endif
uint32 TimeAttackData_GetPackedTime(int32 minutes, int32 seconds, int32 milliseconds);
void TimeAttackData_GetUnpackedTime(int32 time, int32 *minutes, int32 *seconds, int32 *milliseconds);
uint16 *TimeAttackData_GetRecordedTime(uint8 zoneID, uint8 act, uint8 characterID, uint8 rank);

#if MANIA_USE_PLUS
void TimeAttackData_CreateDB(void);
uint16 TimeAttackData_LoadDB(void (*callback)(bool32 success));
void TimeAttackData_SaveDB(void (*callback)(bool32 success));
void TimeAttackData_LoadDBCallback(int32 status);
void TimeAttackData_SaveDBCallback(int32 status);
void TimeAttackData_MigrateLegacySaves(void);
int32 TimeAttackData_AddDBRow(uint8 zoneID, uint8 act, uint8 characterID, uint8 encore, int32 score);
int32 TimeAttackData_AddRecord(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 score, void (*callback)(bool32 success));
int32 TimeAttackData_GetScore(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 rank);
int32 TimeAttackData_GetReplayID(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 rank);
void TimeAttackData_ConfigureTableView(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore);

void TimeAttackData_Leaderboard_GetRank(bool32 success, int32 rank);
void TimeAttackData_AddLeaderboardEntry(uint8 zoneID, uint8 act, uint8 characterID, bool32 isEncore, int32 score);
LeaderboardID *TimeAttackData_GetLeaderboardInfo(uint8 zoneID, uint8 act, uint8 characterID, bool32 isEncore);
#else
void TimeAttackData_AddRecord(uint8 zoneID, uint8 act, uint8 characterID, uint8 rank, uint16 score);
#endif

#endif //! OBJ_TIMEATTACKDATA_H

#ifndef OBJ_TIMEATTACKDATA_H
#define OBJ_TIMEATTACKDATA_H

#include "../SonicMania.h"

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
void TimeAttackData_Update();
void TimeAttackData_LateUpdate();
void TimeAttackData_StaticUpdate();
void TimeAttackData_Draw();
void TimeAttackData_Create(void *data);
void TimeAttackData_StageLoad();
void TimeAttackData_EditorDraw();
void TimeAttackData_EditorLoad();
void TimeAttackData_Serialize();

//Extra Entity FUnctions
#if RETRO_USE_PLUS
int TimeAttackData_LoadCB(int statusCode);
void TimeAttackData_ResetTimeAttackDB();
void TimeAttackData_MigrateLegacyTADB();
int TimeAttackData_AddTimeAttackDBEntry(char zone, char charID, int act, char mode, int time);
int TimeAttackData_AddTADBEntry(char zone, char charID, int act, int mode, int time, void (*callback)(int));
int TimeAttackData_SaveTimeAttackDB(void (*callback)(int));
int TimeAttackData_SaveTimeAttackDB_CB(int statusCode);
int TimeAttackData_SetScore(byte zone, byte charID, byte act, int encore, int dst);
void TimeAttackData_ConfigureTableView(byte zoneID, byte characterID, byte act, int encore);
#endif

#endif //!OBJ_TIMEATTACKDATA_H

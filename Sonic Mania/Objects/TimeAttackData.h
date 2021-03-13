#ifndef OBJ_TIMEATTACKDATA_H
#define OBJ_TIMEATTACKDATA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    byte zoneID;
    byte act;
    byte characterID;
    byte field_7;
    bool32 encore;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int dword1C;
    Entity *loadEntityPtr;
    void (*loadCallback)(bool32);
    int saveEntityPtr;
    void (*saveCallback)(bool32);
} ObjectTimeAttackData;

#if !RETRO_USE_PLUS
// Entity Class
struct EntityTimeAttackData : Entity {
};
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


#endif //!OBJ_TIMEATTACKDATA_H

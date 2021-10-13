#ifndef OBJ_EGGLOCO_H
#define OBJ_EGGLOCO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxSmoke;
    uint16 sfxWhistle;
} ObjectEggLoco;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Animator animator8;
    Animator animator9;
    Animator animator10;
    Animator animator11;
    Animator animator12;
} EntityEggLoco;

// Object Struct
extern ObjectEggLoco *EggLoco;

// Standard Entity Events
void EggLoco_Update(void);
void EggLoco_LateUpdate(void);
void EggLoco_StaticUpdate(void);
void EggLoco_Draw(void);
void EggLoco_Create(void* data);
void EggLoco_StageLoad(void);
void EggLoco_EditorDraw(void);
void EggLoco_EditorLoad(void);
void EggLoco_Serialize(void);

// Extra Entity Functions
bool32 EggLoco_CheckCB(void);
void EggLoco_UpdateCB(int sfx);

void EggLoco_State_Unknown1(void);
void EggLoco_State_Unknown2(void);
void EggLoco_State_Unknown3(void);

#endif //!OBJ_EGGLOCO_H

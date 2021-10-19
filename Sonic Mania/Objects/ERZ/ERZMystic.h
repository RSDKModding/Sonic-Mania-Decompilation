#ifndef OBJ_ERZMYSTIC_H
#define OBJ_ERZMYSTIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectERZMystic;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 field_60;
    Vector2 mysticPos;
    int32 field_70;
    int32 cupY;
    int32 timer;
    int32 invincibilityTimer;
    int32 correctCup;
    int32 cupPos[3];
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
    int32 field_A4;
    int32 cupAlpha[3];
    int32 field_B4;
    int32 field_B8;
    int32 field_BC;
    int32 field_C0;
    int32 field_C4;
    int32 field_C8;
    int32 timer2;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Hitbox hitbox;
} EntityERZMystic;

// Object Struct
extern ObjectERZMystic *ERZMystic;

// Standard Entity Events
void ERZMystic_Update(void);
void ERZMystic_LateUpdate(void);
void ERZMystic_StaticUpdate(void);
void ERZMystic_Draw(void);
void ERZMystic_Create(void* data);
void ERZMystic_StageLoad(void);
void ERZMystic_EditorDraw(void);
void ERZMystic_EditorLoad(void);
void ERZMystic_Serialize(void);

// Extra Entity Functions
void ERZMystic_CheckPlayerCollisions(void);
void ERZMystic_HandleCups(void);

void ERZMystic_StateDraw_Unknown1(void);
void ERZMystic_StateDraw_Unknown2(void);

void ERZMystic_State_Unknown1(void);
void ERZMystic_State_Unknown2(void);
void ERZMystic_State_Unknown3(void);
void ERZMystic_State_Unknown4(void);
void ERZMystic_State_Unknown5(void);
void ERZMystic_State_Unknown6(void);
void ERZMystic_State_Unknown7(void);
void ERZMystic_State_Unknown8(void);
void ERZMystic_State_Unknown9(void);
void ERZMystic_State_Unknown10(void);

#endif //!OBJ_ERZMYSTIC_H

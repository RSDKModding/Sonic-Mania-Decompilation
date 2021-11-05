#ifndef OBJ_PHANTOMMYSTIC_H
#define OBJ_PHANTOMMYSTIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxCupSwap;
    uint16 sfxBigLaser;
    uint16 sfxImpact;
} ObjectPhantomMystic;

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
} EntityPhantomMystic;

// Object Struct
extern ObjectPhantomMystic *PhantomMystic;

// Standard Entity Events
void PhantomMystic_Update(void);
void PhantomMystic_LateUpdate(void);
void PhantomMystic_StaticUpdate(void);
void PhantomMystic_Draw(void);
void PhantomMystic_Create(void* data);
void PhantomMystic_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomMystic_EditorDraw(void);
void PhantomMystic_EditorLoad(void);
#endif
void PhantomMystic_Serialize(void);

// Extra Entity Functions
void PhantomMystic_CheckPlayerCollisions(void);
void PhantomMystic_HandleCups(void);
     
void PhantomMystic_StateDraw_Unknown1(void);
void PhantomMystic_StateDraw_Unknown2(void);
     
void PhantomMystic_State_Unknown1(void);
void PhantomMystic_State_Unknown2(void);
void PhantomMystic_State_Unknown3(void);
void PhantomMystic_State_Unknown4(void);
void PhantomMystic_State_Unknown5(void);
void PhantomMystic_State_Unknown6(void);
void PhantomMystic_State_Unknown7(void);
void PhantomMystic_State_Unknown8(void);
void PhantomMystic_State_Unknown9(void);
void PhantomMystic_State_Unknown10(void);


#endif //!OBJ_PHANTOMMYSTIC_H

#ifndef OBJ_DRILLERDROIDO_H
#define OBJ_DRILLERDROIDO_H

#include "SonicMania.h"

typedef enum {
    DRILLERDROIDO_MAIN,
    DRILLERDROIDO_FIREBALLEMITTER,
    DRILLERDROIDO_TARGET,
    DRILLERDROIDO_UNKNOWN3,
    DRILLERDROIDO_UNKNOWN4,
} DrillerdroidOTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
    Hitbox hitbox6;
    Hitbox hitbox7;
    Hitbox hitbox8;
    Hitbox hitbox9;
    int32 field_4C[2];
    int32 field_54[2];
    int32 field_5C[2];
    int32 field_64[2];
    int32 field_6C[2];
    int32 field_74[2];
    uint8 shouldPlayDrillSfx;
    bool32 playingDrillSfx;
    uint8 field_84;
    int32 childSlotStart;
    uint8 field_8C;
    bool32 emitFireballs;
    Entity *boss;
    uint16 aniFrames;
    uint16 ticFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
    uint16 sfxImpact;
    uint16 sfxJump;
    uint16 sfxTargeting;
    uint16 sfxSizzle;
} ObjectDrillerdroidO;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 type;
    StateMachine(state);
    StateMachine(stateDraw);
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Entity *target;
    int32 field_B0;
    int32 field_B4;
    int32 startY;
    int32 timer;
    uint8 invincibilityTimer;
    int8 health;
} EntityDrillerdroidO;

// Object Struct
extern ObjectDrillerdroidO *DrillerdroidO;

// Standard Entity Events
void DrillerdroidO_Update(void);
void DrillerdroidO_LateUpdate(void);
void DrillerdroidO_StaticUpdate(void);
void DrillerdroidO_Draw(void);
void DrillerdroidO_Create(void* data);
void DrillerdroidO_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DrillerdroidO_EditorDraw(void);
void DrillerdroidO_EditorLoad(void);
#endif
void DrillerdroidO_Serialize(void);

// Extra Entity Functions
void DrillerdroidO_CheckPlayerCollisions(void);
void DrillerdroidO_Explode(void);
void DrillerdroidO_SpawnDebris(int offset);

void DrillerdroidO_State_SetupArena(void);
void DrillerdroidO_State_StartBoss(void);
void DrillerdroidO_State_Unknown2(void);
void DrillerdroidO_State_Unknown3(void);
void DrillerdroidO_State_Unknown4(void);
void DrillerdroidO_State_Unknown5(void);
void DrillerdroidO_State_Unknown6(void);
void DrillerdroidO_State_Unknown7(void);
void DrillerdroidO_State_Unknown8(void);
void DrillerdroidO_State_Unknown9(void);
void DrillerdroidO_State_Unknown10(void);
void DrillerdroidO_State_Unknown11(void);
void DrillerdroidO_State_Unknown12(void);
void DrillerdroidO_State_Unknown13(void);
void DrillerdroidO_State_Unknown14(void);
void DrillerdroidO_State_Unknown15(void);
void DrillerdroidO_State_Unknown1(void);
void DrillerdroidO_State_Unknown16(void);
void DrillerdroidO_State_Unknown17(void);
void DrillerdroidO_State_Unknown18(void);
void DrillerdroidO_State_Destroyed(void);
void DrillerdroidO_State_Finish(void);

void DrillerdroidO_StateDraw_Boss(void);
void DrillerdroidO_StateDraw_Unknown2(void);

void DrillerdroidO_State_Target(void);
void DrillerdroidO_StateDraw_Target(void);

void DrillerdroidO_State_FireballEmitter(void);
void DrillerdroidO_StateDraw_FireballEmitter(void);

#endif //!OBJ_DRILLERDROIDO_H

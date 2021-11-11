#ifndef OBJ_FIREWORM_H
#define OBJ_FIREWORM_H

#include "SonicMania.h"

#define Fireworm_SegmentCount (5)

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Animator animator;
    uint16 aniFrames;
} ObjectFireworm;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    Vector2 positions[Fireworm_SegmentCount];
    Vector2 velocities[Fireworm_SegmentCount];
    uint8 directions[Fireworm_SegmentCount];
    uint16 angles[Fireworm_SegmentCount];
    uint8 timers[Fireworm_SegmentCount];
    uint8 timers2[Fireworm_SegmentCount];
    int32 positionsY[Fireworm_SegmentCount];
    int32 timer;
    int32 field_EC;
    int32 field_F0;
    Animator animators1[Fireworm_SegmentCount];
    Animator animators2[Fireworm_SegmentCount];
} EntityFireworm;

// Object Struct
extern ObjectFireworm *Fireworm;

// Standard Entity Events
void Fireworm_Update(void);
void Fireworm_LateUpdate(void);
void Fireworm_StaticUpdate(void);
void Fireworm_Draw(void);
void Fireworm_Create(void* data);
void Fireworm_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Fireworm_EditorDraw(void);
void Fireworm_EditorLoad(void);
#endif
void Fireworm_Serialize(void);

// Extra Entity Functions
void Fireworm_DebugSpawn(void);
void Fireworm_DebugDraw(void);

void Fireworm_CheckPlayerCollisions(void);
void Fireworm_CheckOnScreen(void);

void Fireworm_State_Setup(void);
void Fireworm_State_Unknown1(void);
void Fireworm_State_Unknown2(void);

void Fireworm_State1_Unknown1(void);
void Fireworm_State1_Unknown2(void);

#endif //!OBJ_FIREWORM_H

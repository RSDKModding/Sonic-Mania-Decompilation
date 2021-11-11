#ifndef OBJ_ROCKDRILL_H
#define OBJ_ROCKDRILL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    uint8 shouldPlayDrillSfx;
    bool32 playingDrillSfx;
    uint16 aniFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
} ObjectRockDrill;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    bool32 lockCamera;
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
    int32 timer;
    int32 field_8C[2];
    int32 field_94[2];
    int32 field_9C[2];
    int32 field_A4[2];
    int32 field_AC[2];
    int32 field_B4[2];
    uint8 invincibilityTimer;
} EntityRockDrill;

// Object Struct
extern ObjectRockDrill *RockDrill;

// Standard Entity Events
void RockDrill_Update(void);
void RockDrill_LateUpdate(void);
void RockDrill_StaticUpdate(void);
void RockDrill_Draw(void);
void RockDrill_Create(void* data);
void RockDrill_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void RockDrill_EditorDraw(void);
void RockDrill_EditorLoad(void);
#endif
void RockDrill_Serialize(void);

// Extra Entity Functions
void RockDrill_CheckPlayerCollisions(void);
void RockDrill_SpawnDebris(int offset);

void RockDrill_State_Unknown1(void);
void RockDrill_State_Unknown2(void);
void RockDrill_State_Unknown3(void);
void RockDrill_State_Unknown4(void);

#endif //!OBJ_ROCKDRILL_H

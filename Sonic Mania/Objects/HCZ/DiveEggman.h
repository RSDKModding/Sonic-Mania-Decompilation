#ifndef OBJ_DIVEEGGMAN_H
#define OBJ_DIVEEGGMAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Entity *screwMobile;
    uint16 diveFrames;
    uint16 aniFrames;
    uint16 sfxBigFan;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxRockemSockem;
} ObjectDiveEggman;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 startY;
    int32 timer;
    int32 invincibilityTimer;
    uint8 health;
    Animator animator;
} EntityDiveEggman;

// Object Struct
extern ObjectDiveEggman *DiveEggman;

// Standard Entity Events
void DiveEggman_Update(void);
void DiveEggman_LateUpdate(void);
void DiveEggman_StaticUpdate(void);
void DiveEggman_Draw(void);
void DiveEggman_Create(void* data);
void DiveEggman_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DiveEggman_EditorDraw(void);
void DiveEggman_EditorLoad(void);
#endif
void DiveEggman_Serialize(void);

// Extra Entity Functions
void DiveEggman_Explode(void);

void DiveEggman_State_Unknown1(void);
void DiveEggman_State_Unknown2(void);
void DiveEggman_State_Unknown3(void);
void DiveEggman_State_Unknown4(void);
void DiveEggman_State_Unknown5(void);
void DiveEggman_State_Unknown6(void);
void DiveEggman_State_Destroyed(void);
void DiveEggman_State_Finish(void);

bool32 DiveEggman_Unknown10(void);
void DiveEggman_State2_Unknown1(void);
void DiveEggman_State2_Unknown2(void);
void DiveEggman_State2_Unknown3(void);
void DiveEggman_State2_Unknown4(void);

#endif //!OBJ_DIVEEGGMAN_H

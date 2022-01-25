#ifndef OBJ_OCTUS_H
#define OBJ_OCTUS_H

#include "SonicMania.h"

// Object Class
struct ObjectOctus {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
    uint16 sfxShot;
};

// Entity Class
struct EntityOctus {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    uint8 field_60;
    Vector2 startPos;
    uint8 startDir;
    int32 field_70;
    Animator animator;
};

// Object Struct
extern ObjectOctus *Octus;

// Standard Entity Events
void Octus_Update(void);
void Octus_LateUpdate(void);
void Octus_StaticUpdate(void);
void Octus_Draw(void);
void Octus_Create(void* data);
void Octus_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Octus_EditorDraw(void);
void Octus_EditorLoad(void);
#endif
void Octus_Serialize(void);

// Extra Entity Functions
void Octus_DebugSpawn(void);
void Octus_DebugDraw(void);

void Octus_CheckPlayerCollisions(void);
void Octus_CheckOnScreen(void);

void Octus_State_Setup(void);
void Octus_Unknown5(void);
void Octus_Unknown6(void);
void Octus_Unknown7(void);
void Octus_Unknown8(void);
void Octus_Unknown9(void);
void Octus_State_Shot(void);

#endif //!OBJ_OCTUS_H

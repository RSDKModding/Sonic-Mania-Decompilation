#ifndef OBJ_SWEEP_H
#define OBJ_SWEEP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
    uint16 sfxPon;
} ObjectSweep;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateStore);
    int32 timer;
    uint8 hasShot;
    Vector2 startPos;
    uint8 startDir;
    Entity *waterPtr;
    Animator animator;
} EntitySweep;

// Object Struct
extern ObjectSweep *Sweep;

// Standard Entity Events
void Sweep_Update(void);
void Sweep_LateUpdate(void);
void Sweep_StaticUpdate(void);
void Sweep_Draw(void);
void Sweep_Create(void* data);
void Sweep_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Sweep_EditorDraw(void);
void Sweep_EditorLoad(void);
#endif
void Sweep_Serialize(void);

// Extra Entity Functions
void Sweep_DebugSpawn(void);
void Sweep_DebugDraw(void);

void Sweep_CheckOnScreen(void);
void Sweep_HandleInteractions(void);
void Sweep_CheckShoot(void);

void Sweep_Unknown5(void);
void Sweep_Unknown6(void);
void Sweep_Unknown7(void);
void Sweep_Unknown8(void);
void Sweep_Unknown9(void);
void Sweep_Unknown10(void);
void Sweep_Unknown11(void);

#endif //!OBJ_SWEEP_H

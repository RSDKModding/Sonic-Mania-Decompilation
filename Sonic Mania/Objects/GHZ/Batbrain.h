#ifndef OBJ_BATBRAIN_H
#define OBJ_BATBRAIN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
    ushort sfxFlap;
} ObjectBatbrain;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int targetY;
    Entity *target;
    Vector2 startPos;
    Animator animator;
} EntityBatbrain;

// Object Struct
extern ObjectBatbrain *Batbrain;

// Standard Entity Events
void Batbrain_Update(void);
void Batbrain_LateUpdate(void);
void Batbrain_StaticUpdate(void);
void Batbrain_Draw(void);
void Batbrain_Create(void* data);
void Batbrain_StageLoad(void);
void Batbrain_EditorDraw(void);
void Batbrain_EditorLoad(void);
void Batbrain_Serialize(void);

// Extra Entity Functions
void Batbrain_DebugDraw(void);
void Batbrain_DebugSpawn(void);
//Helpers
void Batbrain_CheckHit(void);
void Batbrain_CheckOnScreen(void);
//States
void Batbrain_Unknown2(void);
void Batbrain_Unknown3(void);
void Batbrain_Unknown4(void);
void Batbrain_Unknown5(void);
void Batbrain_Unknown6(void);

#endif //!OBJ_BATBRAIN_H

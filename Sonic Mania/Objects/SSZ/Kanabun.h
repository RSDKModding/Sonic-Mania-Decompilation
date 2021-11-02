#ifndef OBJ_KANABUN_H
#define OBJ_KANABUN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
} ObjectKanabun;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    Vector2 startPos;
    uint8 startDir;
    int32 hVel;
    uint8 hDist;
    uint8 bobDist;
    uint8 angleVel;
    Animator animator;
} EntityKanabun;

// Object Struct
extern ObjectKanabun *Kanabun;

// Standard Entity Events
void Kanabun_Update(void);
void Kanabun_LateUpdate(void);
void Kanabun_StaticUpdate(void);
void Kanabun_Draw(void);
void Kanabun_Create(void* data);
void Kanabun_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Kanabun_EditorDraw(void);
void Kanabun_EditorLoad(void);
#endif
void Kanabun_Serialize(void);

// Extra Entity Functions
void Kanabun_DebugSpawn(void);
void Kanabun_DebugDraw(void);

void Kanabun_CheckPlayerCollisions(void);
void Kanabun_CheckOnScreen(void);
void Kanabun_HandleMovement(void);

void Kanabun_State_Setup(void);
void Kanabun_State_Unknown1(void);
void Kanabun_State_Unknown2(void);

#endif //!OBJ_KANABUN_H

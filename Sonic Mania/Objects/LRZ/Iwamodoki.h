#ifndef OBJ_IWAMODOKI_H
#define OBJ_IWAMODOKI_H

#include "SonicMania.h"

// Object Class
struct ObjectIwamodoki {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
};

// Entity Class
struct EntityIwamodoki {
    RSDK_ENTITY
    StateMachine(state);
    bool32 lrzConvPhys;
    Vector2 startPos;
    Vector2 moveOffset;
    Vector2 preMovePos;
    Vector2 postMovePos;
    uint8 startDir;
    int32 timer1;
    int32 timer2;
    Animator animator;
};

// Object Struct
extern ObjectIwamodoki *Iwamodoki;

// Standard Entity Events
void Iwamodoki_Update(void);
void Iwamodoki_LateUpdate(void);
void Iwamodoki_StaticUpdate(void);
void Iwamodoki_Draw(void);
void Iwamodoki_Create(void* data);
void Iwamodoki_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Iwamodoki_EditorDraw(void);
void Iwamodoki_EditorLoad(void);
#endif
void Iwamodoki_Serialize(void);

// Extra Entity Functions
void Iwamodoki_DebugSpawn(void);
void Iwamodoki_DebugDraw(void);

void Iwamodoki_HandlePlayerCollisions(void);
void Iwamodoki_CheckOnScreen(void);

void Iwamodoki_State_Setup(void);
void Iwamodoki_State_Unknown1(void);
void Iwamodoki_State_Unknown2(void);
void Iwamodoki_State_Unknown3(void);
void Iwamodoki_State_Unknown4(void);
void Iwamodoki_State_Debris(void);

#endif //!OBJ_IWAMODOKI_H

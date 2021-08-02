#ifndef OBJ_CRANE_H
#define OBJ_CRANE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    ushort aniFrames;
    ushort sfxGrab;
} ObjectCrane;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Entity *grabbedPlayer;
    ushort distance;
    byte startType;
    byte playerType;
    Vector2 startPos;
    byte startDir;
    int timer2;
    int timer;
} EntityCrane;

// Object Struct
extern ObjectCrane *Crane;

// Standard Entity Events
void Crane_Update(void);
void Crane_LateUpdate(void);
void Crane_StaticUpdate(void);
void Crane_Draw(void);
void Crane_Create(void* data);
void Crane_StageLoad(void);
void Crane_EditorDraw(void);
void Crane_EditorLoad(void);
void Crane_Serialize(void);

// Extra Entity Functions
void Crane_DebugSpawn(void);
void Crane_DebugDraw(void);

void Crane_HandlePlayerInteractions(void);
void Crane_CheckOnScreen(void);

void Crane_State_CheckOnScreen(void);
void Crane_State_Unknown1(void);
void Crane_State_Unknown2(void);
void Crane_State_Unknown3(void);
void Crane_State_Unknown4(void);
void Crane_State_Unknown5(void);
void Crane_State_Unknown6(void);
void Crane_State_Unknown7(void);
void Crane_State_Unknown8(void);
void Crane_State_Unknown9(void);
void Crane_State_Unknown10(void);

#endif //!OBJ_CRANE_H

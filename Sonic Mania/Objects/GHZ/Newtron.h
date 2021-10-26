#ifndef OBJ_NEWTRON_H
#define OBJ_NEWTRON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    uint16 aniFrames;
} ObjectNewtron;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 timer;
    Vector2 startPos;
    Animator animator1;
    Animator animator2;
} EntityNewtron;

// Object Struct
extern ObjectNewtron *Newtron;

// Standard Entity Events
void Newtron_Update(void);
void Newtron_LateUpdate(void);
void Newtron_StaticUpdate(void);
void Newtron_Draw(void);
void Newtron_Create(void* data);
void Newtron_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Newtron_EditorDraw(void);
void Newtron_EditorLoad(void);
#endif
void Newtron_Serialize(void);

// Extra Entity Functions
void Newtron_DebugDraw(void);
void Newtron_DebugSpawn(void);

//Helpers 
void Newtron_CheckHit(void);
void Newtron_CheckOnScreen(void);

//States
void Newtron_Unknown3(void);
void Newtron_Unknown4(void);
void Newtron_Unknown5(void);
void Newtron_Unknown6(void);
void Newtron_Unknown7(void);
void Newtron_Unknown8(void);
void Newtron_Unknown9(void);
void Newtron_Unknown10(void);
void Newtron_State_Projectile(void);

#endif //!OBJ_NEWTRON_H

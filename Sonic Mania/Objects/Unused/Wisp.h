#ifndef OBJ_WISP_H
#define OBJ_WISP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
} ObjectWisp;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int timer2;
    EntityPlayer *targetPtr;
    Vector2 startPos;
    Animator animator1;
    Animator animator2;
} EntityWisp;

// Object Entity
extern ObjectWisp *Wisp;

// Standard Entity Events
void Wisp_Update(void);
void Wisp_LateUpdate(void);
void Wisp_StaticUpdate(void);
void Wisp_Draw(void);
void Wisp_Create(void* data);
void Wisp_StageLoad(void);
void Wisp_EditorDraw(void);
void Wisp_EditorLoad(void);
void Wisp_Serialize(void);

// Extra Entity Functions
void Wisp_DebugSpawn(void);
void Wisp_DebugDraw(void);

void Wisp_HandlePlayerInteractions(void);
void Wisp_CheckOnScreen(void);

void Wisp_State_Setup(void);
void Wisp_Unknown5(void);
void Wisp_Unknown6(void);
void Wisp_Unknown7(void);

#endif //!OBJ_WISP_H

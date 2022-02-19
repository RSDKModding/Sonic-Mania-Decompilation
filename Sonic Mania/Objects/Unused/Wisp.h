#ifndef OBJ_WISP_H
#define OBJ_WISP_H

#include "SonicMania.h"

// Object Class
struct ObjectWisp {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityWisp {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 buzzCount;
    EntityPlayer *targetPtr;
    Vector2 startPos;
    Animator bodyAnimator;
    Animator wingAnimator;
};

// Object Entity
extern ObjectWisp *Wisp;

// Standard Entity Events
void Wisp_Update(void);
void Wisp_LateUpdate(void);
void Wisp_StaticUpdate(void);
void Wisp_Draw(void);
void Wisp_Create(void* data);
void Wisp_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Wisp_EditorDraw(void);
void Wisp_EditorLoad(void);
#endif
void Wisp_Serialize(void);

// Extra Entity Functions
void Wisp_DebugSpawn(void);
void Wisp_DebugDraw(void);

void Wisp_HandlePlayerInteractions(void);
void Wisp_CheckOffScreen(void);

void Wisp_State_Setup(void);
void Wisp_WaitInPlace(void);
void Wisp_FlyTowardTarget(void);
void Wisp_BasicFly(void);

#endif //!OBJ_WISP_H

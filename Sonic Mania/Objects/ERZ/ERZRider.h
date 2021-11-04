#ifndef OBJ_ERZRIDER_H
#define OBJ_ERZRIDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectERZRider;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 startPos;
    bool32 isJimmy;
    int32 field_6C;
    int32 field_70;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
} EntityERZRider;

// Object Struct
extern ObjectERZRider *ERZRider;

// Standard Entity Events
void ERZRider_Update(void);
void ERZRider_LateUpdate(void);
void ERZRider_StaticUpdate(void);
void ERZRider_Draw(void);
void ERZRider_Create(void* data);
void ERZRider_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ERZRider_EditorDraw(void);
void ERZRider_EditorLoad(void);
#endif
void ERZRider_Serialize(void);

// Extra Entity Functions
void ERZRider_StateDraw_Unknown1(void);

#endif //!OBJ_ERZRIDER_H

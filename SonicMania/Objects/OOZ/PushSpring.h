#ifndef OBJ_PUSHSPRING_H
#define OBJ_PUSHSPRING_H

#include "Game.h"

typedef enum { PUSHSPRING_V, PUSHSPRING_H } PushSpringTypes;

// Object Class
struct ObjectPushSpring {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxPush;
    uint16 sfxSpring;
};

// Entity Class
struct EntityPushSpring {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    StateMachine(stateCollide);
    uint8 type;
    int32 pushOffset;
    int32 timer;
    Hitbox hitbox;
    bool32 beingPushed;
    Animator animator;
};

// Object Struct
extern ObjectPushSpring *PushSpring;

// Standard Entity Events
void PushSpring_Update(void);
void PushSpring_LateUpdate(void);
void PushSpring_StaticUpdate(void);
void PushSpring_Draw(void);
void PushSpring_Create(void *data);
void PushSpring_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PushSpring_EditorDraw(void);
void PushSpring_EditorLoad(void);
#endif
void PushSpring_Serialize(void);

// Extra Entity Functions
void PushSpring_Collide_Top(void);
void PushSpring_Collide_Bottom(void);
void PushSpring_Collide_Left(void);
void PushSpring_Collide_Right(void);

void PushSpring_Draw_Top(void);
void PushSpring_Draw_Bottom(void);
void PushSpring_Draw_Left(void);
void PushSpring_Draw_Right(void);

void PushSpring_State_WaitForPushed(void);
void PushSpring_State_BeingPushed(void);
void PushSpring_State_PushRecoil(void);

#endif //! OBJ_PUSHSPRING_H

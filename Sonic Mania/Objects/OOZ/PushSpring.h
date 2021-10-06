#ifndef OBJ_PUSHSPRING_H
#define OBJ_PUSHSPRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort sfxPush;
    ushort sfxSpring;
} ObjectPushSpring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    StateMachine(stateLate);
    byte type;
    int pushOffset;
    int pushTimer;
    Hitbox hitbox;
    bool32 field_78;
    Animator animator;
} EntityPushSpring;

// Object Struct
extern ObjectPushSpring *PushSpring;

// Standard Entity Events
void PushSpring_Update(void);
void PushSpring_LateUpdate(void);
void PushSpring_StaticUpdate(void);
void PushSpring_Draw(void);
void PushSpring_Create(void* data);
void PushSpring_StageLoad(void);
void PushSpring_EditorDraw(void);
void PushSpring_EditorLoad(void);
void PushSpring_Serialize(void);

// Extra Entity Functions
void PushSpring_HandlePlayerCollisions_Top(void);
void PushSpring_HandlePlayerCollisions_Bottom(void);
void PushSpring_HandlePlayerCollisions_Left(void);
void PushSpring_HandlePlayerCollisions_Right(void);

void PushSpring_StateDraw_Top(void);
void PushSpring_StateDraw_Bottom(void);
void PushSpring_StateDraw_Left(void);
void PushSpring_StateDraw_Right(void);

void PushSpring_Unknown9(void);
void PushSpring_Unknown10(void);
void PushSpring_Unknown11(void);


#endif //!OBJ_PUSHSPRING_H

#ifndef OBJ_PUSHSPRING_H
#define OBJ_PUSHSPRING_H

#include "SonicMania.h"

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
    StateMachine(stateLate);
    uint8 type;
    int32 pushOffset;
    int32 pushTimer;
    Hitbox hitbox;
    bool32 field_78;
    Animator animator;
};

// Object Struct
extern ObjectPushSpring *PushSpring;

// Standard Entity Events
void PushSpring_Update(void);
void PushSpring_LateUpdate(void);
void PushSpring_StaticUpdate(void);
void PushSpring_Draw(void);
void PushSpring_Create(void* data);
void PushSpring_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PushSpring_EditorDraw(void);
void PushSpring_EditorLoad(void);
#endif
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

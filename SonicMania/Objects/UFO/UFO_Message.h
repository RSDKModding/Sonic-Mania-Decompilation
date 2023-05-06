#ifndef OBJ_UFO_MESSAGE_H
#define OBJ_UFO_MESSAGE_H

#include "Game.h"

typedef enum {
    UFO_MESSAGE_CATCHUFO,
    UFO_MESSAGE_ESCAPETIME,
    UFO_MESSAGE_COURSEOUT,
    UFO_MESSAGE_TIMEOVER,
} UFO_MessageTypes;

// Object Class
struct ObjectUFO_Message {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUFO_Message {
    RSDK_ENTITY
    StateMachine(state);
    int32 displayTime;
    int32 timer;
    Animator leftAnimator;
    Animator rightAnimator;
    int32 unused1;
    int32 unused2;
};

// Object Struct
extern ObjectUFO_Message *UFO_Message;

// Standard Entity Events
void UFO_Message_Update(void);
void UFO_Message_LateUpdate(void);
void UFO_Message_StaticUpdate(void);
void UFO_Message_Draw(void);
void UFO_Message_Create(void *data);
void UFO_Message_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Message_EditorDraw(void);
void UFO_Message_EditorLoad(void);
#endif
void UFO_Message_Serialize(void);

// Extra Entity Functions
void UFO_Message_State_Appear(void);
void UFO_Message_State_ShowMsg(void);
void UFO_Message_State_Exit(void);

#endif //! OBJ_UFO_MESSAGE_H

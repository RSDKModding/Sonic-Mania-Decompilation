#ifndef OBJ_UFO_MESSAGE_H
#define OBJ_UFO_MESSAGE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectUFO_Message;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer2;
    int32 timer;
    Animator animator1;
    Animator animator2;
    int32 field_94;
    int32 field_98;
} EntityUFO_Message;

// Object Struct
extern ObjectUFO_Message *UFO_Message;

// Standard Entity Events
void UFO_Message_Update(void);
void UFO_Message_LateUpdate(void);
void UFO_Message_StaticUpdate(void);
void UFO_Message_Draw(void);
void UFO_Message_Create(void* data);
void UFO_Message_StageLoad(void);
void UFO_Message_EditorDraw(void);
void UFO_Message_EditorLoad(void);
void UFO_Message_Serialize(void);

// Extra Entity Functions
void UFO_Message_Unknown1(void);
void UFO_Message_Unknown2(void);
void UFO_Message_Unknown3(void);

#endif //!OBJ_UFO_MESSAGE_H

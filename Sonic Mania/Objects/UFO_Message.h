#ifndef OBJ_UFO_MESSAGE_H
#define OBJ_UFO_MESSAGE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectUFO_Message;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    int timer2;
    int timer;
    AnimationData data1;
    AnimationData data2;
    int field_94;
    int field_98;
} EntityUFO_Message;

// Object Struct
extern ObjectUFO_Message *UFO_Message;

// Standard Entity Events
void UFO_Message_Update();
void UFO_Message_LateUpdate();
void UFO_Message_StaticUpdate();
void UFO_Message_Draw();
void UFO_Message_Create(void* data);
void UFO_Message_StageLoad();
void UFO_Message_EditorDraw();
void UFO_Message_EditorLoad();
void UFO_Message_Serialize();

// Extra Entity Functions
void UFO_Message_Unknown1();
void UFO_Message_Unknown2();
void UFO_Message_Unknown3();

#endif //!OBJ_UFO_MESSAGE_H

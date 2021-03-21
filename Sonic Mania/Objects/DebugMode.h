#ifndef OBJ_DEBUGMODE_H
#define OBJ_DEBUGMODE_H

#include "../SonicMania.h"

#define DEBUGMODE_ADD_OBJ(object) DebugMode_AddObject(object->objectID, object##_DebugDraw, object##_DebugSpawn)

// Object Class
typedef struct {
    RSDK_OBJECT
    short objectIDs[0x100];
    void (*draw[0x100])();
    void (*spawn[0x100])();
    AnimationData debugData;
    int objID;
    int itemCount;
    bool32 debugActive;
    byte itemSubType;
    byte subtypeCount;
    byte field_A2A;
    byte field_A2B;
    int field_A2C;
} ObjectDebugMode;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDebugMode;

// Object Struct
extern ObjectDebugMode *DebugMode;

// Standard Entity Events
void DebugMode_Update();
void DebugMode_LateUpdate();
void DebugMode_StaticUpdate();
void DebugMode_Draw();
void DebugMode_Create(void* data);
void DebugMode_StageLoad();
void DebugMode_EditorDraw();
void DebugMode_EditorLoad();
void DebugMode_Serialize();

// Extra Entity Functions
void DebugMode_NullState();
void DebugMode_AddObject(ushort id, void (*draw)(), void (*spawn)());

#endif //!OBJ_DEBUGMODE_H

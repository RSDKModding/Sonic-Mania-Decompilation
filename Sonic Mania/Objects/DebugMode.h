#ifndef OBJ_DEBUGMODE_H
#define OBJ_DEBUGMODE_H

#include "../SonicMania.h"

#define DEBUGMODE_ADD_OBJ(object) DebugMode_AddObject(object->objectID, object##_DebugDraw, object##_DebugSpawn)

// Object Class
typedef struct {
    RSDK_OBJECT
    short objectIDs[0x100];
    void (*draw[0x100])(void);
    void (*spawn[0x100])(void);
    Animator debugData;
    int objID;
    int itemCount;
    bool32 debugActive;
    byte itemSubType;
    byte subtypeCount;
    int field_A2C;
} ObjectDebugMode;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDebugMode;

// Object Struct
extern ObjectDebugMode *DebugMode;

// Standard Entity Events
void DebugMode_Update(void);
void DebugMode_LateUpdate(void);
void DebugMode_StaticUpdate(void);
void DebugMode_Draw(void);
void DebugMode_Create(void* data);
void DebugMode_StageLoad(void);
void DebugMode_EditorDraw(void);
void DebugMode_EditorLoad(void);
void DebugMode_Serialize(void);

// Extra Entity Functions
void DebugMode_NullState(void);
void DebugMode_AddObject(ushort id, void (*draw)(void), void (*spawn)(void));

#endif //!OBJ_DEBUGMODE_H

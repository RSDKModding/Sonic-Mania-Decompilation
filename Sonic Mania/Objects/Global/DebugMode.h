#ifndef OBJ_DEBUGMODE_H
#define OBJ_DEBUGMODE_H

#include "SonicMania.h"

#define DEBUGMODE_ADD_OBJ(object) DebugMode_AddObject(object->objectID, object##_DebugDraw, object##_DebugSpawn)
#define DebugMode_ObjectLimit     (0x100)

// Object Class
typedef struct {
    RSDK_OBJECT
    int16 objectIDs[DebugMode_ObjectLimit];
    StateMachine(draw[DebugMode_ObjectLimit]);
    StateMachine(spawn[DebugMode_ObjectLimit]);
    Animator animator;
    int32 objID;
    int32 itemCount;
    bool32 debugActive;
    uint8 itemSubType;
    uint8 subtypeCount;
    int32 field_A2C;
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
#if RETRO_INCLUDE_EDITOR
void DebugMode_EditorDraw(void);
void DebugMode_EditorLoad(void);
#endif
void DebugMode_Serialize(void);

// Extra Entity Functions
void DebugMode_NullState(void);
void DebugMode_AddObject(uint16 id, void (*draw)(void), void (*spawn)(void));

#endif //!OBJ_DEBUGMODE_H

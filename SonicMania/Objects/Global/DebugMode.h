#ifndef OBJ_DEBUGMODE_H
#define OBJ_DEBUGMODE_H

#include "Game.h"

#define DEBUGMODE_ADD_OBJ(object) DebugMode_AddObject(object->classID, object##_DebugDraw, object##_DebugSpawn)
#define DEBUGMODE_OBJECT_COUNT    (0x100)

// Object Class
struct ObjectDebugMode {
    RSDK_OBJECT
    // Never used, only set, prolly leftover from S1/S2
    int16 classIDs[DEBUGMODE_OBJECT_COUNT];
    StateMachine(draw[DEBUGMODE_OBJECT_COUNT]);
    StateMachine(spawn[DEBUGMODE_OBJECT_COUNT]);
    Animator animator;
    int32 itemID;
    int32 itemCount;
    bool32 debugActive;
    uint8 itemType;
    uint8 itemTypeCount;
    int32 unused1; // no clue, though it could be "exitTimer" assuming this was based on v4's debugMode object?
};

// Entity Class
struct EntityDebugMode {
    RSDK_ENTITY
};

// Object Struct
extern ObjectDebugMode *DebugMode;

// Standard Entity Events
void DebugMode_Update(void);
void DebugMode_LateUpdate(void);
void DebugMode_StaticUpdate(void);
void DebugMode_Draw(void);
void DebugMode_Create(void *data);
void DebugMode_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DebugMode_EditorDraw(void);
void DebugMode_EditorLoad(void);
#endif
void DebugMode_Serialize(void);

// Extra Entity Functions
void DebugMode_NullState(void);
void DebugMode_AddObject(uint16 id, void (*draw)(void), void (*spawn)(void));

#endif //! OBJ_DEBUGMODE_H

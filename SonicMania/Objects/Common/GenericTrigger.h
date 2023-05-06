#ifndef OBJ_GENERICTRIGGER_H
#define OBJ_GENERICTRIGGER_H

#include "Game.h"

typedef enum {
    GENERICTRIGGER_NONE,
} GenericTriggerIDs;

// Object Class
struct ObjectGenericTrigger {
    RSDK_OBJECT
    StateMachine(callbacks[16]);
    int32 playerID;
    uint16 aniFrames;
};

// Entity Class
struct EntityGenericTrigger {
    RSDK_ENTITY
    Vector2 size;
    uint8 triggerID;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectGenericTrigger *GenericTrigger;

// Standard Entity Events
void GenericTrigger_Update(void);
void GenericTrigger_LateUpdate(void);
void GenericTrigger_StaticUpdate(void);
void GenericTrigger_Draw(void);
void GenericTrigger_Create(void *data);
void GenericTrigger_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GenericTrigger_EditorDraw(void);
void GenericTrigger_EditorLoad(void);
#endif
void GenericTrigger_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_GENERICTRIGGER_H

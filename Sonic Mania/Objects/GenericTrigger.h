#ifndef OBJ_GENERICTRIGGER_H
#define OBJ_GENERICTRIGGER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    void (*callbacks[16])(void);
    int playerID;
    int field_48;
} ObjectGenericTrigger;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    byte triggerID;
    Hitbox hitbox;
} EntityGenericTrigger;

// Object Struct
extern ObjectGenericTrigger *GenericTrigger;

// Standard Entity Events
void GenericTrigger_Update(void);
void GenericTrigger_LateUpdate(void);
void GenericTrigger_StaticUpdate(void);
void GenericTrigger_Draw(void);
void GenericTrigger_Create(void* data);
void GenericTrigger_StageLoad(void);
void GenericTrigger_EditorDraw(void);
void GenericTrigger_EditorLoad(void);
void GenericTrigger_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GENERICTRIGGER_H

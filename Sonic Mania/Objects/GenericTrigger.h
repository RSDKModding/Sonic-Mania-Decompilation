#ifndef OBJ_GENERICTRIGGER_H
#define OBJ_GENERICTRIGGER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    void (*callbacks[16])();
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
void GenericTrigger_Update();
void GenericTrigger_LateUpdate();
void GenericTrigger_StaticUpdate();
void GenericTrigger_Draw();
void GenericTrigger_Create(void* data);
void GenericTrigger_StageLoad();
void GenericTrigger_EditorDraw();
void GenericTrigger_EditorLoad();
void GenericTrigger_Serialize();

// Extra Entity Functions


#endif //!OBJ_GENERICTRIGGER_H

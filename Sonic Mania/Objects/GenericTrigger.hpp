#ifndef OBJ_GENERICTRIGGER_H
#define OBJ_GENERICTRIGGER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGenericTrigger : Object {

};

// Entity Class
struct EntityGenericTrigger : Entity {

};

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

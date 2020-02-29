#ifndef OBJ_GENERICTRIGGER_H
#define OBJ_GENERICTRIGGER_H

#include "../../SonicMania.h"

//Object Class
class ObjectGenericTrigger : public Object {
public:

};

//Entity Class
class EntityGenericTrigger : public Entity {
public:

};

//Object Entity
ObjectGenericTrigger GenericTrigger;

//Entity Functions
void GenericTrigger_Update();
void GenericTrigger_EarlyUpdate();
void GenericTrigger_LateUpdate();
void GenericTrigger_Draw();
void GenericTrigger_Setup(void* subtype);
void GenericTrigger_StageLoad();
void GenericTrigger_GetAttributes();

#endif //!OBJ_GENERICTRIGGER_H

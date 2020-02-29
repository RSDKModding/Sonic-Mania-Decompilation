#ifndef OBJ_CRATE_H
#define OBJ_CRATE_H

#include "../../SonicMania.h"

//Object Class
class ObjectCrate : public Object {
public:

};

//Entity Class
class EntityCrate : public Entity {
public:

};

//Object Entity
ObjectCrate Crate;

//Entity Functions
void Crate_Update();
void Crate_EarlyUpdate();
void Crate_LateUpdate();
void Crate_Draw();
void Crate_Setup(void* subtype);
void Crate_StageLoad();
void Crate_GetAttributes();

#endif //!OBJ_CRATE_H

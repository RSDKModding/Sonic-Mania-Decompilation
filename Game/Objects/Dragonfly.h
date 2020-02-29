#ifndef OBJ_DRAGONFLY_H
#define OBJ_DRAGONFLY_H

#include "../../SonicMania.h"

//Object Class
class ObjectDragonfly : public Object {
public:

};

//Entity Class
class EntityDragonfly : public Entity {
public:

};

//Object Entity
ObjectDragonfly Dragonfly;

//Entity Functions
void Dragonfly_Update();
void Dragonfly_EarlyUpdate();
void Dragonfly_LateUpdate();
void Dragonfly_Draw();
void Dragonfly_Setup(void* subtype);
void Dragonfly_StageLoad();
void Dragonfly_GetAttributes();

#endif //!OBJ_DRAGONFLY_H

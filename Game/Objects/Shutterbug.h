#ifndef OBJ_SHUTTERBUG_H
#define OBJ_SHUTTERBUG_H

#include "../../SonicMania.h"

//Object Class
class ObjectShutterbug : public Object {
public:

};

//Entity Class
class EntityShutterbug : public Entity {
public:

};

//Object Entity
ObjectShutterbug Shutterbug;

//Entity Functions
void Shutterbug_Update();
void Shutterbug_EarlyUpdate();
void Shutterbug_LateUpdate();
void Shutterbug_Draw();
void Shutterbug_Setup(void* subtype);
void Shutterbug_StageLoad();
void Shutterbug_GetAttributes();

#endif //!OBJ_SHUTTERBUG_H

#ifndef OBJ_TORNADO_H
#define OBJ_TORNADO_H

#include "../../SonicMania.h"

//Object Class
class ObjectTornado : public Object {
public:

};

//Entity Class
class EntityTornado : public Entity {
public:

};

//Object Entity
ObjectTornado Tornado;

//Entity Functions
void Tornado_Update();
void Tornado_EarlyUpdate();
void Tornado_LateUpdate();
void Tornado_Draw();
void Tornado_Setup(void* subtype);
void Tornado_StageLoad();
void Tornado_GetAttributes();

#endif //!OBJ_TORNADO_H

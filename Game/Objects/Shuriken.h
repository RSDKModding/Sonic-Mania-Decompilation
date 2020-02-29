#ifndef OBJ_SHURIKEN_H
#define OBJ_SHURIKEN_H

#include "../../SonicMania.h"

//Object Class
class ObjectShuriken : public Object {
public:

};

//Entity Class
class EntityShuriken : public Entity {
public:

};

//Object Entity
ObjectShuriken Shuriken;

//Entity Functions
void Shuriken_Update();
void Shuriken_EarlyUpdate();
void Shuriken_LateUpdate();
void Shuriken_Draw();
void Shuriken_Setup(void* subtype);
void Shuriken_StageLoad();
void Shuriken_GetAttributes();

#endif //!OBJ_SHURIKEN_H

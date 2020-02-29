#ifndef OBJ_CONSTELLATION_H
#define OBJ_CONSTELLATION_H

#include "../../SonicMania.h"

//Object Class
class ObjectConstellation : public Object {
public:

};

//Entity Class
class EntityConstellation : public Entity {
public:

};

//Object Entity
ObjectConstellation Constellation;

//Entity Functions
void Constellation_Update();
void Constellation_EarlyUpdate();
void Constellation_LateUpdate();
void Constellation_Draw();
void Constellation_Setup(void* subtype);
void Constellation_StageLoad();
void Constellation_GetAttributes();

#endif //!OBJ_CONSTELLATION_H

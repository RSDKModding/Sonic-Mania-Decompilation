#ifndef OBJ_WATER_H
#define OBJ_WATER_H

#include "../../SonicMania.h"

//Object Class
class ObjectWater : public Object {
public:

};

//Entity Class
class EntityWater : public Entity {
public:

};

//Object Entity
ObjectWater Water;

//Entity Functions
void Water_Update();
void Water_EarlyUpdate();
void Water_LateUpdate();
void Water_Draw();
void Water_Setup(void* subtype);
void Water_StageLoad();
void Water_GetAttributes();

#endif //!OBJ_WATER_H

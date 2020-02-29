#ifndef OBJ_UFO_WATER_H
#define OBJ_UFO_WATER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Water : public Object {
public:

};

//Entity Class
class EntityUFO_Water : public Entity {
public:

};

//Object Entity
ObjectUFO_Water UFO_Water;

//Entity Functions
void UFO_Water_Update();
void UFO_Water_EarlyUpdate();
void UFO_Water_LateUpdate();
void UFO_Water_Draw();
void UFO_Water_Setup(void* subtype);
void UFO_Water_StageLoad();
void UFO_Water_GetAttributes();

#endif //!OBJ_UFO_WATER_H

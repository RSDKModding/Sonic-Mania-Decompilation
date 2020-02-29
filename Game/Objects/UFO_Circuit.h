#ifndef OBJ_UFO_CIRCUIT_H
#define OBJ_UFO_CIRCUIT_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Circuit : public Object {
public:

};

//Entity Class
class EntityUFO_Circuit : public Entity {
public:

};

//Object Entity
ObjectUFO_Circuit UFO_Circuit;

//Entity Functions
void UFO_Circuit_Update();
void UFO_Circuit_EarlyUpdate();
void UFO_Circuit_LateUpdate();
void UFO_Circuit_Draw();
void UFO_Circuit_Setup(void* subtype);
void UFO_Circuit_StageLoad();
void UFO_Circuit_GetAttributes();

#endif //!OBJ_UFO_CIRCUIT_H

#ifndef OBJ_VALVE_H
#define OBJ_VALVE_H

#include "../../SonicMania.h"

//Object Class
class ObjectValve : public Object {
public:

};

//Entity Class
class EntityValve : public Entity {
public:

};

//Object Entity
ObjectValve Valve;

//Entity Functions
void Valve_Update();
void Valve_EarlyUpdate();
void Valve_LateUpdate();
void Valve_Draw();
void Valve_Setup(void* subtype);
void Valve_StageLoad();
void Valve_GetAttributes();

#endif //!OBJ_VALVE_H

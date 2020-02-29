#ifndef OBJ_TURBINE_H
#define OBJ_TURBINE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTurbine : public Object {
public:

};

//Entity Class
class EntityTurbine : public Entity {
public:

};

//Object Entity
ObjectTurbine Turbine;

//Entity Functions
void Turbine_Update();
void Turbine_EarlyUpdate();
void Turbine_LateUpdate();
void Turbine_Draw();
void Turbine_Setup(void* subtype);
void Turbine_StageLoad();
void Turbine_GetAttributes();

#endif //!OBJ_TURBINE_H

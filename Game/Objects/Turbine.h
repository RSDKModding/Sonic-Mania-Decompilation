#ifndef OBJ_TURBINE_H
#define OBJ_TURBINE_H

//Object Class
class Turbine : Object {

};

//Entity Class
class EntityTurbine : Entity {

};

//Entity Functions
void Turbine_Update();
void Turbine_EarlyUpdate();
void Turbine_LateUpdate();
void Turbine_Draw();
void Turbine_Setup(void* subtype);
void Turbine_StageLoad();
void Turbine_GetAttributes();

#endif //!OBJ_TURBINE_H

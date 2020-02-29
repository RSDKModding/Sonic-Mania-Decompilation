#ifndef OBJ_VALVE_H
#define OBJ_VALVE_H

//Object Class
class Valve : Object {

};

//Entity Class
class EntityValve : Entity {

};

//Entity Functions
void Valve_Update();
void Valve_EarlyUpdate();
void Valve_LateUpdate();
void Valve_Draw();
void Valve_Setup(void* subtype);
void Valve_StageLoad();
void Valve_GetAttributes();

#endif //!OBJ_VALVE_H

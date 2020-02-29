#ifndef OBJ_UFO_CIRCUIT_H
#define OBJ_UFO_CIRCUIT_H

//Object Class
class UFO_Circuit : Object {

};

//Entity Class
class EntityUFO_Circuit : Entity {

};

//Entity Functions
void UFO_Circuit_Update();
void UFO_Circuit_EarlyUpdate();
void UFO_Circuit_LateUpdate();
void UFO_Circuit_Draw();
void UFO_Circuit_Setup(void* subtype);
void UFO_Circuit_StageLoad();
void UFO_Circuit_GetAttributes();

#endif //!OBJ_UFO_CIRCUIT_H

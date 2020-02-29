#ifndef OBJ_POHBEE_H
#define OBJ_POHBEE_H

//Object Class
class PohBee : Object {

};

//Entity Class
class EntityPohBee : Entity {

};

//Entity Functions
void PohBee_Update();
void PohBee_EarlyUpdate();
void PohBee_LateUpdate();
void PohBee_Draw();
void PohBee_Setup(void* subtype);
void PohBee_StageLoad();
void PohBee_GetAttributes();

#endif //!OBJ_POHBEE_H

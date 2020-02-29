#ifndef OBJ_POHBEE_H
#define OBJ_POHBEE_H

#include "../../SonicMania.h"

//Object Class
class ObjectPohBee : public Object {
public:

};

//Entity Class
class EntityPohBee : public Entity {
public:

};

//Object Entity
ObjectPohBee PohBee;

//Entity Functions
void PohBee_Update();
void PohBee_EarlyUpdate();
void PohBee_LateUpdate();
void PohBee_Draw();
void PohBee_Setup(void* subtype);
void PohBee_StageLoad();
void PohBee_GetAttributes();

#endif //!OBJ_POHBEE_H

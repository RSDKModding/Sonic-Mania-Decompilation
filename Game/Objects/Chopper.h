#ifndef OBJ_CHOPPER_H
#define OBJ_CHOPPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectChopper : public Object {
public:

};

//Entity Class
class EntityChopper : public Entity {
public:

};

//Object Entity
ObjectChopper Chopper;

//Entity Functions
void Chopper_Update();
void Chopper_EarlyUpdate();
void Chopper_LateUpdate();
void Chopper_Draw();
void Chopper_Setup(void* subtype);
void Chopper_StageLoad();
void Chopper_GetAttributes();

#endif //!OBJ_CHOPPER_H

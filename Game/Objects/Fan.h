#ifndef OBJ_FAN_H
#define OBJ_FAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectFan : public Object {
public:

};

//Entity Class
class EntityFan : public Entity {
public:

};

//Object Entity
ObjectFan Fan;

//Entity Functions
void Fan_Update();
void Fan_EarlyUpdate();
void Fan_LateUpdate();
void Fan_Draw();
void Fan_Setup(void* subtype);
void Fan_StageLoad();
void Fan_GetAttributes();

#endif //!OBJ_FAN_H

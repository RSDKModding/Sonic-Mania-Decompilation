#ifndef OBJ_DEROBOT_H
#define OBJ_DEROBOT_H

#include "../../SonicMania.h"

//Object Class
class ObjectDERobot : public Object {
public:

};

//Entity Class
class EntityDERobot : public Entity {
public:

};

//Object Entity
ObjectDERobot DERobot;

//Entity Functions
void DERobot_Update();
void DERobot_EarlyUpdate();
void DERobot_LateUpdate();
void DERobot_Draw();
void DERobot_Setup(void* subtype);
void DERobot_StageLoad();
void DERobot_GetAttributes();

#endif //!OBJ_DEROBOT_H

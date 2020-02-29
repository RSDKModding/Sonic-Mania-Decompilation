#ifndef OBJ_RHINOBOT_H
#define OBJ_RHINOBOT_H

#include "../../SonicMania.h"

//Object Class
class ObjectRhinobot : public Object {
public:

};

//Entity Class
class EntityRhinobot : public Entity {
public:

};

//Object Entity
ObjectRhinobot Rhinobot;

//Entity Functions
void Rhinobot_Update();
void Rhinobot_EarlyUpdate();
void Rhinobot_LateUpdate();
void Rhinobot_Draw();
void Rhinobot_Setup(void* subtype);
void Rhinobot_StageLoad();
void Rhinobot_GetAttributes();

#endif //!OBJ_RHINOBOT_H

#ifndef OBJ_RHINOBOT_H
#define OBJ_RHINOBOT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRhinobot : Object {

};

// Entity Class
struct EntityRhinobot : Entity {

};

// Object Entity
extern ObjectRhinobot Rhinobot;

// Standard Entity Events
void Rhinobot_Update();
void Rhinobot_LateUpdate();
void Rhinobot_StaticUpdate();
void Rhinobot_Draw();
void Rhinobot_Create(void* data);
void Rhinobot_StageLoad();
void Rhinobot_EditorDraw();
void Rhinobot_EditorLoad();
void Rhinobot_Serialize();

// Extra Entity Functions


#endif //!OBJ_RHINOBOT_H

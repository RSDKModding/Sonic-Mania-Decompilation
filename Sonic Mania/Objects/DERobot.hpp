#ifndef OBJ_DEROBOT_H
#define OBJ_DEROBOT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDERobot : Object {

};

// Entity Class
struct EntityDERobot : Entity {

};

// Object Struct
extern ObjectDERobot *DERobot;

// Standard Entity Events
void DERobot_Update();
void DERobot_LateUpdate();
void DERobot_StaticUpdate();
void DERobot_Draw();
void DERobot_Create(void* data);
void DERobot_StageLoad();
void DERobot_EditorDraw();
void DERobot_EditorLoad();
void DERobot_Serialize();

// Extra Entity Functions


#endif //!OBJ_DEROBOT_H

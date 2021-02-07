#ifndef OBJ_PUSHSPRING_H
#define OBJ_PUSHSPRING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPushSpring : Object{

};

// Entity Class
struct EntityPushSpring : Entity {

};

// Object Struct
extern ObjectPushSpring *PushSpring;

// Standard Entity Events
void PushSpring_Update();
void PushSpring_LateUpdate();
void PushSpring_StaticUpdate();
void PushSpring_Draw();
void PushSpring_Create(void* data);
void PushSpring_StageLoad();
void PushSpring_EditorDraw();
void PushSpring_EditorLoad();
void PushSpring_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUSHSPRING_H

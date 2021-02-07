#ifndef OBJ_LAUNCHSPRING_H
#define OBJ_LAUNCHSPRING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLaunchSpring : Object{

};

// Entity Class
struct EntityLaunchSpring : Entity {

};

// Object Struct
extern ObjectLaunchSpring *LaunchSpring;

// Standard Entity Events
void LaunchSpring_Update();
void LaunchSpring_LateUpdate();
void LaunchSpring_StaticUpdate();
void LaunchSpring_Draw();
void LaunchSpring_Create(void* data);
void LaunchSpring_StageLoad();
void LaunchSpring_EditorDraw();
void LaunchSpring_EditorLoad();
void LaunchSpring_Serialize();

// Extra Entity Functions


#endif //!OBJ_LAUNCHSPRING_H

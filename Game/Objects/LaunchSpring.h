#ifndef OBJ_LAUNCHSPRING_H
#define OBJ_LAUNCHSPRING_H

#include "../../SonicMania.h"

//Object Class
class ObjectLaunchSpring : public Object {
public:

};

//Entity Class
class EntityLaunchSpring : public Entity {
public:

};

//Object Entity
ObjectLaunchSpring LaunchSpring;

//Entity Functions
void LaunchSpring_Update();
void LaunchSpring_EarlyUpdate();
void LaunchSpring_LateUpdate();
void LaunchSpring_Draw();
void LaunchSpring_Setup(void* subtype);
void LaunchSpring_StageLoad();
void LaunchSpring_GetAttributes();

#endif //!OBJ_LAUNCHSPRING_H

#ifndef OBJ_LAUNCHSPRING_H
#define OBJ_LAUNCHSPRING_H

//Object Class
class LaunchSpring : Object {

};

//Entity Class
class EntityLaunchSpring : Entity {

};

//Entity Functions
void LaunchSpring_Update();
void LaunchSpring_EarlyUpdate();
void LaunchSpring_LateUpdate();
void LaunchSpring_Draw();
void LaunchSpring_Setup(void* subtype);
void LaunchSpring_StageLoad();
void LaunchSpring_GetAttributes();

#endif //!OBJ_LAUNCHSPRING_H

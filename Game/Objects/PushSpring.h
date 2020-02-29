#ifndef OBJ_PUSHSPRING_H
#define OBJ_PUSHSPRING_H

//Object Class
class PushSpring : Object {

};

//Entity Class
class EntityPushSpring : Entity {

};

//Entity Functions
void PushSpring_Update();
void PushSpring_EarlyUpdate();
void PushSpring_LateUpdate();
void PushSpring_Draw();
void PushSpring_Setup(void* subtype);
void PushSpring_StageLoad();
void PushSpring_GetAttributes();

#endif //!OBJ_PUSHSPRING_H

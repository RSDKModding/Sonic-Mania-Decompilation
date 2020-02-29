#ifndef OBJ_ICESPRING_H
#define OBJ_ICESPRING_H

//Object Class
class IceSpring : Object {

};

//Entity Class
class EntityIceSpring : Entity {

};

//Entity Functions
void IceSpring_Update();
void IceSpring_EarlyUpdate();
void IceSpring_LateUpdate();
void IceSpring_Draw();
void IceSpring_Setup(void* subtype);
void IceSpring_StageLoad();
void IceSpring_GetAttributes();

#endif //!OBJ_ICESPRING_H

#ifndef OBJ_SSZEGGMAN_H
#define OBJ_SSZEGGMAN_H

//Object Class
class SSZEggman : Object {

};

//Entity Class
class EntitySSZEggman : Entity {

};

//Entity Functions
void SSZEggman_Update();
void SSZEggman_EarlyUpdate();
void SSZEggman_LateUpdate();
void SSZEggman_Draw();
void SSZEggman_Setup(void* subtype);
void SSZEggman_StageLoad();
void SSZEggman_GetAttributes();

#endif //!OBJ_SSZEGGMAN_H

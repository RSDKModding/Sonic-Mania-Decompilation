#ifndef OBJ_BUCKWILDBALL_H
#define OBJ_BUCKWILDBALL_H

//Object Class
class BuckwildBall : Object {

};

//Entity Class
class EntityBuckwildBall : Entity {

};

//Entity Functions
void BuckwildBall_Update();
void BuckwildBall_EarlyUpdate();
void BuckwildBall_LateUpdate();
void BuckwildBall_Draw();
void BuckwildBall_Setup(void* subtype);
void BuckwildBall_StageLoad();
void BuckwildBall_GetAttributes();

#endif //!OBJ_BUCKWILDBALL_H

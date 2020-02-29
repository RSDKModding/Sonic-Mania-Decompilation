#ifndef OBJ_BUCKWILDBALL_H
#define OBJ_BUCKWILDBALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectBuckwildBall : public Object {
public:

};

//Entity Class
class EntityBuckwildBall : public Entity {
public:

};

//Object Entity
ObjectBuckwildBall BuckwildBall;

//Entity Functions
void BuckwildBall_Update();
void BuckwildBall_EarlyUpdate();
void BuckwildBall_LateUpdate();
void BuckwildBall_Draw();
void BuckwildBall_Setup(void* subtype);
void BuckwildBall_StageLoad();
void BuckwildBall_GetAttributes();

#endif //!OBJ_BUCKWILDBALL_H

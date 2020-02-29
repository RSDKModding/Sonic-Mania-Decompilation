#ifndef OBJ_BALLCANNON_H
#define OBJ_BALLCANNON_H

#include "../../SonicMania.h"

//Object Class
class ObjectBallCannon : public Object {
public:

};

//Entity Class
class EntityBallCannon : public Entity {
public:

};

//Object Entity
ObjectBallCannon BallCannon;

//Entity Functions
void BallCannon_Update();
void BallCannon_EarlyUpdate();
void BallCannon_LateUpdate();
void BallCannon_Draw();
void BallCannon_Setup(void* subtype);
void BallCannon_StageLoad();
void BallCannon_GetAttributes();

#endif //!OBJ_BALLCANNON_H

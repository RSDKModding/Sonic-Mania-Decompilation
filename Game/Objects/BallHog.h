#ifndef OBJ_BALLHOG_H
#define OBJ_BALLHOG_H

#include "../../SonicMania.h"

//Object Class
class ObjectBallHog : public Object {
public:

};

//Entity Class
class EntityBallHog : public Entity {
public:

};

//Object Entity
ObjectBallHog BallHog;

//Entity Functions
void BallHog_Update();
void BallHog_EarlyUpdate();
void BallHog_LateUpdate();
void BallHog_Draw();
void BallHog_Setup(void* subtype);
void BallHog_StageLoad();
void BallHog_GetAttributes();

#endif //!OBJ_BALLHOG_H

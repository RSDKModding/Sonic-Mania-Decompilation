#ifndef OBJ_BALLHOG_H
#define OBJ_BALLHOG_H

//Object Class
class BallHog : Object {

};

//Entity Class
class EntityBallHog : Entity {

};

//Entity Functions
void BallHog_Update();
void BallHog_EarlyUpdate();
void BallHog_LateUpdate();
void BallHog_Draw();
void BallHog_Setup(void* subtype);
void BallHog_StageLoad();
void BallHog_GetAttributes();

#endif //!OBJ_BALLHOG_H

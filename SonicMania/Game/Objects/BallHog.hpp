#ifndef OBJ_BALLHOG_H
#define OBJ_BALLHOG_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBallHog : Object {

};

// Entity Class
struct EntityBallHog : Entity {

};

// Object Entity
extern ObjectBallHog BallHog;

// Standard Entity Events
void BallHog_Update();
void BallHog_LateUpdate();
void BallHog_StaticUpdate();
void BallHog_Draw();
void BallHog_Create(void* data);
void BallHog_StageLoad();
void BallHog_EditorDraw();
void BallHog_EditorLoad();
void BallHog_Serialize();

// Extra Entity Functions


#endif //!OBJ_BALLHOG_H

#ifndef OBJ_BALLCANNON_H
#define OBJ_BALLCANNON_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBallCannon : Object {

};

// Entity Class
struct EntityBallCannon : Entity {

};

// Object Entity
extern ObjectBallCannon BallCannon;

// Standard Entity Events
void BallCannon_Update();
void BallCannon_LateUpdate();
void BallCannon_StaticUpdate();
void BallCannon_Draw();
void BallCannon_Create(void* data);
void BallCannon_StageLoad();
void BallCannon_EditorDraw();
void BallCannon_EditorLoad();
void BallCannon_Serialize();

// Extra Entity Functions


#endif //!OBJ_BALLCANNON_H

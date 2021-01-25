#ifndef OBJ_BUCKWILDBALL_H
#define OBJ_BUCKWILDBALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBuckwildBall : Object {

};

// Entity Class
struct EntityBuckwildBall : Entity {

};

// Object Entity
extern ObjectBuckwildBall BuckwildBall;

// Standard Entity Events
void BuckwildBall_Update();
void BuckwildBall_LateUpdate();
void BuckwildBall_StaticUpdate();
void BuckwildBall_Draw();
void BuckwildBall_Create(void* data);
void BuckwildBall_StageLoad();
void BuckwildBall_EditorDraw();
void BuckwildBall_EditorLoad();
void BuckwildBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUCKWILDBALL_H

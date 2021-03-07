#ifndef OBJ_CHECKERBALL_H
#define OBJ_CHECKERBALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCheckerBall : Object {

};

// Entity Class
struct EntityCheckerBall : Entity {

};

// Object Struct
extern ObjectCheckerBall *CheckerBall;

// Standard Entity Events
void CheckerBall_Update();
void CheckerBall_LateUpdate();
void CheckerBall_StaticUpdate();
void CheckerBall_Draw();
void CheckerBall_Create(void* data);
void CheckerBall_StageLoad();
void CheckerBall_EditorDraw();
void CheckerBall_EditorLoad();
void CheckerBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_CHECKERBALL_H

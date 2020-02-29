#ifndef OBJ_CHECKERBALL_H
#define OBJ_CHECKERBALL_H

//Object Class
class CheckerBall : Object {

};

//Entity Class
class EntityCheckerBall : Entity {

};

//Entity Functions
void CheckerBall_Update();
void CheckerBall_EarlyUpdate();
void CheckerBall_LateUpdate();
void CheckerBall_Draw();
void CheckerBall_Setup(void* subtype);
void CheckerBall_StageLoad();
void CheckerBall_GetAttributes();

#endif //!OBJ_CHECKERBALL_H

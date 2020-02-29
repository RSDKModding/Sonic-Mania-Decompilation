#ifndef OBJ_CHECKERBALL_H
#define OBJ_CHECKERBALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectCheckerBall : public Object {
public:

};

//Entity Class
class EntityCheckerBall : public Entity {
public:

};

//Object Entity
ObjectCheckerBall CheckerBall;

//Entity Functions
void CheckerBall_Update();
void CheckerBall_EarlyUpdate();
void CheckerBall_LateUpdate();
void CheckerBall_Draw();
void CheckerBall_Setup(void* subtype);
void CheckerBall_StageLoad();
void CheckerBall_GetAttributes();

#endif //!OBJ_CHECKERBALL_H

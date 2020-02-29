#ifndef OBJ_TETHERBALL_H
#define OBJ_TETHERBALL_H

#include "../../SonicMania.h"

//Object Class
class ObjectTetherBall : public Object {
public:

};

//Entity Class
class EntityTetherBall : public Entity {
public:

};

//Object Entity
ObjectTetherBall TetherBall;

//Entity Functions
void TetherBall_Update();
void TetherBall_EarlyUpdate();
void TetherBall_LateUpdate();
void TetherBall_Draw();
void TetherBall_Setup(void* subtype);
void TetherBall_StageLoad();
void TetherBall_GetAttributes();

#endif //!OBJ_TETHERBALL_H

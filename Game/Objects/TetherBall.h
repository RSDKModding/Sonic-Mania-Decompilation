#ifndef OBJ_TETHERBALL_H
#define OBJ_TETHERBALL_H

//Object Class
class TetherBall : Object {

};

//Entity Class
class EntityTetherBall : Entity {

};

//Entity Functions
void TetherBall_Update();
void TetherBall_EarlyUpdate();
void TetherBall_LateUpdate();
void TetherBall_Draw();
void TetherBall_Setup(void* subtype);
void TetherBall_StageLoad();
void TetherBall_GetAttributes();

#endif //!OBJ_TETHERBALL_H

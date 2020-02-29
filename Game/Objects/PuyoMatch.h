#ifndef OBJ_PUYOMATCH_H
#define OBJ_PUYOMATCH_H

//Object Class
class PuyoMatch : Object {

};

//Entity Class
class EntityPuyoMatch : Entity {

};

//Entity Functions
void PuyoMatch_Update();
void PuyoMatch_EarlyUpdate();
void PuyoMatch_LateUpdate();
void PuyoMatch_Draw();
void PuyoMatch_Setup(void* subtype);
void PuyoMatch_StageLoad();
void PuyoMatch_GetAttributes();

#endif //!OBJ_PUYOMATCH_H

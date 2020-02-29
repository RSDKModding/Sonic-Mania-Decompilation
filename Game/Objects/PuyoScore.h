#ifndef OBJ_PUYOSCORE_H
#define OBJ_PUYOSCORE_H

//Object Class
class PuyoScore : Object {

};

//Entity Class
class EntityPuyoScore : Entity {

};

//Entity Functions
void PuyoScore_Update();
void PuyoScore_EarlyUpdate();
void PuyoScore_LateUpdate();
void PuyoScore_Draw();
void PuyoScore_Setup(void* subtype);
void PuyoScore_StageLoad();
void PuyoScore_GetAttributes();

#endif //!OBJ_PUYOSCORE_H

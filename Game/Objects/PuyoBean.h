#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

//Object Class
class PuyoBean : Object {

};

//Entity Class
class EntityPuyoBean : Entity {

};

//Entity Functions
void PuyoBean_Update();
void PuyoBean_EarlyUpdate();
void PuyoBean_LateUpdate();
void PuyoBean_Draw();
void PuyoBean_Setup(void* subtype);
void PuyoBean_StageLoad();
void PuyoBean_GetAttributes();

#endif //!OBJ_PUYOBEAN_H

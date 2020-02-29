#ifndef OBJ_PUYOINDICATOR_H
#define OBJ_PUYOINDICATOR_H

//Object Class
class PuyoIndicator : Object {

};

//Entity Class
class EntityPuyoIndicator : Entity {

};

//Entity Functions
void PuyoIndicator_Update();
void PuyoIndicator_EarlyUpdate();
void PuyoIndicator_LateUpdate();
void PuyoIndicator_Draw();
void PuyoIndicator_Setup(void* subtype);
void PuyoIndicator_StageLoad();
void PuyoIndicator_GetAttributes();

#endif //!OBJ_PUYOINDICATOR_H

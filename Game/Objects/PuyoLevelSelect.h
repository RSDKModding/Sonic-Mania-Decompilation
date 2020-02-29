#ifndef OBJ_PUYOLEVELSELECT_H
#define OBJ_PUYOLEVELSELECT_H

//Object Class
class PuyoLevelSelect : Object {

};

//Entity Class
class EntityPuyoLevelSelect : Entity {

};

//Entity Functions
void PuyoLevelSelect_Update();
void PuyoLevelSelect_EarlyUpdate();
void PuyoLevelSelect_LateUpdate();
void PuyoLevelSelect_Draw();
void PuyoLevelSelect_Setup(void* subtype);
void PuyoLevelSelect_StageLoad();
void PuyoLevelSelect_GetAttributes();

#endif //!OBJ_PUYOLEVELSELECT_H

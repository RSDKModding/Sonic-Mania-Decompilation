#ifndef OBJ_PUYOLEVELSELECT_H
#define OBJ_PUYOLEVELSELECT_H

#include "../../SonicMania.h"

//Object Class
class ObjectPuyoLevelSelect : public Object {
public:

};

//Entity Class
class EntityPuyoLevelSelect : public Entity {
public:

};

//Object Entity
ObjectPuyoLevelSelect PuyoLevelSelect;

//Entity Functions
void PuyoLevelSelect_Update();
void PuyoLevelSelect_EarlyUpdate();
void PuyoLevelSelect_LateUpdate();
void PuyoLevelSelect_Draw();
void PuyoLevelSelect_Setup(void* subtype);
void PuyoLevelSelect_StageLoad();
void PuyoLevelSelect_GetAttributes();

#endif //!OBJ_PUYOLEVELSELECT_H

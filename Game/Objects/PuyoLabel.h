#ifndef OBJ_PUYOLABEL_H
#define OBJ_PUYOLABEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectPuyoLabel : public Object {
public:

};

//Entity Class
class EntityPuyoLabel : public Entity {
public:

};

//Object Entity
ObjectPuyoLabel PuyoLabel;

//Entity Functions
void PuyoLabel_Update();
void PuyoLabel_EarlyUpdate();
void PuyoLabel_LateUpdate();
void PuyoLabel_Draw();
void PuyoLabel_Setup(void* subtype);
void PuyoLabel_StageLoad();
void PuyoLabel_GetAttributes();

#endif //!OBJ_PUYOLABEL_H

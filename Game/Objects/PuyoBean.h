#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectPuyoBean : public Object {
public:

};

//Entity Class
class EntityPuyoBean : public Entity {
public:

};

//Object Entity
ObjectPuyoBean PuyoBean;

//Entity Functions
void PuyoBean_Update();
void PuyoBean_EarlyUpdate();
void PuyoBean_LateUpdate();
void PuyoBean_Draw();
void PuyoBean_Setup(void* subtype);
void PuyoBean_StageLoad();
void PuyoBean_GetAttributes();

#endif //!OBJ_PUYOBEAN_H

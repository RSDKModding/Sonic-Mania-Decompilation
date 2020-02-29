#ifndef OBJ_PUYOMATCH_H
#define OBJ_PUYOMATCH_H

#include "../../SonicMania.h"

//Object Class
class ObjectPuyoMatch : public Object {
public:

};

//Entity Class
class EntityPuyoMatch : public Entity {
public:

};

//Object Entity
ObjectPuyoMatch PuyoMatch;

//Entity Functions
void PuyoMatch_Update();
void PuyoMatch_EarlyUpdate();
void PuyoMatch_LateUpdate();
void PuyoMatch_Draw();
void PuyoMatch_Setup(void* subtype);
void PuyoMatch_StageLoad();
void PuyoMatch_GetAttributes();

#endif //!OBJ_PUYOMATCH_H

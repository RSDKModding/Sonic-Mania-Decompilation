#ifndef OBJ_PUYOSCORE_H
#define OBJ_PUYOSCORE_H

#include "../../SonicMania.h"

//Object Class
class ObjectPuyoScore : public Object {
public:

};

//Entity Class
class EntityPuyoScore : public Entity {
public:

};

//Object Entity
ObjectPuyoScore PuyoScore;

//Entity Functions
void PuyoScore_Update();
void PuyoScore_EarlyUpdate();
void PuyoScore_LateUpdate();
void PuyoScore_Draw();
void PuyoScore_Setup(void* subtype);
void PuyoScore_StageLoad();
void PuyoScore_GetAttributes();

#endif //!OBJ_PUYOSCORE_H

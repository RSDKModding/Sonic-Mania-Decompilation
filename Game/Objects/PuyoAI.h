#ifndef OBJ_PUYOAI_H
#define OBJ_PUYOAI_H

#include "../../SonicMania.h"

//Object Class
class ObjectPuyoAI : public Object {
public:

};

//Entity Class
class EntityPuyoAI : public Entity {
public:

};

//Object Entity
ObjectPuyoAI PuyoAI;

//Entity Functions
void PuyoAI_Update();
void PuyoAI_EarlyUpdate();
void PuyoAI_LateUpdate();
void PuyoAI_Draw();
void PuyoAI_Setup(void* subtype);
void PuyoAI_StageLoad();
void PuyoAI_GetAttributes();

#endif //!OBJ_PUYOAI_H

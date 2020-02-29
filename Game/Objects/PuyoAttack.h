#ifndef OBJ_PUYOATTACK_H
#define OBJ_PUYOATTACK_H

#include "../../SonicMania.h"

//Object Class
class ObjectPuyoAttack : public Object {
public:

};

//Entity Class
class EntityPuyoAttack : public Entity {
public:

};

//Object Entity
ObjectPuyoAttack PuyoAttack;

//Entity Functions
void PuyoAttack_Update();
void PuyoAttack_EarlyUpdate();
void PuyoAttack_LateUpdate();
void PuyoAttack_Draw();
void PuyoAttack_Setup(void* subtype);
void PuyoAttack_StageLoad();
void PuyoAttack_GetAttributes();

#endif //!OBJ_PUYOATTACK_H

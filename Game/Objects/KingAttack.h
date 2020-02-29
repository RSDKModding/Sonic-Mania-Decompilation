#ifndef OBJ_KINGATTACK_H
#define OBJ_KINGATTACK_H

#include "../../SonicMania.h"

//Object Class
class ObjectKingAttack : public Object {
public:

};

//Entity Class
class EntityKingAttack : public Entity {
public:

};

//Object Entity
ObjectKingAttack KingAttack;

//Entity Functions
void KingAttack_Update();
void KingAttack_EarlyUpdate();
void KingAttack_LateUpdate();
void KingAttack_Draw();
void KingAttack_Setup(void* subtype);
void KingAttack_StageLoad();
void KingAttack_GetAttributes();

#endif //!OBJ_KINGATTACK_H

#ifndef OBJ_PKINGATTACK_H
#define OBJ_PKINGATTACK_H

#include "../../SonicMania.h"

//Object Class
class ObjectPKingAttack : public Object {
public:

};

//Entity Class
class EntityPKingAttack : public Entity {
public:

};

//Object Entity
ObjectPKingAttack PKingAttack;

//Entity Functions
void PKingAttack_Update();
void PKingAttack_EarlyUpdate();
void PKingAttack_LateUpdate();
void PKingAttack_Draw();
void PKingAttack_Setup(void* subtype);
void PKingAttack_StageLoad();
void PKingAttack_GetAttributes();

#endif //!OBJ_PKINGATTACK_H

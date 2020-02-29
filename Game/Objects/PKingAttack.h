#ifndef OBJ_PKINGATTACK_H
#define OBJ_PKINGATTACK_H

//Object Class
class PKingAttack : Object {

};

//Entity Class
class EntityPKingAttack : Entity {

};

//Entity Functions
void PKingAttack_Update();
void PKingAttack_EarlyUpdate();
void PKingAttack_LateUpdate();
void PKingAttack_Draw();
void PKingAttack_Setup(void* subtype);
void PKingAttack_StageLoad();
void PKingAttack_GetAttributes();

#endif //!OBJ_PKINGATTACK_H

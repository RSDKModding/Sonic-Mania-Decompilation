#ifndef OBJ_PKINGATTACK_H
#define OBJ_PKINGATTACK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPKingAttack : Object {

};

// Entity Class
struct EntityPKingAttack : Entity {

};

// Object Entity
extern ObjectPKingAttack PKingAttack;

// Standard Entity Events
void PKingAttack_Update();
void PKingAttack_LateUpdate();
void PKingAttack_StaticUpdate();
void PKingAttack_Draw();
void PKingAttack_Create(void* data);
void PKingAttack_StageLoad();
void PKingAttack_EditorDraw();
void PKingAttack_EditorLoad();
void PKingAttack_Serialize();

// Extra Entity Functions


#endif //!OBJ_PKINGATTACK_H

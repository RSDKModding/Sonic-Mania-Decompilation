#ifndef OBJ_KINGATTACK_H
#define OBJ_KINGATTACK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectKingAttack : Object{

};

// Entity Class
struct EntityKingAttack : Entity {

};

// Object Struct
extern ObjectKingAttack *KingAttack;

// Standard Entity Events
void KingAttack_Update();
void KingAttack_LateUpdate();
void KingAttack_StaticUpdate();
void KingAttack_Draw();
void KingAttack_Create(void* data);
void KingAttack_StageLoad();
void KingAttack_EditorDraw();
void KingAttack_EditorLoad();
void KingAttack_Serialize();

// Extra Entity Functions


#endif //!OBJ_KINGATTACK_H

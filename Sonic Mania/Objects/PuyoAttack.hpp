#ifndef OBJ_PUYOATTACK_H
#define OBJ_PUYOATTACK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoAttack : Object {

};

// Entity Class
struct EntityPuyoAttack : Entity {

};

// Object Struct
extern ObjectPuyoAttack *PuyoAttack;

// Standard Entity Events
void PuyoAttack_Update();
void PuyoAttack_LateUpdate();
void PuyoAttack_StaticUpdate();
void PuyoAttack_Draw();
void PuyoAttack_Create(void* data);
void PuyoAttack_StageLoad();
void PuyoAttack_EditorDraw();
void PuyoAttack_EditorLoad();
void PuyoAttack_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOATTACK_H

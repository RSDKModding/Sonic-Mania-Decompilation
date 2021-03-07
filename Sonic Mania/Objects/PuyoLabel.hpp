#ifndef OBJ_PUYOLABEL_H
#define OBJ_PUYOLABEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoLabel : Object {

};

// Entity Class
struct EntityPuyoLabel : Entity {

};

// Object Struct
extern ObjectPuyoLabel *PuyoLabel;

// Standard Entity Events
void PuyoLabel_Update();
void PuyoLabel_LateUpdate();
void PuyoLabel_StaticUpdate();
void PuyoLabel_Draw();
void PuyoLabel_Create(void* data);
void PuyoLabel_StageLoad();
void PuyoLabel_EditorDraw();
void PuyoLabel_EditorLoad();
void PuyoLabel_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOLABEL_H

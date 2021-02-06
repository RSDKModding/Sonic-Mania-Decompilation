#ifndef OBJ_PUYOAI_H
#define OBJ_PUYOAI_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoAI : Object {

};

// Entity Class
struct EntityPuyoAI : Entity {

};

// Object Entity
extern ObjectPuyoAI PuyoAI;

// Standard Entity Events
void PuyoAI_Update();
void PuyoAI_LateUpdate();
void PuyoAI_StaticUpdate();
void PuyoAI_Draw();
void PuyoAI_Create(void* data);
void PuyoAI_StageLoad();
void PuyoAI_EditorDraw();
void PuyoAI_EditorLoad();
void PuyoAI_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOAI_H

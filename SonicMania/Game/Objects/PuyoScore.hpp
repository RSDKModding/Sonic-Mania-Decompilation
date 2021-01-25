#ifndef OBJ_PUYOSCORE_H
#define OBJ_PUYOSCORE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoScore : Object {

};

// Entity Class
struct EntityPuyoScore : Entity {

};

// Object Entity
extern ObjectPuyoScore PuyoScore;

// Standard Entity Events
void PuyoScore_Update();
void PuyoScore_LateUpdate();
void PuyoScore_StaticUpdate();
void PuyoScore_Draw();
void PuyoScore_Create(void* data);
void PuyoScore_StageLoad();
void PuyoScore_EditorDraw();
void PuyoScore_EditorLoad();
void PuyoScore_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOSCORE_H

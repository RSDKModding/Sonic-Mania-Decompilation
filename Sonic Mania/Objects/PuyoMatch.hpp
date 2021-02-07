#ifndef OBJ_PUYOMATCH_H
#define OBJ_PUYOMATCH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoMatch : Object{

};

// Entity Class
struct EntityPuyoMatch : Entity {

};

// Object Struct
extern ObjectPuyoMatch *PuyoMatch;

// Standard Entity Events
void PuyoMatch_Update();
void PuyoMatch_LateUpdate();
void PuyoMatch_StaticUpdate();
void PuyoMatch_Draw();
void PuyoMatch_Create(void* data);
void PuyoMatch_StageLoad();
void PuyoMatch_EditorDraw();
void PuyoMatch_EditorLoad();
void PuyoMatch_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOMATCH_H

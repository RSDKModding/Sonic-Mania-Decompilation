#ifndef OBJ_PUYOINDICATOR_H
#define OBJ_PUYOINDICATOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoIndicator : Object{

};

// Entity Class
struct EntityPuyoIndicator : Entity {

};

// Object Struct
extern ObjectPuyoIndicator *PuyoIndicator;

// Standard Entity Events
void PuyoIndicator_Update();
void PuyoIndicator_LateUpdate();
void PuyoIndicator_StaticUpdate();
void PuyoIndicator_Draw();
void PuyoIndicator_Create(void* data);
void PuyoIndicator_StageLoad();
void PuyoIndicator_EditorDraw();
void PuyoIndicator_EditorLoad();
void PuyoIndicator_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOINDICATOR_H

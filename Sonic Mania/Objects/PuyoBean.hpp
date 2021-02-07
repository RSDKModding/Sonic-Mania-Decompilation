#ifndef OBJ_PUYOBEAN_H
#define OBJ_PUYOBEAN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoBean : Object{

};

// Entity Class
struct EntityPuyoBean : Entity {

};

// Object Struct
extern ObjectPuyoBean *PuyoBean;

// Standard Entity Events
void PuyoBean_Update();
void PuyoBean_LateUpdate();
void PuyoBean_StaticUpdate();
void PuyoBean_Draw();
void PuyoBean_Create(void* data);
void PuyoBean_StageLoad();
void PuyoBean_EditorDraw();
void PuyoBean_EditorLoad();
void PuyoBean_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOBEAN_H

#ifndef OBJ_PUYOLEVELSELECT_H
#define OBJ_PUYOLEVELSELECT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoLevelSelect : Object{

};

// Entity Class
struct EntityPuyoLevelSelect : Entity {

};

// Object Struct
extern ObjectPuyoLevelSelect *PuyoLevelSelect;

// Standard Entity Events
void PuyoLevelSelect_Update();
void PuyoLevelSelect_LateUpdate();
void PuyoLevelSelect_StaticUpdate();
void PuyoLevelSelect_Draw();
void PuyoLevelSelect_Create(void* data);
void PuyoLevelSelect_StageLoad();
void PuyoLevelSelect_EditorDraw();
void PuyoLevelSelect_EditorLoad();
void PuyoLevelSelect_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOLEVELSELECT_H

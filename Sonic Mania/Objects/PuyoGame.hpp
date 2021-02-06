#ifndef OBJ_PUYOGAME_H
#define OBJ_PUYOGAME_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPuyoGame : Object {

};

// Entity Class
struct EntityPuyoGame : Entity {

};

// Object Entity
extern ObjectPuyoGame PuyoGame;

// Standard Entity Events
void PuyoGame_Update();
void PuyoGame_LateUpdate();
void PuyoGame_StaticUpdate();
void PuyoGame_Draw();
void PuyoGame_Create(void* data);
void PuyoGame_StageLoad();
void PuyoGame_EditorDraw();
void PuyoGame_EditorLoad();
void PuyoGame_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOGAME_H

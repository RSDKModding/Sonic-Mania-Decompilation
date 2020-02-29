#ifndef OBJ_PUYOGAME_H
#define OBJ_PUYOGAME_H

#include "../../SonicMania.h"

//Object Class
class ObjectPuyoGame : public Object {
public:

};

//Entity Class
class EntityPuyoGame : public Entity {
public:

};

//Object Entity
ObjectPuyoGame PuyoGame;

//Entity Functions
void PuyoGame_Update();
void PuyoGame_EarlyUpdate();
void PuyoGame_LateUpdate();
void PuyoGame_Draw();
void PuyoGame_Setup(void* subtype);
void PuyoGame_StageLoad();
void PuyoGame_GetAttributes();

#endif //!OBJ_PUYOGAME_H

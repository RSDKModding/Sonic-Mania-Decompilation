#ifndef OBJ_PUYOGAME_H
#define OBJ_PUYOGAME_H

//Object Class
class PuyoGame : Object {

};

//Entity Class
class EntityPuyoGame : Entity {

};

//Entity Functions
void PuyoGame_Update();
void PuyoGame_EarlyUpdate();
void PuyoGame_LateUpdate();
void PuyoGame_Draw();
void PuyoGame_Setup(void* subtype);
void PuyoGame_StageLoad();
void PuyoGame_GetAttributes();

#endif //!OBJ_PUYOGAME_H

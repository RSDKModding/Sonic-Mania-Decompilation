#ifndef OBJ_PUYOAI_H
#define OBJ_PUYOAI_H

//Object Class
class PuyoAI : Object {

};

//Entity Class
class EntityPuyoAI : Entity {

};

//Entity Functions
void PuyoAI_Update();
void PuyoAI_EarlyUpdate();
void PuyoAI_LateUpdate();
void PuyoAI_Draw();
void PuyoAI_Setup(void* subtype);
void PuyoAI_StageLoad();
void PuyoAI_GetAttributes();

#endif //!OBJ_PUYOAI_H

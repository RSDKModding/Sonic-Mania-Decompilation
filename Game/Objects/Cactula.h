#ifndef OBJ_CACTULA_H
#define OBJ_CACTULA_H

//Object Class
class Cactula : Object {

};

//Entity Class
class EntityCactula : Entity {

};

//Entity Functions
void Cactula_Update();
void Cactula_EarlyUpdate();
void Cactula_LateUpdate();
void Cactula_Draw();
void Cactula_Setup(void* subtype);
void Cactula_StageLoad();
void Cactula_GetAttributes();

#endif //!OBJ_CACTULA_H

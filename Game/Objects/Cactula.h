#ifndef OBJ_CACTULA_H
#define OBJ_CACTULA_H

#include "../../SonicMania.h"

//Object Class
class ObjectCactula : public Object {
public:

};

//Entity Class
class EntityCactula : public Entity {
public:

};

//Object Entity
ObjectCactula Cactula;

//Entity Functions
void Cactula_Update();
void Cactula_EarlyUpdate();
void Cactula_LateUpdate();
void Cactula_Draw();
void Cactula_Setup(void* subtype);
void Cactula_StageLoad();
void Cactula_GetAttributes();

#endif //!OBJ_CACTULA_H

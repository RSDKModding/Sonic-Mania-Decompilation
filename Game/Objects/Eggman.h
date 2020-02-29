#ifndef OBJ_EGGMAN_H
#define OBJ_EGGMAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectEggman : public Object {
public:

};

//Entity Class
class EntityEggman : public Entity {
public:

};

//Object Entity
ObjectEggman Eggman;

//Entity Functions
void Eggman_Update();
void Eggman_EarlyUpdate();
void Eggman_LateUpdate();
void Eggman_Draw();
void Eggman_Setup(void* subtype);
void Eggman_StageLoad();
void Eggman_GetAttributes();

#endif //!OBJ_EGGMAN_H

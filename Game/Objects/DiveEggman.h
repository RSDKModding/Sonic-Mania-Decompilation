#ifndef OBJ_DIVEEGGMAN_H
#define OBJ_DIVEEGGMAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectDiveEggman : public Object {
public:

};

//Entity Class
class EntityDiveEggman : public Entity {
public:

};

//Object Entity
ObjectDiveEggman DiveEggman;

//Entity Functions
void DiveEggman_Update();
void DiveEggman_EarlyUpdate();
void DiveEggman_LateUpdate();
void DiveEggman_Draw();
void DiveEggman_Setup(void* subtype);
void DiveEggman_StageLoad();
void DiveEggman_GetAttributes();

#endif //!OBJ_DIVEEGGMAN_H

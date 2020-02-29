#ifndef OBJ_ANIMALHBH_H
#define OBJ_ANIMALHBH_H

#include "../../SonicMania.h"

//Object Class
class ObjectAnimalHBH : public Object {
public:

};

//Entity Class
class EntityAnimalHBH : public Entity {
public:

};

//Object Entity
ObjectAnimalHBH AnimalHBH;

//Entity Functions
void AnimalHBH_Update();
void AnimalHBH_EarlyUpdate();
void AnimalHBH_LateUpdate();
void AnimalHBH_Draw();
void AnimalHBH_Setup(void* subtype);
void AnimalHBH_StageLoad();
void AnimalHBH_GetAttributes();

#endif //!OBJ_ANIMALHBH_H

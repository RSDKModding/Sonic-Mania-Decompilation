#ifndef OBJ_STAIRCASE_H
#define OBJ_STAIRCASE_H

#include "../../SonicMania.h"

//Object Class
class ObjectStaircase : public Object {
public:

};

//Entity Class
class EntityStaircase : public Entity {
public:

};

//Object Entity
ObjectStaircase Staircase;

//Entity Functions
void Staircase_Update();
void Staircase_EarlyUpdate();
void Staircase_LateUpdate();
void Staircase_Draw();
void Staircase_Setup(void* subtype);
void Staircase_StageLoad();
void Staircase_GetAttributes();

#endif //!OBJ_STAIRCASE_H

#ifndef OBJ_BLOOMINATOR_H
#define OBJ_BLOOMINATOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectBloominator : public Object {
public:

};

//Entity Class
class EntityBloominator : public Entity {
public:

};

//Object Entity
ObjectBloominator Bloominator;

//Entity Functions
void Bloominator_Update();
void Bloominator_EarlyUpdate();
void Bloominator_LateUpdate();
void Bloominator_Draw();
void Bloominator_Setup(void* subtype);
void Bloominator_StageLoad();
void Bloominator_GetAttributes();

#endif //!OBJ_BLOOMINATOR_H

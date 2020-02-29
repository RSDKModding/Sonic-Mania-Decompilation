#ifndef OBJ_DANGO_H
#define OBJ_DANGO_H

#include "../../SonicMania.h"

//Object Class
class ObjectDango : public Object {
public:

};

//Entity Class
class EntityDango : public Entity {
public:

};

//Object Entity
ObjectDango Dango;

//Entity Functions
void Dango_Update();
void Dango_EarlyUpdate();
void Dango_LateUpdate();
void Dango_Draw();
void Dango_Setup(void* subtype);
void Dango_StageLoad();
void Dango_GetAttributes();

#endif //!OBJ_DANGO_H

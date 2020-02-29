#ifndef OBJ_HONKYTONK_H
#define OBJ_HONKYTONK_H

#include "../../SonicMania.h"

//Object Class
class ObjectHonkytonk : public Object {
public:

};

//Entity Class
class EntityHonkytonk : public Entity {
public:

};

//Object Entity
ObjectHonkytonk Honkytonk;

//Entity Functions
void Honkytonk_Update();
void Honkytonk_EarlyUpdate();
void Honkytonk_LateUpdate();
void Honkytonk_Draw();
void Honkytonk_Setup(void* subtype);
void Honkytonk_StageLoad();
void Honkytonk_GetAttributes();

#endif //!OBJ_HONKYTONK_H

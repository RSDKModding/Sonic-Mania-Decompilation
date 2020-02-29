#ifndef OBJ_VULTRON_H
#define OBJ_VULTRON_H

#include "../../SonicMania.h"

//Object Class
class ObjectVultron : public Object {
public:

};

//Entity Class
class EntityVultron : public Entity {
public:

};

//Object Entity
ObjectVultron Vultron;

//Entity Functions
void Vultron_Update();
void Vultron_EarlyUpdate();
void Vultron_LateUpdate();
void Vultron_Draw();
void Vultron_Setup(void* subtype);
void Vultron_StageLoad();
void Vultron_GetAttributes();

#endif //!OBJ_VULTRON_H

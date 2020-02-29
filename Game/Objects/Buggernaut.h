#ifndef OBJ_BUGGERNAUT_H
#define OBJ_BUGGERNAUT_H

#include "../../SonicMania.h"

//Object Class
class ObjectBuggernaut : public Object {
public:

};

//Entity Class
class EntityBuggernaut : public Entity {
public:

};

//Object Entity
ObjectBuggernaut Buggernaut;

//Entity Functions
void Buggernaut_Update();
void Buggernaut_EarlyUpdate();
void Buggernaut_LateUpdate();
void Buggernaut_Draw();
void Buggernaut_Setup(void* subtype);
void Buggernaut_StageLoad();
void Buggernaut_GetAttributes();

#endif //!OBJ_BUGGERNAUT_H

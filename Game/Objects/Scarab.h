#ifndef OBJ_SCARAB_H
#define OBJ_SCARAB_H

#include "../../SonicMania.h"

//Object Class
class ObjectScarab : public Object {
public:

};

//Entity Class
class EntityScarab : public Entity {
public:

};

//Object Entity
ObjectScarab Scarab;

//Entity Functions
void Scarab_Update();
void Scarab_EarlyUpdate();
void Scarab_LateUpdate();
void Scarab_Draw();
void Scarab_Setup(void* subtype);
void Scarab_StageLoad();
void Scarab_GetAttributes();

#endif //!OBJ_SCARAB_H

#ifndef OBJ_BUNGEE_H
#define OBJ_BUNGEE_H

#include "../../SonicMania.h"

//Object Class
class ObjectBungee : public Object {
public:

};

//Entity Class
class EntityBungee : public Entity {
public:

};

//Object Entity
ObjectBungee Bungee;

//Entity Functions
void Bungee_Update();
void Bungee_EarlyUpdate();
void Bungee_LateUpdate();
void Bungee_Draw();
void Bungee_Setup(void* subtype);
void Bungee_StageLoad();
void Bungee_GetAttributes();

#endif //!OBJ_BUNGEE_H

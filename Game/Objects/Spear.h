#ifndef OBJ_SPEAR_H
#define OBJ_SPEAR_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpear : public Object {
public:

};

//Entity Class
class EntitySpear : public Entity {
public:

};

//Object Entity
ObjectSpear Spear;

//Entity Functions
void Spear_Update();
void Spear_EarlyUpdate();
void Spear_LateUpdate();
void Spear_Draw();
void Spear_Setup(void* subtype);
void Spear_StageLoad();
void Spear_GetAttributes();

#endif //!OBJ_SPEAR_H

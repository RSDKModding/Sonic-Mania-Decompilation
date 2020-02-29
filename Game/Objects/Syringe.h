#ifndef OBJ_SYRINGE_H
#define OBJ_SYRINGE_H

#include "../../SonicMania.h"

//Object Class
class ObjectSyringe : public Object {
public:

};

//Entity Class
class EntitySyringe : public Entity {
public:

};

//Object Entity
ObjectSyringe Syringe;

//Entity Functions
void Syringe_Update();
void Syringe_EarlyUpdate();
void Syringe_LateUpdate();
void Syringe_Draw();
void Syringe_Setup(void* subtype);
void Syringe_StageLoad();
void Syringe_GetAttributes();

#endif //!OBJ_SYRINGE_H

#ifndef OBJ_PROPELLER_H
#define OBJ_PROPELLER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPropeller : public Object {
public:

};

//Entity Class
class EntityPropeller : public Entity {
public:

};

//Object Entity
ObjectPropeller Propeller;

//Entity Functions
void Propeller_Update();
void Propeller_EarlyUpdate();
void Propeller_LateUpdate();
void Propeller_Draw();
void Propeller_Setup(void* subtype);
void Propeller_StageLoad();
void Propeller_GetAttributes();

#endif //!OBJ_PROPELLER_H

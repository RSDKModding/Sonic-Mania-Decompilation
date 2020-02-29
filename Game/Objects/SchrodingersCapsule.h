#ifndef OBJ_SCHRODINGERSCAPSULE_H
#define OBJ_SCHRODINGERSCAPSULE_H

#include "../../SonicMania.h"

//Object Class
class ObjectSchrodingersCapsule : public Object {
public:

};

//Entity Class
class EntitySchrodingersCapsule : public Entity {
public:

};

//Object Entity
ObjectSchrodingersCapsule SchrodingersCapsule;

//Entity Functions
void SchrodingersCapsule_Update();
void SchrodingersCapsule_EarlyUpdate();
void SchrodingersCapsule_LateUpdate();
void SchrodingersCapsule_Draw();
void SchrodingersCapsule_Setup(void* subtype);
void SchrodingersCapsule_StageLoad();
void SchrodingersCapsule_GetAttributes();

#endif //!OBJ_SCHRODINGERSCAPSULE_H

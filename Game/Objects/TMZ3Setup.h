#ifndef OBJ_TMZ3SETUP_H
#define OBJ_TMZ3SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectTMZ3Setup : public Object {
public:

};

//Entity Class
class EntityTMZ3Setup : public Entity {
public:

};

//Object Entity
ObjectTMZ3Setup TMZ3Setup;

//Entity Functions
void TMZ3Setup_Update();
void TMZ3Setup_EarlyUpdate();
void TMZ3Setup_LateUpdate();
void TMZ3Setup_Draw();
void TMZ3Setup_Setup(void* subtype);
void TMZ3Setup_StageLoad();
void TMZ3Setup_GetAttributes();

#endif //!OBJ_TMZ3SETUP_H

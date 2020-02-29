#ifndef OBJ_GIANTPISTOL_H
#define OBJ_GIANTPISTOL_H

#include "../../SonicMania.h"

//Object Class
class ObjectGiantPistol : public Object {
public:

};

//Entity Class
class EntityGiantPistol : public Entity {
public:

};

//Object Entity
ObjectGiantPistol GiantPistol;

//Entity Functions
void GiantPistol_Update();
void GiantPistol_EarlyUpdate();
void GiantPistol_LateUpdate();
void GiantPistol_Draw();
void GiantPistol_Setup(void* subtype);
void GiantPistol_StageLoad();
void GiantPistol_GetAttributes();

#endif //!OBJ_GIANTPISTOL_H

#ifndef OBJ_PBL_SETUP_H
#define OBJ_PBL_SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_Setup : public Object {
public:

};

//Entity Class
class EntityPBL_Setup : public Entity {
public:

};

//Object Entity
ObjectPBL_Setup PBL_Setup;

//Entity Functions
void PBL_Setup_Update();
void PBL_Setup_EarlyUpdate();
void PBL_Setup_LateUpdate();
void PBL_Setup_Draw();
void PBL_Setup_Setup(void* subtype);
void PBL_Setup_StageLoad();
void PBL_Setup_GetAttributes();

#endif //!OBJ_PBL_SETUP_H

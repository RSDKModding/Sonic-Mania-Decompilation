#ifndef OBJ_PBL_BUMPER_H
#define OBJ_PBL_BUMPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_Bumper : public Object {
public:

};

//Entity Class
class EntityPBL_Bumper : public Entity {
public:

};

//Object Entity
ObjectPBL_Bumper PBL_Bumper;

//Entity Functions
void PBL_Bumper_Update();
void PBL_Bumper_EarlyUpdate();
void PBL_Bumper_LateUpdate();
void PBL_Bumper_Draw();
void PBL_Bumper_Setup(void* subtype);
void PBL_Bumper_StageLoad();
void PBL_Bumper_GetAttributes();

#endif //!OBJ_PBL_BUMPER_H

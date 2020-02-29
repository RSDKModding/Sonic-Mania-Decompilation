#ifndef OBJ_PBL_CRANE_H
#define OBJ_PBL_CRANE_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_Crane : public Object {
public:

};

//Entity Class
class EntityPBL_Crane : public Entity {
public:

};

//Object Entity
ObjectPBL_Crane PBL_Crane;

//Entity Functions
void PBL_Crane_Update();
void PBL_Crane_EarlyUpdate();
void PBL_Crane_LateUpdate();
void PBL_Crane_Draw();
void PBL_Crane_Setup(void* subtype);
void PBL_Crane_StageLoad();
void PBL_Crane_GetAttributes();

#endif //!OBJ_PBL_CRANE_H

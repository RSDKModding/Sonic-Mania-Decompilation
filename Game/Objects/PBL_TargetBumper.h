#ifndef OBJ_PBL_TARGETBUMPER_H
#define OBJ_PBL_TARGETBUMPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_TargetBumper : public Object {
public:

};

//Entity Class
class EntityPBL_TargetBumper : public Entity {
public:

};

//Object Entity
ObjectPBL_TargetBumper PBL_TargetBumper;

//Entity Functions
void PBL_TargetBumper_Update();
void PBL_TargetBumper_EarlyUpdate();
void PBL_TargetBumper_LateUpdate();
void PBL_TargetBumper_Draw();
void PBL_TargetBumper_Setup(void* subtype);
void PBL_TargetBumper_StageLoad();
void PBL_TargetBumper_GetAttributes();

#endif //!OBJ_PBL_TARGETBUMPER_H

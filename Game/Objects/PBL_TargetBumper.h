#ifndef OBJ_PBL_TARGETBUMPER_H
#define OBJ_PBL_TARGETBUMPER_H

//Object Class
class PBL_TargetBumper : Object {

};

//Entity Class
class EntityPBL_TargetBumper : Entity {

};

//Entity Functions
void PBL_TargetBumper_Update();
void PBL_TargetBumper_EarlyUpdate();
void PBL_TargetBumper_LateUpdate();
void PBL_TargetBumper_Draw();
void PBL_TargetBumper_Setup(void* subtype);
void PBL_TargetBumper_StageLoad();
void PBL_TargetBumper_GetAttributes();

#endif //!OBJ_PBL_TARGETBUMPER_H

#ifndef OBJ_CRIMSONEYE_H
#define OBJ_CRIMSONEYE_H

//Object Class
class CrimsonEye : Object {

};

//Entity Class
class EntityCrimsonEye : Entity {

};

//Entity Functions
void CrimsonEye_Update();
void CrimsonEye_EarlyUpdate();
void CrimsonEye_LateUpdate();
void CrimsonEye_Draw();
void CrimsonEye_Setup(void* subtype);
void CrimsonEye_StageLoad();
void CrimsonEye_GetAttributes();

#endif //!OBJ_CRIMSONEYE_H

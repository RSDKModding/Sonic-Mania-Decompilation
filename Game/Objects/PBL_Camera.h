#ifndef OBJ_PBL_CAMERA_H
#define OBJ_PBL_CAMERA_H

//Object Class
class PBL_Camera : Object {

};

//Entity Class
class EntityPBL_Camera : Entity {

};

//Entity Functions
void PBL_Camera_Update();
void PBL_Camera_EarlyUpdate();
void PBL_Camera_LateUpdate();
void PBL_Camera_Draw();
void PBL_Camera_Setup(void* subtype);
void PBL_Camera_StageLoad();
void PBL_Camera_GetAttributes();

#endif //!OBJ_PBL_CAMERA_H

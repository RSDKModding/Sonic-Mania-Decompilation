#ifndef OBJ_PBL_CAMERA_H
#define OBJ_PBL_CAMERA_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_Camera : public Object {
public:

};

//Entity Class
class EntityPBL_Camera : public Entity {
public:

};

//Object Entity
ObjectPBL_Camera PBL_Camera;

//Entity Functions
void PBL_Camera_Update();
void PBL_Camera_EarlyUpdate();
void PBL_Camera_LateUpdate();
void PBL_Camera_Draw();
void PBL_Camera_Setup(void* subtype);
void PBL_Camera_StageLoad();
void PBL_Camera_GetAttributes();

#endif //!OBJ_PBL_CAMERA_H

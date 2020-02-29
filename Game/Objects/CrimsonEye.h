#ifndef OBJ_CRIMSONEYE_H
#define OBJ_CRIMSONEYE_H

#include "../../SonicMania.h"

//Object Class
class ObjectCrimsonEye : public Object {
public:

};

//Entity Class
class EntityCrimsonEye : public Entity {
public:

};

//Object Entity
ObjectCrimsonEye CrimsonEye;

//Entity Functions
void CrimsonEye_Update();
void CrimsonEye_EarlyUpdate();
void CrimsonEye_LateUpdate();
void CrimsonEye_Draw();
void CrimsonEye_Setup(void* subtype);
void CrimsonEye_StageLoad();
void CrimsonEye_GetAttributes();

#endif //!OBJ_CRIMSONEYE_H

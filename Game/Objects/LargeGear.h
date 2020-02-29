#ifndef OBJ_LARGEGEAR_H
#define OBJ_LARGEGEAR_H

#include "../../SonicMania.h"

//Object Class
class ObjectLargeGear : public Object {
public:

};

//Entity Class
class EntityLargeGear : public Entity {
public:

};

//Object Entity
ObjectLargeGear LargeGear;

//Entity Functions
void LargeGear_Update();
void LargeGear_EarlyUpdate();
void LargeGear_LateUpdate();
void LargeGear_Draw();
void LargeGear_Setup(void* subtype);
void LargeGear_StageLoad();
void LargeGear_GetAttributes();

#endif //!OBJ_LARGEGEAR_H

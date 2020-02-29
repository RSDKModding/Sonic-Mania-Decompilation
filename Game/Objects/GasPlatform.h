#ifndef OBJ_GASPLATFORM_H
#define OBJ_GASPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectGasPlatform : public Object {
public:

};

//Entity Class
class EntityGasPlatform : public Entity {
public:

};

//Object Entity
ObjectGasPlatform GasPlatform;

//Entity Functions
void GasPlatform_Update();
void GasPlatform_EarlyUpdate();
void GasPlatform_LateUpdate();
void GasPlatform_Draw();
void GasPlatform_Setup(void* subtype);
void GasPlatform_StageLoad();
void GasPlatform_GetAttributes();

#endif //!OBJ_GASPLATFORM_H

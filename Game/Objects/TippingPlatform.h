#ifndef OBJ_TIPPINGPLATFORM_H
#define OBJ_TIPPINGPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectTippingPlatform : public Object {
public:

};

//Entity Class
class EntityTippingPlatform : public Entity {
public:

};

//Object Entity
ObjectTippingPlatform TippingPlatform;

//Entity Functions
void TippingPlatform_Update();
void TippingPlatform_EarlyUpdate();
void TippingPlatform_LateUpdate();
void TippingPlatform_Draw();
void TippingPlatform_Setup(void* subtype);
void TippingPlatform_StageLoad();
void TippingPlatform_GetAttributes();

#endif //!OBJ_TIPPINGPLATFORM_H

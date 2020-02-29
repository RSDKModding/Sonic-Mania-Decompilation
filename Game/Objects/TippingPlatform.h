#ifndef OBJ_TIPPINGPLATFORM_H
#define OBJ_TIPPINGPLATFORM_H

//Object Class
class TippingPlatform : Object {

};

//Entity Class
class EntityTippingPlatform : Entity {

};

//Entity Functions
void TippingPlatform_Update();
void TippingPlatform_EarlyUpdate();
void TippingPlatform_LateUpdate();
void TippingPlatform_Draw();
void TippingPlatform_Setup(void* subtype);
void TippingPlatform_StageLoad();
void TippingPlatform_GetAttributes();

#endif //!OBJ_TIPPINGPLATFORM_H

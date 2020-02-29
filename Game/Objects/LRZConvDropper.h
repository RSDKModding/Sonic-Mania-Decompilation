#ifndef OBJ_LRZCONVDROPPER_H
#define OBJ_LRZCONVDROPPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZConvDropper : public Object {
public:

};

//Entity Class
class EntityLRZConvDropper : public Entity {
public:

};

//Object Entity
ObjectLRZConvDropper LRZConvDropper;

//Entity Functions
void LRZConvDropper_Update();
void LRZConvDropper_EarlyUpdate();
void LRZConvDropper_LateUpdate();
void LRZConvDropper_Draw();
void LRZConvDropper_Setup(void* subtype);
void LRZConvDropper_StageLoad();
void LRZConvDropper_GetAttributes();

#endif //!OBJ_LRZCONVDROPPER_H

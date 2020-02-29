#ifndef OBJ_LRZCONVCONTROL_H
#define OBJ_LRZCONVCONTROL_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZConvControl : public Object {
public:

};

//Entity Class
class EntityLRZConvControl : public Entity {
public:

};

//Object Entity
ObjectLRZConvControl LRZConvControl;

//Entity Functions
void LRZConvControl_Update();
void LRZConvControl_EarlyUpdate();
void LRZConvControl_LateUpdate();
void LRZConvControl_Draw();
void LRZConvControl_Setup(void* subtype);
void LRZConvControl_StageLoad();
void LRZConvControl_GetAttributes();

#endif //!OBJ_LRZCONVCONTROL_H

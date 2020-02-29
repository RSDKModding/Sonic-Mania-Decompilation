#ifndef OBJ_LRZCONVCONTROL_H
#define OBJ_LRZCONVCONTROL_H

//Object Class
class LRZConvControl : Object {

};

//Entity Class
class EntityLRZConvControl : Entity {

};

//Entity Functions
void LRZConvControl_Update();
void LRZConvControl_EarlyUpdate();
void LRZConvControl_LateUpdate();
void LRZConvControl_Draw();
void LRZConvControl_Setup(void* subtype);
void LRZConvControl_StageLoad();
void LRZConvControl_GetAttributes();

#endif //!OBJ_LRZCONVCONTROL_H

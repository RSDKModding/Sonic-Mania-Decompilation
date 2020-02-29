#ifndef OBJ_LRZCONVDROPPER_H
#define OBJ_LRZCONVDROPPER_H

//Object Class
class LRZConvDropper : Object {

};

//Entity Class
class EntityLRZConvDropper : Entity {

};

//Entity Functions
void LRZConvDropper_Update();
void LRZConvDropper_EarlyUpdate();
void LRZConvDropper_LateUpdate();
void LRZConvDropper_Draw();
void LRZConvDropper_Setup(void* subtype);
void LRZConvDropper_StageLoad();
void LRZConvDropper_GetAttributes();

#endif //!OBJ_LRZCONVDROPPER_H

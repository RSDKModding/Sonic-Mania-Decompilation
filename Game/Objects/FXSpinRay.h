#ifndef OBJ_FXSPINRAY_H
#define OBJ_FXSPINRAY_H

//Object Class
class FXSpinRay : Object {

};

//Entity Class
class EntityFXSpinRay : Entity {

};

//Entity Functions
void FXSpinRay_Update();
void FXSpinRay_EarlyUpdate();
void FXSpinRay_LateUpdate();
void FXSpinRay_Draw();
void FXSpinRay_Setup(void* subtype);
void FXSpinRay_StageLoad();
void FXSpinRay_GetAttributes();

#endif //!OBJ_FXSPINRAY_H

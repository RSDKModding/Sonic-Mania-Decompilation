#ifndef OBJ_FXSPINRAY_H
#define OBJ_FXSPINRAY_H

#include "../../SonicMania.h"

//Object Class
class ObjectFXSpinRay : public Object {
public:

};

//Entity Class
class EntityFXSpinRay : public Entity {
public:

};

//Object Entity
ObjectFXSpinRay FXSpinRay;

//Entity Functions
void FXSpinRay_Update();
void FXSpinRay_EarlyUpdate();
void FXSpinRay_LateUpdate();
void FXSpinRay_Draw();
void FXSpinRay_Setup(void* subtype);
void FXSpinRay_StageLoad();
void FXSpinRay_GetAttributes();

#endif //!OBJ_FXSPINRAY_H

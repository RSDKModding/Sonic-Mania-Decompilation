#ifndef OBJ_FXSPINRAY_H
#define OBJ_FXSPINRAY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFXSpinRay : Object {

};

// Entity Class
struct EntityFXSpinRay : Entity {

};

// Object Entity
extern ObjectFXSpinRay FXSpinRay;

// Standard Entity Events
void FXSpinRay_Update();
void FXSpinRay_LateUpdate();
void FXSpinRay_StaticUpdate();
void FXSpinRay_Draw();
void FXSpinRay_Create(void* data);
void FXSpinRay_StageLoad();
void FXSpinRay_EditorDraw();
void FXSpinRay_EditorLoad();
void FXSpinRay_Serialize();

// Extra Entity Functions


#endif //!OBJ_FXSPINRAY_H

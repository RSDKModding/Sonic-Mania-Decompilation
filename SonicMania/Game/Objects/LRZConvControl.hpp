#ifndef OBJ_LRZCONVCONTROL_H
#define OBJ_LRZCONVCONTROL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZConvControl : Object {

};

// Entity Class
struct EntityLRZConvControl : Entity {

};

// Object Entity
extern ObjectLRZConvControl LRZConvControl;

// Standard Entity Events
void LRZConvControl_Update();
void LRZConvControl_LateUpdate();
void LRZConvControl_StaticUpdate();
void LRZConvControl_Draw();
void LRZConvControl_Create(void* data);
void LRZConvControl_StageLoad();
void LRZConvControl_EditorDraw();
void LRZConvControl_EditorLoad();
void LRZConvControl_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZCONVCONTROL_H

#ifndef OBJ_TIPPINGPLATFORM_H
#define OBJ_TIPPINGPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTippingPlatform : Object{

};

// Entity Class
struct EntityTippingPlatform : Entity {

};

// Object Struct
extern ObjectTippingPlatform *TippingPlatform;

// Standard Entity Events
void TippingPlatform_Update();
void TippingPlatform_LateUpdate();
void TippingPlatform_StaticUpdate();
void TippingPlatform_Draw();
void TippingPlatform_Create(void* data);
void TippingPlatform_StageLoad();
void TippingPlatform_EditorDraw();
void TippingPlatform_EditorLoad();
void TippingPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_TIPPINGPLATFORM_H

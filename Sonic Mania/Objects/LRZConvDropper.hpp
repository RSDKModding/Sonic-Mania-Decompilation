#ifndef OBJ_LRZCONVDROPPER_H
#define OBJ_LRZCONVDROPPER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZConvDropper : Object {

};

// Entity Class
struct EntityLRZConvDropper : Entity {

};

// Object Struct
extern ObjectLRZConvDropper *LRZConvDropper;

// Standard Entity Events
void LRZConvDropper_Update();
void LRZConvDropper_LateUpdate();
void LRZConvDropper_StaticUpdate();
void LRZConvDropper_Draw();
void LRZConvDropper_Create(void* data);
void LRZConvDropper_StageLoad();
void LRZConvDropper_EditorDraw();
void LRZConvDropper_EditorLoad();
void LRZConvDropper_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZCONVDROPPER_H

#ifndef OBJ_LRZCONVDROPPER_H
#define OBJ_LRZCONVDROPPER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZConvDropper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZConvDropper;

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

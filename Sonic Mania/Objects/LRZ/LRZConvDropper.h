#ifndef OBJ_LRZCONVDROPPER_H
#define OBJ_LRZCONVDROPPER_H

#include "SonicMania.h"

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
void LRZConvDropper_Update(void);
void LRZConvDropper_LateUpdate(void);
void LRZConvDropper_StaticUpdate(void);
void LRZConvDropper_Draw(void);
void LRZConvDropper_Create(void* data);
void LRZConvDropper_StageLoad(void);
void LRZConvDropper_EditorDraw(void);
void LRZConvDropper_EditorLoad(void);
void LRZConvDropper_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZCONVDROPPER_H

#ifndef OBJ_LRZCONVCONTROL_H
#define OBJ_LRZCONVCONTROL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZConvControl;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZConvControl;

// Object Struct
extern ObjectLRZConvControl *LRZConvControl;

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

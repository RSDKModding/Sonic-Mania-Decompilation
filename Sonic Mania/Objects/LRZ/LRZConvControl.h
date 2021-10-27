#ifndef OBJ_LRZCONVCONTROL_H
#define OBJ_LRZCONVCONTROL_H

#include "SonicMania.h"

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
void LRZConvControl_Update(void);
void LRZConvControl_LateUpdate(void);
void LRZConvControl_StaticUpdate(void);
void LRZConvControl_Draw(void);
void LRZConvControl_Create(void* data);
void LRZConvControl_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LRZConvControl_EditorDraw(void);
void LRZConvControl_EditorLoad(void);
#endif
void LRZConvControl_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZCONVCONTROL_H

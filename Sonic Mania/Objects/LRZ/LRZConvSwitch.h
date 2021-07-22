#ifndef OBJ_LRZCONVSWITCH_H
#define OBJ_LRZCONVSWITCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZConvSwitch;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZConvSwitch;

// Object Struct
extern ObjectLRZConvSwitch *LRZConvSwitch;

// Standard Entity Events
void LRZConvSwitch_Update(void);
void LRZConvSwitch_LateUpdate(void);
void LRZConvSwitch_StaticUpdate(void);
void LRZConvSwitch_Draw(void);
void LRZConvSwitch_Create(void* data);
void LRZConvSwitch_StageLoad(void);
void LRZConvSwitch_EditorDraw(void);
void LRZConvSwitch_EditorLoad(void);
void LRZConvSwitch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZCONVSWITCH_H

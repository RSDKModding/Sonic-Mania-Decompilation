#ifndef OBJ_LRZCONVITEM_H
#define OBJ_LRZCONVITEM_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZConvItem;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZConvItem;

// Object Struct
extern ObjectLRZConvItem *LRZConvItem;

// Standard Entity Events
void LRZConvItem_Update(void);
void LRZConvItem_LateUpdate(void);
void LRZConvItem_StaticUpdate(void);
void LRZConvItem_Draw(void);
void LRZConvItem_Create(void* data);
void LRZConvItem_StageLoad(void);
void LRZConvItem_EditorDraw(void);
void LRZConvItem_EditorLoad(void);
void LRZConvItem_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZCONVITEM_H

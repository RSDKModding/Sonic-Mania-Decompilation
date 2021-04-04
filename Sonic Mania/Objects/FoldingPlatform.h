#ifndef OBJ_FOLDINGPLATFORM_H
#define OBJ_FOLDINGPLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFoldingPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFoldingPlatform;

// Object Struct
extern ObjectFoldingPlatform *FoldingPlatform;

// Standard Entity Events
void FoldingPlatform_Update(void);
void FoldingPlatform_LateUpdate(void);
void FoldingPlatform_StaticUpdate(void);
void FoldingPlatform_Draw(void);
void FoldingPlatform_Create(void* data);
void FoldingPlatform_StageLoad(void);
void FoldingPlatform_EditorDraw(void);
void FoldingPlatform_EditorLoad(void);
void FoldingPlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FOLDINGPLATFORM_H

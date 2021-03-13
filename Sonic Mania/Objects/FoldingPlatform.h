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
void FoldingPlatform_Update();
void FoldingPlatform_LateUpdate();
void FoldingPlatform_StaticUpdate();
void FoldingPlatform_Draw();
void FoldingPlatform_Create(void* data);
void FoldingPlatform_StageLoad();
void FoldingPlatform_EditorDraw();
void FoldingPlatform_EditorLoad();
void FoldingPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_FOLDINGPLATFORM_H

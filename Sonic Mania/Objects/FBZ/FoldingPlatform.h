#ifndef OBJ_FOLDINGPLATFORM_H
#define OBJ_FOLDINGPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFoldingPlatform;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
    uint16 interval;
    uint16 intervalOffset;
    int32 duration;
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
#if RETRO_INCLUDE_EDITOR
void FoldingPlatform_EditorDraw(void);
void FoldingPlatform_EditorLoad(void);
#endif
void FoldingPlatform_Serialize(void);

// Extra Entity Functions
void FoldingPlatform_Unknown1(void);
void FoldingPlatform_Unknown2(void);

#endif //!OBJ_FOLDINGPLATFORM_H

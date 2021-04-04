#ifndef OBJ_STICKYPLATFORM_H
#define OBJ_STICKYPLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectStickyPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityStickyPlatform;

// Object Struct
extern ObjectStickyPlatform *StickyPlatform;

// Standard Entity Events
void StickyPlatform_Update(void);
void StickyPlatform_LateUpdate(void);
void StickyPlatform_StaticUpdate(void);
void StickyPlatform_Draw(void);
void StickyPlatform_Create(void* data);
void StickyPlatform_StageLoad(void);
void StickyPlatform_EditorDraw(void);
void StickyPlatform_EditorLoad(void);
void StickyPlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_STICKYPLATFORM_H

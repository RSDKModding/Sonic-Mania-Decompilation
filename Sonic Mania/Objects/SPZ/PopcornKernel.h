#ifndef OBJ_POPCORNKERNEL_H
#define OBJ_POPCORNKERNEL_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPopcornKernel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPopcornKernel;

// Object Struct
extern ObjectPopcornKernel *PopcornKernel;

// Standard Entity Events
void PopcornKernel_Update(void);
void PopcornKernel_LateUpdate(void);
void PopcornKernel_StaticUpdate(void);
void PopcornKernel_Draw(void);
void PopcornKernel_Create(void* data);
void PopcornKernel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PopcornKernel_EditorDraw(void);
void PopcornKernel_EditorLoad(void);
#endif
void PopcornKernel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_POPCORNKERNEL_H

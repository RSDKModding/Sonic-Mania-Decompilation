#ifndef OBJ_FERNPARALLAX_H
#define OBJ_FERNPARALLAX_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFernParallax;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFernParallax;

// Object Struct
extern ObjectFernParallax *FernParallax;

// Standard Entity Events
void FernParallax_Update();
void FernParallax_LateUpdate();
void FernParallax_StaticUpdate();
void FernParallax_Draw();
void FernParallax_Create(void* data);
void FernParallax_StageLoad();
void FernParallax_EditorDraw();
void FernParallax_EditorLoad();
void FernParallax_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_FERNPARALLAX_H

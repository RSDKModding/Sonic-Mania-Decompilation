#ifndef OBJ_TRYAGAINE_H
#define OBJ_TRYAGAINE_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTryAgainE;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTryAgainE;

// Object Struct
extern ObjectTryAgainE *TryAgainE;

// Standard Entity Events
void TryAgainE_Update();
void TryAgainE_LateUpdate();
void TryAgainE_StaticUpdate();
void TryAgainE_Draw();
void TryAgainE_Create(void* data);
void TryAgainE_StageLoad();
void TryAgainE_EditorDraw();
void TryAgainE_EditorLoad();
void TryAgainE_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_TRYAGAINE_H

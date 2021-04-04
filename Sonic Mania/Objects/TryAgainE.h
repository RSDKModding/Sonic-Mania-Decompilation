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
void TryAgainE_Update(void);
void TryAgainE_LateUpdate(void);
void TryAgainE_StaticUpdate(void);
void TryAgainE_Draw(void);
void TryAgainE_Create(void* data);
void TryAgainE_StageLoad(void);
void TryAgainE_EditorDraw(void);
void TryAgainE_EditorLoad(void);
void TryAgainE_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_TRYAGAINE_H

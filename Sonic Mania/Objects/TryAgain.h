#ifndef OBJ_TRYAGAIN_H
#define OBJ_TRYAGAIN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTryAgain;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTryAgain;

// Object Struct
extern ObjectTryAgain *TryAgain;

// Standard Entity Events
void TryAgain_Update(void);
void TryAgain_LateUpdate(void);
void TryAgain_StaticUpdate(void);
void TryAgain_Draw(void);
void TryAgain_Create(void* data);
void TryAgain_StageLoad(void);
void TryAgain_EditorDraw(void);
void TryAgain_EditorLoad(void);
void TryAgain_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TRYAGAIN_H

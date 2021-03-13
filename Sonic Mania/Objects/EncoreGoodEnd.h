#ifndef OBJ_ENCOREGOODEND_H
#define OBJ_ENCOREGOODEND_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEncoreGoodEnd;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEncoreGoodEnd;

// Object Struct
extern ObjectEncoreGoodEnd *EncoreGoodEnd;

// Standard Entity Events
void EncoreGoodEnd_Update();
void EncoreGoodEnd_LateUpdate();
void EncoreGoodEnd_StaticUpdate();
void EncoreGoodEnd_Draw();
void EncoreGoodEnd_Create(void* data);
void EncoreGoodEnd_StageLoad();
void EncoreGoodEnd_EditorDraw();
void EncoreGoodEnd_EditorLoad();
void EncoreGoodEnd_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_ENCOREGOODEND_H

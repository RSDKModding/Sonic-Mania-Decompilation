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
void EncoreGoodEnd_Update(void);
void EncoreGoodEnd_LateUpdate(void);
void EncoreGoodEnd_StaticUpdate(void);
void EncoreGoodEnd_Draw(void);
void EncoreGoodEnd_Create(void* data);
void EncoreGoodEnd_StageLoad(void);
void EncoreGoodEnd_EditorDraw(void);
void EncoreGoodEnd_EditorLoad(void);
void EncoreGoodEnd_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_ENCOREGOODEND_H

#ifndef OBJ_OOZ2OUTRO_H
#define OBJ_OOZ2OUTRO_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectOOZ2Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityOOZ2Outro;

// Object Struct
extern ObjectOOZ2Outro *OOZ2Outro;

// Standard Entity Events
void OOZ2Outro_Update(void);
void OOZ2Outro_LateUpdate(void);
void OOZ2Outro_StaticUpdate(void);
void OOZ2Outro_Draw(void);
void OOZ2Outro_Create(void* data);
void OOZ2Outro_StageLoad(void);
void OOZ2Outro_EditorDraw(void);
void OOZ2Outro_EditorLoad(void);
void OOZ2Outro_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_OOZ2OUTRO_H

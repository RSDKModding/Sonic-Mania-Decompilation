#ifndef OBJ_MMZ2OUTRO_H
#define OBJ_MMZ2OUTRO_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMMZ2Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMMZ2Outro;

// Object Struct
extern ObjectMMZ2Outro *MMZ2Outro;

// Standard Entity Events
void MMZ2Outro_Update(void);
void MMZ2Outro_LateUpdate(void);
void MMZ2Outro_StaticUpdate(void);
void MMZ2Outro_Draw(void);
void MMZ2Outro_Create(void* data);
void MMZ2Outro_StageLoad(void);
void MMZ2Outro_EditorDraw(void);
void MMZ2Outro_EditorLoad(void);
void MMZ2Outro_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_MMZ2OUTRO_H

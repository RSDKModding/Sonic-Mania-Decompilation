#ifndef OBJ_MSBOMB_H
#define OBJ_MSBOMB_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSBomb;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSBomb;

// Object Struct
extern ObjectMSBomb *MSBomb;

// Standard Entity Events
void MSBomb_Update(void);
void MSBomb_LateUpdate(void);
void MSBomb_StaticUpdate(void);
void MSBomb_Draw(void);
void MSBomb_Create(void* data);
void MSBomb_StageLoad(void);
void MSBomb_EditorDraw(void);
void MSBomb_EditorLoad(void);
void MSBomb_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_MSBOMB_H

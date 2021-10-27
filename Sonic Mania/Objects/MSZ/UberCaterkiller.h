#ifndef OBJ_UBERCATERKILLER_H
#define OBJ_UBERCATERKILLER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUberCaterkiller;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUberCaterkiller;

// Object Struct
extern ObjectUberCaterkiller *UberCaterkiller;

// Standard Entity Events
void UberCaterkiller_Update(void);
void UberCaterkiller_LateUpdate(void);
void UberCaterkiller_StaticUpdate(void);
void UberCaterkiller_Draw(void);
void UberCaterkiller_Create(void* data);
void UberCaterkiller_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UberCaterkiller_EditorDraw(void);
void UberCaterkiller_EditorLoad(void);
#endif
void UberCaterkiller_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UBERCATERKILLER_H

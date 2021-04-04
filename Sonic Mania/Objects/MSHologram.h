#ifndef OBJ_MSHOLOGRAM_H
#define OBJ_MSHOLOGRAM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSHologram;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSHologram;

// Object Struct
extern ObjectMSHologram *MSHologram;

// Standard Entity Events
void MSHologram_Update(void);
void MSHologram_LateUpdate(void);
void MSHologram_StaticUpdate(void);
void MSHologram_Draw(void);
void MSHologram_Create(void* data);
void MSHologram_StageLoad(void);
void MSHologram_EditorDraw(void);
void MSHologram_EditorLoad(void);
void MSHologram_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MSHOLOGRAM_H

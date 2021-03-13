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
void MSHologram_Update();
void MSHologram_LateUpdate();
void MSHologram_StaticUpdate();
void MSHologram_Draw();
void MSHologram_Create(void* data);
void MSHologram_StageLoad();
void MSHologram_EditorDraw();
void MSHologram_EditorLoad();
void MSHologram_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSHOLOGRAM_H

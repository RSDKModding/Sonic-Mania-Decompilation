#ifndef OBJ_FLOWERPOD_H
#define OBJ_FLOWERPOD_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFlowerPod;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFlowerPod;

// Object Struct
extern ObjectFlowerPod *FlowerPod;

// Standard Entity Events
void FlowerPod_Update(void);
void FlowerPod_LateUpdate(void);
void FlowerPod_StaticUpdate(void);
void FlowerPod_Draw(void);
void FlowerPod_Create(void* data);
void FlowerPod_StageLoad(void);
void FlowerPod_EditorDraw(void);
void FlowerPod_EditorLoad(void);
void FlowerPod_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FLOWERPOD_H

#ifndef OBJ_FLOWERPOD_H
#define OBJ_FLOWERPOD_H

#include "../SonicMania.h"

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
void FlowerPod_Update();
void FlowerPod_LateUpdate();
void FlowerPod_StaticUpdate();
void FlowerPod_Draw();
void FlowerPod_Create(void* data);
void FlowerPod_StageLoad();
void FlowerPod_EditorDraw();
void FlowerPod_EditorLoad();
void FlowerPod_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLOWERPOD_H

#ifndef OBJ_HEAVYRIDER_H
#define OBJ_HEAVYRIDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHeavyRider;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHeavyRider;

// Object Struct
extern ObjectHeavyRider *HeavyRider;

// Standard Entity Events
void HeavyRider_Update();
void HeavyRider_LateUpdate();
void HeavyRider_StaticUpdate();
void HeavyRider_Draw();
void HeavyRider_Create(void* data);
void HeavyRider_StageLoad();
void HeavyRider_EditorDraw();
void HeavyRider_EditorLoad();
void HeavyRider_Serialize();

// Extra Entity Functions


#endif //!OBJ_HEAVYRIDER_H

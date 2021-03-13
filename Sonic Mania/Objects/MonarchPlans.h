#ifndef OBJ_MONARCHPLANS_H
#define OBJ_MONARCHPLANS_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMonarchPlans;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMonarchPlans;

// Object Struct
extern ObjectMonarchPlans *MonarchPlans;

// Standard Entity Events
void MonarchPlans_Update();
void MonarchPlans_LateUpdate();
void MonarchPlans_StaticUpdate();
void MonarchPlans_Draw();
void MonarchPlans_Create(void* data);
void MonarchPlans_StageLoad();
void MonarchPlans_EditorDraw();
void MonarchPlans_EditorLoad();
void MonarchPlans_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_MONARCHPLANS_H

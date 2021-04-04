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
void MonarchPlans_Update(void);
void MonarchPlans_LateUpdate(void);
void MonarchPlans_StaticUpdate(void);
void MonarchPlans_Draw(void);
void MonarchPlans_Create(void* data);
void MonarchPlans_StageLoad(void);
void MonarchPlans_EditorDraw(void);
void MonarchPlans_EditorLoad(void);
void MonarchPlans_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_MONARCHPLANS_H

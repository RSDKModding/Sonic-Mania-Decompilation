#ifndef OBJ_HEAVYRIDER_H
#define OBJ_HEAVYRIDER_H

#include "SonicMania.h"

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
void HeavyRider_Update(void);
void HeavyRider_LateUpdate(void);
void HeavyRider_StaticUpdate(void);
void HeavyRider_Draw(void);
void HeavyRider_Create(void* data);
void HeavyRider_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HeavyRider_EditorDraw(void);
void HeavyRider_EditorLoad(void);
#endif
void HeavyRider_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HEAVYRIDER_H

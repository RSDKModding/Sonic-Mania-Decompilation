#ifndef OBJ_ROCKDRILL_H
#define OBJ_ROCKDRILL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRockDrill;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRockDrill;

// Object Struct
extern ObjectRockDrill *RockDrill;

// Standard Entity Events
void RockDrill_Update(void);
void RockDrill_LateUpdate(void);
void RockDrill_StaticUpdate(void);
void RockDrill_Draw(void);
void RockDrill_Create(void* data);
void RockDrill_StageLoad(void);
void RockDrill_EditorDraw(void);
void RockDrill_EditorLoad(void);
void RockDrill_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ROCKDRILL_H

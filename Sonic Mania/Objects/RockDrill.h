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
void RockDrill_Update();
void RockDrill_LateUpdate();
void RockDrill_StaticUpdate();
void RockDrill_Draw();
void RockDrill_Create(void* data);
void RockDrill_StageLoad();
void RockDrill_EditorDraw();
void RockDrill_EditorLoad();
void RockDrill_Serialize();

// Extra Entity Functions


#endif //!OBJ_ROCKDRILL_H

#ifndef OBJ_AIZEGGROBO_H
#define OBJ_AIZEGGROBO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAIZEggRobo;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAIZEggRobo;

// Object Struct
extern ObjectAIZEggRobo *AIZEggRobo;

// Standard Entity Events
void AIZEggRobo_Update();
void AIZEggRobo_LateUpdate();
void AIZEggRobo_StaticUpdate();
void AIZEggRobo_Draw();
void AIZEggRobo_Create(void* data);
void AIZEggRobo_StageLoad();
void AIZEggRobo_EditorDraw();
void AIZEggRobo_EditorLoad();
void AIZEggRobo_Serialize();

// Extra Entity Functions


#endif //!OBJ_AIZEGGROBO_H

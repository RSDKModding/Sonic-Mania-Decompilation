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
void AIZEggRobo_Update(void);
void AIZEggRobo_LateUpdate(void);
void AIZEggRobo_StaticUpdate(void);
void AIZEggRobo_Draw(void);
void AIZEggRobo_Create(void* data);
void AIZEggRobo_StageLoad(void);
void AIZEggRobo_EditorDraw(void);
void AIZEggRobo_EditorLoad(void);
void AIZEggRobo_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_AIZEGGROBO_H

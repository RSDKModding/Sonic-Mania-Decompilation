#ifndef OBJ_AIZROCKPILE_H
#define OBJ_AIZROCKPILE_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAIZRockPile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAIZRockPile;

// Object Struct
extern ObjectAIZRockPile *AIZRockPile;

// Standard Entity Events
void AIZRockPile_Update();
void AIZRockPile_LateUpdate();
void AIZRockPile_StaticUpdate();
void AIZRockPile_Draw();
void AIZRockPile_Create(void* data);
void AIZRockPile_StageLoad();
void AIZRockPile_EditorDraw();
void AIZRockPile_EditorLoad();
void AIZRockPile_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_AIZROCKPILE_H

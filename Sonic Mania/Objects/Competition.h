#ifndef OBJ_COMPETITION_H
#define OBJ_COMPETITION_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCompetition;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCompetition;

// Object Struct
extern ObjectCompetition *Competition;

// Standard Entity Events
void Competition_Update();
void Competition_LateUpdate();
void Competition_StaticUpdate();
void Competition_Draw();
void Competition_Create(void* data);
void Competition_StageLoad();
void Competition_EditorDraw();
void Competition_EditorLoad();
void Competition_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_COMPETITION_H

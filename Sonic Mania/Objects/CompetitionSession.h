#ifndef OBJ_COMPETITIONSESSION_H
#define OBJ_COMPETITIONSESSION_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCompetitionSession;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCompetitionSession;

// Object Entity
extern ObjectCompetitionSession *CompetitionSession;

// Standard Entity Events
void CompetitionSession_Update();
void CompetitionSession_LateUpdate();
void CompetitionSession_StaticUpdate();
void CompetitionSession_Draw();
void CompetitionSession_Create(void* data);
void CompetitionSession_StageLoad();
void CompetitionSession_EditorDraw();
void CompetitionSession_EditorLoad();
void CompetitionSession_Serialize();

// Extra Entity Functions


#endif //!OBJ_COMPETITIONSESSION_H

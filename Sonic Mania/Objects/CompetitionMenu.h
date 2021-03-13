#ifndef OBJ_COMPETITIONMENU_H
#define OBJ_COMPETITIONMENU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCompetitionMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCompetitionMenu;

// Object Struct
extern ObjectCompetitionMenu *CompetitionMenu;

// Standard Entity Events
void CompetitionMenu_Update();
void CompetitionMenu_LateUpdate();
void CompetitionMenu_StaticUpdate();
void CompetitionMenu_Draw();
void CompetitionMenu_Create(void* data);
void CompetitionMenu_StageLoad();
void CompetitionMenu_EditorDraw();
void CompetitionMenu_EditorLoad();
void CompetitionMenu_Serialize();

// Extra Entity Functions


#endif //!OBJ_COMPETITIONMENU_H

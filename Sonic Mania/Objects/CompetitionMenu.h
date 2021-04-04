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
void CompetitionMenu_Update(void);
void CompetitionMenu_LateUpdate(void);
void CompetitionMenu_StaticUpdate(void);
void CompetitionMenu_Draw(void);
void CompetitionMenu_Create(void* data);
void CompetitionMenu_StageLoad(void);
void CompetitionMenu_EditorDraw(void);
void CompetitionMenu_EditorLoad(void);
void CompetitionMenu_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_COMPETITIONMENU_H

#ifndef OBJ_COMPETITIONMENU_H
#define OBJ_COMPETITIONMENU_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCompetitionMenu : Object {

};

// Entity Class
struct EntityCompetitionMenu : Entity {

};

// Object Entity
extern ObjectCompetitionMenu CompetitionMenu;

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

#ifndef OBJ_COMPETITIONMENU_H
#define OBJ_COMPETITIONMENU_H

#include "../../SonicMania.h"

//Object Class
class ObjectCompetitionMenu : public Object {
public:

};

//Entity Class
class EntityCompetitionMenu : public Entity {
public:

};

//Object Entity
ObjectCompetitionMenu CompetitionMenu;

//Entity Functions
void CompetitionMenu_Update();
void CompetitionMenu_EarlyUpdate();
void CompetitionMenu_LateUpdate();
void CompetitionMenu_Draw();
void CompetitionMenu_Setup(void* subtype);
void CompetitionMenu_StageLoad();
void CompetitionMenu_GetAttributes();

#endif //!OBJ_COMPETITIONMENU_H

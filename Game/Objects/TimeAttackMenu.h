#ifndef OBJ_TIMEATTACKMENU_H
#define OBJ_TIMEATTACKMENU_H

#include "../../SonicMania.h"

//Object Class
class ObjectTimeAttackMenu : public Object {
public:

};

//Entity Class
class EntityTimeAttackMenu : public Entity {
public:

};

//Object Entity
ObjectTimeAttackMenu TimeAttackMenu;

//Entity Functions
void TimeAttackMenu_Update();
void TimeAttackMenu_EarlyUpdate();
void TimeAttackMenu_LateUpdate();
void TimeAttackMenu_Draw();
void TimeAttackMenu_Setup(void* subtype);
void TimeAttackMenu_StageLoad();
void TimeAttackMenu_GetAttributes();

#endif //!OBJ_TIMEATTACKMENU_H

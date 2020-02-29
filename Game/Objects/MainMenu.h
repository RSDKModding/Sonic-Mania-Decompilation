#ifndef OBJ_MAINMENU_H
#define OBJ_MAINMENU_H

#include "../../SonicMania.h"

//Object Class
class ObjectMainMenu : public Object {
public:

};

//Entity Class
class EntityMainMenu : public Entity {
public:

};

//Object Entity
ObjectMainMenu MainMenu;

//Entity Functions
void MainMenu_Update();
void MainMenu_EarlyUpdate();
void MainMenu_LateUpdate();
void MainMenu_Draw();
void MainMenu_Setup(void* subtype);
void MainMenu_StageLoad();
void MainMenu_GetAttributes();

#endif //!OBJ_MAINMENU_H

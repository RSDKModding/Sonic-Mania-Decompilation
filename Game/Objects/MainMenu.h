#ifndef OBJ_MAINMENU_H
#define OBJ_MAINMENU_H

//Object Class
class MainMenu : Object {

};

//Entity Class
class EntityMainMenu : Entity {

};

//Entity Functions
void MainMenu_Update();
void MainMenu_EarlyUpdate();
void MainMenu_LateUpdate();
void MainMenu_Draw();
void MainMenu_Setup(void* subtype);
void MainMenu_StageLoad();
void MainMenu_GetAttributes();

#endif //!OBJ_MAINMENU_H

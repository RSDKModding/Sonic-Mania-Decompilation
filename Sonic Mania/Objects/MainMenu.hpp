#ifndef OBJ_MAINMENU_H
#define OBJ_MAINMENU_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMainMenu : Object {

};

// Entity Class
struct EntityMainMenu : Entity {

};

// Object Struct
extern ObjectMainMenu *MainMenu;

// Standard Entity Events
void MainMenu_Update();
void MainMenu_LateUpdate();
void MainMenu_StaticUpdate();
void MainMenu_Draw();
void MainMenu_Create(void* data);
void MainMenu_StageLoad();
void MainMenu_EditorDraw();
void MainMenu_EditorLoad();
void MainMenu_Serialize();

// Extra Entity Functions


#endif //!OBJ_MAINMENU_H

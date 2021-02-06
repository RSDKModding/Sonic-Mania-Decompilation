#ifndef OBJ_PAUSEMENU_H
#define OBJ_PAUSEMENU_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPauseMenu : Object {

};

// Entity Class
struct EntityPauseMenu : Entity {

};

// Object Entity
extern ObjectPauseMenu PauseMenu;

// Standard Entity Events
void PauseMenu_Update();
void PauseMenu_LateUpdate();
void PauseMenu_StaticUpdate();
void PauseMenu_Draw();
void PauseMenu_Create(void* data);
void PauseMenu_StageLoad();
void PauseMenu_EditorDraw();
void PauseMenu_EditorLoad();
void PauseMenu_Serialize();

// Extra Entity Functions


#endif //!OBJ_PAUSEMENU_H

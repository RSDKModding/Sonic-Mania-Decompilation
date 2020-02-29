#ifndef OBJ_PAUSEMENU_H
#define OBJ_PAUSEMENU_H

//Object Class
class PauseMenu : Object {

};

//Entity Class
class EntityPauseMenu : Entity {

};

//Entity Functions
void PauseMenu_Update();
void PauseMenu_EarlyUpdate();
void PauseMenu_LateUpdate();
void PauseMenu_Draw();
void PauseMenu_Setup(void* subtype);
void PauseMenu_StageLoad();
void PauseMenu_GetAttributes();

#endif //!OBJ_PAUSEMENU_H

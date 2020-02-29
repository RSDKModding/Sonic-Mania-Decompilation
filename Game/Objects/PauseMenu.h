#ifndef OBJ_PAUSEMENU_H
#define OBJ_PAUSEMENU_H

#include "../../SonicMania.h"

//Object Class
class ObjectPauseMenu : public Object {
public:

};

//Entity Class
class EntityPauseMenu : public Entity {
public:

};

//Object Entity
ObjectPauseMenu PauseMenu;

//Entity Functions
void PauseMenu_Update();
void PauseMenu_EarlyUpdate();
void PauseMenu_LateUpdate();
void PauseMenu_Draw();
void PauseMenu_Setup(void* subtype);
void PauseMenu_StageLoad();
void PauseMenu_GetAttributes();

#endif //!OBJ_PAUSEMENU_H

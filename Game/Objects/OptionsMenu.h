#ifndef OBJ_OPTIONSMENU_H
#define OBJ_OPTIONSMENU_H

#include "../../SonicMania.h"

//Object Class
class ObjectOptionsMenu : public Object {
public:

};

//Entity Class
class EntityOptionsMenu : public Entity {
public:

};

//Object Entity
ObjectOptionsMenu OptionsMenu;

//Entity Functions
void OptionsMenu_Update();
void OptionsMenu_EarlyUpdate();
void OptionsMenu_LateUpdate();
void OptionsMenu_Draw();
void OptionsMenu_Setup(void* subtype);
void OptionsMenu_StageLoad();
void OptionsMenu_GetAttributes();

#endif //!OBJ_OPTIONSMENU_H

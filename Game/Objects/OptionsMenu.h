#ifndef OBJ_OPTIONSMENU_H
#define OBJ_OPTIONSMENU_H

//Object Class
class OptionsMenu : Object {

};

//Entity Class
class EntityOptionsMenu : Entity {

};

//Entity Functions
void OptionsMenu_Update();
void OptionsMenu_EarlyUpdate();
void OptionsMenu_LateUpdate();
void OptionsMenu_Draw();
void OptionsMenu_Setup(void* subtype);
void OptionsMenu_StageLoad();
void OptionsMenu_GetAttributes();

#endif //!OBJ_OPTIONSMENU_H

#ifndef OBJ_OPTIONSMENU_H
#define OBJ_OPTIONSMENU_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectOptionsMenu : Object {

};

// Entity Class
struct EntityOptionsMenu : Entity {

};

// Object Struct
extern ObjectOptionsMenu *OptionsMenu;

// Standard Entity Events
void OptionsMenu_Update();
void OptionsMenu_LateUpdate();
void OptionsMenu_StaticUpdate();
void OptionsMenu_Draw();
void OptionsMenu_Create(void* data);
void OptionsMenu_StageLoad();
void OptionsMenu_EditorDraw();
void OptionsMenu_EditorLoad();
void OptionsMenu_Serialize();

// Extra Entity Functions


#endif //!OBJ_OPTIONSMENU_H

#ifndef OBJ_MANIAMODEMENU_H
#define OBJ_MANIAMODEMENU_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectManiaModeMenu : Object {

};

// Entity Class
struct EntityManiaModeMenu : Entity {

};

// Object Struct
extern ObjectManiaModeMenu *ManiaModeMenu;

// Standard Entity Events
void ManiaModeMenu_Update();
void ManiaModeMenu_LateUpdate();
void ManiaModeMenu_StaticUpdate();
void ManiaModeMenu_Draw();
void ManiaModeMenu_Create(void* data);
void ManiaModeMenu_StageLoad();
void ManiaModeMenu_EditorDraw();
void ManiaModeMenu_EditorLoad();
void ManiaModeMenu_Serialize();

// Extra Entity Functions


#endif //!OBJ_MANIAMODEMENU_H

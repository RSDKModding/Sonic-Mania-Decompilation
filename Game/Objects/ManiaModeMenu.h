#ifndef OBJ_MANIAMODEMENU_H
#define OBJ_MANIAMODEMENU_H

//Object Class
class ManiaModeMenu : Object {

};

//Entity Class
class EntityManiaModeMenu : Entity {

};

//Entity Functions
void ManiaModeMenu_Update();
void ManiaModeMenu_EarlyUpdate();
void ManiaModeMenu_LateUpdate();
void ManiaModeMenu_Draw();
void ManiaModeMenu_Setup(void* subtype);
void ManiaModeMenu_StageLoad();
void ManiaModeMenu_GetAttributes();

#endif //!OBJ_MANIAMODEMENU_H

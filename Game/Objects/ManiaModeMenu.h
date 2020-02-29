#ifndef OBJ_MANIAMODEMENU_H
#define OBJ_MANIAMODEMENU_H

#include "../../SonicMania.h"

//Object Class
class ObjectManiaModeMenu : public Object {
public:

};

//Entity Class
class EntityManiaModeMenu : public Entity {
public:

};

//Object Entity
ObjectManiaModeMenu ManiaModeMenu;

//Entity Functions
void ManiaModeMenu_Update();
void ManiaModeMenu_EarlyUpdate();
void ManiaModeMenu_LateUpdate();
void ManiaModeMenu_Draw();
void ManiaModeMenu_Setup(void* subtype);
void ManiaModeMenu_StageLoad();
void ManiaModeMenu_GetAttributes();

#endif //!OBJ_MANIAMODEMENU_H

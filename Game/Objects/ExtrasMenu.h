#ifndef OBJ_EXTRASMENU_H
#define OBJ_EXTRASMENU_H

//Object Class
class ExtrasMenu : Object {

};

//Entity Class
class EntityExtrasMenu : Entity {

};

//Entity Functions
void ExtrasMenu_Update();
void ExtrasMenu_EarlyUpdate();
void ExtrasMenu_LateUpdate();
void ExtrasMenu_Draw();
void ExtrasMenu_Setup(void* subtype);
void ExtrasMenu_StageLoad();
void ExtrasMenu_GetAttributes();

#endif //!OBJ_EXTRASMENU_H

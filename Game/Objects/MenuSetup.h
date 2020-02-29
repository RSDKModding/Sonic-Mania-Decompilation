#ifndef OBJ_MENUSETUP_H
#define OBJ_MENUSETUP_H

//Object Class
class MenuSetup : Object {

};

//Entity Class
class EntityMenuSetup : Entity {

};

//Entity Functions
void MenuSetup_Update();
void MenuSetup_EarlyUpdate();
void MenuSetup_LateUpdate();
void MenuSetup_Draw();
void MenuSetup_Setup(void* subtype);
void MenuSetup_StageLoad();
void MenuSetup_GetAttributes();

#endif //!OBJ_MENUSETUP_H

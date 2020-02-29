#ifndef OBJ_MENUPARAM_H
#define OBJ_MENUPARAM_H

//Object Class
class MenuParam : Object {

};

//Entity Class
class EntityMenuParam : Entity {

};

//Entity Functions
void MenuParam_Update();
void MenuParam_EarlyUpdate();
void MenuParam_LateUpdate();
void MenuParam_Draw();
void MenuParam_Setup(void* subtype);
void MenuParam_StageLoad();
void MenuParam_GetAttributes();

#endif //!OBJ_MENUPARAM_H

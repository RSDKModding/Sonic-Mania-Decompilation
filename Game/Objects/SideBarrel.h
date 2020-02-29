#ifndef OBJ_SIDEBARREL_H
#define OBJ_SIDEBARREL_H

//Object Class
class SideBarrel : Object {

};

//Entity Class
class EntitySideBarrel : Entity {

};

//Entity Functions
void SideBarrel_Update();
void SideBarrel_EarlyUpdate();
void SideBarrel_LateUpdate();
void SideBarrel_Draw();
void SideBarrel_Setup(void* subtype);
void SideBarrel_StageLoad();
void SideBarrel_GetAttributes();

#endif //!OBJ_SIDEBARREL_H

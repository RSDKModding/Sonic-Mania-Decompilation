#ifndef OBJ_SIDEBARREL_H
#define OBJ_SIDEBARREL_H

#include "../../SonicMania.h"

//Object Class
class ObjectSideBarrel : public Object {
public:

};

//Entity Class
class EntitySideBarrel : public Entity {
public:

};

//Object Entity
ObjectSideBarrel SideBarrel;

//Entity Functions
void SideBarrel_Update();
void SideBarrel_EarlyUpdate();
void SideBarrel_LateUpdate();
void SideBarrel_Draw();
void SideBarrel_Setup(void* subtype);
void SideBarrel_StageLoad();
void SideBarrel_GetAttributes();

#endif //!OBJ_SIDEBARREL_H

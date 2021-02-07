#ifndef OBJ_SIDEBARREL_H
#define OBJ_SIDEBARREL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSideBarrel : Object{

};

// Entity Class
struct EntitySideBarrel : Entity {

};

// Object Struct
extern ObjectSideBarrel *SideBarrel;

// Standard Entity Events
void SideBarrel_Update();
void SideBarrel_LateUpdate();
void SideBarrel_StaticUpdate();
void SideBarrel_Draw();
void SideBarrel_Create(void* data);
void SideBarrel_StageLoad();
void SideBarrel_EditorDraw();
void SideBarrel_EditorLoad();
void SideBarrel_Serialize();

// Extra Entity Functions


#endif //!OBJ_SIDEBARREL_H

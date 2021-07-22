#ifndef OBJ_SIDEBARREL_H
#define OBJ_SIDEBARREL_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSideBarrel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySideBarrel;

// Object Struct
extern ObjectSideBarrel *SideBarrel;

// Standard Entity Events
void SideBarrel_Update(void);
void SideBarrel_LateUpdate(void);
void SideBarrel_StaticUpdate(void);
void SideBarrel_Draw(void);
void SideBarrel_Create(void* data);
void SideBarrel_StageLoad(void);
void SideBarrel_EditorDraw(void);
void SideBarrel_EditorLoad(void);
void SideBarrel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SIDEBARREL_H

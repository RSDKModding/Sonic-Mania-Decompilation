#ifndef OBJ_DEMOMENU_H
#define OBJ_DEMOMENU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDemoMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDemoMenu;

// Object Entity
extern ObjectDemoMenu *DemoMenu;

// Standard Entity Events
void DemoMenu_Update();
void DemoMenu_LateUpdate();
void DemoMenu_StaticUpdate();
void DemoMenu_Draw();
void DemoMenu_Create(void* data);
void DemoMenu_StageLoad();
void DemoMenu_EditorDraw();
void DemoMenu_EditorLoad();
void DemoMenu_Serialize();

// Extra Entity Functions


#endif //!OBJ_DEMOMENU_H

#ifndef OBJ_MANIAMODEMENU_H
#define OBJ_MANIAMODEMENU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectManiaModeMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityManiaModeMenu;

// Object Struct
extern ObjectManiaModeMenu *ManiaModeMenu;

// Standard Entity Events
void ManiaModeMenu_Update();
void ManiaModeMenu_LateUpdate();
void ManiaModeMenu_StaticUpdate();
void ManiaModeMenu_Draw();
void ManiaModeMenu_Create(void* data);
void ManiaModeMenu_StageLoad();
void ManiaModeMenu_EditorDraw();
void ManiaModeMenu_EditorLoad();
void ManiaModeMenu_Serialize();

// Extra Entity Functions


#endif //!OBJ_MANIAMODEMENU_H

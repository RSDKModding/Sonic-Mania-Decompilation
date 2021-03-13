#ifndef OBJ_EXTRASMENU_H
#define OBJ_EXTRASMENU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectExtrasMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityExtrasMenu;

// Object Struct
extern ObjectExtrasMenu *ExtrasMenu;

// Standard Entity Events
void ExtrasMenu_Update();
void ExtrasMenu_LateUpdate();
void ExtrasMenu_StaticUpdate();
void ExtrasMenu_Draw();
void ExtrasMenu_Create(void* data);
void ExtrasMenu_StageLoad();
void ExtrasMenu_EditorDraw();
void ExtrasMenu_EditorLoad();
void ExtrasMenu_Serialize();

// Extra Entity Functions


#endif //!OBJ_EXTRASMENU_H

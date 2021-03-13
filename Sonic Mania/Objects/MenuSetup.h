#ifndef OBJ_MENUSETUP_H
#define OBJ_MENUSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMenuSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMenuSetup;

// Object Struct
extern ObjectMenuSetup *MenuSetup;

// Standard Entity Events
void MenuSetup_Update();
void MenuSetup_LateUpdate();
void MenuSetup_StaticUpdate();
void MenuSetup_Draw();
void MenuSetup_Create(void* data);
void MenuSetup_StageLoad();
void MenuSetup_EditorDraw();
void MenuSetup_EditorLoad();
void MenuSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_MENUSETUP_H

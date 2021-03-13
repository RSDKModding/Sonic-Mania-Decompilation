#ifndef OBJ_E3MENUSETUP_H
#define OBJ_E3MENUSETUP_H

#include "../SonicMania.h"

#if !RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectE3MenuSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityE3MenuSetup;

// Object Entity
extern ObjectE3MenuSetup *E3MenuSetup;

// Standard Entity Events
void E3MenuSetup_Update();
void E3MenuSetup_LateUpdate();
void E3MenuSetup_StaticUpdate();
void E3MenuSetup_Draw();
void E3MenuSetup_Create(void* data);
void E3MenuSetup_StageLoad();
void E3MenuSetup_EditorDraw();
void E3MenuSetup_EditorLoad();
void E3MenuSetup_Serialize();

// Extra Entity Functions

#endif


#endif //!OBJ_E3MENUSETUP_H

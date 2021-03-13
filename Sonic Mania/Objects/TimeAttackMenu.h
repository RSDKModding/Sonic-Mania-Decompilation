#ifndef OBJ_TIMEATTACKMENU_H
#define OBJ_TIMEATTACKMENU_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTimeAttackMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTimeAttackMenu;

// Object Struct
extern ObjectTimeAttackMenu *TimeAttackMenu;

// Standard Entity Events
void TimeAttackMenu_Update();
void TimeAttackMenu_LateUpdate();
void TimeAttackMenu_StaticUpdate();
void TimeAttackMenu_Draw();
void TimeAttackMenu_Create(void* data);
void TimeAttackMenu_StageLoad();
void TimeAttackMenu_EditorDraw();
void TimeAttackMenu_EditorLoad();
void TimeAttackMenu_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_TIMEATTACKMENU_H

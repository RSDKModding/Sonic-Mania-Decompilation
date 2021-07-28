#ifndef OBJ_TIMEATTACKMENU_H
#define OBJ_TIMEATTACKMENU_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTimeAttackMenu;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(callback);
    int timeOut;
} EntityTimeAttackMenu;

// Object Struct
extern ObjectTimeAttackMenu *TimeAttackMenu;

// Standard Entity Events
void TimeAttackMenu_Update(void);
void TimeAttackMenu_LateUpdate(void);
void TimeAttackMenu_StaticUpdate(void);
void TimeAttackMenu_Draw(void);
void TimeAttackMenu_Create(void* data);
void TimeAttackMenu_StageLoad(void);
void TimeAttackMenu_EditorDraw(void);
void TimeAttackMenu_EditorLoad(void);
void TimeAttackMenu_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_TIMEATTACKMENU_H

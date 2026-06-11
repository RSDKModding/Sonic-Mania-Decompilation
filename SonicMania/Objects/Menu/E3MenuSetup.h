#ifndef OBJ_E3MENUSETUP_H
#define OBJ_E3MENUSETUP_H

#include "Game.h"

#if !MANIA_USE_PLUS
#include "UIControl.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 unused1;
    bool32 initialized;
    int32 unused2;
    EntityUIControl *charSelControl;
    EntityUIControl *zoneControl;
} ObjectE3MenuSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(callback);
    int32 timer;
    int32 delay;
    int32 fadeSpeed;
    int32 fadeTimer;
    int32 fadeColor;
    int32 unused;
} EntityE3MenuSetup;

// Object Entity
extern ObjectE3MenuSetup *E3MenuSetup;

// Standard Entity Events
void E3MenuSetup_Update(void);
void E3MenuSetup_LateUpdate(void);
void E3MenuSetup_StaticUpdate(void);
void E3MenuSetup_Draw(void);
void E3MenuSetup_Create(void *data);
void E3MenuSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void E3MenuSetup_EditorDraw(void);
void E3MenuSetup_EditorLoad(void);
#endif
void E3MenuSetup_Serialize(void);

// Extra Entity Functions
void E3MenuSetup_SetupUI(void);
void E3MenuSetup_SetupButtons(void);

void E3MenuSetup_Callback_LoadScene(void);

void E3MenuSetup_ActionCB_ZoneSelect(void);

void E3MenuSetup_ActionCB_Sonic(void);
void E3MenuSetup_ActionCB_Tails(void);
void E3MenuSetup_ActionCB_Knux(void);

void E3MenuSetup_State_FadeOut(void);
#endif

#endif //! OBJ_E3MENUSETUP_H

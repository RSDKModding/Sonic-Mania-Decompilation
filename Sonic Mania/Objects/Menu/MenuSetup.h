#ifndef OBJ_MENUSETUP_H
#define OBJ_MENUSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
#if RETRO_USE_PLUS
    int field_4;
    int dword8;
    bool32 initialized;
    int dword10;
    bool32 gameLoaded;
    int dword18;
    EntityFXFade *fxFade;
#else

#endif
} ObjectMenuSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(timedState);
    int timer;
    int timeOut;
    int field_68;
    int fadeTimer;
    int fadeColour;
} EntityMenuSetup;

// Object Struct
extern ObjectMenuSetup *MenuSetup;

// Standard Entity Events
void MenuSetup_Update(void);
void MenuSetup_LateUpdate(void);
void MenuSetup_StaticUpdate(void);
void MenuSetup_Draw(void);
void MenuSetup_Create(void* data);
void MenuSetup_StageLoad(void);
void MenuSetup_EditorDraw(void);
void MenuSetup_EditorLoad(void);
void MenuSetup_Serialize(void);

// Extra Entity Functions
void MenuSetup_StartTransition(void (*callback)(void), int time);

#endif //!OBJ_MENUSETUP_H

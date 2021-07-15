#ifndef OBJ_E3MENUSETUP_H
#define OBJ_E3MENUSETUP_H

#include "../SonicMania.h"

#if !RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    bool32 flag;
    int field_C;
    Entity *charSelControl;
    Entity *zoneControl;
} ObjectE3MenuSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(timedState);
    int timer;
    int timeOut;
    int field_70;
    int fadeTimer;
    int fadeColour;
    int field_7C;
} EntityE3MenuSetup;

// Object Entity
extern ObjectE3MenuSetup *E3MenuSetup;

// Standard Entity Events
void E3MenuSetup_Update(void);
void E3MenuSetup_LateUpdate(void);
void E3MenuSetup_StaticUpdate(void);
void E3MenuSetup_Draw(void);
void E3MenuSetup_Create(void* data);
void E3MenuSetup_StageLoad(void);
void E3MenuSetup_EditorDraw(void);
void E3MenuSetup_EditorLoad(void);
void E3MenuSetup_Serialize(void);

// Extra Entity Functions
void E3MenuSetup_Unknown1(void);
void E3MenuSetup_Unknown2(void);
void E3MenuSetup_Unknown3(void);
void E3MenuSetup_Unknown4(void);
void E3MenuSetup_Unknown5(void);
void E3MenuSetup_Unknown6(void);
void E3MenuSetup_Unknown7(void);
void E3MenuSetup_Unknown8(void);

#endif


#endif //!OBJ_E3MENUSETUP_H

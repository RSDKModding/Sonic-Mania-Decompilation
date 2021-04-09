#ifndef OBJ_THANKSSETUP_H
#define OBJ_THANKSSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort sfx_Sega;
} ObjectThanksSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Entity *picture;
    int timer;
    int offset;
} EntityThanksSetup;

// Object Struct
extern ObjectThanksSetup *ThanksSetup;

// Standard Entity Events
void ThanksSetup_Update(void);
void ThanksSetup_LateUpdate(void);
void ThanksSetup_StaticUpdate(void);
void ThanksSetup_Draw(void);
void ThanksSetup_Create(void* data);
void ThanksSetup_StageLoad(void);
void ThanksSetup_EditorDraw(void);
void ThanksSetup_EditorLoad(void);
void ThanksSetup_Serialize(void);

// Extra Entity Functions
void ThanksSetup_Unknown1(void);
void ThanksSetup_Unknown2(void);
void ThanksSetup_Unknown3(void);
void ThanksSetup_Unknown4(void);
void ThanksSetup_Unknown5(void);
void ThanksSetup_Unknown6(void);

#endif //!OBJ_THANKSSETUP_H

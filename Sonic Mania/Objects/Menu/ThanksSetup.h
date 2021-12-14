#ifndef OBJ_THANKSSETUP_H
#define OBJ_THANKSSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sfxSega;
} ObjectThanksSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Entity *picture;
    int32 timer;
    int32 radius;
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
#if RETRO_INCLUDE_EDITOR
void ThanksSetup_EditorDraw(void);
void ThanksSetup_EditorLoad(void);
#endif
void ThanksSetup_Serialize(void);

// Extra Entity Functions
void ThanksSetup_HandleIconsPos(void);
void ThanksSetup_State_ThanksForPlaying(void);
void ThanksSetup_State_FlipOverIcon(void);
void ThanksSetup_State_Mania2017(void);
void ThanksSetup_State_FadeOut(void);
void ThanksSetup_Draw_Fade(void);

#endif //!OBJ_THANKSSETUP_H

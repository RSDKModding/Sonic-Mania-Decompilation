#ifndef OBJ_TITLELOGO_H
#define OBJ_TITLELOGO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 logoIndex;
#if RETRO_USE_PLUS
    uint16 plusIndex;
    uint16 sfx_Plus;
#endif
} ObjectTitleLogo;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
#if RETRO_USE_PLUS
    StateMachine(state);
#endif
    bool32 flag;
    int timer;
    int storeY;
    Animator animator1;
    Animator animator2;
#if RETRO_USE_PLUS
    Animator animator3;
#endif
} EntityTitleLogo;

// Object Struct
extern ObjectTitleLogo *TitleLogo;

// Standard Entity Events
void TitleLogo_Update(void);
void TitleLogo_LateUpdate(void);
void TitleLogo_StaticUpdate(void);
void TitleLogo_Draw(void);
void TitleLogo_Create(void* data);
void TitleLogo_StageLoad(void);
void TitleLogo_EditorDraw(void);
void TitleLogo_EditorLoad(void);
void TitleLogo_Serialize(void);

// Extra Entity Functions
void TitleLogo_Unknown1(void);
#if RETRO_USE_PLUS
void TitleLogo_Unknown2(void);
void TitleLogo_Unknown3(void);
void TitleLogo_Unknown4(void);
void TitleLogo_Unknown5(void);
void TitleLogo_Unknown6(void);
#endif

#endif //!OBJ_TITLELOGO_H

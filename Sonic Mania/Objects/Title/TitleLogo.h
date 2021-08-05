#ifndef OBJ_TITLELOGO_H
#define OBJ_TITLELOGO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort logoIndex;
#if RETRO_USE_PLUS
    ushort plusIndex;
    ushort sfx_Plus;
#endif
} ObjectTitleLogo;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
    StateMachine(state);
    bool32 flag;
    int timer;
    int storeY;
    Animator data1;
    Animator animator2;
    Animator data3;
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
void TitleLogo_Unknown2(void);
void TitleLogo_Unknown3(void);
void TitleLogo_Unknown4(void);
void TitleLogo_Unknown5(void);
void TitleLogo_Unknown6(void);

#endif //!OBJ_TITLELOGO_H

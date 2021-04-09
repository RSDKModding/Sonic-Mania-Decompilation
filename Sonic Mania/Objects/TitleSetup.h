#ifndef OBJ_TITLESETUP_H
#define OBJ_TITLESETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 altMusic;
    ushort spriteIndex;
    ushort sfx_MenuBleep;
    ushort sfx_MenuAccept;
    ushort sfx_Ring;
    byte cheatCode[8];
} ObjectTitleSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int timer;
    Vector2 drawPos;
    int touched;
    Animator data;
} EntityTitleSetup;

// Object Struct
extern ObjectTitleSetup *TitleSetup;

// Standard Entity Events
void TitleSetup_Update(void);
void TitleSetup_LateUpdate(void);
void TitleSetup_StaticUpdate(void);
void TitleSetup_Draw(void);
void TitleSetup_Create(void* data);
void TitleSetup_StageLoad(void);
void TitleSetup_EditorDraw(void);
void TitleSetup_EditorLoad(void);
void TitleSetup_Serialize(void);

// Extra Entity Functions
void TitleSetup_HandleCheatInputs(void);
void TitleSetup_CheckCheatCode(void);
bool32 TitleSetup_IntroCallback(void);

void TitleSetup_Unknown4(void);
void TitleSetup_Unknown5(void);
void TitleSetup_Unknown6(void);
void TitleSetup_Unknown7(void);
void TitleSetup_SetupLogo_NoPlus(void);
void TitleSetup_SetupLogo_Plus(void);
void TitleSetup_Unknown10(void);
void TitleSetup_Unknown11(void);
void TitleSetup_Unknown12(void);
void TitleSetup_Unknown13(void);
void TitleSetup_Unknown14(void);
void TitleSetup_Unknown15(void);

#endif //!OBJ_TITLESETUP_H

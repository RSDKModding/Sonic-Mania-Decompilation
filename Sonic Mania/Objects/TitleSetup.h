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
    void (*state)();
    void (*stateDraw)();
    int timer;
    Vector2 drawPos;
    int touched;
    AnimationData data;
} EntityTitleSetup;

// Object Struct
extern ObjectTitleSetup *TitleSetup;

// Standard Entity Events
void TitleSetup_Update();
void TitleSetup_LateUpdate();
void TitleSetup_StaticUpdate();
void TitleSetup_Draw();
void TitleSetup_Create(void* data);
void TitleSetup_StageLoad();
void TitleSetup_EditorDraw();
void TitleSetup_EditorLoad();
void TitleSetup_Serialize();

// Extra Entity Functions
void TitleSetup_HandleCheatInputs();
void TitleSetup_CheckCheatCode();
bool32 TitleSetup_IntroCallback(void);

void TitleSetup_Unknown4();
void TitleSetup_Unknown5();
void TitleSetup_Unknown6();
void TitleSetup_Unknown7();
void TitleSetup_SetupLogo_NoPlus();
void TitleSetup_SetupLogo_Plus();
void TitleSetup_Unknown10();
void TitleSetup_Unknown11();
void TitleSetup_Unknown12();
void TitleSetup_Unknown13();
void TitleSetup_Unknown14();
void TitleSetup_Unknown15();

#endif //!OBJ_TITLESETUP_H

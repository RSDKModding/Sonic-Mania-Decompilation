#ifndef OBJ_TITLESETUP_H
#define OBJ_TITLESETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTitleSetup : Object {
    bool32 altMusic;
    ushort spriteIndex;
    ushort sfx_MenuBleep;
    ushort sfx_MenuAccept;
    ushort sfx_Ring;
    byte cheatCode[8];
};

// Entity Class
struct EntityTitleSetup : Entity {
    void (*state)();
    void (*stateDraw)();
    int timer;
    Vector2 drawPos;
    int touched;
    EntityAnimationData data;
};

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

void TitleSetup_Unknown7();
void TitleSetup_Unknown10();
void TitleSetup_Unknown11();
void TitleSetup_Unknown12();
void TitleSetup_Unknown13();
void TitleSetup_Unknown14();
void TitleSetup_Unknown15();

#endif //!OBJ_TITLESETUP_H

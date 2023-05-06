#ifndef OBJ_TITLESETUP_H
#define OBJ_TITLESETUP_H

#include "Game.h"

// Object Class
struct ObjectTitleSetup {
    RSDK_OBJECT
    bool32 useAltIntroMusic;
    uint16 aniFrames;
    uint16 sfxMenuBleep;
    uint16 sfxMenuAccept;
    uint16 sfxRing;
#if MANIA_USE_PLUS
    uint8 cheatCode[8];
#endif
};

// Entity Class
struct EntityTitleSetup {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    Vector2 drawPos;
    int32 touched;
    Animator animator;
};

// Object Struct
extern ObjectTitleSetup *TitleSetup;

// Standard Entity Events
void TitleSetup_Update(void);
void TitleSetup_LateUpdate(void);
void TitleSetup_StaticUpdate(void);
void TitleSetup_Draw(void);
void TitleSetup_Create(void *data);
void TitleSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TitleSetup_EditorDraw(void);
void TitleSetup_EditorLoad(void);
#endif
void TitleSetup_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void TitleSetup_HandleCheatInputs(void);
void TitleSetup_CheckCheatCode(void);
#endif
bool32 TitleSetup_VideoSkipCB(void);

void TitleSetup_State_Wait(void);
void TitleSetup_State_AnimateUntilFlash(void);
void TitleSetup_State_FlashIn(void);
void TitleSetup_State_WaitForSonic(void);
void TitleSetup_State_SetupLogo(void);
#if MANIA_USE_PLUS
void TitleSetup_State_SetupPlusLogo(void);
#endif
void TitleSetup_State_WaitForEnter(void);
void TitleSetup_State_FadeToMenu(void);
void TitleSetup_State_FadeToVideo(void);

void TitleSetup_Draw_FadeBlack(void);
void TitleSetup_Draw_DrawRing(void);
void TitleSetup_Draw_Flash(void);

#endif //! OBJ_TITLESETUP_H

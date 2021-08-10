#ifndef OBJ_HUD_H
#define OBJ_HUD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort hudMappings;
    ushort superButtonMappings;
    ushort sfxClick;
    ushort sfxStarpost;
    int dwordC;
    int replaySaveEnabled;
    int compSession[4];
    int swapCooldown;
    int stockFlashTimers[PLAYER_MAX];
} ObjectHUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 dword5C[4];
    int field_7C[4];
    int playerLives[PLAYER_MAX];
    int field_9C;
    int superButtonPos;
    void (*competitionStates[PLAYER_MAX])(void);
    Vector2 dwordB4[4];
    Vector2 dwordD4[4];
    Vector2 dwordF4[4];
    Vector2 dword114[4];
    int field_134[4];
    int screenID;
    int timeFlashFrame;
    int ringFlashFrame;
    int field_150;
    int field_154;
    Animator hudElementsData;
    Animator numbersData;
    Animator hyperNumbersData;
    Animator lifeIconsData;
    Animator playerIDData;
#if RETRO_GAMEVER != VER_100
    Animator superButtonsData;
    Animator superButtonData1;
#endif
    Animator taData1;
    Animator taData4;
    Animator taData2;
    Animator taData3;
} EntityHUD;

// Object Struct
extern ObjectHUD *HUD;

// Standard Entity Events
void HUD_Update(void);
void HUD_LateUpdate(void);
void HUD_StaticUpdate(void);
void HUD_Draw(void);
void HUD_Create(void* data);
void HUD_StageLoad(void);
void HUD_EditorDraw(void);
void HUD_EditorLoad(void);
void HUD_Serialize(void);

// Extra Entity Functions
void HUD_DrawNumbersBase10(Vector2 *drawPos, int value, signed int maxDigits);
void HUD_DrawNumbersBase16(Vector2 *drawPos, int value);
void HUD_DrawNumbersHyperRing(Vector2 *drawPos, int value);
void HUD_GetKeyFrame(Animator *animator, int buttonID);
void HUD_GetSuperFrames(void);
void HUD_State_ComeOnScreen(void);
void HUD_State_GoOffScreen(void);

#endif //!OBJ_HUD_H

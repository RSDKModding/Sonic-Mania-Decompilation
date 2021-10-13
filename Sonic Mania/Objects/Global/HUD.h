#ifndef OBJ_HUD_H
#define OBJ_HUD_H

#include "SonicMania.h"

typedef enum {
    HUDOFF_SCORE,
    HUDOFF_TIME,
    HUDOFF_RINGS,
    HUDOFF_LIFE,
}HUDOffsetTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 hudMappings;
#if RETRO_GAMEVER != VER_100
    uint16 superButtonMappings;
#endif
#if RETRO_USE_PLUS
    uint16 sfxClick;
    uint16 sfxStarpost;
    bool32 dwordC;
    int replaySaveEnabled;
    int compSession[4];
    int swapCooldown;
    int stockFlashTimers[PLAYER_MAX];
#endif
} ObjectHUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 offsets[4];
#if RETRO_USE_PLUS
    int lifeFrameIDs[4];
    int lives[PLAYER_MAX];
#endif
    int maxOffset;
#if RETRO_GAMEVER != VER_100
    int superButtonPos;
#endif
#if RETRO_USE_PLUS
    void (*vsStates[PLAYER_MAX])(void);
    Vector2 vsScoreOffsets[4];
    Vector2 vsTimeOffsets[4];
    Vector2 vsRingsOffsets[4];
    Vector2 vsLifeOffsets[4];
    int vsMaxOffsets[4];
    int screenID;
#endif
#if RETRO_GAMEVER != VER_100
    int timeFlashFrame;
#endif
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
#if RETRO_USE_PLUS
    Animator taData1;
    Animator taData4;
    Animator taData2;
    Animator taData3;
#endif
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

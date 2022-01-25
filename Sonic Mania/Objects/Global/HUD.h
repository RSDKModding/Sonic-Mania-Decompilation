#ifndef OBJ_HUD_H
#define OBJ_HUD_H

#include "SonicMania.h"

typedef enum {
    HUDOFF_SCORE,
    HUDOFF_TIME,
    HUDOFF_RINGS,
    HUDOFF_LIFE,
} HUDOffsetTypes;

typedef enum {
    KEY_A,
    KEY_B,
    KEY_X,
    KEY_Y,
    KEY_START,
} KeyIDs;

// Object Class
struct ObjectHUD {
    RSDK_OBJECT
    uint16 aniFrames;
#if RETRO_GAMEVER != VER_100
    uint16 superButtonFrames;
#endif
#if RETRO_USE_PLUS
    uint16 sfxClick;
    uint16 sfxStarpost;
    bool32 showTAPrompt;
    bool32 replaySaveEnabled;
    int32 screenBorderType[PLAYER_MAX];
    int32 swapCooldown;
    int32 stockFlashTimers[PLAYER_MAX];
#endif
};

// Entity Class
struct EntityHUD {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 scoreOffset;
    Vector2 timeOffset;
    Vector2 ringsOffset;
    Vector2 lifeOffset;
#if RETRO_USE_PLUS
    int32 lifeFrameIDs[PLAYER_MAX];
    int32 lives[PLAYER_MAX];
#endif
    int32 maxOffset;
#if RETRO_GAMEVER != VER_100
    int32 superButtonPos;
#endif
#if RETRO_USE_PLUS
    StateMachine(vsStates[PLAYER_MAX]);
    Vector2 vsScoreOffsets[PLAYER_MAX];
    Vector2 vsTimeOffsets[PLAYER_MAX];
    Vector2 vsRingsOffsets[PLAYER_MAX];
    Vector2 vsLifeOffsets[PLAYER_MAX];
    int32 vsMaxOffsets[PLAYER_MAX];
    int32 screenID;
#endif
#if RETRO_GAMEVER != VER_100
    int32 timeFlashFrame;
#endif
    int32 ringFlashFrame;
    bool32 enableTimeFlash;
    bool32 enableRingFlash;
    Animator hudElementsAnimator;
    Animator numbersAnimator;
    Animator hyperNumbersAnimator;
    Animator lifeIconAnimator;
    Animator playerIDAnimator;
#if RETRO_GAMEVER != VER_100
    Animator superButtonAnimator1;
    Animator superButtonAnimator2;
#endif
#if RETRO_USE_PLUS
    Animator taAnimator1;
    Animator taAnimator4;
    Animator taAnimator2;
    Animator taAnimator3;
#endif
};

// Object Struct
extern ObjectHUD *HUD;

// Standard Entity Events
void HUD_Update(void);
void HUD_LateUpdate(void);
void HUD_StaticUpdate(void);
void HUD_Draw(void);
void HUD_Create(void* data);
void HUD_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HUD_EditorDraw(void);
void HUD_EditorLoad(void);
#endif
void HUD_Serialize(void);

// Extra Entity Functions
void HUD_DrawNumbersBase10(Vector2 *drawPos, int32 value, int32 maxDigits);
void HUD_DrawNumbersBase16(Vector2 *drawPos, int32 value);
void HUD_DrawNumbersHyperRing(Vector2 *drawPos, int32 value);
void HUD_GetKeyFrame(Animator *animator, int32 buttonID);
void HUD_GetSuperFrames(void);
void HUD_State_ComeOnScreen(void);
void HUD_State_GoOffScreen(void);

#endif //!OBJ_HUD_H

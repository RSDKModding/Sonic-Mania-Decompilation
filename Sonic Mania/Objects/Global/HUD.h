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
    uint16 aniFrames;
#if RETRO_GAMEVER != VER_100
    uint16 superButtonFrames;
#endif
#if RETRO_USE_PLUS
    uint16 sfxClick;
    uint16 sfxStarpost;
    bool32 showTAPrompt;
    int32 replaySaveEnabled;
    int32 compSession[PLAYER_MAX];
    int32 swapCooldown;
    int32 stockFlashTimers[PLAYER_MAX];
#endif
} ObjectHUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 offsets[PLAYER_MAX];
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
void HUD_DrawNumbersBase10(Vector2 *drawPos, int32 value, int32 maxDigits);
void HUD_DrawNumbersBase16(Vector2 *drawPos, int32 value);
void HUD_DrawNumbersHyperRing(Vector2 *drawPos, int32 value);
void HUD_GetKeyFrame(Animator *animator, int32 buttonID);
void HUD_GetSuperFrames(void);
void HUD_State_ComeOnScreen(void);
void HUD_State_GoOffScreen(void);

#endif //!OBJ_HUD_H

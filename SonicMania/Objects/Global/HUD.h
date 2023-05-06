#ifndef OBJ_HUD_H
#define OBJ_HUD_H

#include "Game.h"

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
#if GAME_VERSION != VER_100
    uint16 superButtonFrames;
#endif
#if MANIA_USE_PLUS
    uint16 sfxClick;
    uint16 sfxStarpost;
    bool32 showTAPrompt;
    bool32 replaySaveEnabled;
    int32 screenBorderType[PLAYER_COUNT];
    int32 swapCooldown;
    int32 stockFlashTimers[PLAYER_COUNT];
#endif
};

// Entity Class
struct EntityHUD {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 scorePos;
    Vector2 timePos;
    Vector2 ringsPos;
    Vector2 lifePos;
#if MANIA_USE_PLUS
    int32 lifeFrameIDs[PLAYER_COUNT];
    int32 lives[PLAYER_COUNT];
#endif
    int32 targetPos;
#if GAME_VERSION != VER_100
    int32 actionPromptPos;
#endif
#if MANIA_USE_PLUS
    StateMachine(vsStates[PLAYER_COUNT]);
    Vector2 vsScorePos[PLAYER_COUNT];
    Vector2 vsTimePos[PLAYER_COUNT];
    Vector2 vsRingsPos[PLAYER_COUNT];
    Vector2 vsLifePos[PLAYER_COUNT];
    int32 vsTargetPos[PLAYER_COUNT];
    int32 screenID;
#endif
#if GAME_VERSION != VER_100
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
#if GAME_VERSION != VER_100
    Animator superIconAnimator;
    Animator superButtonAnimator;
#endif
#if MANIA_USE_PLUS
    Animator thumbsUpIconAnimator;
    Animator thumbsUpButtonAnimator;
    Animator replayClapAnimator;
    Animator saveReplayButtonAnimator;
#endif
};

// Object Struct
extern ObjectHUD *HUD;

// Standard Entity Events
void HUD_Update(void);
void HUD_LateUpdate(void);
void HUD_StaticUpdate(void);
void HUD_Draw(void);
void HUD_Create(void *data);
void HUD_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HUD_EditorDraw(void);
void HUD_EditorLoad(void);
#endif
void HUD_Serialize(void);

// Extra Entity Functions
void HUD_DrawNumbersBase10(Vector2 *drawPos, int32 value, int32 digitCount);
void HUD_DrawNumbersBase16(Vector2 *drawPos, int32 value);
void HUD_DrawNumbersHyperRing(Vector2 *drawPos, int32 value);

void HUD_GetButtonFrame(Animator *animator, int32 buttonID);
void HUD_GetActionButtonFrames(void);

void HUD_State_MoveIn(void);
void HUD_State_MoveOut(void);

void HUD_MoveIn(EntityHUD *hud);
void HUD_MoveOut(void);

void HUD_EnableRingFlash(void);

int32 HUD_CharacterIndexFromID(int32 characterID);

#endif //! OBJ_HUD_H

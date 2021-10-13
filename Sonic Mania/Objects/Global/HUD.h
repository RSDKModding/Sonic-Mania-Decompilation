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
    int32 replaySaveEnabled;
    int32 compSession[4];
    int32 swapCooldown;
    int32 stockFlashTimers[PLAYER_MAX];
#endif
} ObjectHUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 offsets[4];
#if RETRO_USE_PLUS
    int32 lifeFrameIDs[4];
    int32 lives[PLAYER_MAX];
#endif
    int32 maxOffset;
#if RETRO_GAMEVER != VER_100
    int32 superButtonPos;
#endif
#if RETRO_USE_PLUS
    void (*vsStates[PLAYER_MAX])(void);
    Vector2 vsScoreOffsets[4];
    Vector2 vsTimeOffsets[4];
    Vector2 vsRingsOffsets[4];
    Vector2 vsLifeOffsets[4];
    int32 vsMaxOffsets[4];
    int32 screenID;
#endif
#if RETRO_GAMEVER != VER_100
    int32 timeFlashFrame;
#endif
    int32 ringFlashFrame;
    int32 field_150;
    int32 field_154;
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
void HUD_DrawNumbersBase10(Vector2 *drawPos, int32 value, int32 maxDigits);
void HUD_DrawNumbersBase16(Vector2 *drawPos, int32 value);
void HUD_DrawNumbersHyperRing(Vector2 *drawPos, int32 value);
void HUD_GetKeyFrame(Animator *animator, int32 buttonID);
void HUD_GetSuperFrames(void);
void HUD_State_ComeOnScreen(void);
void HUD_State_GoOffScreen(void);

#endif //!OBJ_HUD_H

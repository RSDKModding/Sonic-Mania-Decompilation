#ifndef OBJ_SPECIALCLEAR_H
#define OBJ_SPECIALCLEAR_H

#include "Game.h"

typedef enum {
    SC_MSG_SPECIALCLEAR,
    SC_MSG_GOTEMERALD,
    SC_MSG_ALLEMERALDS,
    SC_MSG_SUPER,
} SpecialClearTypes;

typedef enum {
    SC_ANI_SONIC,
    SC_ANI_TAILS,
    SC_ANI_KNUX,
#if MANIA_USE_PLUS
    SC_ANI_MIGHTY,
    SC_ANI_RAY,
#endif
    SC_ANI_BONUS,
    SC_ANI_NUMBERS,
    SC_ANI_EMERALDS,
#if MANIA_USE_PLUS
    SC_ANI_CONTINUE,
#endif
} SpecialClearAniIDs;

// Object Class
struct ObjectSpecialClear {
    RSDK_OBJECT
    uint16 aniFrames;
#if !MANIA_USE_PLUS
    uint16 continueFrames;
#endif
    uint16 sfxScoreAdd;
    uint16 sfxScoreTotal;
    uint16 sfxEvent;
    uint16 sfxSpecialWarp;
    uint16 sfxContinue;
    uint16 sfxEmerald;
};

// Entity Class
struct EntitySpecialClear {
    RSDK_ENTITY
    StateMachine(state);
    bool32 isBSS;
    int32 messageType;
    int32 timer;
    bool32 showFade;
    bool32 continueIconVisible;
    bool32 hasContinues;
    int32 fillColor;
    int32 score;
    int32 score1UP;
    int32 lives;
    int32 ringBonus;
    int32 perfectBonus;
    int32 machBonus;
    Vector2 messagePos1;
    Vector2 messagePos2;
    Vector2 scoreBonusPos;
    Vector2 ringBonusPos;
    Vector2 perfectBonusPos;
    Vector2 machBonusPos;
    Vector2 continuePos;
    int32 emeraldPositions[7];
    int32 emeraldSpeeds[7];
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    int32 unused6;
    int32 unused7;
    int32 unused8;
    bool32 saveInProgress;
    Animator playerNameAnimator;
    Animator bonusAnimator;
    Animator numbersAnimator;
    Animator emeraldsAnimator;
    Animator continueAnimator;
};

// Object Struct
extern ObjectSpecialClear *SpecialClear;

// Standard Entity Events
void SpecialClear_Update(void);
void SpecialClear_LateUpdate(void);
void SpecialClear_StaticUpdate(void);
void SpecialClear_Draw(void);
void SpecialClear_Create(void *data);
void SpecialClear_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpecialClear_EditorDraw(void);
void SpecialClear_EditorLoad(void);
#endif
void SpecialClear_Serialize(void);

// Extra Entity Functions
void SpecialClear_DrawNumbers(Vector2 *pos, int32 value);
void SpecialClear_GiveScoreBonus(int32 score);

#if MANIA_USE_PLUS
void SpecialClear_SaveCB(bool32 success);
#else
void SpecialClear_SaveCB(void);
#endif

void SpecialClear_State_SetupDelay(void);
void SpecialClear_State_EnterText(void);
void SpecialClear_State_AdjustText(void);
void SpecialClear_HandleEmeraldAppear(void);
void SpecialClear_State_EnterBonuses(void);
void SpecialClear_State_ScoreShownDelay(void);
void SpecialClear_State_TallyScore(void);
void SpecialClear_State_ShowTotalScore_Continues(void);
void SpecialClear_State_ShowTotalScore_NoContinues(void);
void SpecialClear_State_ExitFinishMessage(void);
void SpecialClear_State_EnterSuperMessage(void);
void SpecialClear_State_ShowSuperMessage(void);
void SpecialClear_State_ExitFadeOut(void);
void SpecialClear_State_ExitResults(void);

#endif //! OBJ_SPECIALCLEAR_H

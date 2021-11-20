#ifndef OBJ_ACTCLEAR_H
#define OBJ_ACTCLEAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxScoreAdd;
    uint16 sfxScoreTotal;
#if RETRO_USE_PLUS
    uint16 sfxEvent;
#endif
    bool32 isTimeAttack;
    int32 finishedSavingGame;
#if RETRO_USE_PLUS
    int32 disableResultsInput;
#endif
    int32 actID;
    bool32 finished;
#if RETRO_USE_PLUS
    bool32 forceNoSave;
    StateMachine(bufferMove_CB);
    StateMachine(saveReplay_CB);
    int32 hasSavedReplay;
    int32 disableTimeBonus;
    bool32 actClearActive;
#endif
} ObjectActClear;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 stageFinishTimer;
    int32 scoreBonus;
    int32 ringBonus;
    int32 coolBonus;
    int32 totalScore;
    int32 time;
    int32 newRecordTimer;
    bool32 achievedRank;
    bool32 isNewRecord;
    bool32 showCoolBonus;
    Vector2 playerNamePos;
    Vector2 gotThroughPos;
    Vector2 timeBonusPos;
    Vector2 ringBonusPos;
    Vector2 coolBonusPos;
    Vector2 totalScorePos;
    void *playerPtr;
    Animator hudElementsAnimator;
    Animator numbersAnimator;
    Animator playerNameAnimator;
    Animator gotThroughAnimator;
    Animator actNumAnimator;
#if RETRO_USE_PLUS
    Animator timeElementsAnimator;
#endif
} EntityActClear;

// Object Struct
extern ObjectActClear *ActClear;

// Standard Entity Events
void ActClear_Update(void);
void ActClear_LateUpdate(void);
void ActClear_StaticUpdate(void);
void ActClear_Draw(void);
void ActClear_Create(void* data);
void ActClear_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ActClear_EditorDraw(void);
void ActClear_EditorLoad(void);
#endif
void ActClear_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void ActClear_DrawTime(Vector2 *pos, int32 mins, int32 secs, int32 millisecs);
#endif
void ActClear_DrawNumbers(Vector2 *pos, int32 value, int32 maxVals);
void ActClear_CheckPlayerVictory(void);
void ActClear_SaveGameCallback(int32 success);
void ActClear_Unknown5(void);

void ActClear_State_EnterText(void);
void ActClear_State_AdjustText(void);
void ActClear_State_EnterBonuses(void);
void ActClear_State_ScoreShownDelay(void);
void ActClear_State_TallyScore(void);
void ActClear_State_SaveGameProgress(void);
#if RETRO_USE_PLUS
void ActClear_State_ShowResultsTA(void);
#endif
void ActClear_State_StartExitSequence(void);
void ActClear_State_ExitActClear(void);

void ActClear_State_ForcePlayerOnScreen(void);

#endif //!OBJ_ACTCLEAR_H

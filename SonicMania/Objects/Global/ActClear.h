#ifndef OBJ_ACTCLEAR_H
#define OBJ_ACTCLEAR_H

#include "Game.h"

// Object Class
struct ObjectActClear {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxScoreAdd;
    uint16 sfxScoreTotal;
#if MANIA_USE_PLUS
    uint16 sfxEvent;
#endif
    bool32 bufferMoveEnabled;
    bool32 isSavingGame;
#if MANIA_USE_PLUS
    bool32 disableResultsInput;
#endif
    int32 displayedActID;
    bool32 finished;
#if MANIA_USE_PLUS
    bool32 forceNoSave;
    StateMachine(bufferMove_CB);
    StateMachine(saveReplay_CB);
    bool32 hasSavedReplay;
    bool32 disableTimeBonus;
    bool32 actClearActive;
#endif
};

// Entity Class
struct EntityActClear {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 stageFinishTimer;
    int32 timeBonus;
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
    EntityPlayer *targetPlayer;
    Animator hudElementsAnimator;
    Animator numbersAnimator;
    Animator playerNameAnimator;
    Animator gotThroughAnimator;
    Animator actNumAnimator;
#if MANIA_USE_PLUS
    Animator timeElementsAnimator;
#endif
};

// Object Struct
extern ObjectActClear *ActClear;

// Standard Entity Events
void ActClear_Update(void);
void ActClear_LateUpdate(void);
void ActClear_StaticUpdate(void);
void ActClear_Draw(void);
void ActClear_Create(void *data);
void ActClear_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ActClear_EditorDraw(void);
void ActClear_EditorLoad(void);
#endif
void ActClear_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void ActClear_DrawTime(Vector2 *drawPosPtr, int32 mins, int32 secs, int32 millisecs);
#endif
void ActClear_DrawNumbers(Vector2 *drawPos, int32 value, int32 digitCount);
void ActClear_CheckPlayerVictory(void);

#if MANIA_USE_PLUS
void ActClear_SaveGameCallback(bool32 success);
#else
void ActClear_SaveGameCallback(void);
#endif
void ActClear_SetupRecoverPlayers(void);

void ActClear_State_EnterText(void);
void ActClear_State_AdjustText(void);
void ActClear_State_EnterResults(void);
void ActClear_State_ScoreShownDelay(void);
void ActClear_State_TallyScore(void);
void ActClear_State_SaveGameProgress(void);
#if MANIA_USE_PLUS
void ActClear_State_ShowResultsTA(void);
#endif
void ActClear_State_WaitForSave(void);
void ActClear_State_ExitActClear(void);

void ActClear_State_RecoverPlayers(void);

#endif //! OBJ_ACTCLEAR_H

#ifndef OBJ_ACTCLEAR_H
#define OBJ_ACTCLEAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
    uint16 sfx_ScoreAdd;
    uint16 sfx_ScoreTotal;
#if RETRO_USE_PLUS
    uint16 sfx_Event;
#endif
    bool32 isTimeAttack;
    int32 field_10;
#if RETRO_USE_PLUS
    int32 field_14;
#endif
    int32 actID;
    int32 field_1C;
#if RETRO_USE_PLUS
    bool32 forceNoSave;
    void (*bufferMove_CB)(void);
    void (*saveReplay_CB)(void);
    int32 field_2C;
    int32 field_30;
    bool32 dword34;
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
    int32 dword78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    Vector2 posUnknown;
    Vector2 posUnknown3;
    Vector2 posUnknown2;
    Vector2 posUnknown4;
    Vector2 posUnknown5;
    Vector2 posUnknown6;
    void *playerPtr;
    Animator animator1;
    Animator animator2;
    Animator playerNameData;
    Animator gotThroughData;
    Animator actNoData;
#if RETRO_USE_PLUS
    Animator data3;
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
void ActClear_EditorDraw(void);
void ActClear_EditorLoad(void);
void ActClear_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void ActClear_DrawTime(int32 mins, Vector2 *pos, int32 secs, int32 millisecs);
#endif
void ActClear_DrawNumbers(Vector2 *pos, int32 value, int32 maxVals);
void ActClear_CheckPlayerVictory(void);
void ActClear_SaveGameCallback(int32 success);
void ActClear_Unknown5(void);

void ActClear_Unknown6(void);
void ActClear_Unknown7(void);
void ActClear_State_TAFinish(void);
void ActClear_Unknown8(void);
void ActClear_TallyScore(void);
void ActClear_LoadNextScene(void);
#if RETRO_USE_PLUS
void ActClear_State_TAResults(void);
#endif
void ActClear_Unknown10(void);
void ActClear_State_ActFinish(void);

void ActClear_ForcePlayerOnScreen(void);

#endif //!OBJ_ACTCLEAR_H

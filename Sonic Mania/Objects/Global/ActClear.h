#ifndef OBJ_ACTCLEAR_H
#define OBJ_ACTCLEAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfx_ScoreAdd;
    ushort sfx_ScoreTotal;
#if RETRO_USE_PLUS
    ushort sfx_Event;
#endif
    bool32 isTimeAttack;
    int field_10;
#if RETRO_USE_PLUS
    int field_14;
#endif
    int actID;
    int field_1C;
#if RETRO_USE_PLUS
    bool32 forceNoSave;
    void (*bufferMove_CB)(void);
    void (*saveReplay_CB)(void);
    int field_2C;
    int field_30;
    bool32 dword34;
#endif
} ObjectActClear;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int stageFinishTimer;
    int scoreBonus;
    int ringBonus;
    int coolBonus;
    int totalScore;
    int time;
    int dword78;
    int field_7C;
    int field_80;
    int field_84;
    Vector2 posUnknown;
    Vector2 posUnknown3;
    Vector2 posUnknown2;
    Vector2 posUnknown4;
    Vector2 posUnknown5;
    Vector2 posUnknown6;
    void *playerPtr;
    Animator data1;
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
void ActClear_DrawTime(int mins, Vector2 *pos, int secs, int millisecs);
#endif
void ActClear_DrawNumbers(Vector2 *pos, int value, signed int maxVals);
void ActClear_CheckPlayerVictory(void);
void ActClear_SaveGameCallback(int success);
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

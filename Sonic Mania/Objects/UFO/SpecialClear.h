#ifndef OBJ_SPECIALCLEAR_H
#define OBJ_SPECIALCLEAR_H

#include "SonicMania.h"

typedef enum {
    SC_ANI_SONIC,
    SC_ANI_TAILS,
    SC_ANI_KNUX,
#if RETRO_USE_PLUS
    SC_ANI_MIGHTY,
    SC_ANI_RAY,
#endif
    SC_ANI_BONUS,
    SC_ANI_NUMBERS,
    SC_ANI_EMERALDS,
#if RETRO_USE_PLUS
    SC_ANI_CONTINUE,
#endif
}SpecialClearAniIDs;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
#if !RETRO_USE_PLUS
    uint16 continueFrames;
#endif
    uint16 sfx_ScoreAdd;
    uint16 sfx_ScoreTotal;
    uint16 sfx_Event;
    uint16 sfx_SpecialWarp;
    uint16 sfx_Continue;
    uint16 sfx_Emerald;
} ObjectSpecialClear;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    bool32 isBSS;
    int32 finishType;
    int32 timer;
    int32 flag;
    int32 field_6C;
    bool32 continuesFlag;
    int32 fillColour;
    int32 score;
    int32 score1UP;
    int32 lives;
    int32 ringBonus;
    int32 perfectBonus;
    int32 machBonus;
    Vector2 positions[7];
    int32 emeraldPositions[7];
    int32 emeraldSpeeds[7];
    int32 field_100;
    int32 field_104;
    int32 field_108;
    int32 field_10C;
    int32 field_110;
    int32 field_114;
    int32 field_118;
    int32 field_11C;
    int32 field_120;
    Animator data1;
    Animator animator2;
    Animator data3;
    Animator data4;
    Animator data5;
} EntitySpecialClear;

// Object Struct
extern ObjectSpecialClear *SpecialClear;

// Standard Entity Events
void SpecialClear_Update(void);
void SpecialClear_LateUpdate(void);
void SpecialClear_StaticUpdate(void);
void SpecialClear_Draw(void);
void SpecialClear_Create(void* data);
void SpecialClear_StageLoad(void);
void SpecialClear_EditorDraw(void);
void SpecialClear_EditorLoad(void);
void SpecialClear_Serialize(void);

// Extra Entity Functions
void SpecialClear_DrawNumbers(Vector2 *pos, int32 value);
void SpecialClear_GiveScoreBonus(int32 score);
void SpecialClear_SaveCB(int32 success);
void SpecialClear_LoadScene(void);
void SpecialClear_TallyScore(void);

void SpecialClear_Unknown3(void);
void SpecialClear_Unknown4(void);
void SpecialClear_Unknown5(void);
void SpecialClear_HandlePositions(void);
void SpecialClear_Unknown6(void);
void SpecialClear_Unknown7(void);
void SpecialClear_Unknown9(void);
void SpecialClear_Unknown10(void);
void SpecialClear_Unknown11(void);
void SpecialClear_Unknown12(void);
void SpecialClear_Unknown13(void);
void SpecialClear_Unknown15(void);

#endif //!OBJ_SPECIALCLEAR_H

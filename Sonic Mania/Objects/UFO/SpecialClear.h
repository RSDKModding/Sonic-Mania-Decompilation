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
struct ObjectSpecialClear {
    RSDK_OBJECT
    uint16 aniFrames;
#if !RETRO_USE_PLUS
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
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    int32 unused6;
    int32 unused7;
    int32 unused8;
    bool32 saveInProgress;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
};

// Object Struct
extern ObjectSpecialClear *SpecialClear;

// Standard Entity Events
void SpecialClear_Update(void);
void SpecialClear_LateUpdate(void);
void SpecialClear_StaticUpdate(void);
void SpecialClear_Draw(void);
void SpecialClear_Create(void* data);
void SpecialClear_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SpecialClear_EditorDraw(void);
void SpecialClear_EditorLoad(void);
#endif
void SpecialClear_Serialize(void);

// Extra Entity Functions
void SpecialClear_DrawNumbers(Vector2 *pos, int32 value);
void SpecialClear_GiveScoreBonus(int32 score);
void SpecialClear_SaveCB(bool32 success);
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

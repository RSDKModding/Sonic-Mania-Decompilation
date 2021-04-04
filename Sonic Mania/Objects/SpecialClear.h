#ifndef OBJ_SPECIALCLEAR_H
#define OBJ_SPECIALCLEAR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfx_ScoreAdd;
    ushort sfx_ScoreTotal;
    ushort sfx_Event;
    ushort sfx_SpecialWarp;
    ushort sfx_Continue;
    ushort sfx_Emerald;
} ObjectSpecialClear;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)(void);
    bool32 isBSS;
    int finishType;
    int timer;
    int flag;
    int field_6C;
    bool32 continuesFlag;
    int fillColour;
    int score;
    int score1UP;
    int lives;
    int ringBonus;
    int perfectBonus;
    int machBonus;
    Vector2 positions[15];
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    AnimationData data1;
    AnimationData data2;
    AnimationData data3;
    AnimationData data4;
    AnimationData data5;
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
void SpecialClear_DrawNumbers(Vector2 *pos, int value);
void SpecialClear_GiveScoreBonus(int score);
void SpecialClear_SaveCB(int success);
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

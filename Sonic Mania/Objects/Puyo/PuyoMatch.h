#ifndef OBJ_PUYOMATCH_H
#define OBJ_PUYOMATCH_H

#include "SonicMania.h"

// Object Class
struct ObjectPuyoMatch {
    RSDK_OBJECT
    int32 value1;
    TABLE(int32 value2[24], { 4, 20, 24, 32, 48, 96, 160, 240, 320, 480, 600, 700, 800, 900, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999 });
    TABLE(int32 value3[24], { 0, 8, 16, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 480, 512, 544, 576, 608, 640, 672 });
    TABLE(int32 value4[8], { 0, 2, 3, 4, 5, 6, 7, 10 });
    TABLE(int32 value5[6], { 0, 0, 3, 6, 12, 24 });
    TABLE(int32 value6[24], { 0, 2, 5, 1, 3, 4, 5, 3, 4, 0, 1, 2, 3, 1, 0, 4, 2, 5, 0, 2, 1, 3, 5, 4 });
    uint16 aniFrames;
};

// Entity Class
struct EntityPuyoMatch {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateInput);
    StateMachine(matchFinishCB);
    StateMachine(matchLoseCB);
    int32 playerID;
    int32 score;
    int32 comboScore;
    int32 field_74;
    int32 comboCount;
    int32 comboBeanCount;
    int32 *field_80;
    int32 field_84;
    int32 field_88;
    int32 timer;
    int32 field_90;
    int32 field_94;
    Vector2 beanDropPos;
    EntityPuyoBean *beanPtr;
    int32 matchKey;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
};

// Object Struct
extern ObjectPuyoMatch *PuyoMatch;

// Standard Entity Events
void PuyoMatch_Update(void);
void PuyoMatch_LateUpdate(void);
void PuyoMatch_StaticUpdate(void);
void PuyoMatch_Draw(void);
void PuyoMatch_Create(void* data);
void PuyoMatch_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PuyoMatch_EditorDraw(void);
void PuyoMatch_EditorLoad(void);
#endif
void PuyoMatch_Serialize(void);

// Extra Entity Functions
void PuyoMatch_StartPuyoAttack(int playerID, int score);
void PuyoMatch_SetupNextBeans(EntityPuyoMatch *match);
void PuyoMatch_DropNextBeans(void);
void PuyoMatch_Unknown4(void);
void PuyoMatch_DrawNumbers(void);

void PuyoMatch_State_HandleMatch(void);
void PuyoMatch_State_HandleCombos(void);
void PuyoMatch_State_HandleComboEnd(void);
void PuyoMatch_State_Lose(void);

#endif //!OBJ_PUYOMATCH_H

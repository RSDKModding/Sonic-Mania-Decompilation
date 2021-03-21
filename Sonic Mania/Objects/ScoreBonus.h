#ifndef OBJ_SCOREBONUS_H
#define OBJ_SCOREBONUS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectScoreBonus;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int timer;
    AnimationData data;
} EntityScoreBonus;

// Object Struct
extern ObjectScoreBonus *ScoreBonus;

// Standard Entity Events
void ScoreBonus_Update();
void ScoreBonus_LateUpdate();
void ScoreBonus_StaticUpdate();
void ScoreBonus_Draw();
void ScoreBonus_Create(void* data);
void ScoreBonus_StageLoad();
void ScoreBonus_EditorDraw();
void ScoreBonus_EditorLoad();
void ScoreBonus_Serialize();

// Extra Entity Functions


#endif //!OBJ_SCOREBONUS_H

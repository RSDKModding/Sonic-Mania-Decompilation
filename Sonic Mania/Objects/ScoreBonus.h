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
void ScoreBonus_Update(void);
void ScoreBonus_LateUpdate(void);
void ScoreBonus_StaticUpdate(void);
void ScoreBonus_Draw(void);
void ScoreBonus_Create(void* data);
void ScoreBonus_StageLoad(void);
void ScoreBonus_EditorDraw(void);
void ScoreBonus_EditorLoad(void);
void ScoreBonus_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SCOREBONUS_H

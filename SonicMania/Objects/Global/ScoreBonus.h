#ifndef OBJ_SCOREBONUS_H
#define OBJ_SCOREBONUS_H

#include "Game.h"

// Object Class
struct ObjectScoreBonus {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityScoreBonus {
    RSDK_ENTITY
    int32 timer;
    Animator animator;
};

// Object Struct
extern ObjectScoreBonus *ScoreBonus;

// Standard Entity Events
void ScoreBonus_Update(void);
void ScoreBonus_LateUpdate(void);
void ScoreBonus_StaticUpdate(void);
void ScoreBonus_Draw(void);
void ScoreBonus_Create(void *data);
void ScoreBonus_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ScoreBonus_EditorDraw(void);
void ScoreBonus_EditorLoad(void);
#endif
void ScoreBonus_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_SCOREBONUS_H

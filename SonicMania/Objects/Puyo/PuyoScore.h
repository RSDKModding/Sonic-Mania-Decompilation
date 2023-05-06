#ifndef OBJ_PUYOSCORE_H
#define OBJ_PUYOSCORE_H

#include "Game.h"

// Object Class
struct ObjectPuyoScore {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityPuyoScore {
    RSDK_ENTITY
    StateMachine(state);
    int32 playerID;
    bool32 counter;
    bool32 flashing;
    int32 score;
    Animator animator;
};

// Object Struct
extern ObjectPuyoScore *PuyoScore;

// Standard Entity Events
void PuyoScore_Update(void);
void PuyoScore_LateUpdate(void);
void PuyoScore_StaticUpdate(void);
void PuyoScore_Draw(void);
void PuyoScore_Create(void *data);
void PuyoScore_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PuyoScore_EditorDraw(void);
void PuyoScore_EditorLoad(void);
#endif
void PuyoScore_Serialize(void);

// Extra Entity Functions
void PuyoScore_DrawScore(int32 score);

#endif //! OBJ_PUYOSCORE_H

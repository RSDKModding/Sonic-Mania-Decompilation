#ifndef OBJ_PUYOGAME_H
#define OBJ_PUYOGAME_H

#include "Game.h"

typedef enum {
    PUYOGAME_PLAYER1,
    PUYOGAME_PLAYER2,
} PuyoGamePlayerIDs;

// Object Class
struct ObjectPuyoGame {
    RSDK_OBJECT
    uint16 aniFrames;
    EntityPuyoMatch *managers[2];
    EntityPuyoIndicator *indicators[2];
    EntityCollapsingPlatform *platforms[2];
    EntityPuyoScore *score1[2];
    EntityPuyoScore *score2[2];
    EntityPuyoLevelSelect *levelSel[2];
    EntityFXFade *fxFade;
};

// Entity Class
struct EntityPuyoGame {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    Vector2 startPos;
    int32 timer;
    int32 selectedLevels[2];
    int32 unused2;
    int32 unused3;
    Animator animator;
    bool32 started;
    int32 finishedMatch;
    bool32 determinedLoser;
    uint8 roundWinner;
    uint8 matchWinner;
};

// Object Struct
extern ObjectPuyoGame *PuyoGame;

// Standard Entity Events
void PuyoGame_Update(void);
void PuyoGame_LateUpdate(void);
void PuyoGame_StaticUpdate(void);
void PuyoGame_Draw(void);
void PuyoGame_Create(void *data);
void PuyoGame_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PuyoGame_EditorDraw(void);
void PuyoGame_EditorLoad(void);
#endif
void PuyoGame_Serialize(void);

// Extra Entity Functions
void PuyoGame_SetupStartingEntities(void);
void PuyoGame_SetLoser(uint8 loser);
void PuyoGame_DestroyPuyoBeans(void);
void PuyoGame_SetupGameState(void);
void PuyoGame_CheckMatchFinish(void);
void PuyoGame_State_Init(void);
void PuyoGame_State_Wait(void);
void PuyoGame_State_SelectingLevel(void);
void PuyoGame_State_SetupRound(void);
void PuyoGame_State_SetupEntities(void);
void PuyoGame_State_HandleRound(void);
void PuyoGame_State_ShowRoundResults(void);
void PuyoGame_State_ShowMatchResults(void);
void PuyoGame_State_FadeToMenu(void);

#endif //! OBJ_PUYOGAME_H

#ifndef OBJ_GAMEOVER_H
#define OBJ_GAMEOVER_H

#include "Game.h"

#define GAMEOVER_LETTER_COUNT (8)

// Object Class
struct ObjectGameOver {
    RSDK_OBJECT
    int32 activeScreens;
    uint16 aniFrames;
};

// Entity Class
struct EntityGameOver {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 playerID;
    Vector2 barPos;
    Vector2 verts[4];
    Vector2 letterPositions[GAMEOVER_LETTER_COUNT];
    Vector2 finalOffsets[GAMEOVER_LETTER_COUNT];
    Vector2 letterPosMove[GAMEOVER_LETTER_COUNT];
    int32 letterBounceCount[GAMEOVER_LETTER_COUNT];
    int32 letterRotations[GAMEOVER_LETTER_COUNT];
    int32 letterRotateSpeed[GAMEOVER_LETTER_COUNT];
    Animator animator;
};

// Object Struct
extern ObjectGameOver *GameOver;

// Standard Entity Events
void GameOver_Update(void);
void GameOver_LateUpdate(void);
void GameOver_StaticUpdate(void);
void GameOver_Draw(void);
void GameOver_Create(void *data);
void GameOver_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GameOver_EditorDraw(void);
void GameOver_EditorLoad(void);
#endif
void GameOver_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void GameOver_SaveGameCallback(bool32 success);
#else
void GameOver_SaveGameCallback(void);
#endif

void GameOver_State_EnterLetters(void);
#if MANIA_USE_PLUS
void GameOver_State_WaitComp(void);
#endif
void GameOver_State_Wait(void);
void GameOver_State_ExitLetters(void);

#endif //! OBJ_GAMEOVER_H

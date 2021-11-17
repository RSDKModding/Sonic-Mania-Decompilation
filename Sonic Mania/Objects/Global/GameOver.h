#ifndef OBJ_GAMEOVER_H
#define OBJ_GAMEOVER_H

#include "SonicMania.h"

#define GameOver_LetterCount (8)

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 activeScreens;
    uint16 aniFrames;
} ObjectGameOver;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 playerID;
    Vector2 barPos;
    Vector2 verts[4];
    Vector2 letterPositions[GameOver_LetterCount];
    Vector2 unknownPos1[GameOver_LetterCount];
    Vector2 letterPosMove[GameOver_LetterCount];
    int32 letterBounceCount[GameOver_LetterCount];
    int32 letterRotations[GameOver_LetterCount];
    int32 letterRotateSpeed[GameOver_LetterCount];
    Animator animator;
} EntityGameOver;

// Object Struct
extern ObjectGameOver *GameOver;

// Standard Entity Events
void GameOver_Update(void);
void GameOver_LateUpdate(void);
void GameOver_StaticUpdate(void);
void GameOver_Draw(void);
void GameOver_Create(void* data);
void GameOver_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GameOver_EditorDraw(void);
void GameOver_EditorLoad(void);
#endif
void GameOver_Serialize(void);

// Extra Entity Functions
void GameOver_SaveGameCallback(int32 status);
void GameOver_State_EnterLetters(void);
void GameOver_State_HandleMultiplayer(void);
void GameOver_State_ShowMessage(void);
void GameOver_State_ExitLetters(void);

#endif //!OBJ_GAMEOVER_H

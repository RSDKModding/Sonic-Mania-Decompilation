#ifndef OBJ_GAMEOVER_H
#define OBJ_GAMEOVER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int activeScreens;
    ushort spriteIndex;
} ObjectGameOver;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int playerID;
    Vector2 dword64;
    Vector2 verts[4];
    Vector2 letterPositions[8];
    Vector2 unknownPos1[8];
    Vector2 letterPosMove[8];
    int unknownArray1[8];
    int letterRotations[8];
    int letterRotateSpeed[8];
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
void GameOver_EditorDraw(void);
void GameOver_EditorLoad(void);
void GameOver_Serialize(void);

// Extra Entity Functions
void GameOver_SaveGameCallback(int status);
void GameOver_Unknown2(void);
void GameOver_Unknown3(void);
void GameOver_Unknown4(void);
void GameOver_Unknown5(void);

#endif //!OBJ_GAMEOVER_H

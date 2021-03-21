#ifndef OBJ_GAMEOVER_H
#define OBJ_GAMEOVER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int activeScreens;
    ushort spriteIndex;
} ObjectGameOver;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    int timer;
    int playerID;
    int dword64;
    int dword68;
    Vector2 verts[4];
    Vector2 letterPositions[8];
    Vector2 unknownPos1[8];
    Vector2 letterPosMove[8];
    int unknownArray1[8];
    int letterRotations[8];
    int letterRotateSpeed[8];
    AnimationData data;
} EntityGameOver;

// Object Struct
extern ObjectGameOver *GameOver;

// Standard Entity Events
void GameOver_Update();
void GameOver_LateUpdate();
void GameOver_StaticUpdate();
void GameOver_Draw();
void GameOver_Create(void* data);
void GameOver_StageLoad();
void GameOver_EditorDraw();
void GameOver_EditorLoad();
void GameOver_Serialize();

// Extra Entity Functions
void GameOver_SaveGameCallback();
void GameOver_Unknown2();
void GameOver_Unknown3();
void GameOver_Unknown4();
void GameOver_Unknown5();

#endif //!OBJ_GAMEOVER_H

#ifndef OBJ_GAMEOVER_H
#define OBJ_GAMEOVER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGameOver : Object {

};

// Entity Class
struct EntityGameOver : Entity {

};

// Object Entity
extern ObjectGameOver GameOver;

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


#endif //!OBJ_GAMEOVER_H

#ifndef OBJ_GAMEPROGRESS_H
#define OBJ_GAMEPROGRESS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGameProgress : Object {

};

#if !RETRO_USE_PLUS
// Entity Class
struct EntityGameProgress : Entity {
};
#endif

// Object Struct
extern ObjectGameProgress *GameProgress;

// Standard Entity Events
void GameProgess_Update();
void GameProgess_LateUpdate();
void GameProgess_StaticUpdate();
void GameProgess_Draw();
void GameProgess_Create(void *data);
void GameProgess_StageLoad();
void GameProgess_EditorDraw();
void GameProgess_EditorLoad();
void GameProgess_Serialize();

#endif //!OBJ_GAMEPROGRESS_H

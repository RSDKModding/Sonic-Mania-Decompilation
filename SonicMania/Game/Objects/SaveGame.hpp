#ifndef OBJ_SAVEGAME_H
#define OBJ_SAVEGAME_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSaveGame : Object {

};

// Entity Class
struct EntitySaveGame : Entity {

};

// Object Entity
extern ObjectSaveGame SaveGame;

// Standard Entity Events
void SaveGame_Update();
void SaveGame_LateUpdate();
void SaveGame_StaticUpdate();
void SaveGame_Draw();
void SaveGame_Create(void* data);
void SaveGame_StageLoad();
void SaveGame_EditorDraw();
void SaveGame_EditorLoad();
void SaveGame_Serialize();

// Extra Entity Functions


#endif //!OBJ_SAVEGAME_H

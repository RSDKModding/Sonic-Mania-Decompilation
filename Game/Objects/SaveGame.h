#ifndef OBJ_SAVEGAME_H
#define OBJ_SAVEGAME_H

#include "../../SonicMania.h"

//Object Class
class ObjectSaveGame : public Object {
public:

};

//Entity Class
class EntitySaveGame : public Entity {
public:

};

//Object Entity
ObjectSaveGame SaveGame;

//Entity Functions
void SaveGame_Update();
void SaveGame_EarlyUpdate();
void SaveGame_LateUpdate();
void SaveGame_Draw();
void SaveGame_Setup(void* subtype);
void SaveGame_StageLoad();
void SaveGame_GetAttributes();

#endif //!OBJ_SAVEGAME_H

#ifndef OBJ_LEVELSELECT_H
#define OBJ_LEVELSELECT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLevelSelect : Object {

};

// Entity Class
struct EntityLevelSelect : Entity {

};

// Object Entity
extern ObjectLevelSelect LevelSelect;

// Standard Entity Events
void LevelSelect_Update();
void LevelSelect_LateUpdate();
void LevelSelect_StaticUpdate();
void LevelSelect_Draw();
void LevelSelect_Create(void* data);
void LevelSelect_StageLoad();
void LevelSelect_EditorDraw();
void LevelSelect_EditorLoad();
void LevelSelect_Serialize();

// Extra Entity Functions


#endif //!OBJ_LEVELSELECT_H

#ifndef OBJ_LEVELSELECT_H
#define OBJ_LEVELSELECT_H

#include "../../SonicMania.h"

//Object Class
class ObjectLevelSelect : public Object {
public:

};

//Entity Class
class EntityLevelSelect : public Entity {
public:

};

//Object Entity
ObjectLevelSelect LevelSelect;

//Entity Functions
void LevelSelect_Update();
void LevelSelect_EarlyUpdate();
void LevelSelect_LateUpdate();
void LevelSelect_Draw();
void LevelSelect_Setup(void* subtype);
void LevelSelect_StageLoad();
void LevelSelect_GetAttributes();

#endif //!OBJ_LEVELSELECT_H

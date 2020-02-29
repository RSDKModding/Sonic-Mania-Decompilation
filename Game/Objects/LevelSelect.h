#ifndef OBJ_LEVELSELECT_H
#define OBJ_LEVELSELECT_H

//Object Class
class LevelSelect : Object {

};

//Entity Class
class EntityLevelSelect : Entity {

};

//Entity Functions
void LevelSelect_Update();
void LevelSelect_EarlyUpdate();
void LevelSelect_LateUpdate();
void LevelSelect_Draw();
void LevelSelect_Setup(void* subtype);
void LevelSelect_StageLoad();
void LevelSelect_GetAttributes();

#endif //!OBJ_LEVELSELECT_H

#ifndef OBJ_CUTSCENEHBH_H
#define OBJ_CUTSCENEHBH_H

//Object Class
class CutsceneHBH : Object {

};

//Entity Class
class EntityCutsceneHBH : Entity {

};

//Entity Functions
void CutsceneHBH_Update();
void CutsceneHBH_EarlyUpdate();
void CutsceneHBH_LateUpdate();
void CutsceneHBH_Draw();
void CutsceneHBH_Setup(void* subtype);
void CutsceneHBH_StageLoad();
void CutsceneHBH_GetAttributes();

#endif //!OBJ_CUTSCENEHBH_H

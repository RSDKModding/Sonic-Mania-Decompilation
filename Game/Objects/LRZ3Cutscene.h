#ifndef OBJ_LRZ3CUTSCENE_H
#define OBJ_LRZ3CUTSCENE_H

//Object Class
class LRZ3Cutscene : Object {

};

//Entity Class
class EntityLRZ3Cutscene : Entity {

};

//Entity Functions
void LRZ3Cutscene_Update();
void LRZ3Cutscene_EarlyUpdate();
void LRZ3Cutscene_LateUpdate();
void LRZ3Cutscene_Draw();
void LRZ3Cutscene_Setup(void* subtype);
void LRZ3Cutscene_StageLoad();
void LRZ3Cutscene_GetAttributes();

#endif //!OBJ_LRZ3CUTSCENE_H

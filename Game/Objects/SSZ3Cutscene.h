#ifndef OBJ_SSZ3CUTSCENE_H
#define OBJ_SSZ3CUTSCENE_H

//Object Class
class SSZ3Cutscene : Object {

};

//Entity Class
class EntitySSZ3Cutscene : Entity {

};

//Entity Functions
void SSZ3Cutscene_Update();
void SSZ3Cutscene_EarlyUpdate();
void SSZ3Cutscene_LateUpdate();
void SSZ3Cutscene_Draw();
void SSZ3Cutscene_Setup(void* subtype);
void SSZ3Cutscene_StageLoad();
void SSZ3Cutscene_GetAttributes();

#endif //!OBJ_SSZ3CUTSCENE_H

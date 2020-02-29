#ifndef OBJ_MSZ2CUTSCENE_H
#define OBJ_MSZ2CUTSCENE_H

//Object Class
class MSZ2Cutscene : Object {

};

//Entity Class
class EntityMSZ2Cutscene : Entity {

};

//Entity Functions
void MSZ2Cutscene_Update();
void MSZ2Cutscene_EarlyUpdate();
void MSZ2Cutscene_LateUpdate();
void MSZ2Cutscene_Draw();
void MSZ2Cutscene_Setup(void* subtype);
void MSZ2Cutscene_StageLoad();
void MSZ2Cutscene_GetAttributes();

#endif //!OBJ_MSZ2CUTSCENE_H

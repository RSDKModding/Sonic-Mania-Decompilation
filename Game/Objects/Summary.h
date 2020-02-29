#ifndef OBJ_SUMMARY_H
#define OBJ_SUMMARY_H

//Object Class
class Summary : Object {

};

//Entity Class
class EntitySummary : Entity {

};

//Entity Functions
void Summary_Update();
void Summary_EarlyUpdate();
void Summary_LateUpdate();
void Summary_Draw();
void Summary_Setup(void* subtype);
void Summary_StageLoad();
void Summary_GetAttributes();

#endif //!OBJ_SUMMARY_H

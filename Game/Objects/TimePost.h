#ifndef OBJ_TIMEPOST_H
#define OBJ_TIMEPOST_H

//Object Class
class TimePost : Object {

};

//Entity Class
class EntityTimePost : Entity {

};

//Entity Functions
void TimePost_Update();
void TimePost_EarlyUpdate();
void TimePost_LateUpdate();
void TimePost_Draw();
void TimePost_Setup(void* subtype);
void TimePost_StageLoad();
void TimePost_GetAttributes();

#endif //!OBJ_TIMEPOST_H

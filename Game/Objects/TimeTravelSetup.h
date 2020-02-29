#ifndef OBJ_TIMETRAVELSETUP_H
#define OBJ_TIMETRAVELSETUP_H

//Object Class
class TimeTravelSetup : Object {

};

//Entity Class
class EntityTimeTravelSetup : Entity {

};

//Entity Functions
void TimeTravelSetup_Update();
void TimeTravelSetup_EarlyUpdate();
void TimeTravelSetup_LateUpdate();
void TimeTravelSetup_Draw();
void TimeTravelSetup_Setup(void* subtype);
void TimeTravelSetup_StageLoad();
void TimeTravelSetup_GetAttributes();

#endif //!OBJ_TIMETRAVELSETUP_H

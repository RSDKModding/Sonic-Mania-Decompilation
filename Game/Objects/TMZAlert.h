#ifndef OBJ_TMZALERT_H
#define OBJ_TMZALERT_H

//Object Class
class TMZAlert : Object {

};

//Entity Class
class EntityTMZAlert : Entity {

};

//Entity Functions
void TMZAlert_Update();
void TMZAlert_EarlyUpdate();
void TMZAlert_LateUpdate();
void TMZAlert_Draw();
void TMZAlert_Setup(void* subtype);
void TMZAlert_StageLoad();
void TMZAlert_GetAttributes();

#endif //!OBJ_TMZALERT_H

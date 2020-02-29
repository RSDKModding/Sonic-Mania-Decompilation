#ifndef OBJ_TMZCABLE_H
#define OBJ_TMZCABLE_H

//Object Class
class TMZCable : Object {

};

//Entity Class
class EntityTMZCable : Entity {

};

//Entity Functions
void TMZCable_Update();
void TMZCable_EarlyUpdate();
void TMZCable_LateUpdate();
void TMZCable_Draw();
void TMZCable_Setup(void* subtype);
void TMZCable_StageLoad();
void TMZCable_GetAttributes();

#endif //!OBJ_TMZCABLE_H

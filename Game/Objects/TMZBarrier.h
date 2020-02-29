#ifndef OBJ_TMZBARRIER_H
#define OBJ_TMZBARRIER_H

//Object Class
class TMZBarrier : Object {

};

//Entity Class
class EntityTMZBarrier : Entity {

};

//Entity Functions
void TMZBarrier_Update();
void TMZBarrier_EarlyUpdate();
void TMZBarrier_LateUpdate();
void TMZBarrier_Draw();
void TMZBarrier_Setup(void* subtype);
void TMZBarrier_StageLoad();
void TMZBarrier_GetAttributes();

#endif //!OBJ_TMZBARRIER_H

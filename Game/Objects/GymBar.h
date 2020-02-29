#ifndef OBJ_GYMBAR_H
#define OBJ_GYMBAR_H

//Object Class
class GymBar : Object {

};

//Entity Class
class EntityGymBar : Entity {

};

//Entity Functions
void GymBar_Update();
void GymBar_EarlyUpdate();
void GymBar_LateUpdate();
void GymBar_Draw();
void GymBar_Setup(void* subtype);
void GymBar_StageLoad();
void GymBar_GetAttributes();

#endif //!OBJ_GYMBAR_H

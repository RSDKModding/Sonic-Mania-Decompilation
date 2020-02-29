#ifndef OBJ_STAIRCASE_H
#define OBJ_STAIRCASE_H

//Object Class
class Staircase : Object {

};

//Entity Class
class EntityStaircase : Entity {

};

//Entity Functions
void Staircase_Update();
void Staircase_EarlyUpdate();
void Staircase_LateUpdate();
void Staircase_Draw();
void Staircase_Setup(void* subtype);
void Staircase_StageLoad();
void Staircase_GetAttributes();

#endif //!OBJ_STAIRCASE_H

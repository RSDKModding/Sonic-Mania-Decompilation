#ifndef OBJ_CURRENT_H
#define OBJ_CURRENT_H

//Object Class
class Current : Object {

};

//Entity Class
class EntityCurrent : Entity {

};

//Entity Functions
void Current_Update();
void Current_EarlyUpdate();
void Current_LateUpdate();
void Current_Draw();
void Current_Setup(void* subtype);
void Current_StageLoad();
void Current_GetAttributes();

#endif //!OBJ_CURRENT_H

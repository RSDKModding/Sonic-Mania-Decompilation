#ifndef OBJ_ERZMYSTIC_H
#define OBJ_ERZMYSTIC_H

//Object Class
class ERZMystic : Object {

};

//Entity Class
class EntityERZMystic : Entity {

};

//Entity Functions
void ERZMystic_Update();
void ERZMystic_EarlyUpdate();
void ERZMystic_LateUpdate();
void ERZMystic_Draw();
void ERZMystic_Setup(void* subtype);
void ERZMystic_StageLoad();
void ERZMystic_GetAttributes();

#endif //!OBJ_ERZMYSTIC_H

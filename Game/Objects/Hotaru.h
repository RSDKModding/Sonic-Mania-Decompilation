#ifndef OBJ_HOTARU_H
#define OBJ_HOTARU_H

//Object Class
class Hotaru : Object {

};

//Entity Class
class EntityHotaru : Entity {

};

//Entity Functions
void Hotaru_Update();
void Hotaru_EarlyUpdate();
void Hotaru_LateUpdate();
void Hotaru_Draw();
void Hotaru_Setup(void* subtype);
void Hotaru_StageLoad();
void Hotaru_GetAttributes();

#endif //!OBJ_HOTARU_H

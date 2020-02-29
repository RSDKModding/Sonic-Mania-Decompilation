#ifndef OBJ_CRANE_H
#define OBJ_CRANE_H

//Object Class
class Crane : Object {

};

//Entity Class
class EntityCrane : Entity {

};

//Entity Functions
void Crane_Update();
void Crane_EarlyUpdate();
void Crane_LateUpdate();
void Crane_Draw();
void Crane_Setup(void* subtype);
void Crane_StageLoad();
void Crane_GetAttributes();

#endif //!OBJ_CRANE_H

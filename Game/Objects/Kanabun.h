#ifndef OBJ_KANABUN_H
#define OBJ_KANABUN_H

//Object Class
class Kanabun : Object {

};

//Entity Class
class EntityKanabun : Entity {

};

//Entity Functions
void Kanabun_Update();
void Kanabun_EarlyUpdate();
void Kanabun_LateUpdate();
void Kanabun_Draw();
void Kanabun_Setup(void* subtype);
void Kanabun_StageLoad();
void Kanabun_GetAttributes();

#endif //!OBJ_KANABUN_H

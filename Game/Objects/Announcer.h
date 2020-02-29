#ifndef OBJ_ANNOUNCER_H
#define OBJ_ANNOUNCER_H

//Object Class
class Announcer : Object {

};

//Entity Class
class EntityAnnouncer : Entity {

};

//Entity Functions
void Announcer_Update();
void Announcer_EarlyUpdate();
void Announcer_LateUpdate();
void Announcer_Draw();
void Announcer_Setup(void* subtype);
void Announcer_StageLoad();
void Announcer_GetAttributes();

#endif //!OBJ_ANNOUNCER_H

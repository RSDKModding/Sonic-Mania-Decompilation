#ifndef OBJ_KABASIRA_H
#define OBJ_KABASIRA_H

//Object Class
class Kabasira : Object {

};

//Entity Class
class EntityKabasira : Entity {

};

//Entity Functions
void Kabasira_Update();
void Kabasira_EarlyUpdate();
void Kabasira_LateUpdate();
void Kabasira_Draw();
void Kabasira_Setup(void* subtype);
void Kabasira_StageLoad();
void Kabasira_GetAttributes();

#endif //!OBJ_KABASIRA_H

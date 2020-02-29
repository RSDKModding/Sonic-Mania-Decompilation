#ifndef OBJ_KABASIRA_H
#define OBJ_KABASIRA_H

#include "../../SonicMania.h"

//Object Class
class ObjectKabasira : public Object {
public:

};

//Entity Class
class EntityKabasira : public Entity {
public:

};

//Object Entity
ObjectKabasira Kabasira;

//Entity Functions
void Kabasira_Update();
void Kabasira_EarlyUpdate();
void Kabasira_LateUpdate();
void Kabasira_Draw();
void Kabasira_Setup(void* subtype);
void Kabasira_StageLoad();
void Kabasira_GetAttributes();

#endif //!OBJ_KABASIRA_H

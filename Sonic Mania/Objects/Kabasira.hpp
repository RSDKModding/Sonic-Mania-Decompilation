#ifndef OBJ_KABASIRA_H
#define OBJ_KABASIRA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectKabasira : Object{

};

// Entity Class
struct EntityKabasira : Entity {

};

// Object Struct
extern ObjectKabasira *Kabasira;

// Standard Entity Events
void Kabasira_Update();
void Kabasira_LateUpdate();
void Kabasira_StaticUpdate();
void Kabasira_Draw();
void Kabasira_Create(void* data);
void Kabasira_StageLoad();
void Kabasira_EditorDraw();
void Kabasira_EditorLoad();
void Kabasira_Serialize();

// Extra Entity Functions


#endif //!OBJ_KABASIRA_H

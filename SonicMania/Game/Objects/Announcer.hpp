#ifndef OBJ_ANNOUNCER_H
#define OBJ_ANNOUNCER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAnnouncer : Object {

};

// Entity Class
struct EntityAnnouncer : Entity {

};

// Object Entity
extern ObjectAnnouncer Announcer;

// Standard Entity Events
void Announcer_Update();
void Announcer_LateUpdate();
void Announcer_StaticUpdate();
void Announcer_Draw();
void Announcer_Create(void* data);
void Announcer_StageLoad();
void Announcer_EditorDraw();
void Announcer_EditorLoad();
void Announcer_Serialize();

// Extra Entity Functions


#endif //!OBJ_ANNOUNCER_H

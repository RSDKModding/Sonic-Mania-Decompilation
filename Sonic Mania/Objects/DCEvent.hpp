#ifndef OBJ_DCEVENT_H
#define OBJ_DCEVENT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDCEvent : Object {

};

// Entity Class
struct EntityDCEvent : Entity {

};

// Object Struct
extern ObjectDCEvent *DCEvent;

// Standard Entity Events
void DCEvent_Update();
void DCEvent_LateUpdate();
void DCEvent_StaticUpdate();
void DCEvent_Draw();
void DCEvent_Create(void* data);
void DCEvent_StageLoad();
void DCEvent_EditorDraw();
void DCEvent_EditorLoad();
void DCEvent_Serialize();

// Extra Entity Functions


#endif //!OBJ_DCEVENT_H

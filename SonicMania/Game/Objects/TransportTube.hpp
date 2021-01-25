#ifndef OBJ_TRANSPORTTUBE_H
#define OBJ_TRANSPORTTUBE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTransportTube : Object {

};

// Entity Class
struct EntityTransportTube : Entity {

};

// Object Entity
extern ObjectTransportTube TransportTube;

// Standard Entity Events
void TransportTube_Update();
void TransportTube_LateUpdate();
void TransportTube_StaticUpdate();
void TransportTube_Draw();
void TransportTube_Create(void* data);
void TransportTube_StageLoad();
void TransportTube_EditorDraw();
void TransportTube_EditorLoad();
void TransportTube_Serialize();

// Extra Entity Functions


#endif //!OBJ_TRANSPORTTUBE_H

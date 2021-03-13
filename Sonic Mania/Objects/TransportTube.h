#ifndef OBJ_TRANSPORTTUBE_H
#define OBJ_TRANSPORTTUBE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTransportTube;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTransportTube;

// Object Struct
extern ObjectTransportTube *TransportTube;

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

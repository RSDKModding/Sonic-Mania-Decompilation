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
void TransportTube_Update(void);
void TransportTube_LateUpdate(void);
void TransportTube_StaticUpdate(void);
void TransportTube_Draw(void);
void TransportTube_Create(void* data);
void TransportTube_StageLoad(void);
void TransportTube_EditorDraw(void);
void TransportTube_EditorLoad(void);
void TransportTube_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TRANSPORTTUBE_H

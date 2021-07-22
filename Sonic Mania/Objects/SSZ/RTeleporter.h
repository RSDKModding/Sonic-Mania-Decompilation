#ifndef OBJ_RTELEPORTER_H
#define OBJ_RTELEPORTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRTeleporter;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRTeleporter;

// Object Struct
extern ObjectRTeleporter *RTeleporter;

// Standard Entity Events
void RTeleporter_Update(void);
void RTeleporter_LateUpdate(void);
void RTeleporter_StaticUpdate(void);
void RTeleporter_Draw(void);
void RTeleporter_Create(void* data);
void RTeleporter_StageLoad(void);
void RTeleporter_EditorDraw(void);
void RTeleporter_EditorLoad(void);
void RTeleporter_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_RTELEPORTER_H

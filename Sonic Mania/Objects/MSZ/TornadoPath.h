#ifndef OBJ_TORNADOPATH_H
#define OBJ_TORNADOPATH_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTornadoPath;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTornadoPath;

// Object Struct
extern ObjectTornadoPath *TornadoPath;

// Standard Entity Events
void TornadoPath_Update(void);
void TornadoPath_LateUpdate(void);
void TornadoPath_StaticUpdate(void);
void TornadoPath_Draw(void);
void TornadoPath_Create(void* data);
void TornadoPath_StageLoad(void);
void TornadoPath_EditorDraw(void);
void TornadoPath_EditorLoad(void);
void TornadoPath_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TORNADOPATH_H

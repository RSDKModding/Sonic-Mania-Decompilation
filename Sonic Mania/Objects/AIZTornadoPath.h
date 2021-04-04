#ifndef OBJ_AIZTORNADOPATH_H
#define OBJ_AIZTORNADOPATH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAIZTornadoPath;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAIZTornadoPath;

// Object Struct
extern ObjectAIZTornadoPath *AIZTornadoPath;

// Standard Entity Events
void AIZTornadoPath_Update(void);
void AIZTornadoPath_LateUpdate(void);
void AIZTornadoPath_StaticUpdate(void);
void AIZTornadoPath_Draw(void);
void AIZTornadoPath_Create(void* data);
void AIZTornadoPath_StageLoad(void);
void AIZTornadoPath_EditorDraw(void);
void AIZTornadoPath_EditorLoad(void);
void AIZTornadoPath_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_AIZTORNADOPATH_H

#ifndef OBJ_RHINOBOT_H
#define OBJ_RHINOBOT_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRhinobot;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRhinobot;

// Object Struct
extern ObjectRhinobot *Rhinobot;

// Standard Entity Events
void Rhinobot_Update(void);
void Rhinobot_LateUpdate(void);
void Rhinobot_StaticUpdate(void);
void Rhinobot_Draw(void);
void Rhinobot_Create(void* data);
void Rhinobot_StageLoad(void);
void Rhinobot_EditorDraw(void);
void Rhinobot_EditorLoad(void);
void Rhinobot_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_RHINOBOT_H

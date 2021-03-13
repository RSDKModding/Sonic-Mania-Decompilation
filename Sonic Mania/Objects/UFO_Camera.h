#ifndef OBJ_UFO_CAMERA_H
#define OBJ_UFO_CAMERA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_Camera;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Camera;

// Object Struct
extern ObjectUFO_Camera *UFO_Camera;

// Standard Entity Events
void UFO_Camera_Update();
void UFO_Camera_LateUpdate();
void UFO_Camera_StaticUpdate();
void UFO_Camera_Draw();
void UFO_Camera_Create(void* data);
void UFO_Camera_StageLoad();
void UFO_Camera_EditorDraw();
void UFO_Camera_EditorLoad();
void UFO_Camera_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_CAMERA_H

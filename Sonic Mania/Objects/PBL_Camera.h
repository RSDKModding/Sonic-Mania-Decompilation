#ifndef OBJ_PBL_CAMERA_H
#define OBJ_PBL_CAMERA_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_Camera;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_Camera;

// Object Struct
extern ObjectPBL_Camera *PBL_Camera;

// Standard Entity Events
void PBL_Camera_Update();
void PBL_Camera_LateUpdate();
void PBL_Camera_StaticUpdate();
void PBL_Camera_Draw();
void PBL_Camera_Create(void* data);
void PBL_Camera_StageLoad();
void PBL_Camera_EditorDraw();
void PBL_Camera_EditorLoad();
void PBL_Camera_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_CAMERA_H

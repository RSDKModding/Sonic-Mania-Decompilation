#ifndef OBJ_TUBESPRING_H
#define OBJ_TUBESPRING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTubeSpring;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTubeSpring;

// Object Struct
extern ObjectTubeSpring *TubeSpring;

// Standard Entity Events
void TubeSpring_Update();
void TubeSpring_LateUpdate();
void TubeSpring_StaticUpdate();
void TubeSpring_Draw();
void TubeSpring_Create(void* data);
void TubeSpring_StageLoad();
void TubeSpring_EditorDraw();
void TubeSpring_EditorLoad();
void TubeSpring_Serialize();

// Extra Entity Functions


#endif //!OBJ_TUBESPRING_H

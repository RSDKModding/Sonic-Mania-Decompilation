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
void TubeSpring_Update(void);
void TubeSpring_LateUpdate(void);
void TubeSpring_StaticUpdate(void);
void TubeSpring_Draw(void);
void TubeSpring_Create(void* data);
void TubeSpring_StageLoad(void);
void TubeSpring_EditorDraw(void);
void TubeSpring_EditorLoad(void);
void TubeSpring_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TUBESPRING_H

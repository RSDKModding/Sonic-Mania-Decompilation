#ifndef OBJ_TUBESPRING_H
#define OBJ_TUBESPRING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	ushort aniFrames;
	ushort sfxExit;
	Hitbox hitbox;
} ObjectTubeSpring;

// Entity Class
// rdc's gonna fire me LQWDOJWQ
typedef EntitySpring EntityTubeSpring;

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
void TubeSpring_Interact(void);
void TubeSpring_Springing(void);
void TubeSpring_Pullback(void);

bool32 TubeSpring_Spring(bool32 interact);

#endif //!OBJ_TUBESPRING_H

#ifndef OBJ_FLAMESPRING_H
#define OBJ_FLAMESPRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFlameSpring;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFlameSpring;

// Object Struct
extern ObjectFlameSpring *FlameSpring;

// Standard Entity Events
void FlameSpring_Update(void);
void FlameSpring_LateUpdate(void);
void FlameSpring_StaticUpdate(void);
void FlameSpring_Draw(void);
void FlameSpring_Create(void* data);
void FlameSpring_StageLoad(void);
void FlameSpring_EditorDraw(void);
void FlameSpring_EditorLoad(void);
void FlameSpring_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FLAMESPRING_H

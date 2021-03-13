#ifndef OBJ_UFO_PLASMA_H
#define OBJ_UFO_PLASMA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_Plasma;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Plasma;

// Object Struct
extern ObjectUFO_Plasma *UFO_Plasma;

// Standard Entity Events
void UFO_Plasma_Update();
void UFO_Plasma_LateUpdate();
void UFO_Plasma_StaticUpdate();
void UFO_Plasma_Draw();
void UFO_Plasma_Create(void* data);
void UFO_Plasma_StageLoad();
void UFO_Plasma_EditorDraw();
void UFO_Plasma_EditorLoad();
void UFO_Plasma_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_PLASMA_H

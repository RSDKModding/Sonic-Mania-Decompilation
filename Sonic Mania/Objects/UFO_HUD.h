#ifndef OBJ_UFO_HUD_H
#define OBJ_UFO_HUD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_HUD;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_HUD;

// Object Struct
extern ObjectUFO_HUD *UFO_HUD;

// Standard Entity Events
void UFO_HUD_Update();
void UFO_HUD_LateUpdate();
void UFO_HUD_StaticUpdate();
void UFO_HUD_Draw();
void UFO_HUD_Create(void* data);
void UFO_HUD_StageLoad();
void UFO_HUD_EditorDraw();
void UFO_HUD_EditorLoad();
void UFO_HUD_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_HUD_H

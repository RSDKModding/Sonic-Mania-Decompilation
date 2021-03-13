#ifndef OBJ_UFO_DECORATION_H
#define OBJ_UFO_DECORATION_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_Decoration;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Decoration;

// Object Struct
extern ObjectUFO_Decoration *UFO_Decoration;

// Standard Entity Events
void UFO_Decoration_Update();
void UFO_Decoration_LateUpdate();
void UFO_Decoration_StaticUpdate();
void UFO_Decoration_Draw();
void UFO_Decoration_Create(void* data);
void UFO_Decoration_StageLoad();
void UFO_Decoration_EditorDraw();
void UFO_Decoration_EditorLoad();
void UFO_Decoration_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_DECORATION_H

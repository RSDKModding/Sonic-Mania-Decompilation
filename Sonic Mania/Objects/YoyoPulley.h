#ifndef OBJ_YOYOPULLEY_H
#define OBJ_YOYOPULLEY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectYoyoPulley;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityYoyoPulley;

// Object Struct
extern ObjectYoyoPulley *YoyoPulley;

// Standard Entity Events
void YoyoPulley_Update();
void YoyoPulley_LateUpdate();
void YoyoPulley_StaticUpdate();
void YoyoPulley_Draw();
void YoyoPulley_Create(void* data);
void YoyoPulley_StageLoad();
void YoyoPulley_EditorDraw();
void YoyoPulley_EditorLoad();
void YoyoPulley_Serialize();

// Extra Entity Functions


#endif //!OBJ_YOYOPULLEY_H

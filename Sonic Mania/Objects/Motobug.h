#ifndef OBJ_MOTOBUG_H
#define OBJ_MOTOBUG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMotobug;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMotobug;

// Object Struct
extern ObjectMotobug *Motobug;

// Standard Entity Events
void Motobug_Update();
void Motobug_LateUpdate();
void Motobug_StaticUpdate();
void Motobug_Draw();
void Motobug_Create(void* data);
void Motobug_StageLoad();
void Motobug_EditorDraw();
void Motobug_EditorLoad();
void Motobug_Serialize();

// Extra Entity Functions


#endif //!OBJ_MOTOBUG_H

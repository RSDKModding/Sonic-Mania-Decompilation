#ifndef OBJ_INK_H
#define OBJ_INK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectInk;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityInk;

// Object Struct
extern ObjectInk *Ink;

// Standard Entity Events
void Ink_Update();
void Ink_LateUpdate();
void Ink_StaticUpdate();
void Ink_Draw();
void Ink_Create(void* data);
void Ink_StageLoad();
void Ink_EditorDraw();
void Ink_EditorLoad();
void Ink_Serialize();

// Extra Entity Functions


#endif //!OBJ_INK_H

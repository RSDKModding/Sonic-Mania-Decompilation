#ifndef OBJ_WISP_H
#define OBJ_WISP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWisp;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWisp;

// Object Entity
extern ObjectWisp *Wisp;

// Standard Entity Events
void Wisp_Update();
void Wisp_LateUpdate();
void Wisp_StaticUpdate();
void Wisp_Draw();
void Wisp_Create(void* data);
void Wisp_StageLoad();
void Wisp_EditorDraw();
void Wisp_EditorLoad();
void Wisp_Serialize();

// Extra Entity Functions


#endif //!OBJ_WISP_H

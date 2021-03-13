#ifndef OBJ_ACTCLEAR_H
#define OBJ_ACTCLEAR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectActClear;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityActClear;

// Object Struct
extern ObjectActClear *ActClear;

// Standard Entity Events
void ActClear_Update();
void ActClear_LateUpdate();
void ActClear_StaticUpdate();
void ActClear_Draw();
void ActClear_Create(void* data);
void ActClear_StageLoad();
void ActClear_EditorDraw();
void ActClear_EditorLoad();
void ActClear_Serialize();

// Extra Entity Functions


#endif //!OBJ_ACTCLEAR_H

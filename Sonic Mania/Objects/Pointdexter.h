#ifndef OBJ_POINTDEXTER_H
#define OBJ_POINTDEXTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPointdexter;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPointdexter;

// Object Struct
extern ObjectPointdexter *Pointdexter;

// Standard Entity Events
void Pointdexter_Update();
void Pointdexter_LateUpdate();
void Pointdexter_StaticUpdate();
void Pointdexter_Draw();
void Pointdexter_Create(void* data);
void Pointdexter_StageLoad();
void Pointdexter_EditorDraw();
void Pointdexter_EditorLoad();
void Pointdexter_Serialize();

// Extra Entity Functions


#endif //!OBJ_POINTDEXTER_H

#ifndef OBJ_FUNNEL_H
#define OBJ_FUNNEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFunnel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFunnel;

// Object Struct
extern ObjectFunnel *Funnel;

// Standard Entity Events
void Funnel_Update();
void Funnel_LateUpdate();
void Funnel_StaticUpdate();
void Funnel_Draw();
void Funnel_Create(void* data);
void Funnel_StageLoad();
void Funnel_EditorDraw();
void Funnel_EditorLoad();
void Funnel_Serialize();

// Extra Entity Functions


#endif //!OBJ_FUNNEL_H

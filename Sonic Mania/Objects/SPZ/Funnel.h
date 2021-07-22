#ifndef OBJ_FUNNEL_H
#define OBJ_FUNNEL_H

#include "SonicMania.h"

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
void Funnel_Update(void);
void Funnel_LateUpdate(void);
void Funnel_StaticUpdate(void);
void Funnel_Draw(void);
void Funnel_Create(void* data);
void Funnel_StageLoad(void);
void Funnel_EditorDraw(void);
void Funnel_EditorLoad(void);
void Funnel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FUNNEL_H

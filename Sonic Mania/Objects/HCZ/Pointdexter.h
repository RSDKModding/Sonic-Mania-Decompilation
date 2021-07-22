#ifndef OBJ_POINTDEXTER_H
#define OBJ_POINTDEXTER_H

#include "SonicMania.h"

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
void Pointdexter_Update(void);
void Pointdexter_LateUpdate(void);
void Pointdexter_StaticUpdate(void);
void Pointdexter_Draw(void);
void Pointdexter_Create(void* data);
void Pointdexter_StageLoad(void);
void Pointdexter_EditorDraw(void);
void Pointdexter_EditorLoad(void);
void Pointdexter_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_POINTDEXTER_H

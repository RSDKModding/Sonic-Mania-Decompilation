#ifndef OBJ_IWAMODOKI_H
#define OBJ_IWAMODOKI_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectIwamodoki;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityIwamodoki;

// Object Struct
extern ObjectIwamodoki *Iwamodoki;

// Standard Entity Events
void Iwamodoki_Update(void);
void Iwamodoki_LateUpdate(void);
void Iwamodoki_StaticUpdate(void);
void Iwamodoki_Draw(void);
void Iwamodoki_Create(void* data);
void Iwamodoki_StageLoad(void);
void Iwamodoki_EditorDraw(void);
void Iwamodoki_EditorLoad(void);
void Iwamodoki_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_IWAMODOKI_H

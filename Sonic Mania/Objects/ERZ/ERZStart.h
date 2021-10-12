#ifndef OBJ_ERZSTART_H
#define OBJ_ERZSTART_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectERZStart;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityERZStart;

// Object Struct
extern ObjectERZStart *ERZStart;

// Standard Entity Events
void ERZStart_Update(void);
void ERZStart_LateUpdate(void);
void ERZStart_StaticUpdate(void);
void ERZStart_Draw(void);
void ERZStart_Create(void* data);
void ERZStart_StageLoad(void);
void ERZStart_EditorDraw(void);
void ERZStart_EditorLoad(void);
void ERZStart_Serialize(void);

// Extra Entity Functions
void ERZStart_Unknown16(void);
void ERZStart_Unknown17(void);

void ERZStart_Player_HandleSuperDash(void *p);
void ERZStart_State_PlayerSuperFly(void);
void ERZStart_Player_StartSuperFly(void);
void ERZStart_State_PlayerRebound(void);

#endif //!OBJ_ERZSTART_H
